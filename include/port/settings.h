// Credits: TwilitRealm

#ifndef MP5_NATIVE_CONFIG_H
#define MP5_NATIVE_CONFIG_H

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <card.h>

#ifdef __cplusplus
#include "port/config_var.hpp"

#include <os/OSRtc.h>

namespace mp5native {

using namespace config;

enum class GameLanguage : u8 {
    English = OS_LANGUAGE_ENGLISH,
    German = OS_LANGUAGE_GERMAN,
    French = OS_LANGUAGE_FRENCH,
    Spanish = OS_LANGUAGE_SPANISH,
    Italian = OS_LANGUAGE_ITALIAN,
};

enum class DiscVerificationState : u8 {
    Unknown = 0,
    Success,
    HashMismatch,
};

namespace config {
template <>
struct ConfigEnumRange<GameLanguage> {
    static constexpr auto min = GameLanguage::English;
    static constexpr auto max = GameLanguage::Italian;
};

template <>
struct ConfigEnumRange<DiscVerificationState> {
    static constexpr auto min = DiscVerificationState::Unknown;
    static constexpr auto max = DiscVerificationState::HashMismatch;
};

}

// Persistent user settings

struct UserSettings {
    // Program settings

    struct {
        // Video
        ConfigVar<bool> enableFullscreen;
        ConfigVar<bool> enableVsync;
        ConfigVar<bool> lockAspectRatio;
        ConfigVar<bool> widescreen;
        ConfigVar<bool> hudCentered;
        ConfigVar<bool> enableFpsOverlay;
        ConfigVar<int> fpsOverlayCorner;
        ConfigVar<int> frameRate; // 60 = original, 120/144/240/360, 0 = display refresh rate
        ConfigVar<bool> frameInterpolation;
        ConfigVar<int> display; // monitor the window opens on: -1 = system default, else 0-based index
    } video;

    struct {
        // Audio
        ConfigVar<int> masterVolume;
        ConfigVar<int> mainMusicVolume;
        ConfigVar<int> subMusicVolume;
        ConfigVar<int> soundEffectsVolume;
        ConfigVar<int> fanfareVolume;
        ConfigVar<bool> enableReverb;
        ConfigVar<bool> enableHrtf;
        ConfigVar<bool> menuSounds;
    } audio;

    // Game settings

    struct {
        ConfigVar<GameLanguage> language;

        // QoL
        ConfigVar<bool> enableQuickTransform;

        // Preferences
        ConfigVar<bool> pauseOnFocusLost;
        ConfigVar<bool> enableAchievementToasts;
        ConfigVar<bool> enableControllerToasts;

        // Graphics
        ConfigVar<int> internalResolutionScale;
        ConfigVar<int> shadowResolutionMultiplier;
        ConfigVar<int> msaaSamples;        // 1 = off (original); read at start-up
        ConfigVar<int> textureAnisotropy;  // 1 = off (original) .. 16; read at start-up
        ConfigVar<int> loadingSpeed;       // 0 = fast (the disc image is read at once), 1 = GameCube drive timing

        // Audio

        // Input
        ConfigVar<bool> allowBackgroundInput;

        // Cheats
        ConfigVar<bool> infiniteHearts;
        ConfigVar<bool> unlockAllMinigames;
        ConfigVar<bool> unlockBowsersGnarlyParty;

        // Technical

        // Controls
        ConfigVar<bool> enableTurboKeybind;

        // Tools
        ConfigVar<bool> speedrunMode;
        ConfigVar<bool> recordingMode;
    } game;

    struct {
        ConfigVar<std::string> isoPath;
        ConfigVar<DiscVerificationState> isoVerification;
        ConfigVar<std::string> graphicsBackend;
        ConfigVar<bool> skipPreLaunchUI;
        ConfigVar<bool> skipBootSequence;
        ConfigVar<bool> showPipelineCompilation;
        ConfigVar<bool> wasPresetChosen;
        ConfigVar<bool> enableCrashReporting;
        ConfigVar<bool> checkForUpdates;
        ConfigVar<int> cardFileType;
        ConfigVar<bool> enableAdvancedSettings;
    } backend;

    struct {
        // RetroAchievements (src/port/retroachievements.cpp). Only the session
        // token is kept, never the password.
        ConfigVar<bool> enabled;
        ConfigVar<std::string> username;
        ConfigVar<std::string> token;
    } retroAchievements;
};

UserSettings& getSettings();

void registerSettings();

}
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

CARDFileType mp5native_settings_card_file_type(void);
bool mp5native_settings_enableTurboKeybind(void);
bool mp5native_settings_skipBootSequence(void);
bool mp5native_settings_unlock_all_minigames(void);
bool mp5native_settings_unlock_bowsers_gnarly_party(void);

#ifdef __cplusplus
}
#endif

#endif // MP5_NATIVE_CONFIG_H
