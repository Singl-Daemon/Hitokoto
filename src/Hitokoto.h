#pragma once

#include <nlohmann/json.hpp>

#include <ll/api/event/EventBus.h>

namespace Hitokoto {
inline auto&                  eventBus = ll::event::EventBus::getInstance();
inline ll::event::ListenerPtr playerJoinEventListener;
std::optional<std::pair<std::string, std::string>>
GetHitokoto(const std::string& hitokotoUrl, const std::string& hitokotoType = "a");

void RegisterListener();
} // namespace Hitokoto