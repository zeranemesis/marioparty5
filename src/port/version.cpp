#include "port/port_version.h"
#include "aurora/lib/logging.hpp"

#include <dvd.h>

namespace mp5native::version {

aurora::Module Mp5NativeVersionLog("mp5native::version");

using namespace std::string_view_literals;

static bool versionInitialized;
static GameVersion gameVersion;
static DVDDiskID diskId;

void init() {
    versionInitialized = true;

    if (!DVDLowReadDiskID(&diskId, nullptr)) {
        Mp5NativeVersionLog.fatal("DVDLowReadDiskID failed to return instantly.");
    }

    std::string_view company(diskId.company, sizeof(diskId.company));
    std::string_view game(diskId.gameName, sizeof(diskId.gameName));

    if (company != "01"sv) {
        Mp5NativeVersionLog.fatal("Wrong company ID in disc: {}", company);
    }

    if (game == "GP5E"sv && diskId.gameVersion == 0) {
        gameVersion = GameVersion::UsaRev0;
    } else if (game == "GP5P"sv && diskId.gameVersion == 0) {
        gameVersion = GameVersion::EurRev0;
    } else {
        // The decompilation only matches GP5E01 revision 0.
        Mp5NativeVersionLog.fatal("Unsupported game in disc: {}{} revision {}", game, company, diskId.gameVersion);
    }

    Mp5NativeVersionLog.info("Loaded game disc is {}{}", game, company);
}

bool isRegionJpn() {
    return false;
}

bool isRegionPal() {
    return getGameVersion() == GameVersion::EurRev0;
}

bool isRegionUsa() {
    return getGameVersion() == GameVersion::UsaRev0;
}

GameVersion getGameVersion() {
    if (!versionInitialized) {
        abort();
    }

    return gameVersion;
}

const DVDDiskID& getDiskID() {
    return diskId;
}

}  // namespace mp5native::version

extern "C" u8 mp5native_version_get_version(void)
{
    return static_cast<u8>(mp5native::version::getGameVersion());
}

extern "C" BOOL mp5native_version_is_pal(void)
{
    return mp5native::version::isRegionPal();
}

extern "C" BOOL mp5native_version_is_usa(void)
{
    return mp5native::version::isRegionUsa();
}

extern "C" BOOL mp5native_version_is_ntsc(void)
{
    return mp5native::version::isRegionUsa() || mp5native::version::isRegionJpn();
}