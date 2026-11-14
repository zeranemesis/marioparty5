#include "REL/sd00dll.h"

typedef struct SdSys_s {
    u8 pad000[0xA80];
    f32 speed; //0xA80
} SdSys;

extern SdSys *sys;

#include "stage_b.inc"

#include "stage_c.inc"

#include "stage_d.inc"

#include "stage_e.inc"
