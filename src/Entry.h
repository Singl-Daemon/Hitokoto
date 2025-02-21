#pragma once

#include "ll/api/io/Logger.h"
#include "ll/api/mod/NativeMod.h"

#include "Config.h"

namespace Hitokoto {

class Entry {

public:
    static Entry& getInstance();

    Entry() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

    Config& getConfig();

private:
    ll::mod::NativeMod& mSelf;
};
inline std::optional<Config>  config;
inline ll::io::Logger& logger = Entry::getInstance().getSelf().getLogger();

} // namespace Hitokoto
