// Credits: TwilitRealm

#include "port/ui.h"

#include <chrono>

#include "ui.hpp"

extern "C" {

bool ui_initialize(void)
{
    return mp5native::ui::initialize();
}

bool ui_is_prelaunch_open(void)
{
    return mp5native::ui::is_prelaunch_open();
}

void ui_shutdown(void)
{
    mp5native::ui::shutdown();
}

void ui_handle_sdl_event(const SDL_Event *event)
{
    if (event != nullptr) {
        mp5native::ui::handle_event(*event);
    }
}

void ui_update(void)
{
    mp5native::ui::update();
}

void ui_push_toast(const char *type, const char *title, const char *content, uint32_t duration_ms)
{
    mp5native::ui::push_toast({
        .type = type != nullptr ? type : "",
        .title = title != nullptr ? title : "",
        .content = content != nullptr ? content : "",
        .duration = std::chrono::milliseconds(duration_ms),
    });
}

}
