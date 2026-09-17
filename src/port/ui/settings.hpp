// Credits: TwilitRealm

#pragma once
#include "window.hpp"

namespace mp5native::ui {

class SettingsWindow : public Window {
public:
    SettingsWindow(bool prelaunch = false);

    void update() override;

protected:
    bool mPrelaunch;
};

} // namespace mp5native::ui
