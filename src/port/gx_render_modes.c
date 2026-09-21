/*
 * Aurora declares GXNtsc480Prog and GXNtsc480ProgAa (extern/aurora/include/
 * dolphin/gx/GXFrameBuffer.h) but, unlike GXNtsc480IntDf, never defines them:
 * a real game always builds its own progressive-scan render mode from the
 * interlaced one when OSGetProgressiveMode() says the console is set to it.
 * This port's OSGetProgressiveMode (src/port/sdk_stubs.c) always answers no,
 * so nothing ever reads these two structures for real; they only need to
 * exist so the handful of places that take their address still link.
 *
 * Aurora itself is linked as a static library, so GXNtsc480IntDf naturally
 * gets its own copy inside every binary that links it (dol.dll, each REL,
 * the executable) without needing to cross a DLL boundary. This file is
 * compiled into dol AND into every REL module the same way (see
 * CMakeLists.txt's REL loop), for the same reason: exporting a Windows DATA
 * symbol across DLLs needs SHARED_SYM, and these two are not worth it.
 */

#include "dolphin/gx.h"

GXRenderModeObj GXNtsc480Prog;
GXRenderModeObj GXNtsc480ProgAa;
