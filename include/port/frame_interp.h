#ifndef MP5_NATIVE_FRAME_INTERP_H
#define MP5_NATIVE_FRAME_INTERP_H

/*
 * High frame rate on the native port (src/port/frame_interp.cpp).
 *
 * Game logic keeps its 60 Hz tick: one full original main-loop pass (pads,
 * processes, Hu3DExec, wipe, VI retrace) per 1/60 s of real time. Between
 * ticks the renderer draws extra frames from the last two ticks' state
 * (cameras, model root transforms and skeleton poses, sprites), lerped at
 * alpha = time since the tick / 1/60 s. Those frames run Hu3DExec with
 * HuInterpDrawF set: no motion evaluation, no motion/sprite/texture advance,
 * and they are drawn with GlobalCounter held at the tick's value and
 * Hu3DPauseF forced on so hook-side updates guarded by either do not run again.
 */

#ifdef TARGET_PC
#include "dolphin/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HuInterpDrawF (game/hu3d.h) is set while an interpolated (draw-only) frame runs Hu3DExec */

/* Hu3DExec on an interpolated frame, instead of motion evaluation: writes the
 * model's lerped skeleton pose into its objects; TRUE if it wrote one */
BOOL HuInterpPoseSet(s16 modelId);

/* main loop: logic ticks to run this pass (1 in the original 60 fps mode),
 * or -1 when there is nothing to draw yet (it has waited) */
int HuInterpFrameBegin(void);
/* after each logic tick: record the state frames are drawn from */
void HuInterpTickEnd(void);
/* before the frame is presented: draw the interpolated frame (no-op at 60 fps) */
void HuInterpDraw(void);
/* after the frame is presented: frame pacing */
void HuInterpFrameEnd(void);
/* netplay rollback: the game's memory went back to an earlier tick */
void HuInterpInvalidate(void);
/* netplay time sync: hold the next logic ticks back by this long in all (spread over a few frames) */
void HuInterpPaceDelay(double seconds);

#ifdef __cplusplus
}
#endif
#endif

#endif
