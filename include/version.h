#ifndef _VERSION_H
#define _VERSION_H

/*
 * VERSION is passed on the command line by configure.py and CMake.
 * The decompilation targets a single version of Mario Party 5.
 */
#define VERSION_NO_ENG0 0 /* GP5E01_00, USA revision 0 */

#define VERSION_ENG (VERSION == VERSION_NO_ENG0)
#define VERSION_PAL 0
#define VERSION_JP 0
#define VERSION_NTSC 1

#define REFRESH_RATE 60
#define REFRESH_RATE_F 60.0f

/*
 * Data defined in the DOL and used by REL modules. On Windows the port builds
 * the DOL and each REL as separate DLLs, so such data has to be exported by one
 * and imported by the others. Elsewhere, and in the matching build, it is empty.
 */
#if defined(_WIN32) && defined(TARGET_PC)
#ifdef TARGET_DOL
#define SHARED_SYM __declspec(dllexport)
#else
#define SHARED_SYM __declspec(dllimport)
#endif
#else
#define SHARED_SYM
#endif

#endif
