#include "Entry.h"
#include "Config.h"
#include "Hitokoto.h"

#include <ll/api/Config.h>
#include <ll/api/mod/RegisterHelper.h>

#include <future>

namespace Hitokoto {

Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() /* NOLINT */ {
    config.emplace();
    if (!ll::config::loadConfig(*config, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*config, getSelf().getConfigDir() / u8"config.json");
    }
    if (!config->enabled) {
        logger.warn("Plugin will not be enabled because 'enabled' is set to false in the config.");
        return false;
    }
    return true;
}

bool Entry::enable() /* NOLINT */ {
    RegisterListener();
    auto getHitokotoTask = std::async(std::launch::async, [this]() {
        return GetHitokoto(getConfig().hitokotoUrl, getConfig().hitokotoType);
    });
    getHitokotoTask.wait();
    if (const auto hitokoto = getHitokotoTask.get(); hitokoto.has_value()) {
        logger.info(hitokoto->first);
        logger.info("---- {}", hitokoto->second);
    } else {
        logger.warn("Failed to get hitokoto.");
    }
    return true;
}

bool Entry::disable() /* NOLINT */ {
    eventBus.removeListener(playerJoinEventListener);
    return true;
}

bool Entry::unload() /* NOLINT */ { return true; }

Config& Entry::getConfig() /* NOLINT */ { return config.value(); }

} // namespace Hitokoto

LL_REGISTER_MOD(Hitokoto::Entry, Hitokoto::Entry::getInstance());