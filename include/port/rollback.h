/*
 * Netplay rollback groundwork (src/port/rollback.cpp): the game's whole state saved and restored at a tick boundary.
 *
 * The state is: the game's variables in every loaded module (the sections of include/port/gamestate.h), the HuMem
 * heaps and the coroutine stacks' heap (all at fixed addresses for the whole run), and the session's clock and input
 * position (netplay.cpp). Restoring it at the top of a tick and running the ticks again with the same pads must give
 * the same bytes: MP5_RB_SYNCTEST=<n> checks exactly that every n ticks, on one machine.
 */
#ifndef MP5_PORT_ROLLBACK_H
#define MP5_PORT_ROLLBACK_H
#include <dolphin/pad.h>
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gamestate_sections.cpp: a module's game sections, while it is loaded */
void Mp5Rb_Sections(const void *key, void *d0, void *d1, void *b0, void *b1);
void Mp5Rb_Forget(const void *key);

/* main.c, before a display frame's ticks: how many ticks to run again first (a rollback restored an earlier tick) */
int Mp5Rb_FrameBegin(void);
/* main.c, top of each logic tick */
int Mp5Rb_TickBegin(void);
/* main.c, inside a tick: synctest diagnostics (MP5_RB_EACH) */
void Mp5Rb_Probe(int point);

/* netplay.cpp, once per pad frame: 1 when the pads of that frame are replayed from the record (status filled) */
int Mp5Rb_PadReplay(u32 frame, PADStatus *status);
void Mp5Rb_PadRecord(u32 frame, const PADStatus *status);

/* netplay.cpp: the session state that lives outside the game's memory */
typedef struct {
    u32 frame;
    s64 lastTime;
    u64 audioFrames;
} Mp5NetTickState;
void Mp5Net_TickSave(Mp5NetTickState *state);
/* netplay.cpp, rollback sessions */
int Mp5Net_RollbackActive(void);
int Mp5Net_TakeMisprediction(u32 *frame); /* the first frame that ran on wrong pads, if any */
void Mp5Net_RollbackDone(u32 frames);
void Mp5Net_WaitConfirmed(void);
/* rollback.cpp: ticks are running again (no drawing, no sound out, no pads sent) */
int Mp5Rb_Resimulating(void);
int Mp5Rb_TickRestorable(void);
void Mp5Net_TickLoad(const Mp5NetTickState *state);

#ifdef __cplusplus
}
#endif
#endif
