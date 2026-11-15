#include "port/display_select.h"

#include <SDL3/SDL.h>

#include "aurora/lib/window.hpp"

namespace mp5native::display {
namespace {
SDL_DisplayID id_at(int index) {
    if (index < 0 || !SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        return 0;
    }
    int n = 0;
    SDL_DisplayID* ids = SDL_GetDisplays(&n);
    SDL_DisplayID id = (ids != nullptr && index < n) ? ids[index] : 0;
    SDL_free(ids);
    return id;
}
} // namespace

int count() {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        return 0;
    }
    int n = 0;
    SDL_free(SDL_GetDisplays(&n));
    return n;
}

std::string name(int index) {
    const SDL_DisplayID id = id_at(index);
    const char* n = id ? SDL_GetDisplayName(id) : nullptr;
    std::string label = std::to_string(index + 1);
    if (const SDL_DisplayMode* mode = id ? SDL_GetDesktopDisplayMode(id) : nullptr) {
        label += " - " + std::to_string((int)(mode->refresh_rate + 0.5f)) + " Hz";
    }
    return n != nullptr ? label + " (" + n + ")" : label;
}

void start_pos(int index, int w, int h, int* x, int* y) {
    SDL_Rect r;
    const SDL_DisplayID id = id_at(index);
    if (id && SDL_GetDisplayBounds(id, &r)) {
        *x = r.x + (r.w - w) / 2;
        *y = r.y + (r.h - h) / 2;
    }
}

void move_window(int index) {
    SDL_Window* win = aurora::window::get_sdl_window();
    SDL_Rect r;
    const SDL_DisplayID id = id_at(index);
    if (win == nullptr || !id || !SDL_GetDisplayBounds(id, &r)) {
        return;
    }
    const bool fullscreen = (SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN) != 0;
    if (fullscreen) {
        SDL_SetWindowFullscreen(win, false);
    }
    int w = 0, h = 0;
    SDL_GetWindowSize(win, &w, &h);
    SDL_SetWindowPosition(win, r.x + (r.w - w) / 2, r.y + (r.h - h) / 2);
    if (fullscreen) {
        SDL_SetWindowFullscreen(win, true);
    }
}
} // namespace mp5native::display
