/*
 * Force-included into MusyX (CMakeLists.txt): its variables go to the game's sections, and the memory it takes with
 * malloc (the sample directory of a group, when no hooks are set) comes from the game's sound heap, so that netplay
 * rollback saves and restores the synth whole (a restored pointer never names host memory freed since).
 */
#ifndef MP5_MUSYX_GAMESTATE_H
#define MP5_MUSYX_GAMESTATE_H
#include "gamestate.h" /* MusyX has no include path to the game: next to this file */
#include <stdlib.h>
void *msmPCSndMalloc(size_t size); /* src/msm/msmsys.c */
void msmPCSndFree(void *ptr);
/* object-like: SND_HOOKS' members named malloc/free are renamed alike, its layout does not change */
#define malloc msmPCSndMalloc
#define free msmPCSndFree
#endif
