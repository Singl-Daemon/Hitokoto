#pragma once

#include <string>

namespace Hitokoto {

struct Config {
    int         version      = 1;
    bool        enabled      = true;
    std::string hitokotoUrl  = "https://v1.hitokoto.cn/";
    std::string hitokotoType = "a";
    std::string contentColor = "§e";
    std::string authorColor  = "§7";
};
} // namespace Hitokoto