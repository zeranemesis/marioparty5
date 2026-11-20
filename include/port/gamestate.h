/*
 * Force-included into the game's own sources (src/game, src/msm, src/REL; see CMakeLists.txt): their variables go to
 * sections of their own, so that netplay rollback (src/port/rollback.cpp) saves and restores exactly the game's
 * state, never the port's (whose variables hold host objects: windows, containers, GPU resources).
 */
#ifndef MP5_GAMESTATE_H
#define MP5_GAMESTATE_H
#if defined(_MSC_VER) && !defined(__clang__)
#pragma data_seg("mp5gd")
#pragma bss_seg("mp5gb")
#elif defined(__clang__)
#pragma clang section data = "mp5gd" bss = "mp5gb"
#endif
#endif
