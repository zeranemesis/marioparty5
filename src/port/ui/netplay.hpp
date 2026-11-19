#pragma once

#include "pane.hpp"
#include "window.hpp"

namespace mp5native::ui {

// Settings > Online Play: what to do at the next start-up (host or join), with whom.
void build_netplay_settings(Pane &leftPane, Pane &rightPane);

// Shown before the game boots when Online Play is on: hosts or joins, then boots everyone together.
class NetplayLobby : public Window {
public:
    NetplayLobby();
    ~NetplayLobby() override;
    void update() override;

private:
    void connect();
    Rml::Element *mStatus = nullptr;
    Button *mStart = nullptr;
};

// portmain.cpp runs the UI until the lobby is gone (game started together, or offline).
bool netplay_lobby_open() noexcept;

} // namespace mp5native::ui
