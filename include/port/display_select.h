#pragma once
// Which monitor the game window opens on (setting video.display: -1 = system default, else 0-based index).
#include <string>

namespace mp5native::display {
int count();
std::string name(int index);
// window position centred on monitor `index` for a w x h window; leaves x/y untouched when index < 0 or unknown
void start_pos(int index, int w, int h, int* x, int* y);
// moves the live window to monitor `index` (keeps fullscreen)
void move_window(int index);
} // namespace mp5native::display
