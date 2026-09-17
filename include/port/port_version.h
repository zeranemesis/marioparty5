#pragma once

#ifdef __cplusplus

#include <cstdlib>
#include <dvd.h>
#include <initializer_list>
#include <types.h>
#include "version.h"

/**
 * Functionality for switching game behavior based on the loaded game version (e.g. PAL/JPN, GC/Wii)
 */
namespace mp5native::version {
enum class GameVersion : u8 {
    UsaRev0 = VERSION_NO_ENG0, // GP5E01_00, the only version decompiled
};

bool isRegionPal();
bool isRegionJpn();
bool isRegionUsa();

GameVersion getGameVersion();

const DVDDiskID& getDiskID();

void init();

template<typename T>
struct VersionOption {
    GameVersion mVersion;
    T mValue;

    constexpr VersionOption(GameVersion version, T value) : mVersion(version), mValue(value) {}
};

template<typename T>
const T& versionSelect(const std::initializer_list<VersionOption<T>> options) {
    const auto version = getGameVersion();
    for (const auto& opt : options) {
        if (opt.mVersion == version) {
            return opt.mValue;
        }
    }

    // Unable to find value.
    abort();
}

template<typename T>
const T& versionSelect(const std::initializer_list<VersionOption<T>> options, const T& defaultValue) {
    const auto version = getGameVersion();
    for (const auto& opt : options) {
        if (opt.mVersion == version) {
            return opt.mValue;
        }
    }

    return defaultValue;
}
}  // namespace mp5native::version

#else

u8 mp5native_version_get_version(void);

BOOL mp5native_version_is_pal(void);
BOOL mp5native_version_is_usa(void);
BOOL mp5native_version_is_ntsc(void);

#endif