// Credits: TwilitRealm

#include "settings.hpp"

#include "aurora/gfx.h"
#include "bool_button.hpp"
#include "controller_config.hpp"
#include "port/config.hpp"
#include "../imgui/ImGuiEngine.hpp"
#include "../file_select.hpp"
#include "graphics_tuner.hpp"
#include "menu_bar.hpp"
#include "number_button.hpp"
#include "port/display_select.h"
#include "pane.hpp"
#include "prelaunch.hpp"
#include "string_button.hpp"
#include "ui.hpp"
#include "port/retroachievements.h"
#include "port/cheats.h"

#include <algorithm>
#include <fmt/format.h>
#include <vector>
#include <aurora/aurora.h>
#include <game/disp.h>
#include <gx/GXAurora.h>
#include <vi.h>

namespace {
// The RetroAchievements login fields. Typed in the settings and handed to
// rcheevos on "Log In"; the password is never saved and is cleared as soon
// as it has been sent.
Rml::String s_raPendingUser;
Rml::String s_raPendingPassword;
} // namespace

namespace mp5native::ui {
namespace {

    constexpr std::array kLanguageNames = {
        "English",
        "German",
        "French",
        "Spanish",
        "Italian",
    };

    constexpr std::array kCardFileTypes = {
        "Card Image",
        "GCI Folder",
    };

    constexpr std::array kFpsOverlayCornerNames = {
        "Top Left",
        "Top Right",
        "Bottom Left",
        "Bottom Right",
    };

    constexpr std::array kGyroInputModeLabels = {
        "Sensor",
        "Mouse",
    };

    bool try_parse_backend(std::string_view backend, AuroraBackend &outBackend)
    {
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

    std::string_view backend_name(AuroraBackend backend)
    {
        switch (backend) {
            default:
                return "Auto";
            case BACKEND_D3D12:
                return "D3D12";
            case BACKEND_D3D11:
                return "D3D11";
            case BACKEND_METAL:
                return "Metal";
            case BACKEND_VULKAN:
                return "Vulkan";
            case BACKEND_OPENGL:
                return "OpenGL";
            case BACKEND_OPENGLES:
                return "OpenGL ES";
            case BACKEND_WEBGPU:
                return "WebGPU";
            case BACKEND_NULL:
                return "Null";
        }
    }

    std::string_view backend_id(AuroraBackend backend)
    {
        switch (backend) {
            default:
                return "auto";
            case BACKEND_D3D12:
                return "d3d12";
            case BACKEND_D3D11:
                return "d3d11";
            case BACKEND_METAL:
                return "metal";
            case BACKEND_VULKAN:
                return "vulkan";
            case BACKEND_OPENGL:
                return "opengl";
            case BACKEND_OPENGLES:
                return "opengles";
            case BACKEND_WEBGPU:
                return "webgpu";
            case BACKEND_NULL:
                return "null";
        }
    }

    std::vector<AuroraBackend> available_backends()
    {
        std::vector<AuroraBackend> backends;
        backends.emplace_back(BACKEND_AUTO);
        size_t backendCount = 0;
        const AuroraBackend *raw = aurora_get_available_backends(&backendCount);
        for (size_t i = 0; i < backendCount; ++i) {
            // Do not expose NULL or D3D11
            if (raw[i] != BACKEND_NULL && raw[i] != BACKEND_D3D11) {
                backends.emplace_back(raw[i]);
            }
        }
        return backends;
    }

    AuroraBackend configured_backend()
    {
        AuroraBackend configuredBackend = BACKEND_AUTO;
        const auto configuredId = getSettings().backend.graphicsBackend.getValue();
        if (!try_parse_backend(configuredId, configuredBackend)) {
            configuredBackend = BACKEND_AUTO;
        }
        return configuredBackend;
    }

    // TODO PC
    void reset_for_speedrun_mode()
    {
        getSettings().game.infiniteCoins.setValue(false);
        getSettings().game.enableCheatCodes.setValue(false);

        getSettings().game.enableTurboKeybind.setValue(false);
    }

    const Rml::String kInternalResolutionHelpText = "Configure the resolution used for rendering the game. Higher values are more demanding on "
                                                    "your graphics hardware.";
    const Rml::String kShadowResolutionHelpText = "Configure the shadow-map resolution. Higher values improve shadow quality but increase GPU "
                                                  "and memory usage.";
    const Rml::String kBloomHelpText = "Configure the post-processing bloom effect. Classic uses the original bloom pass; Dusk uses "
                                       "a higher-quality bloom pass.";
    const Rml::String kBloomBrightnessHelpText = "Configure bloom intensity. Higher values make bright areas glow more strongly.";
    const Rml::String kUnlockFramerateHelpText = "Uses inter-frame interpolation to enable higher frame rates.<br/><br/>May introduce minor "
                                                 "visual artifacts or animation glitches.";

    int float_setting_percent(ConfigVar<float> &var)
    {
        return static_cast<int>(var.getValue() * 100.0f + 0.5f);
    }

    struct ConfigBoolProps {
        Rml::String key;
        Rml::String icon;
        Rml::String helpText;
        std::function<void(bool)> onChange;
        std::function<bool()> isDisabled;
    };

    SelectButton &config_bool_select(Pane &leftPane, Pane &rightPane, ConfigVar<bool> &var, ConfigBoolProps props)
    {
        auto &button = leftPane.add_child<BoolButton>(BoolButton::Props {
            .key = std::move(props.key),
            .icon = std::move(props.icon),
            .getValue = [&var] { return var.getValue(); },
            .setValue =
                [&var, callback = std::move(props.onChange)](bool value) {
                    if (value == var.getValue()) {
                        return;
                    }
                    var.setValue(value);
                    config::Save();
                    if (callback) {
                        callback(value);
                    }
                },
            .isDisabled = std::move(props.isDisabled),
            .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
        });
        leftPane.register_control(button, rightPane, [helpText = std::move(props.helpText)](Pane &pane) {
            pane.clear();
            pane.add_rml(helpText);
        });
        return button;
    }

    SelectButton &config_percent_select(Pane &leftPane, Pane &rightPane, ConfigVar<float> &var, Rml::String key, Rml::String helpText, int min,
        int max, int step = 5, std::function<bool()> isDisabled = {})
    {
        auto &button = leftPane.add_child<NumberButton>(NumberButton::Props {
            .key = std::move(key),
            .getValue = [&var] { return float_setting_percent(var); },
            .setValue =
                [&var, min, max](int value) {
                    var.setValue(std::clamp(value, min, max) / 100.0f);
                    config::Save();
                },
            .isDisabled = std::move(isDisabled),
            .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
            .min = min,
            .max = max,
            .step = step,
            .suffix = "%",
        });
        leftPane.register_control(button, rightPane, [helpText = std::move(helpText)](Pane &pane) {
            pane.clear();
            pane.add_text(helpText);
        });
        return button;
    }

    template <typename T>
    void graphics_tuner_control(Window &window, Pane &leftPane, Pane &rightPane, ConfigVar<T> &var, const GraphicsTunerProps &props, bool prelaunch)
    {
        leftPane.register_control(leftPane
                                      .add_select_button({
                                          .key = props.title,
                                          .getValue =
                                              [&var, option = props.option] {
                                                  if constexpr (std::is_same_v<T, float>) {
                                                      return format_graphics_setting_value(option, float_setting_percent(var));
                                                  }
                                                  else {
                                                      return format_graphics_setting_value(option, static_cast<int>(var.getValue()));
                                                  }
                                              },
                                          .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
                                          .submit = false,
                                      })
                                      .on_nav_command([&window, props, prelaunch](Rml::Event &, NavCommand cmd) {
                                          if (cmd == NavCommand::Confirm || cmd == NavCommand::Left || cmd == NavCommand::Right) {
                                              window.push(std::make_unique<GraphicsTuner>(props, prelaunch));
                                              return true;
                                          }
                                          return false;
                                      }),
            rightPane, [helpText = props.helpText](Pane &pane) {
                pane.clear();
                pane.add_text(helpText);
            });
    }

} // namespace

SettingsWindow::SettingsWindow(bool prelaunch)
    : mPrelaunch(prelaunch)
{
    if (prelaunch) {
        mSuppressNavFallback = true;
        add_tab("Prelaunch", [this](Rml::Element *content) {
            auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
            auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

            leftPane.register_control(leftPane
                                          .add_select_button({
                                              .key = "Disc Image",
                                              .getValue =
                                                  [] {
                                                      const auto &path = prelaunch_state().configuredDiscPath;
                                                      std::string display;
                                                      if (path.empty()) {
                                                          display = "(none)";
                                                      }
                                                      else {
                                                          display = display_name_for_path(path);
                                                          if (display.empty()) {
                                                              display = path;
                                                          }
                                                      }
                                                      return display;
                                                  },
                                              .isModified =
                                                  [] {
                                                      const auto &state = prelaunch_state();
                                                      const auto &active = state.activeDiscPath;
                                                      return !active.empty() && state.configuredDiscPath != active;
                                                  },
                                          })
                                          .on_pressed([] { open_iso_picker(); }),
                rightPane, [](Pane &pane) {
                    pane.add_rml("Set the disc image that MP5 Native uses to launch the game.<br/><br/>"
                                 "Changes require a restart.");
                });
            leftPane.register_control(leftPane.add_select_button({
                                          .key = "Language",
                                          .getValue =
                                              [] {
                                                  const auto &state = prelaunch_state();
                                                  if (!state.configuredDiscCanLaunch || !state.configuredDiscInfo.isPal) {
                                                      return kLanguageNames[0];
                                                  }
                                                  const u8 idx = static_cast<u8>(getSettings().game.language.getValue());
                                                  return kLanguageNames[idx];
                                              },
                                          .isDisabled =
                                              [] {
                                                  const auto &state = prelaunch_state();
                                                  return !state.configuredDiscCanLaunch || !state.configuredDiscInfo.isPal;
                                              },
                                          .isModified = [] { return getSettings().game.language.getValue() != prelaunch_state().initialLanguage; },
                                      }),
                rightPane, [](Pane &pane) {
                    for (int i = 0; i < kLanguageNames.size(); i++) {
                        pane.add_button({
                                            .text = kLanguageNames[i],
                                            .isSelected = [i] { return getSettings().game.language.getValue() == static_cast<GameLanguage>(i); },
                                        })
                            .on_pressed([i] {
                                // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                                getSettings().game.language.setValue(static_cast<GameLanguage>(i));
                                config::Save();
                            });
                    }
                    pane.add_rml("<br/>Changes require a restart.");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "Graphics Backend",
                    .getValue = [] { return Rml::String { backend_name(configured_backend()) }; },
                    .isModified = [] { return getSettings().backend.graphicsBackend.getValue() != prelaunch_state().initialGraphicsBackend; },
                }),
                rightPane, [](Pane &pane) {
                    const auto availableBackends = available_backends();
                    for (const auto backend : availableBackends) {
                        pane.add_button({
                                            .text = Rml::String { backend_name(backend) },
                                            .isSelected = [backend] { return configured_backend() == backend; },
                                        })
                            .on_pressed([backend] {
                                // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                                getSettings().backend.graphicsBackend.setValue(std::string { backend_id(backend) });
                                config::Save();
                            });
                    }
                    pane.add_rml("<br/>Changes require a restart.");
                });
            leftPane.register_control(
                leftPane.add_select_button({
                    .key = "Save File Type",
                    .getValue = [] { return kCardFileTypes[getSettings().backend.cardFileType.getValue()]; },
                    .isModified = [] { return getSettings().backend.cardFileType.getValue() != prelaunch_state().initialCardFileType; },
                }),
                rightPane, [](Pane &pane) {
                    for (int i = 0; i < kCardFileTypes.size(); i++) {
                        pane.add_button({
                                            .text = kCardFileTypes[i],
                                            .isSelected = [i] { return getSettings().backend.cardFileType.getValue() == i; },
                                        })
                            .on_pressed([i] {
                                // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                                getSettings().backend.cardFileType.setValue(i);
                                config::Save();
                            });
                    }
                });
        });
    }

    add_tab("Video", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("Display");

        leftPane.register_control(leftPane.add_button("Toggle Fullscreen").on_pressed([] {
            // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        }),
            rightPane, [](Pane &pane) { pane.clear(); });
        leftPane.register_control(leftPane.add_button("Restore Default Window Size").on_pressed([] {
            // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
            getSettings().video.enableFullscreen.setValue(false);
            VISetWindowFullscreen(false);
            VISetWindowSize(HU_FB_WIDTH * 2, HU_FB_HEIGHT * 2);
            VICenterWindow();
        }),
            rightPane, [](Pane &pane) { pane.clear(); });
        config_bool_select(leftPane, rightPane, getSettings().video.enableVsync,
            {
                .key = "Enable VSync",
                .helpText = "Synchronizes the frame rate to your monitor's refresh rate.",
                .onChange = [](bool value) { aurora_enable_vsync(value); },
            });
        leftPane.register_control(leftPane.add_select_button({
                                      .key = "Display",
                                      .getValue =
                                          [] {
                                              const int d = getSettings().video.display.getValue();
                                              return d < 0 ? Rml::String { "Default" } : Rml::String { display::name(d) };
                                          },
                                      .isModified =
                                          [] {
                                              const auto &v = getSettings().video.display;
                                              return v.getValue() != v.getDefaultValue();
                                          },
                                  }),
            rightPane, [](Pane &pane) {
                for (int d = -1; d < display::count(); d++) {
                    pane.add_button({
                                        .text = d < 0 ? Rml::String { "Default" } : Rml::String { display::name(d) },
                                        .isSelected = [d] { return getSettings().video.display.getValue() == d; },
                                    })
                        .on_pressed([d] {
                            getSettings().video.display.setValue(d);
                            config::Save();
                            display::move_window(d);
                        });
                }
                pane.add_rml("<br/>Monitor the game window opens on. The frame rate cap follows this monitor's refresh rate.");
            });
        leftPane.register_control(leftPane.add_select_button({
                                      .key = "Frame Rate",
                                      .getValue =
                                          [] {
                                              const int fps = getSettings().video.frameRate.getValue();
                                              return fps == 60 ? Rml::String { "60 (Original)" } : fps <= 0 ? Rml::String { "Display" } : std::to_string(fps);
                                          },
                                      .isModified =
                                          [] {
                                              const auto &v = getSettings().video.frameRate;
                                              return v.getValue() != v.getDefaultValue();
                                          },
                                  }),
            rightPane, [](Pane &pane) {
                for (int fps : { 60, 120, 144, 240, 360, 0 }) {
                    pane.add_button({
                                        .text = fps == 60 ? Rml::String { "60 (Original)" } : fps == 0 ? Rml::String { "Display" } : std::to_string(fps),
                                        .isSelected = [fps] { return getSettings().video.frameRate.getValue() == fps; },
                                    })
                        .on_pressed([fps] {
                            getSettings().video.frameRate.setValue(fps);
                            config::Save();
                        });
                }
                pane.add_rml("<br/>The game still runs at 60 ticks per second; frames in between are interpolated. "
                             "Limited to the display's refresh rate. Display follows the refresh rate.");
            });
        config_bool_select(leftPane, rightPane, getSettings().video.frameInterpolation,
            {
                .key = "Frame Interpolation",
                .helpText = "Draw the frames between the game's 60 ticks per second with interpolated cameras, models and HUD. "
                            "Off keeps the original 60 fps whatever the frame rate setting.",
            });
        config_bool_select(leftPane, rightPane, getSettings().video.lockAspectRatio,
            {
                .key = "Lock 4:3 Aspect Ratio",
                .helpText = "Lock the game's aspect ratio to the original.",
                .onChange = [](bool value) { AuroraSetViewportPolicy(value ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH); },
            });
        config_bool_select(leftPane, rightPane, getSettings().video.widescreen,
            {
                .key = "Widescreen",
                .helpText = "Widen the 3D view to the window's aspect ratio and keep the HUD unstretched. Ignored while the 4:3 lock is on.",
            });
        leftPane.register_control(leftPane.add_select_button({
                                      .key = "HUD Layout",
                                      .getValue =
                                          [] {
                                              return Rml::String { getSettings().video.hudCentered.getValue() ? "Centered" : "Adaptive" };
                                          },
                                      .isModified =
                                          [] {
                                              const auto &v = getSettings().video.hudCentered;
                                              return v.getValue() != v.getDefaultValue();
                                          },
                                  }),
            rightPane, [](Pane &pane) {
                for (bool centered : { false, true }) {
                    pane.add_button({
                                        .text = centered ? "Centered" : "Adaptive",
                                        .isSelected = [centered] { return getSettings().video.hudCentered.getValue() == centered; },
                                    })
                        .on_pressed([centered] {
                            getSettings().video.hudCentered.setValue(centered);
                            config::Save();
                        });
                }
                pane.add_rml("<br/>Widescreen only. Adaptive moves HUD elements near the sides of the 4:3 image to the window edges; Centered keeps the whole HUD in the middle 4:3 area.");
            });
        config_bool_select(leftPane, rightPane, getSettings().game.pauseOnFocusLost,
            {
                .key = "Pause on Focus Lost",
                .isDisabled = [] { return IsMobile; },
            });
        leftPane.register_control(leftPane.add_select_button({
                                      .key = "Show FPS Counter",
                                      .getValue =
                                          [] {
                                              if (!getSettings().video.enableFpsOverlay.getValue()) {
                                                  return Rml::String { "Off" };
                                              }
                                              const int idx = getSettings().video.fpsOverlayCorner.getValue();
                                              return Rml::String { kFpsOverlayCornerNames[idx] };
                                          },
                                      .isModified =
                                          [] {
                                              const auto &enable = getSettings().video.enableFpsOverlay;
                                              const auto &corner = getSettings().video.fpsOverlayCorner;
                                              return enable.getValue() != enable.getDefaultValue()
                                                  || (enable.getValue() && corner.getValue() != corner.getDefaultValue());
                                          },
                                  }),
            rightPane, [](Pane &pane) {
                pane.add_button({
                                    .text = "Off",
                                    .isSelected = [] { return !getSettings().video.enableFpsOverlay.getValue(); },
                                })
                    .on_pressed([] {
                        // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                        getSettings().video.enableFpsOverlay.setValue(false);
                        config::Save();
                    });
                for (int i = 0; i < static_cast<int>(kFpsOverlayCornerNames.size()); ++i) {
                    pane
                        .add_button({
                            .text = kFpsOverlayCornerNames[i],
                            .isSelected
                            = [i] { return getSettings().video.enableFpsOverlay.getValue() && getSettings().video.fpsOverlayCorner.getValue() == i; },
                        })
                        .on_pressed([i] {
                            // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                            getSettings().video.enableFpsOverlay.setValue(true);
                            getSettings().video.fpsOverlayCorner.setValue(i);
                            config::Save();
                        });
                }
                pane.add_rml("<br/>Display the current framerate in a corner of the screen while playing.");
            });

        leftPane.add_section("Resolution");
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.internalResolutionScale,
            GraphicsTunerProps {
                .option = GraphicsOption::InternalResolution,
                .title = "Internal Resolution",
                .helpText = kInternalResolutionHelpText,
                .valueMin = 0,
                .valueMax = 12,
                .defaultValue = 0,
            },
            mPrelaunch);
        graphics_tuner_control(*this, leftPane, rightPane, getSettings().game.shadowResolutionMultiplier,
            GraphicsTunerProps {
                .option = GraphicsOption::ShadowResolution,
                .title = "Shadow Resolution",
                .helpText = kShadowResolutionHelpText,
                .valueMin = 1,
                .valueMax = 8,
                .defaultValue = 1,
            },
            mPrelaunch);

        leftPane.add_section("Image Quality");
        const auto int_choice = [&](ConfigVar<int> &var, const char *key, std::vector<int> values, const char *off,
                                    const char *help) {
            leftPane.register_control(leftPane.add_select_button({
                                          .key = key,
                                          .getValue =
                                              [&var, off] {
                                                  const int v = var.getValue();
                                                  return v <= 1 ? Rml::String { off } : fmt::format("{}x", v);
                                              },
                                          .isModified = [&var] { return var.getValue() != var.getDefaultValue(); },
                                      }),
                rightPane, [&var, values, off, help](Pane &pane) {
                    for (int v : values) {
                        pane.add_button({
                                            .text = v <= 1 ? Rml::String { off } : fmt::format("{}x", v),
                                            .isSelected = [&var, v] { return var.getValue() == v; },
                                        })
                            .on_pressed([&var, v] {
                                var.setValue(v);
                                config::Save();
                            });
                    }
                    pane.add_rml(help);
                });
        };
        int_choice(getSettings().game.msaaSamples, "Anti-Aliasing (MSAA)", { 1, 2, 4, 8 }, "Off (Original)",
            "<br/>Smooths the jagged edges of 3D models. Costs graphics card time; leave it off on a modest computer. "
            "Takes effect the next time the game starts.");
        int_choice(getSettings().game.textureAnisotropy, "Texture Filtering", { 1, 2, 4, 8, 16 }, "Off (Original)",
            "<br/>Keeps textures sharp on surfaces seen at an angle, such as the board floor. Lower it on a modest "
            "graphics card. Takes effect the next time the game starts.");

        // leftPane.add_section("Post-Processing");

        // leftPane.add_section("Rendering");
    });

    add_tab("Input", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addOption = [&](const Rml::String &key, ConfigVar<bool> &value, const Rml::String &helpText, std::function<bool()> isDisabled = {}) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                    .isDisabled = std::move(isDisabled),
                });
        };

        leftPane.add_section("Controller");
        leftPane.register_control(
            leftPane.add_button("Configure Controller").on_pressed([this] { push(std::make_unique<ControllerConfigWindow>()); }), rightPane,
            [](Pane &pane) {
                pane.clear();
                pane.add_text("Open controller binding configuration.");
            });
        config_bool_select(leftPane, rightPane, getSettings().game.allowBackgroundInput,
            {
                .key = "Allow Background Input",
                .helpText = "Allow controller input even when the game window is not focused.",
                .onChange = [](bool value) { aurora_set_background_input(value); },
            });

        leftPane.add_section("Tools");
        addOption("Turbo Key", getSettings().game.enableTurboKeybind, "Hold Tab to unlock the FPS, speeding up the game.",
            [] { return getSettings().game.speedrunMode; });
    });

    // add_tab("Audio", [this](Rml::Element *content) {
    //     auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
    //     auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
    //
    //     // TODO: Individual sliders for Main Music, Sub Music, Sound Effects, and Fanfare.
    //     // leftPane.add_section("Volume");
    //     // leftPane.register_control(
    //     //     leftPane.add_child<NumberButton>(NumberButton::Props {
    //     //         .key = "Master Volume",
    //     //         .getValue = [] { return getSettings().audio.masterVolume.getValue(); },
    //     //         .setValue =
    //     //             [](int value) {
    //     //                 getSettings().audio.masterVolume.setValue(value);
    //     //                 config::Save();
    //     //                 // audio::SetMasterVolume(value / 100.f); // TODO PC
    //     //             },
    //     //         .isModified = [] { return getSettings().audio.masterVolume.getValue() != getSettings().audio.masterVolume.getDefaultValue(); },
    //     //         .max = 100,
    //     //         .suffix = "%",
    //     //     }),
    //     //     rightPane, [](Pane &pane) {
    //     //         pane.clear();
    //     //         pane.add_text("Adjusts the volume of all sounds in the game.");
    //     //     });
    //
    //     leftPane.add_section("Effects");
    //     // config_bool_select(leftPane, rightPane, getSettings().audio.enableReverb,
    //     //     {
    //     //         .key = "Enable Reverb",
    //     //         .helpText = "Enables the reverb effect in game audio.",
    //     //         .onChange = [](bool value) {
    //     //             // audio::SetEnableReverb(value); // TODO PC
    //     //         },
    //     //     });
    //     // config_bool_select(leftPane, rightPane, getSettings().audio.enableHrtf,
    //     //     {
    //     //         .key = "Enable Spatial Sound",
    //     //         .helpText = "Emulate surround sound via HRTF. Recommended only for use with headphones!",
    //     //         .onChange = [](bool value) {
    //     //             // audio::EnableHrtf = value; // TODO PC
    //     //         },
    //     //     });
    //     // config_bool_select(leftPane, rightPane, getSettings().audio.menuSounds,
    //     //     {
    //     //         .key = "MP5 Native Menu Sounds",
    //     //         .helpText = "Play sound effects when navigating the MP5 Native menu.",
    //     //     });
    //
    //     leftPane.add_section("Tweaks");
    // });

    add_tab("Gameplay", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.register_control(leftPane.add_select_button({
                                      .key = "Loading Speed",
                                      .getValue =
                                          [] {
                                              return getSettings().game.loadingSpeed.getValue() == 1 ? Rml::String { "GameCube" }
                                                                                                    : Rml::String { "Fast" };
                                          },
                                      .isModified =
                                          [] {
                                              const auto &v = getSettings().game.loadingSpeed;
                                              return v.getValue() != v.getDefaultValue();
                                          },
                                  }),
            rightPane, [](Pane &pane) {
                for (int speed : { 0, 1 }) {
                    pane.add_button({
                                        .text = speed == 1 ? Rml::String { "GameCube" } : Rml::String { "Fast" },
                                        .isSelected = [speed] { return getSettings().game.loadingSpeed.getValue() == speed; },
                                    })
                        .on_pressed([speed] {
                            getSettings().game.loadingSpeed.setValue(speed);
                            config::Save();
                        });
                }
                pane.add_rml("<br/>Fast reads the disc image at once. GameCube makes every load take the time the "
                             "console's disc drive took, so screens follow each other at the original pace.");
            });

        auto addOption = [&](const Rml::String &key, ConfigVar<bool> &value, const Rml::String &helpText) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                });
        };
        auto addSpeedrunDisabledOption = [&](const Rml::String &key, ConfigVar<bool> &value, const Rml::String &helpText) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                    .isDisabled = [] { return getSettings().game.speedrunMode; },
                });
        };

        // leftPane.add_section("General");
        // leftPane.add_section("Difficulty");
        // leftPane.add_section("Quality of Life");
        leftPane.add_section("Speedrunning");
        config_bool_select(leftPane, rightPane, getSettings().game.speedrunMode,
            {
                .key = "Speedrun Mode",
                .helpText = "Enables speedrunning options while restricting certain gameplay modifiers.",
                .onChange = [](bool) { reset_for_speedrun_mode(); },
            });
    });

    add_tab("Cheats", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        auto addCheat = [&](const Rml::String &key, ConfigVar<bool> &value, const Rml::String &helpText) {
            config_bool_select(leftPane, rightPane, value,
                {
                    .key = key,
                    .helpText = helpText,
                    .isDisabled = [] { return getSettings().game.speedrunMode; },
                });
        };

        leftPane.add_section("Minigames");
        addCheat("Unlock All Minigames", getSettings().game.unlockAllMinigames,
            "Treat every minigame as unlocked when the game checks minigame availability.");

        leftPane.add_section("Boards");
        addCheat("Unlock Bowser's Gnarly Party", getSettings().game.unlockBowsersGnarlyParty,
            "Treat Bowser's Gnarly Party as unlocked when the game checks board availability.");

        leftPane.add_section("Party");
        addCheat("Infinite Coins", getSettings().game.infiniteCoins,
            "Human players keep 999 coins on the board (COM players are left alone).");

        leftPane.add_section("Cheat Codes");
        addCheat("Enable Cheat Codes", getSettings().game.enableCheatCodes,
            "Run the codes of cheats.txt in the data folder: Gecko or unencrypted Action Replay codes for "
            "Mario Party 5 (USA). $Name turns a code on, -$Name off. Only codes writing whole player or "
            "game fields (coins, stars, capsules, turns) can work; the others are reported and skipped.");
        leftPane.register_control(leftPane.add_button("Reload Codes").on_pressed([] { mp5native::cheats::reload(); }),
            rightPane, [](Pane &pane) {
                pane.add_text("Read cheats.txt again after editing it. " + Rml::String { mp5native::cheats::status() });
            });
#if MP5_NATIVE_CAN_OPEN_DATA_FOLDER
        leftPane.register_control(leftPane.add_button("Open Data Folder").on_pressed([] { mp5native::OpenDataFolder(); }),
            rightPane, [](Pane &pane) { pane.add_text("cheats.txt is created here the first time codes are enabled."); });
#endif
    });

    add_tab("RetroAchievements", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        if (s_raPendingUser.empty()) {
            s_raPendingUser = getSettings().retroAchievements.username.getValue();
        }

        leftPane.add_section("Account");
        leftPane.register_control(leftPane.add_select_button({
                                      .key = "Status",
                                      .getValue = [] { return Rml::String { ra::statusText() }; },
                                  }),
            rightPane, [](Pane &pane) {
                pane.add_text("Achievements are unlocked in softcore mode. Hardcore needs the "
                              "RetroAchievements team to validate this client first.");
            });
        config_bool_select(leftPane, rightPane, getSettings().retroAchievements.enabled,
            {
                .key = "Enable RetroAchievements",
                .helpText = "Connect to retroachievements.org and unlock achievements while you play. "
                            "Takes effect the next time the game starts.",
            });
        leftPane.register_control(leftPane.add_child<StringButton>(StringButton::Props {
                                      .key = "Username",
                                      .getValue = [] { return s_raPendingUser; },
                                      .setValue = [](Rml::String value) { s_raPendingUser = std::move(value); },
                                      .maxLength = 64,
                                  }),
            rightPane, [](Pane &pane) { pane.add_text("Your retroachievements.org username."); });
        leftPane.register_control(leftPane.add_child<StringButton>(StringButton::Props {
                                      .key = "Password",
                                      .getValue = [] { return s_raPendingPassword; },
                                      .setValue = [](Rml::String value) { s_raPendingPassword = std::move(value); },
                                      .maxLength = 256,
                                      .type = "password",
                                      .secret = true,
                                  }),
            rightPane, [](Pane &pane) {
                pane.add_text("Only used to log in. It is not saved: the game keeps the session "
                              "token the server returns instead.");
            });
        leftPane.register_control(leftPane.add_button("Log In").on_pressed([] {
            ra::loginWithPassword(s_raPendingUser, s_raPendingPassword);
            s_raPendingPassword.clear();
        }),
            rightPane, [](Pane &pane) { pane.add_text("Log in with the username and password above."); });
        leftPane.register_control(leftPane.add_button("Log Out").on_pressed([] { ra::logout(); }),
            rightPane, [](Pane &pane) { pane.add_text("Forget the saved session on this computer."); });
    });

    add_tab("Interface", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);

        leftPane.add_section("MP5 Native");
#if MP5_NATIVE_CAN_OPEN_DATA_FOLDER
        leftPane.register_control(leftPane.add_button("Open Data Folder").on_pressed([] {
            // mDoAud_seStartMenu(kSoundClick); // TODO PC
            mp5native::OpenDataFolder();
        }),
            rightPane, [](Pane &pane) {
                pane.add_text("Open the folder where MP5 Native stores settings, saves, logs, texture "
                              "replacements, and other app data.");
            });
#endif
        leftPane.register_control(leftPane.add_select_button({
                                      .key = "Notifications",
                                      .getValue =
                                          [] {
                                              const bool ach = getSettings().game.enableAchievementToasts.getValue();
                                              const bool ctl = getSettings().game.enableControllerToasts.getValue();
                                              if (!ach && !ctl) {
                                                  return Rml::String { "Off" };
                                              }
                                              if (ach && ctl) {
                                                  return Rml::String { "All" };
                                              }
                                              return Rml::String { "Some" };
                                          },
                                      .isModified =
                                          [] {
                                              const auto &ach = getSettings().game.enableAchievementToasts;
                                              const auto &ctl = getSettings().game.enableControllerToasts;
                                              return ach.getValue() != ach.getDefaultValue() || ctl.getValue() != ctl.getDefaultValue();
                                          },
                                  }),
            rightPane, [](Pane &pane) {
                pane.clear();
                pane.add_button("Select All").on_pressed([] {
                    // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                    getSettings().game.enableAchievementToasts.setValue(true);
                    getSettings().game.enableControllerToasts.setValue(true);
                    config::Save();
                });
                pane.add_button("Select None").on_pressed([] {
                    // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                    getSettings().game.enableAchievementToasts.setValue(false);
                    getSettings().game.enableControllerToasts.setValue(false);
                    config::Save();
                });

                pane.add_section("Types");
                pane.add_button({
                                    .text = "Achievements",
                                    .isSelected = [] { return getSettings().game.enableAchievementToasts.getValue(); },
                                })
                    .on_pressed([] {
                        // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                        auto &v = getSettings().game.enableAchievementToasts;
                        v.setValue(!v.getValue());
                        config::Save();
                    });
                pane.add_button({
                                    .text = "Controller",
                                    .isSelected = [] { return getSettings().game.enableControllerToasts.getValue(); },
                                })
                    .on_pressed([] {
                        // mDoAud_seStartMenu(kSoundItemChange); // TODO PC
                        auto &v = getSettings().game.enableControllerToasts;
                        v.setValue(!v.getValue());
                        config::Save();
                    });
                pane.add_rml("<br/>Choose which notifications can be displayed.");
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.skipPreLaunchUI,
            {
                .key = "Skip MP5 Native Main Menu",
                .helpText = "When starting MP5 Native, skip the main menu and boot straight into the "
                            "game if a disc image is available.",
            });
        config_bool_select(leftPane, rightPane, getSettings().backend.skipBootSequence,
            {
                .key = "Skip Game Boot Sequence",
                .helpText = "Start directly at mode select after running the startup initialization normally "
                            "performed by the logos, title, and file select screens.",
            });
        // config_bool_select(leftPane, rightPane, getSettings().backend.showPipelineCompilation,
        //     {
        //         .key = "Show Pipeline Compilation",
        //         .helpText = "Show an overlay when shaders are being compiled for your hardware.",
        //     });
        config_bool_select(leftPane, rightPane, getSettings().game.pauseOnFocusLost,
            {
                .key = "Pause On Focus Lost",
                .helpText = "Pause the game when window focus is lost.",
                .onChange = [](bool value) { aurora_set_pause_on_focus_lost(value); },
            });
        // config_bool_select(leftPane, rightPane, getSettings().backend.enableAdvancedSettings,
        //     {
        //         .key = "Enable Advanced Settings",
        //         .icon = "warning",
        //         .helpText = "Show advanced settings and debugging tools with "
        //                     "Shift+F1.<br/><br/><icon class=\"warning\"/> WARNING: Debugging tools "
        //                     "can easily break your game. Do not use on a regular save!",
        //         .onChange =
        //             [](bool) {
        //                 for (auto &doc : get_document_stack()) {
        //                     if (dynamic_cast<MenuBar *>(doc.get())) {
        //                         doc = std::make_unique<MenuBar>();
        //                         break;
        //                     }
        //                 }
        //             },
        //     });

        // leftPane.add_section("Game");
        // config_bool_select(leftPane, rightPane, getSettings().game.recordingMode,
        //     {
        //         .key = "Recording Mode",
        //         .helpText = "Disables the game HUD and all background music.<br/><br/>Useful for "
        //                     "recording footage.",
        //     });
    });
}

void SettingsWindow::update()
{
    if (mPrelaunch && top_document() == this) {
        try_push_verification_modal(*this);
    }

    Window::update();
}

} // namespace mp5native::ui
