/*
 * VI entry points RELs call (boot/e3boot set the TV mode). Aurora drives the
 * display itself, so these are no-ops; each REL gets its own copy because dol
 * does not export aurora's VI code (see tools/gen_gx_def.py).
 */

#include "dolphin/vi.h"

#ifndef TARGET_DOL /* src/port is globbed into dol too, which has the real ones */

void VIConfigure(const GXRenderModeObj *rm) { (void) rm; }

void VIFlush(void) {}

u32 VIGetTvFormat(void) { return 0; /* VI_NTSC */ }

#endif
