// Credits: TwilitRealm

#include "imgui/ImGuiEngine.hpp"
#include "port/retroachievements.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif
#include "iso_validate.hpp"
#include "ui/ui.hpp"

#include "mp5native_version.h"
#include "ui/menu_bar.hpp"
#include "ui/overlay.hpp"
#include "ui/netplay.hpp"
#include "ui/prelaunch.hpp"
#include "ui/preset.hpp"

#include <algorithm>
#include <SDL3/SDL_filesystem.h>
#include "port/display_select.h"
#include <aurora/aurora.h>
#include <aurora/event.h>
#include <aurora/gfx.h>
#include <dolphin/gx/GXAurora.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include <game/disp.h>
#include <port/config.hpp>
#include <port/dolassets.h>
#include <port/main.h>
#include <port/netplay.h>
#include <port/settings.h>
#include <port/port_version.h>

#include <aurora/dvd.h>
#include <aurora/lib/logging.hpp>
#include <game/card.h>
#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <thread>

extern "C" int game_main();
extern "C" void sndQuit(void);
extern "C" bool sndIsInstalled(void);
extern "C" void sndPCSetOutputCallback(void (*callback)(int16_t* output, size_t frames, uint32_t rate, uint32_t channels)); // musyx/pc.h

using namespace std::string_literals;
using namespace std::string_view_literals;

static bool mainCalled = false;

AuroraInfo auroraInfo;
std::filesystem::path Mp5Native_ConfigPath;

aurora::Module Mp5NativeMainLog("mp5native::main");

static u8 selectedLanguage;

u8 OSGetLanguage() {
    return selectedLanguage;
}

static void LanguageInit() {
    // Keep language at 0 (English) if not on a PAL disc.
    // Doubt this matters, but avoid funky shit.
    if (!mp5native::version::isRegionPal()) {
        return;
    }

    // Cache this to avoid funky shenanigans.
    selectedLanguage = static_cast<u8>(mp5native::getSettings().game.language.getValue());
}

// keepGoing: run while it returns true instead (the online play lobby)
bool launchUILoop(bool (*keepGoing)() = nullptr) {
    while (Mp5Native_IsRunning && (keepGoing != nullptr ? keepGoing() : !Mp5Native_IsGameLaunched)) {
        const AuroraEvent* event = aurora_update();
        while (event != nullptr && event->type != AURORA_NONE) {
            switch (event->type) {
                case AURORA_SDL_EVENT:
                    mp5native::ui::handle_event(event->sdl);
                // mp5native::g_imguiConsole.HandleSDLEvent(event->sdl);
                break;
                case AURORA_DISPLAY_SCALE_CHANGED:
                    // TODO PC
                    // mp5native::ImGuiEngine_Initialize(event->windowSize.scale);
                break;
                case AURORA_EXIT:
                    return false;
            }

            event++;
        }

        if (!aurora_begin_frame()) {
            Mp5NativeMainLog.debug("aurora_begin_frame returned false, skipping draw this frame");
            continue;
        }

        mp5native::ui::update();

        // mp5native::g_imguiConsole.PreDraw();
        // mp5native::g_imguiConsole.PostDraw();

        aurora_end_frame();
    }

    return Mp5Native_IsRunning;
}

void aurora_log_callback(AuroraLogLevel level, const char* module, const char *message, unsigned int len)
{
    const char *levelStr = "??";
    FILE *out = stdout;
    switch (level) {
        case LOG_DEBUG:
            levelStr = "DEBUG";
            break;
        case LOG_INFO:
            levelStr = "INFO";
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            out = stderr;
            break;
        case LOG_FATAL:
            levelStr = "FATAL";
            out = stderr;
            break;
    }
    fprintf(out, "[%s | %s] %s\n", levelStr, module, message);
#ifdef __ANDROID__
    /* stdout/stderr go nowhere on Android: mirror every line to logcat (tag MP5) */
    __android_log_print(level >= LOG_ERROR ? ANDROID_LOG_ERROR : level == LOG_WARNING ? ANDROID_LOG_WARN : ANDROID_LOG_INFO,
                        "MP5", "[%s] %s", module, message);
#endif
    if (level == LOG_FATAL) {
        fflush(out);
        abort();
    }
}

static bool try_parse_backend(std::string_view backend, AuroraBackend& outBackend) {
    if (backend == "auto") {
        outBackend = BACKEND_AUTO;
        return true;
    }
    if (backend == "d3d11") {
        outBackend = BACKEND_D3D11;
        return true;
    }
    if (backend == "d3d12") {
        outBackend = BACKEND_D3D12;
        return true;
    }
    if (backend == "metal") {
        outBackend = BACKEND_METAL;
        return true;
    }
    if (backend == "vulkan") {
        outBackend = BACKEND_VULKAN;
        return true;
    }
    if (backend == "opengl") {
        outBackend = BACKEND_OPENGL;
        return true;
    }
    if (backend == "opengles") {
        outBackend = BACKEND_OPENGLES;
        return true;
    }
    if (backend == "webgpu") {
        outBackend = BACKEND_WEBGPU;
        return true;
    }
    if (backend == "null") {
        outBackend = BACKEND_NULL;
        return true;
    }

    return false;
}

static std::string_view backend_name(AuroraBackend backend) {
    switch (backend) {
        default:
            return "Auto"sv;
        case BACKEND_D3D12:
            return "D3D12"sv;
        case BACKEND_D3D11:
            return "D3D11"sv;
        case BACKEND_METAL:
            return "Metal"sv;
        case BACKEND_VULKAN:
            return "Vulkan"sv;
        case BACKEND_OPENGL:
            return "OpenGL"sv;
        case BACKEND_OPENGLES:
            return "OpenGL ES"sv;
        case BACKEND_WEBGPU:
            return "WebGPU"sv;
        case BACKEND_NULL:
            return "Null"sv;
    }
}

static bool IsBackendAvailable(AuroraBackend backend) {
    if (backend == BACKEND_AUTO) {
        return true;
    }

    size_t availableBackendCount = 0;
    const AuroraBackend* availableBackends = aurora_get_available_backends(&availableBackendCount);
    for (size_t i = 0; i < availableBackendCount; ++i) {
        if (availableBackends[i] == backend) {
            return true;
        }
    }

    return false;
}

static AuroraBackend ResolveDesiredBackend() {
    AuroraBackend desiredBackend = BACKEND_AUTO;

    if (!try_parse_backend(
                   static_cast<const std::string&>(mp5native::getSettings().backend.graphicsBackend),
                   desiredBackend))
    {
        Mp5NativeMainLog.warn("Unknown configured backend '{}', falling back to Auto",
                     static_cast<const std::string&>(mp5native::getSettings().backend.graphicsBackend));
        desiredBackend = BACKEND_AUTO;
    }

    if (!IsBackendAvailable(desiredBackend)) {
        Mp5NativeMainLog.warn("Requested backend '{}' is unavailable, falling back to Auto",
                     backend_name(desiredBackend));
        desiredBackend = BACKEND_AUTO;
    }

    return desiredBackend;
}


static void migrate_directory(const std::filesystem::path& from, const std::filesystem::path& to) {
    std::error_code ec;
    std::filesystem::create_directories(to, ec);
    if (ec) {
        return;
    }

    for (std::filesystem::recursive_directory_iterator it(
             from, std::filesystem::directory_options::skip_permission_denied, ec);
        it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
    {
        if (ec) {
            return;
        }

        const auto relativePath = std::filesystem::relative(it->path(), from, ec);
        if (ec) {
            return;
        }

        const auto targetPath = to / relativePath;
        if (it->is_directory(ec)) {
            std::filesystem::create_directories(targetPath, ec);
            if (ec) {
                return;
            }
        } else if (it->is_regular_file(ec) && !std::filesystem::exists(targetPath, ec)) {
            std::filesystem::create_directories(targetPath.parent_path(), ec);
            if (ec) {
                return;
            }
            std::filesystem::copy_file(
                it->path(), targetPath, std::filesystem::copy_options::skip_existing, ec);
            if (ec) {
                return;
            }
        }
    }
}

static std::filesystem::path calculate_config_path() {
#ifdef __APPLE__
#if TARGET_OS_IOS && !TARGET_OS_TV
    const char* documentsPath = SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS);
    if (!documentsPath) {
        Mp5NativeMainLog.error("Unable to get iOS Documents path: {}", SDL_GetError());
    }

    std::filesystem::path configPath = reinterpret_cast<const char8_t*>(documentsPath);

    char* oldPrefPath = SDL_GetPrefPath("zeranemesis", "MP5 Native");
    if (oldPrefPath) {
        const std::filesystem::path oldConfigPath = reinterpret_cast<const char8_t*>(oldPrefPath);
        SDL_free(oldPrefPath);

        std::error_code ec;
        if (oldConfigPath != configPath && std::filesystem::exists(oldConfigPath, ec)) {
            migrate_directory(oldConfigPath, configPath);
        }
    }

    return configPath;
#endif
#endif

    // test bench: MP5_TEST_CONFIG_DIR=<dir> keeps the config and the memory card of a test run apart from the player's
    if (const char* dir = getenv("MP5_TEST_CONFIG_DIR")) {
        return std::filesystem::path(reinterpret_cast<const char8_t*>(dir)) / "";
    }
    const auto result = SDL_GetPrefPath("zeranemesis", "MP5 Native");
    if (!result) {
        Mp5NativeMainLog.error("Unable to get PrefPath: {}", SDL_GetError());
    }

    return reinterpret_cast<const char8_t*>(result);
}

static constexpr PADDefaultMapping defaultPadMapping = {
    .buttons = {
        {SDL_GAMEPAD_BUTTON_SOUTH, PAD_BUTTON_A},
        {SDL_GAMEPAD_BUTTON_EAST, PAD_BUTTON_B},
        {SDL_GAMEPAD_BUTTON_WEST, PAD_BUTTON_X},
        {SDL_GAMEPAD_BUTTON_NORTH, PAD_BUTTON_Y},
        {SDL_GAMEPAD_BUTTON_START, PAD_BUTTON_START},
        {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, PAD_TRIGGER_Z},
        {PAD_NATIVE_BUTTON_INVALID, PAD_TRIGGER_L},
        {PAD_NATIVE_BUTTON_INVALID, PAD_TRIGGER_R},
        {SDL_GAMEPAD_BUTTON_DPAD_UP, PAD_BUTTON_UP},
        {SDL_GAMEPAD_BUTTON_DPAD_DOWN, PAD_BUTTON_DOWN},
        {SDL_GAMEPAD_BUTTON_DPAD_LEFT, PAD_BUTTON_LEFT},
        {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, PAD_BUTTON_RIGHT},
    },
    .axes = {
        {{SDL_GAMEPAD_AXIS_LEFTX, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_X_POS},
        {{SDL_GAMEPAD_AXIS_LEFTX, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_X_NEG},
        // SDL's gamepad y-axis is inverted from GC's
        {{SDL_GAMEPAD_AXIS_LEFTY, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_Y_POS},
        {{SDL_GAMEPAD_AXIS_LEFTY, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_Y_NEG},
        {{SDL_GAMEPAD_AXIS_RIGHTX, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_X_POS},
        {{SDL_GAMEPAD_AXIS_RIGHTX, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_X_NEG},
        // see above
        {{SDL_GAMEPAD_AXIS_RIGHTY, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_Y_POS},
        {{SDL_GAMEPAD_AXIS_RIGHTY, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_Y_NEG},
        {{SDL_GAMEPAD_AXIS_LEFT_TRIGGER, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_TRIGGER_L},
        {{SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_TRIGGER_R},
    },
};

extern "C" void Mp5InstallCrashReport(void);

#ifdef __ANDROID__
#include <jni.h>
#include <SDL3/SDL_system.h>
// Asks Android for a display refresh matching the game's frame rate (Mp5NativeActivity.setPreferredRefreshRate):
// the Thor's 120 Hz panel otherwise refreshes twice per 60 fps frame. 0 (the display's own rate) asks for nothing.
static void AndroidPreferRefreshRate(float hz) {
    auto* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    jobject activity = env != nullptr ? static_cast<jobject>(SDL_GetAndroidActivity()) : nullptr;
    if (activity == nullptr) {
        return;
    }
    jclass cls = env->GetObjectClass(activity);
    jmethodID method = cls != nullptr ? env->GetMethodID(cls, "setPreferredRefreshRate", "(F)V") : nullptr;
    if (method != nullptr) {
        env->CallVoidMethod(activity, method, static_cast<jfloat>(hz));
    }
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }
    if (cls != nullptr) {
        env->DeleteLocalRef(cls);
    }
    env->DeleteLocalRef(activity);
}
#endif

extern "C" int port_main(int argc, char* argv[]) {
    Mp5InstallCrashReport();
#ifdef __ANDROID__
    // An app has no environment of its own: the test bench passes MP5_TEST_*=value as launch arguments
    // (adb shell am start ... --es mp5native_args "MP5_TEST_BOARD=0 MP5_TEST_COM=1")
    for (int i = 1; i < argc; ++i) {
        if (const char* eq = strchr(argv[i], '='); eq != nullptr && (strncmp(argv[i], "MP5_TEST_", 9) == 0 || strncmp(argv[i], "MP5_NET_", 8) == 0)) {
            setenv(std::string(argv[i], eq - argv[i]).c_str(), eq + 1, 1);
        }
    }
#endif
    // Buffered, flushed every 200 ms and by the crash report: a log line per write cost a system call each, and the
    // game's OSReport floods (heap dumps, free errors) stalled frames for up to a second. A kill loses <= 200 ms.
    setvbuf(stdout, nullptr, _IOFBF, 1 << 16);
    std::thread([] {
        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            fflush(stdout);
        }
    }).detach();
    // On iOS, when connected to an external monitor, SDLUIKitSceneDelegate scene:willConnectToSession:
    // can call our main function again. Explicitly guard against this reinitialization.
    if (mainCalled) {
        return 0;
    }
    mainCalled = true;

    mp5native::registerSettings();
    mp5native::config::FinishRegistration();

    Mp5Native_ConfigPath = calculate_config_path();
    Mp5Net_SetConfigPath(reinterpret_cast<const char*>(Mp5Native_ConfigPath.u8string().c_str()));

    mp5native::config::LoadFromUserPreferences();
    // test bench: MP5_TEST_ASPECT=lock|stretch|wide overrides the aspect settings for this run, unsaved
    if (const char* aspect = getenv("MP5_TEST_ASPECT")) {
        auto& video = mp5native::getSettings().video;
        video.lockAspectRatio.setOverrideValue(strcmp(aspect, "lock") == 0);
        video.widescreen.setOverrideValue(strcmp(aspect, "wide") == 0);
        video.hudCentered.setOverrideValue(getenv("MP5_TEST_HUD_CENTERED") != nullptr);
    }
    if (getenv("MP5_TEST_PRELAUNCH") != nullptr) { // test bench: show the prelaunch screen for this run, unsaved
        mp5native::getSettings().backend.skipPreLaunchUI.setOverrideValue(false);
    }
    // test bench: MP5_TEST_FPS=<cap> runs the high frame rate path (src/port/frame_interp.cpp) with vsync off, unsaved
    if (getenv("MP5_TEST_FPS") != nullptr) {
        mp5native::getSettings().video.enableVsync.setOverrideValue(false);
    }
    // test bench: MP5_TEST_BACKEND=vulkan|d3d12|... picks the graphics backend for this run, unsaved
    if (const char* backend = getenv("MP5_TEST_BACKEND")) {
        mp5native::getSettings().backend.graphicsBackend.setOverrideValue(std::string(backend));
    }
    // TODO: How to handle this?
    //PADSetDefaultMapping(&defaultPadMapping, PAD_TYPE_STANDARD);

    {
        const auto configPathString = Mp5Native_ConfigPath.u8string();
        AuroraConfig config{};
        config.appName = "MP5 Native";
        config.userPath = reinterpret_cast<const char*>(configPathString.c_str());
        config.vsync = mp5native::getSettings().video.enableVsync;
        config.msaa = static_cast<uint32_t>(std::clamp(mp5native::getSettings().game.msaaSamples.getValue(), 1, 8));
        config.maxTextureAnisotropy = static_cast<uint16_t>(std::clamp(mp5native::getSettings().game.textureAnisotropy.getValue(), 1, 16));
        config.startFullscreen = mp5native::getSettings().video.enableFullscreen;
        config.windowPosX = -1;
        config.windowPosY = -1;
        config.windowWidth = HU_FB_WIDTH * 2;
        config.windowHeight = HU_FB_HEIGHT * 2;
        // open on the monitor chosen in Video > Display (the window's monitor also sets the high frame rate cap)
        mp5native::display::start_pos(mp5native::getSettings().video.display.getValue(), config.windowWidth,
                                      config.windowHeight, &config.windowPosX, &config.windowPosY);
        config.desiredBackend = ResolveDesiredBackend();
        config.logCallback = &aurora_log_callback;
        config.mem1Size = 64 * 1024 * 1024;
        config.mem2Size = 24 * 1024 * 1024;
        config.allowJoystickBackgroundEvents = mp5native::getSettings().game.allowBackgroundInput;
        config.pauseOnFocusLost = mp5native::getSettings().game.pauseOnFocusLost;
        // config.imGuiInitCallback = &aurora_imgui_init_callback;
        config.allowTextureDumps = false;
        // test bench: MP5_TEST_CACHE_DIR=<dir> keeps parallel runs from sharing (and locking) one pipeline cache
        config.cachePath = getenv("MP5_TEST_CACHE_DIR");
        auroraInfo = aurora_initialize(argc, argv, &config);
    }

#ifdef MP5_NATIVE_DISCORD
    mp5native::discord::initialize();
#endif

    char windowTitle[100];
    snprintf(windowTitle, sizeof(windowTitle), "Mp5Native %s", MP5_NATIVE_WC_DESCRIBE);
    VISetWindowTitle(windowTitle);

    if (mp5native::getSettings().video.lockAspectRatio) {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
    } else {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
    }
    VISetFrameBufferScale(mp5native::getSettings().game.internalResolutionScale.getValue());

    // TODO PC
    // mp5native::audio::SetMasterVolume(mp5native::getSettings().audio.masterVolume / 100.0f);
    // mp5native::audio::SetEnableReverb(mp5native::getSettings().audio.enableReverb);
    // mp5native::audio::EnableHrtf = mp5native::getSettings().audio.enableHrtf;

    // Run ImGui UI loop if Aurora couldn't initialize a backend
    if (auroraInfo.backend == BACKEND_NULL) {
        launchUILoop();
        fflush(stdout);
        fflush(stderr);
#ifdef MP5_NATIVE_DISCORD
        mp5native::discord::shutdown();
#endif
        mp5native::ui::shutdown();
        aurora_shutdown();
        return 0;
    }

    mp5native::ui::initialize();
    mp5native::ui::push_document(std::make_unique<mp5native::ui::Overlay>(), true, true);
    mp5native::ui::push_document(std::make_unique<mp5native::ui::MenuBar>(), false);

    // Invalidate a bad saved isoPath so that MP5 Native can't get blocked from starting up.
    // This is only a metadata check; full hash verification is handled by the prelaunch UI.
    bool forcePreLaunchUI = false;
    bool saveConfigBeforePrelaunch = false;

    const std::string p = mp5native::getSettings().backend.isoPath;
    mp5native::iso::DiscInfo discInfo{};
    if (!p.empty() &&
        mp5native::iso::inspect(p.c_str(), discInfo) != mp5native::iso::ValidationError::Success)
    {
        Mp5NativeMainLog.info("Saved DVD image path failed validation, clearing configured path: {}", p);
        mp5native::getSettings().backend.isoPath.setValue("");
        mp5native::getSettings().backend.isoVerification.setValue(mp5native::DiscVerificationState::Unknown);
        forcePreLaunchUI = true;
        saveConfigBeforePrelaunch = true;
    }

    mp5native::iso::log_verification_state(
        mp5native::getSettings().backend.isoPath.getValue(),
        mp5native::getSettings().backend.isoVerification.getValue());

    if (mp5native::getSettings().backend.isoPath.getValue().empty()) {
        forcePreLaunchUI = true;
    }
    if (forcePreLaunchUI && mp5native::getSettings().backend.skipPreLaunchUI.getValue()) {
        Mp5NativeMainLog.info("Prelaunch UI was disabled with no usable DVD image, enabling prelaunch UI");
        mp5native::getSettings().backend.skipPreLaunchUI.setValue(false);
        saveConfigBeforePrelaunch = true;
    }
    if (saveConfigBeforePrelaunch) {
        mp5native::config::Save();
    }

    bool keepRunning = true;
    if (!mp5native::getSettings().backend.skipPreLaunchUI) {
        mp5native::ui::push_document(std::make_unique<mp5native::ui::Prelaunch>(), true);

        // pre game launch ui main loop
        keepRunning = launchUILoop();
    }
    // Online Play: the lobby runs before the game boots, so that every machine starts it from the same state
    if (keepRunning && mp5native::getSettings().netplay.mode.getValue() != 0 && getenv("MP5_NET_HOST") == nullptr &&
        getenv("MP5_NET_JOIN") == nullptr) {
        mp5native::ui::push_document(std::make_unique<mp5native::ui::NetplayLobby>());
        keepRunning = launchUILoop([] { return mp5native::ui::netplay_lobby_open(); });
    }
    // The shaders the pipeline cache knows (seeded from the bundled initial_pipeline_cache.db) build in the
    // background while the game boots: a draw whose shader is still compiling waits for it (Aurora get_pipeline),
    // so nothing is skipped, as on the original.
    Mp5NativeMainLog.info("Shaders ready ({} built, {} building in the background)",
                          aurora_get_stats()->createdPipelines, aurora_get_stats()->queuedPipelines);
    {
        if (!keepRunning) {
            fflush(stdout);
            fflush(stderr);
#ifdef MP5_NATIVE_DISCORD
            mp5native::discord::shutdown();
#endif
            mp5native::ui::shutdown();
            aurora_shutdown();
            return 0;
        }
    }

     std::string dvd_path = mp5native::getSettings().backend.isoPath;

    if (dvd_path.empty()) {
        Mp5NativeMainLog.error("No DVD image specified, unable to boot!");
    }
    if (!Mp5Native_IsGameLaunched &&
        mp5native::iso::inspect(dvd_path.c_str(), discInfo) != mp5native::iso::ValidationError::Success)
    {
        Mp5NativeMainLog.error("DVD image failed validation: {}", dvd_path);
    }
    Mp5NativeMainLog.info("Loading DVD image: {}", dvd_path);
    if (!aurora_dvd_open(dvd_path.c_str())) {
        Mp5NativeMainLog.error("Failed to open DVD image: {}", dvd_path);
    }

    Mp5Native_IsGameLaunched = true;

    if (!mp5native::getSettings().backend.wasPresetChosen) {
        mp5native::ui::push_document(std::make_unique<mp5native::ui::PresetWindow>());
    }

    // netplay (MP5_NET_HOST / MP5_NET_JOIN): the lobby, then the host's seed, settings and cards, before anything reads them
    if (!Mp5Net_Boot()) {
        Mp5NativeMainLog.error("Netplay session failed, quitting");
        aurora_shutdown();
        return 1;
    }
    mp5native::version::init();
    LanguageInit();
#ifdef __ANDROID__
    if (getenv("MP5_TEST_NO_REFRESH_PREF") == nullptr) { // test bench: compare with the panel left at its own rate
        AndroidPreferRefreshRate(static_cast<float>(mp5native::getSettings().video.frameRate.getValue()));
    }
#endif
    // After the disc is known: the achievement set is chosen by its hash.
    Mp5Native_RAInit();

    // OSInit();

    // Reset Data
    // TODO PC
    // static mDoRstData sResetData = {0};
    // mDoRst::setResetData(&sResetData);
    // mDoRst::offReset();
    // mDoRst::setLogoScnFlag(0);

    InitializeDol();

    game_main();

    // mp5native::MoviePlayerShutdown();

    fflush(stdout);
    fflush(stderr);

    // Notifies all CVs and causes threads to exit
    OSResetSystem(OS_RESET_SHUTDOWN, 0, 0);
    // OSResetSystem is a stub here: stop MusyX so its audio thread is joined before Aurora shuts SDL down
    // (it read the destroyed SDL stream on exit)
    if (sndIsInstalled()) {
        sndQuit();
    }
    Mp5Native_RAShutdown();

#ifdef MP5_NATIVE_DISCORD
    mp5native::discord::shutdown();
#endif
    mp5native::ui::shutdown();
    aurora_shutdown();

    return 0;
}

/*
 * Mini-game test bench: MP5_TEST_MG=<index into mgDataTbl> (e.g. 1 = m502).
 * Unset, none of this does anything. Set, the game boots as usual and:
 *  - all four pads are scripted (START taps through the title, then random
 *    stick/D-pad/A/B/X/Y/Z/L/R input, never START so the game is not paused);
 *  - the title's jump to the file select is redirected to the instruction
 *    overlay with 4 human players set up as Free Play would, so the mini-game
 *    runs and returns (via instdll) to the title;
 *  - "TESTMG:" lines are logged: start, a heartbeat every 120 frames.
 * tools-side driver: scratchpad mgtest/run_mg.py.
 */
extern "C" {
#include <game/charman.h>
#include <game/flag.h>
#include <game/gamework.h>
#include <game/mgdata.h>
#include <game/object.h>
}

static int tmMg = -2; // -2: env not read yet, -1: disabled
static bool tmCom = false; // MP5_TEST_COM=1: four COM players, no scripted input once started (the Dolphin reference bench does the same)
static bool tmStarted = false;
static u32 tmFrame = 0;
static u32 tmRng = 12345;

static u32 tmRand() {
    tmRng ^= tmRng << 13;
    tmRng ^= tmRng >> 17;
    tmRng ^= tmRng << 5;
    return tmRng;
}

static void tmInit() {
    if (tmMg != -2) {
        return;
    }
    const char* env = getenv("MP5_TEST_MG");
    tmMg = env != nullptr && *env != 0 ? atoi(env) : -1;
    const char* com = getenv("MP5_TEST_COM");
    tmCom = com != nullptr && *com == '1';
    tmRng = 12345 + tmMg * 7919;
}

static void tmSetupPlayers() {
    static const s16 grpTbl[][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 3, 3}};
    int type = mgDataTbl[tmMg].type;
    for (int i = 0; i < GW_PLAYER_MAX; i++) {
        s16 grp = type < 7 ? grpTbl[type][i] : 0;
        GwPlayerConf[i].charNo = GwPlayer[i].charNo = i;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo = i;
        GwPlayerConf[i].dif = GwPlayer[i].dif = 1;
        GwPlayerConf[i].type = GwPlayer[i].comF = tmCom ? GW_TYPE_COM : GW_TYPE_MAN;
        GwPlayerConf[i].grp = GwPlayer[i].grp = grp;
    }
    GwSystem.mgNo = tmMg;
    GwSystem.mgInstF = 1;
    GwSystem.subGameNo = 0;
    _SetFlag(FLAG_INST_MGMODE);
    if (getenv("MP5_TEST_INST") == nullptr) {
        _ClearFlag(FLAG_MGINST_ON); // skip the instruction screens
    } else {
        _SetFlag(FLAG_MGINST_ON); // MP5_TEST_INST: keep them (see the pad hook)
    }
}

// MP5_TEST_BOARD=n (0..6 = w01..w07): the title's jump to the file select goes straight into board n
// with 4 COM players, 10 turns, all mini-game packs, set up the way mdparty does before a party.
extern "C" {
#include <game/board/main.h>
#include <game/board/pause.h>
#include <game/data.h>
#include <game/armem.h>
#include "datadir_enum.h"
}
static int tbBoard = -2; // -2: env not read yet
static bool tbStarted = false;

static OMOVL tbBoardHook(OMOVL ovl) {
    static const OMOVL dll[7] = {DLL_w01dll, DLL_w02dll, DLL_w03dll, DLL_w04dll, DLL_w05dll, DLL_w06dll, DLL_w07dll};
    if (tbBoard == -2) {
        const char* env = getenv("MP5_TEST_BOARD");
        tbBoard = env != nullptr && *env != 0 ? atoi(env) : -1;
    }
    if (tbBoard < 0 || tbBoard > 6 || tbStarted || ovl != DLL_fileseldll) {
        return ovl;
    }
    tbStarted = true;
    for (int i = 0; i < GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].charNo = GwPlayer[i].charNo = i;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo = i;
        GwPlayerConf[i].dif = GwPlayer[i].dif = 1;
        // MP5_TEST_BOARD_PAUSE: player 0 is human so START can open the pause menu
        // MP5_TEST_BOARD_HUMANS=n: players 0..n-1 human (netplay: one per machine, see MP5_TEST_NETMASH)
        const int humans = getenv("MP5_TEST_BOARD_HUMANS") ? atoi(getenv("MP5_TEST_BOARD_HUMANS"))
                                                            : getenv("MP5_TEST_BOARD_PAUSE") != nullptr ? 1 : 0;
        GwPlayerConf[i].type = GwPlayer[i].comF = i < humans ? GW_TYPE_MAN : GW_TYPE_COM;
        GwPlayerConf[i].grp = GwPlayer[i].grp = 0;
        GwPlayer[i].handicap = 0;
    }
    MBPartySaveInit(tbBoard);
    MBPlayerSaveInit(FALSE, TRUE, GW_MGPACK_ALL, 10, 0, 0, 0, 0);
    GwCommon.partyMgPack = GwSystem.mgPack;
    _SetFlag(FLAG_BOARD_STAR_RESET);
    // what mdparty's load process does before the board: common board data to ARAM, char motions, board dir
    static const int dir[7] = {DATA_w01, DATA_w02, DATA_w03, DATA_w04, DATA_w05, DATA_w06, DATA_w07};
    CharDataClose(-1);
    HuDataDirRead(DATA_board);
    HuAR_MRAMtoARAM(DATA_board);
    while (HuARDMACheck()) {
    }
    HuDataDirClose(DATA_board);
    for (int i = 0; i < GW_PLAYER_MAX; i++) {
        CharMotionInit(GwPlayerConf[i].charNo);
    }
    HuDataDirRead(dir[tbBoard]);
    printf("TESTBOARD: start board=%d ovl=%d\n", tbBoard, dll[tbBoard]);
    fflush(stdout);
    return dll[tbBoard];
}

// MP5_TEST_OVL=<overlay id> [MP5_TEST_DIR=<data dir id>]: the title's jump to the file select goes straight to that
// overlay (e.g. DLL_sdroomdll with DATA_sdroom), after reading the data dir the way the mode select does.
static bool tbOvlDone = false;
static OMOVL tbOvlHook(OMOVL ovl) {
    bool& done = tbOvlDone;
    const char* env = getenv("MP5_TEST_OVL");
    if (env == nullptr || *env == 0 || done || ovl != DLL_fileseldll) {
        return ovl;
    }
    done = true;
    if (const char* dir = getenv("MP5_TEST_DIR")) {
        HuDataDirRead(atoi(dir));
    }
    printf("TESTOVL: start ovl=%d\n", atoi(env));
    fflush(stdout);
    return (OMOVL)atoi(env);
}

// test bench: MP5_TEST_AUDIODUMP=<file> writes the mixed output as raw s16 PCM (rate/channels printed once),
// for comparing the sound with Dolphin's audio dump. The output callback is msm's server (msmsys.c), which
// frees the finished sound effects: the dump takes it over after msmSysInit set it, and chains to it.
extern "C" volatile uint64_t Mp5Test_AudioFrames = 0; // frames written so far (audio.c's voice trace)
extern "C" void msmSysPCCycle(int16_t* output, size_t frames, uint32_t rate, uint32_t channels);
static void tbAudioDumpArm() {
    static const char* path = getenv("MP5_TEST_AUDIODUMP");
    static FILE* file = path != nullptr ? fopen(path, "wb") : nullptr;
    if (file == nullptr) {
        return;
    }
    sndPCSetOutputCallback([](int16_t* output, size_t frames, uint32_t rate, uint32_t channels) {
        msmSysPCCycle(output, frames, rate, channels);
        static bool told = false;
        if (!told) {
            printf("AUDIODUMP: rate %u channels %u\n", (unsigned)rate, (unsigned)channels);
            fflush(stdout);
            told = true;
        }
        fwrite(output, sizeof(int16_t) * channels, frames, file);
        Mp5Test_AudioFrames += frames;
    });
}

extern "C" OMOVL Mp5Test_OvlCallHook(OMOVL ovl) {
    tbAudioDumpArm();
    ovl = tbOvlHook(ovl);
    ovl = tbBoardHook(ovl);
    tmInit();
    if (tmMg < 0) {
        return ovl;
    }
    // the boot overlay blanks charNo right after its omOvlCall returns, so set the players up again for the mini-game itself
    if (tmStarted && ovl == (OMOVL)mgDataTbl[tmMg].ovl) {
        tmSetupPlayers();
    }
    // every title -> file select call goes to the mini-game again, so it loops like the Dolphin bench (gecko.py)
    if (ovl != DLL_fileseldll) {
        return ovl;
    }
    tmStarted = true;
    tmSetupPlayers();
    // as mdmini (Mini-Game mode) does before a game: the players' motions go to ARAM (the Dolphin bench does the same)
    CharDataClose(-1);
    for (int i = 0; i < GW_PLAYER_MAX; i++) {
        CharMotionInit(GwPlayerConf[i].charNo);
    }
    printf("TESTMG: start mg=%d mgovl=%d bootovl=%d instovl=%d\n", tmMg, mgDataTbl[tmMg].ovl, DLL_bootdll, DLL_instdll);
    fflush(stdout);
    return DLL_instdll;
}

extern "C" u32* Mp5Test_FrandSeed(void);
extern "C" {
#include <game/main.h>
}

extern "C" void Mp5Test_PadHook(PADStatus* st) {
    tbAudioDumpArm(); // every frame: a sound re-init (an overlay's) clears the output callback
    tmInit();
    // MP5_TEST_SEED=<n>: the game's RNG starts from n (else from the clock), so two runs can be compared;
    // "TESTSEED:" lines every 600 ticks show where it is
    static int seedState = 0;
    if (seedState == 0) {
        const char* seed = getenv("MP5_TEST_SEED");
        seedState = seed != nullptr && *seed != 0 ? 1 : -1;
        if (seedState > 0) {
            *Mp5Test_FrandSeed() = (u32)strtoul(seed, nullptr, 0);
        }
    }
    if (seedState > 0 && GlobalCounter % 600 == 0) {
        printf("TESTSEED: tick %u seed %08x\n", GlobalCounter, *Mp5Test_FrandSeed());
        fflush(stdout);
    }
    if (tmMg < 0) {
        // board test: press START/A on the title until the board starts, then leave the pads to the COM players
        static u32 tbFrame = 0;
        if (tbBoard == -2) {
            tbBoardHook((OMOVL)-1); // reads MP5_TEST_BOARD only
        }
        // boot blanks GwPlayerConf[].charNo right after its (redirected) omOvlCall, before the board loads: put them back
        // same for MP5_TEST_OVL: sub-modes such as mgfree expect the characters the parent menu picked
        if (((tbBoard >= 0 && tbStarted) || tbOvlDone) && tbFrame < 100000 && GwPlayerConf[0].charNo == CHARNO_NONE) {
            for (int i = 0; i < GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].charNo = GwPlayer[i].charNo = i;
                if (i != 0 && getenv("MP5_TEST_COM") != nullptr) { // players 1-3 COM; pad 0 stays the random human (all-COM messages never advance)
                    GwPlayerConf[i].type = GwPlayer[i].comF = GW_TYPE_COM;
                    GwPlayerConf[i].dif = GwPlayer[i].dif = 1;
                }
            }
            tbFrame = 100000;
        }
        // MP5_TEST_PADALL=1: keep tapping START+A in every overlay (menus walk themselves), like the Dolphin bench's pad code
        if ((tbBoard >= 0 && !tbStarted) || (getenv("MP5_TEST_OVL") != nullptr && !tbOvlDone) ||
            getenv("MP5_TEST_PADALL") != nullptr) {
            memset(st, 0, sizeof(PADStatus) * 4);
            if ((++tbFrame / 20) % 2) {
                st[0].button = PAD_BUTTON_START | PAD_BUTTON_A;
            }
            // MP5_TEST_PADALL=2: the Dolphin bench's MASH (voiceref.py): a real Party game, pad 0 taps A (START only
            // on the title, so the board's pause menu stays shut), pads 1-3 unplugged so those players are COM
            const char* padAll = getenv("MP5_TEST_PADALL");
            if (padAll != nullptr && atoi(padAll) == 2) {
                if (omcurovl != DLL_bootdll) {
                    st[0].button &= ~PAD_BUTTON_START;
                }
                for (int i = 1; i < 4; i++) {
                    st[i].err = PAD_ERR_NO_CONTROLLER;
                }
            }
        }
        // MP5_TEST_NETMASH=1: in the board party, pad 0 taps A, A, A, A, B (messages, dice, mini-games) and START on the
        // instruction screen, which waits for it; in netplay each machine's pad 0 is its own player
        if (tbBoard >= 0 && tbStarted && getenv("MP5_TEST_NETMASH") != nullptr) {
            static u32 nmFrame = 0;
            st[0].err = PAD_ERR_NONE;
            if (++nmFrame % 40 < 3) {
                // every 5th press B (odd: a too-costly capsule takes 2 presses, select + dismiss): leaves a capsule menu ("Dice Block") when the chosen capsule costs too much
                st[0].button |= omcurovl == DLL_instdll ? PAD_BUTTON_START
                                : (nmFrame / 40) % 5 == 4      ? PAD_BUTTON_B
                                                               : PAD_BUTTON_A;
            }
        }
        // MP5_TEST_BOARD_PAUSE=n: pad 0 plugged in and tapping A (messages, dice) until n frames into the board,
        // then tapping START until the pause menu opens, then A once into Change Settings
        if (tbBoard >= 0 && tbStarted && getenv("MP5_TEST_BOARD_PAUSE") != nullptr) {
            static u32 bpFrame = 0;
            static bool bpOpened = false;
            const u32 at = (u32)atoi(getenv("MP5_TEST_BOARD_PAUSE"));
            st[0].err = PAD_ERR_NONE;
            static u32 bpOpenFrame = 0;
            if (!bpOpened && MBPauseProcCheck()) {
                bpOpened = true;
                bpOpenFrame = bpFrame;
            }
            // then A once: Change Settings (the option capsules)
            if (bpOpened && bpFrame >= bpOpenFrame + 120 && bpFrame < bpOpenFrame + 123) {
                st[0].button |= PAD_BUTTON_A;
            }
            if (++bpFrame < at) {
                if (bpFrame % 40 < 3) {
                    st[0].button |= PAD_BUTTON_A;
                }
            } else if (!bpOpened) {
                // keep advancing messages while trying START (the menu won't open over a message)
                st[0].button |= bpFrame % 60 < 3 ? PAD_BUTTON_START : bpFrame % 60 >= 30 && bpFrame % 60 < 33 ? PAD_BUTTON_A : 0;
            }
        }
        // MP5_TEST_OVL: once in the overlay, random D-pad/A/B/X/Y/L/R presses on all pads (never START) and a heartbeat
        if (tbOvlDone && getenv("MP5_TEST_OVL") != nullptr) {
            static u32 ovFrame = 0;
            if (++ovFrame % 120 == 0) {
                printf("TESTOVL: frame %u\n", ovFrame);
                fflush(stdout);
            }
            // MP5_TEST_OVL_STEP=<frames>: instead of random presses, pad 0 taps DOWN once every <frames> frames
            // (a menu entry stays selected long enough for what it shows, e.g. the free-play mini-game preview)
            // MP5_TEST_OVL_KEYS=<letters>: the presses to cycle through instead of DOWN (D U L R d-pad, P = R trigger, A)
            if (const char* step = getenv("MP5_TEST_OVL_STEP")) {
                const u32 n = (u32)atoi(step);
                const char* keys = getenv("MP5_TEST_OVL_KEYS");
                memset(st, 0, sizeof(PADStatus) * 4);
                if (n != 0 && ovFrame % n < 4 && ovFrame > n) {
                    const char k = (keys != nullptr && *keys) ? keys[(ovFrame / n - 1) % strlen(keys)] : 'D';
                    st[0].button = k == 'U' ? PAD_BUTTON_UP : k == 'L' ? PAD_BUTTON_LEFT : k == 'R' ? PAD_BUTTON_RIGHT
                                 : k == 'P' ? PAD_TRIGGER_R : k == 'A' ? PAD_BUTTON_A : PAD_BUTTON_DOWN;
                }
                return;
            }
            for (int i = 0; i < 4; i++) {
                u32 r = tmRand();
                memset(&st[i], 0, sizeof(st[i]));
                if (i != 0 && getenv("MP5_TEST_COM") != nullptr) {
                    continue;
                }
                if ((ovFrame / 15) % 2 == 0) {
                    st[i].button = (u16)((r & 0x300) ? 0 : (1 << ((r >> 4) & 3)));
                } else {
                    if (!(r & 0x30)) st[i].button |= PAD_BUTTON_A;
                    if (getenv("MP5_TEST_B") != nullptr && !(r & 0xC0)) st[i].button |= PAD_BUTTON_B; // B backs out of the menus, off by default
                    if (!((r >> 8) & 0x7)) st[i].button |= PAD_BUTTON_X;
                    if (!((r >> 11) & 0x7)) st[i].button |= PAD_BUTTON_Y;
                    if (!((r >> 14) & 0xF)) st[i].button |= PAD_TRIGGER_L;
                    if (!((r >> 18) & 0xF)) st[i].button |= PAD_TRIGGER_R;
                }
            }
        }
        return;
    }
    tmFrame++;
    if (tmFrame % 120 == 0) {
        printf("TESTMG: frame %u started=%d\n", tmFrame, tmStarted);
        fflush(stdout);
    }
    static u16 held[4];
    static s8 stk[4][4];
    for (int i = 0; i < 4; i++) {
        memset(&st[i], 0, sizeof(st[i]));
        if (!tmStarted || omcurovl == DLL_bootdll) { // back on the title after the mini-game: start it again
            if (i == 0 && (tmFrame / 20) % 2) {
                st[i].button = PAD_BUTTON_START | PAD_BUTTON_A;
            }
            continue;
        }
        // MP5_TEST_INST=<frames>: on the instruction screen pad 0 taps the R trigger (next page) every <frames>
        // frames and nothing else, so the screen stays up and its pages can be captured
        if (const char* inst = getenv("MP5_TEST_INST"); inst != nullptr && omcurovl == DLL_instdll) {
            const u32 n = (u32)atoi(inst);
            if (i == 0 && n != 0 && tmFrame % n < 4) {
                st[i].button = PAD_TRIGGER_R;
                st[i].triggerRight = 255;
            }
            continue;
        }
        if (tmCom) {
            continue;
        }
        if (tmFrame % 24 == (u32)i * 6) { // new stick/D-pad pose every 24 frames
            u32 r = tmRand();
            held[i] = (r & 1) ? (u16)(1 << ((r >> 1) & 3)) : 0;
            stk[i][0] = (r & 0x100) ? (s8)((r >> 9) % 141) - 70 : 0;
            stk[i][1] = (r & 0x200) ? (s8)((r >> 17) % 141) - 70 : 0;
            stk[i][2] = (s8)((r >> 3) % 100) - 50;
            stk[i][3] = (s8)((r >> 11) % 100) - 50;
        }
        u32 r = tmRand();
        u16 b = held[i];
        if (!(r & 0x30)) b |= PAD_BUTTON_A;
        if (!(r & 0xC0)) b |= PAD_BUTTON_B;
        if (!((r >> 8) & 0x7)) b |= PAD_BUTTON_X;
        if (!((r >> 11) & 0x7)) b |= PAD_BUTTON_Y;
        if (!((r >> 14) & 0xF)) b |= PAD_TRIGGER_Z;
        if (!((r >> 18) & 0xF)) b |= PAD_TRIGGER_L;
        if (!((r >> 22) & 0xF)) b |= PAD_TRIGGER_R;
        st[i].button = b;
        st[i].stickX = stk[i][0]; st[i].stickY = stk[i][1]; st[i].substickX = stk[i][2]; st[i].substickY = stk[i][3];
        st[i].triggerLeft = (b & PAD_TRIGGER_L) ? 255 : 0;
        st[i].triggerRight = (b & PAD_TRIGGER_R) ? 255 : 0;
    }
}
