#ifndef _GAME_GXARRAY_H
#define _GAME_GXARRAY_H

#include "dolphin.h"

/*
 * Indexed vertex arrays.
 *
 * The GameCube's GX only needs the address of an array and its stride: the
 * hardware reads the vertices straight out of main memory while it draws.
 * Aurora has to upload them to the GPU, so it also needs the size of the array
 * and whether its values are in host order (le), which they are once the file
 * they came from has been byteswapped.
 */
#ifdef TARGET_PC
#define GXSETARRAY(attr, data, size, stride, le) GXSetArray((attr), (data), (u32)(size), (u8)(stride), (le))
#else
#define GXSETARRAY(attr, data, size, stride, le) GXSetArray((attr), (data), (stride))
#endif

#endif
