// m555Dll game unit: the minigame logic lives in the fragment files below
// (kept as .inc so each piece can be diffed against the original on its own).
#include "REL/m555Dll.h"

/* float table in .data that no function of this unit references */
f32 lbl_1_data_37C[25] = {
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    913.0f,
    0.0f,
    -1177.0f,
    993.0f,
    0.0f,
    -1038.0f,
    550.0f,
    1000.0f,
    -3250.0f,
    593.0f,
    312.0f,
    -1119.0f,
    -462.0f,
    330.0f,
    -1100.0f,
    -1000.0f,
    40.0f,
    -2100.0f,
    -2500.0f,
    0.0f,
    -3900.0f
};

/* the pointer to the stage control record (bss of game.o) */
M555_CTRL *lbl_1_bss_3030;

#include "REL/m555Dll/game_1.inc"
#include "REL/m555Dll/game_2.inc"
#include "REL/m555Dll/game_3.inc"
#include "REL/m555Dll/game_4.inc"
#include "REL/m555Dll/game_5.inc"
#include "REL/m555Dll/game_6.inc"
#include "REL/m555Dll/game_7.inc"
#include "REL/m555Dll/game_8.inc"
#include "REL/m555Dll/game_9.inc"
#include "REL/m555Dll/game_10.inc"
