// Credits: TwilitRealm

#include "imgui/ImGuiEngine.hpp"
#include "iso_validate.hpp"
#include "ui/ui.hpp"

#include "mp5native_version.h"
#include "ui/menu_bar.hpp"
#include "ui/overlay.hpp"
#include "ui/prelaunch.hpp"
#include "ui/preset.hpp"

#include <SDL3/SDL_filesystem.h>
#include "port/display_select.h"
#include <aurora/aurora.h>
#include <aurora/event.h>
#include <dolphin/gx/GXAurora.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include <game/disp.h>
#include <port/config.hpp>
#include <port/dolassets.h>
#include <port/main.h>
#include <port/settings.h>
#include <port/port_version.h>

#include <aurora/dvd.h>
#include <aurora/lib/logging.hpp>
#include <game/card.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" int game_main();

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

bool launchUILoop() {
    while (Mp5Native_IsRunning && !Mp5Native_IsGameLaunched) {
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

    const auto result = SDL_GetPrefPath("zeranemesis", "MP5 Native");
    if (!result) {
        Mp5NativeMainLog.error("Unable to get PrefPath: {}", SDL_GetError());
    }

    return reinterpret_cast<const char8_t*>(result);
}

static void EnsureInitialPipelineCache(const std::filesystem::path& configDir) {
    if (configDir.empty()) {
        return;
    }

    const std::filesystem::path pipelineCachePath = configDir / "pipeline_cache.db";
    if (std::filesystem::exists(pipelineCachePath)) {
        return;
    }

    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        Mp5NativeMainLog.error("Unable to resolve base path while seeding pipeline cache: {}", SDL_GetError());
        return;
    }

    const std::filesystem::path initialPipelineCachePath =
        std::filesystem::path(basePath) / "initial_pipeline_cache.db";
    if (!std::filesystem::exists(initialPipelineCachePath)) {
        Mp5NativeMainLog.error("No bundled initial pipeline cache found at '{}'", initialPipelineCachePath.string());
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(configDir, ec);
    if (ec) {
        Mp5NativeMainLog.error("Failed to create config directory '{}' for pipeline cache: {}",
                     configDir.string(), ec.message());
        return;
    }

    std::filesystem::copy_file(initialPipelineCachePath, pipelineCachePath, std::filesystem::copy_options::none, ec);
    if (ec) {
        Mp5NativeMainLog.error("Failed to seed pipeline cache from '{}' to '{}': {}",
                     initialPipelineCachePath.string(), pipelineCachePath.string(), ec.message());
        return;
    }

    Mp5NativeMainLog.info("Seeded pipeline cache from '{}'", initialPipelineCachePath.string());
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

extern "C" int port_main(int argc, char* argv[]) {
    Mp5InstallCrashReport();
    setvbuf(stdout, nullptr, _IONBF, 0); // unbuffered so logs survive a kill/crash
    // On iOS, when connected to an external monitor, SDLUIKitSceneDelegate scene:willConnectToSession:
    // can call our main function again. Explicitly guard against this reinitialization.
    if (mainCalled) {
        return 0;
    }
    mainCalled = true;

    mp5native::registerSettings();
    mp5native::config::FinishRegistration();

    Mp5Native_ConfigPath = calculate_config_path();

    mp5native::config::LoadFromUserPreferences();
    // test bench: MP5_TEST_ASPECT=lock|stretch|wide overrides the aspect settings for this run, unsaved
    if (const char* aspect = getenv("MP5_TEST_ASPECT")) {
        auto& video = mp5native::getSettings().video;
        video.lockAspectRatio.setOverrideValue(strcmp(aspect, "lock") == 0);
        video.widescreen.setOverrideValue(strcmp(aspect, "wide") == 0);
        video.hudCentered.setOverrideValue(getenv("MP5_TEST_HUD_CENTERED") != nullptr);
    }
    // test bench: MP5_TEST_FPS=<cap> runs the high frame rate path (src/port/frame_interp.cpp) with vsync off, unsaved
    if (getenv("MP5_TEST_FPS") != nullptr) {
        mp5native::getSettings().video.enableVsync.setOverrideValue(false);
    }
    // test bench: MP5_TEST_BACKEND=vulkan|d3d12|... picks the graphics backend for this run, unsaved
    if (const char* backend = getenv("MP5_TEST_BACKEND")) {
        mp5native::getSettings().backend.graphicsBackend.setOverrideValue(std::string(backend));
    }
    EnsureInitialPipelineCache(Mp5Native_ConfigPath);
    // TODO: How to handle this?
    //PADSetDefaultMapping(&defaultPadMapping, PAD_TYPE_STANDARD);

    {
        const auto configPathString = Mp5Native_ConfigPath.u8string();
        AuroraConfig config{};
        config.appName = "MP5 Native";
        config.userPath = reinterpret_cast<const char*>(configPathString.c_str());
        config.vsync = mp5native::getSettings().video.enableVsync;
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

    if (!mp5native::getSettings().backend.skipPreLaunchUI) {
        mp5native::ui::push_document(std::make_unique<mp5native::ui::Prelaunch>(), true);

        // pre game launch ui main loop
        if (!launchUILoop()) {
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

    mp5native::version::init();
    LanguageInit();

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
    _ClearFlag(FLAG_MGINST_ON); // skip the instruction screens
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
        GwPlayerConf[i].type = GwPlayer[i].comF = (i == 0 && getenv("MP5_TEST_BOARD_PAUSE") != nullptr) ? GW_TYPE_MAN : GW_TYPE_COM;
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

extern "C" OMOVL Mp5Test_OvlCallHook(OMOVL ovl) {
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
    if (tmStarted || ovl != DLL_fileseldll) {
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

extern "C" void Mp5Test_PadHook(PADStatus* st) {
    tmInit();
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
        if ((tbBoard >= 0 && !tbStarted) || (getenv("MP5_TEST_OVL") != nullptr && !tbOvlDone)) {
            memset(st, 0, sizeof(PADStatus) * 4);
            if ((++tbFrame / 20) % 2) {
                st[0].button = PAD_BUTTON_START | PAD_BUTTON_A;
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
        if (!tmStarted) {
            if (i == 0 && (tmFrame / 20) % 2) {
                st[i].button = PAD_BUTTON_START | PAD_BUTTON_A;
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
