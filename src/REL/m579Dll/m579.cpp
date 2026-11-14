/* m579 support library (new/delete, camera, link lists, game objects, models, RNG, threads, timers).
 * Translated from the shared m5xx C++ library (see src/REL/m501Dll/m501.cpp) into free functions that
 * take the object as their first argument, because the other m579 units call it as fn_1_XXXX. */
#include <stddef.h>
#include <stdarg.h>
#include "dolphin.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/charman.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/mgdata.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/flag.h"
#include "game/process.h"
extern "C" {
#include "game/mg/timer.h"
}
#include "REL/m579Dll/m579types.h"

#if !defined(M579_ONLY) || M579_ONLY == 1
#include "REL/m579Dll/m579_a.inc"
#endif
#if !defined(M579_ONLY) || M579_ONLY == 2
#include "REL/m579Dll/m579_b.inc"
#endif
#if !defined(M579_ONLY) || M579_ONLY == 3
#include "REL/m579Dll/m579_c.inc"
#endif
#if !defined(M579_ONLY) || M579_ONLY == 4
#include "REL/m579Dll/m579_d.inc"
#endif
