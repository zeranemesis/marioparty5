// Credits: TwilitRealm

#pragma once

namespace mp5native::ui {

enum class NavCommand {
    None,
    Up,
    Down,
    Left,
    Right,
    Next, // R1
    Previous, // L1
    Confirm, // A
    Cancel, // B
    Menu, // Back/Minus, or R + Start
};

} // namespace mp5native::ui
