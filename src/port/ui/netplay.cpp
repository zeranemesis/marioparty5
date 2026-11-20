#include "netplay.hpp"

#include "number_button.hpp"
#include "string_button.hpp"
#include "ui.hpp"
#include "port/config.hpp"
#include "port/netplay.h"
#include "port/settings.h"

namespace mp5native::ui {
namespace {

bool sLobbyOpen = false;
Rml::String sLanResult;

const char* const kModes[] = {"Off", "Host a Game", "Join a Game"};

} // namespace

bool netplay_lobby_open() noexcept { return sLobbyOpen; }

void build_netplay_settings(Pane &leftPane, Pane &rightPane)
{
    leftPane.register_control(leftPane.add_select_button({
                                  .key = "Online Play at Start-up",
                                  .getValue = [] { return Rml::String { kModes[std::clamp(getSettings().netplay.mode.getValue(), 0, 2)] }; },
                                  .isModified = [] { return getSettings().netplay.mode.getValue() != 0; },
                              }),
        rightPane, [](Pane &pane) {
            for (int i = 0; i < 3; i++) {
                pane.add_button({
                                    .text = kModes[i],
                                    .isSelected = [i] { return getSettings().netplay.mode.getValue() == i; },
                                })
                    .on_pressed([i] {
                        getSettings().netplay.mode.setValue(i);
                        config::Save();
                    });
            }
            pane.add_rml("<br/>Every player runs the game on their own machine with their own controller. "
                         "The host's save, settings and random seed are used by everyone. "
                         "Takes effect the next time the game starts: a lobby opens before the game boots.");
        });
    leftPane.register_control(leftPane.add_child<StringButton>(StringButton::Props {
                                  .key = "Host Address",
                                  .getValue = [] { return getSettings().netplay.address.getValue(); },
                                  .setValue =
                                      [](Rml::String value) {
                                          getSettings().netplay.address.setValue(std::move(value));
                                          config::Save();
                                      },
                                  .maxLength = 128,
                              }),
        rightPane, [](Pane &pane) {
            pane.add_text("To join: the host's IP address, with its port after a colon if it is not the default "
                          "(for example 192.168.1.20 or 203.0.113.5:51525). Over the Internet the host's game "
                          "opens its port on the router by itself (UPnP) and shows its public address in the lobby; "
                          "if the router does not allow it, the host forwards its port (TCP) by hand.");
        });
    leftPane.register_control(leftPane.add_child<NumberButton>(NumberButton::Props {
                                  .key = "Port",
                                  .getValue = [] { return getSettings().netplay.port.getValue(); },
                                  .setValue =
                                      [](int value) {
                                          getSettings().netplay.port.setValue(value);
                                          config::Save();
                                      },
                                  .min = 1024,
                                  .max = 65535,
                              }),
        rightPane, [](Pane &pane) { pane.add_text("To host: the TCP port players connect to (default 51525)."); });
    leftPane.register_control(leftPane.add_child<NumberButton>(NumberButton::Props {
                                  .key = "Input Delay",
                                  .getValue = [] { return getSettings().netplay.delay.getValue(); },
                                  .setValue =
                                      [](int value) {
                                          getSettings().netplay.delay.setValue(value);
                                          config::Save();
                                      },
                                  .min = 0,
                                  .max = 10,
                                  .suffix = " frames",
                              }),
        rightPane, [](Pane &pane) {
            pane.add_text("To host: how late every press reaches the game. With rollback, 1 or 2 is enough even "
                          "over the Internet; without it, more delay hides more network lag (2 on a local network, 4 to "
                          "6 over the Internet) and too little makes the game stutter.");
        });
    leftPane.register_control(leftPane.add_select_button({
                                  .key = "Network Lag",
                                  .getValue = [] {
                                      return Rml::String { getSettings().netplay.rollback.getValue() ? "Rollback" : "Wait for Every Player" };
                                  },
                                  .isModified = [] { return !getSettings().netplay.rollback.getValue(); },
                              }),
        rightPane, [](Pane &pane) {
            for (const bool rollback : {true, false}) {
                pane.add_button({
                                    .text = rollback ? "Rollback" : "Wait for Every Player",
                                    .isSelected = [rollback] { return getSettings().netplay.rollback.getValue() == rollback; },
                                })
                    .on_pressed([rollback] {
                        getSettings().netplay.rollback.setValue(rollback);
                        config::Save();
                    });
            }
            pane.add_rml("<br/>To host. Rollback: the game goes on when another player's presses are late, guessing "
                         "them, and quietly corrects itself when they arrive (smooth, as in modern fighting games). "
                         "Wait for every player: each frame waits for everyone's presses (simple, stutters with lag).");
        });
    leftPane.register_control(leftPane.add_button("Find a Game on the Local Network").on_pressed([] {
        const std::string found = Mp5Net_FindLan();
        sLanResult = found.empty() ? "No game found on the local network." : "Found a game at " + found + ".";
        if (!found.empty()) {
            getSettings().netplay.address.setValue(found);
            config::Save();
        }
    }),
        rightPane, [](Pane &pane) {
            pane.add_text("Looks for a host on this network and puts its address above.");
            if (!sLanResult.empty()) {
                pane.add_rml("<br/>");
                pane.add_text(sLanResult);
            }
        });
}

NetplayLobby::NetplayLobby()
{
    sLobbyOpen = true;
    add_tab("Online Play", [this](Rml::Element *content) {
        auto &leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto &rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
        mButtons = &leftPane;
        mStatus = leftPane.add_text("");
        if (getSettings().netplay.mode.getValue() == 1) {
            mStart = &leftPane.add_button("Start Game").on_pressed([] { Mp5Net_Start(); });
            leftPane.register_control(*mStart, rightPane,
                [](Pane &pane) { pane.add_text("Everyone who joined boots the game now."); });
        }
        leftPane.register_control(leftPane.add_button("Retry").on_pressed([this] { connect(); }), rightPane,
            [](Pane &pane) { pane.add_text("Host or connect again."); });
        leftPane.register_control(leftPane.add_button("Play Offline").on_pressed([this] { request_close(); }),
            rightPane, [](Pane &pane) {
                pane.add_text("Boots the game alone. Online Play stays on for the next start: turn it off in "
                              "Settings > Online Play.");
            });
    });
    connect();
}

NetplayLobby::~NetplayLobby()
{
    sLobbyOpen = false;
    if (Mp5Net_Poll() != NetLobby::Started) {
        Mp5Net_Leave();
    }
}

// straight on the buttons, not the tab bar: one press of A starts the game
bool NetplayLobby::focus()
{
    return (mButtons != nullptr && mButtons->focus()) || Window::focus();
}

void NetplayLobby::connect()
{
    const auto &np = getSettings().netplay;
    if (np.mode.getValue() == 1) {
        Mp5Net_Host((u16)np.port.getValue(), np.delay.getValue());
    } else {
        Mp5Net_Join(np.address.getValue());
    }
}

void NetplayLobby::update()
{
    if (Mp5Net_Poll() == NetLobby::Started && sLobbyOpen) {
        sLobbyOpen = false; // the game boots now, on every machine
        hide(true);
    }
    // joining before the host listens: try again every 2 s (not after the host refused us)
    if (getSettings().netplay.mode.getValue() == 2 && Mp5Net_Poll() == NetLobby::Failed &&
        Mp5Net_Status().rfind("The host refused", 0) != 0 && clock::now() >= mRetryAt) {
        mRetryAt = clock::now() + std::chrono::seconds(2);
        connect();
    }
    if (mStatus != nullptr) {
        mStatus->SetInnerRML(escape(Mp5Net_Status()));
    }
    if (mStart != nullptr) {
        mStart->set_disabled(Mp5Net_Poll() != NetLobby::Hosting || Mp5Net_Players() == 0);
    }
    Window::update();
}

} // namespace mp5native::ui
