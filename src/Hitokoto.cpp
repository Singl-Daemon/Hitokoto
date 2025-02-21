#include "Hitokoto.h"
#include "Entry.h"

#include <curl/curl.h>

#include <ll/api/event/player/PlayerJoinEvent.h>

#include <optional>
#include <string>
#include <utility>

namespace Hitokoto {

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

std::optional<std::pair<std::string, std::string>>
GetHitokoto(const std::string& hitokotoUrl, const std::string& hitokotoType) {
    const std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), curl_easy_cleanup);
    if (!curl) {
        return std::nullopt;
    }

    std::string       readBuffer;
    const std::string requestUrl = hitokotoUrl + "?c=" + hitokotoType;
    curl_easy_setopt(curl.get(), CURLOPT_URL, requestUrl.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &readBuffer);

    if (const CURLcode res = curl_easy_perform(curl.get()); res != CURLE_OK) {
        return std::nullopt;
    }

    try {
        const auto jsonResponse = nlohmann::json::parse(readBuffer);
        if (jsonResponse.contains("hitokoto") && jsonResponse.contains("from")) {
            return std::make_pair(jsonResponse["hitokoto"].get<std::string>(), jsonResponse["from"].get<std::string>());
        }
        return std::nullopt;
    } catch (const nlohmann::json::parse_error&) {
        return std::nullopt;
    }
}

void RegisterListener() {
    playerJoinEventListener =
        eventBus.emplaceListener<ll::event::player::PlayerJoinEvent>([](const ll::event::player::PlayerJoinEvent& event
                                                                     ) {
            const auto& player = event.self();
            auto        getHitokotoTask =
                std::async(std::launch::async, []() { return GetHitokoto(config->hitokotoUrl, config->hitokotoType); });
            getHitokotoTask.wait();
            if (const auto hitokoto = getHitokotoTask.get(); hitokoto.has_value()) {
                player.sendMessage(config->contentColor + hitokoto->first);
                player.sendMessage(config->authorColor + "---- " + hitokoto->second);
            } else {
                player.sendMessage("Failed to get hitokoto.");
            }
        });
}


} // namespace Hitokoto