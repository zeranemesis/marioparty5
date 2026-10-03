#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/window.h"
#include "datadir_enum.h"

#include "REL/e3setupdll.h"

/* Glyph animation handles, indexed by font slot (see fontFileTbl). */
static ANIMDATA **mgNameAnim;

/* Glyph file ids per font slot. 0x20xxxx is DATANUM(DATA_e3setup, n);
   the list is terminated by -1, and the trailing 0 is never reached. */
static u32 fontFileTbl[200] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0x20004A, 0x20004B,
    0x20004C, 0x20004D, 0x20004E, 0x20004F, 0x200050, 0x200051, 0x200052, 0x200053, 0x20004A, 0x20004A,
    0x20004A, 0x200054, 0x20004A, 0x20004A, 0x20004A, 0x200058, 0x200059, 0x20005A, 0x20005B, 0x20005C,
    0x20005D, 0x20005E, 0x20005F, 0x200060, 0x200061, 0x200062, 0x200063, 0x200064, 0x200065, 0x200066,
    0x200067, 0x200068, 0x200069, 0x20006A, 0x20006B, 0x20006C, 0x20006D, 0x20006E, 0x20006F, 0x200070,
    0x200071, 0x200066, 0x200057, 0x200066, 0x200066, 0x200066, 0x200066, 0x200072, 0x200073, 0x200074,
    0x200075, 0x200076, 0x200077, 0x200078, 0x200079, 0x20007A, 0x20007B, 0x20007C, 0x20007D, 0x20007E,
    0x20007F, 0x200080, 0x200081, 0x200082, 0x200083, 0x200084, 0x200085, 0x200086, 0x200087, 0x200088,
    0x200089, 0x20008A, 0x20008B, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A,
    0x20004A, 0x20004A, 0x200054, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A,
    0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A,
    0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A,
    0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A,
    0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A,
    0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x20004A, 0x200055, 0x200056, 0x20004A, 0x20004A,
    0xFFFFFFFF, 0
};

void MgNameInit(void) {
    s16 count;
    s16 i;

    for (count = 0; fontFileTbl[count] != 0xFFFFFFFF; count++) {
    }

    mgNameAnim = HuMemDirectMallocNum(HEAP_HEAP, count * 4, HU_MEMNUM_OVL);

    for (i = 0; i < count; i++) {
        if (fontFileTbl[i] == 0) {
            mgNameAnim[i] = NULL;
        } else {
            mgNameAnim[i] =
                HuSprAnimRead(HuDataSelHeapReadNum(fontFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
            HuSprAnimLock(mgNameAnim[i]);
        }
    }

    HuDataDirClose(DATA_gamemes);
}

HUSPRGRPID MgNameCreate(void *arg) {
    HUSPRID sprId;
    HUSPRGRP *grp;
    HUSPRGRPID grpId;
    s16 *xTbl;
    ANIMDATA **animTbl;
    s32 font;
    s16 i;
    s16 *wTbl;
    s16 width;
    s16 count;
    u8 *p;

    animTbl = HuMemDirectMalloc(HEAP_HEAP, 0x190);
    xTbl = HuMemDirectMalloc(HEAP_HEAP, 0xC8);
    wTbl = HuMemDirectMalloc(HEAP_HEAP, 0xC8);

    for (p = (u8 *)HuWinMesPtrGet((u32)arg), width = 0, count = 0; *p != 0; p++) {
        if (*p == ' ' || *p == 0x10) {
            width += 0x14;
            continue;
        }

        if (*p < 0x30) {
            continue;
        }

        if (*p == 0x80 || *p == 0x81) {
            continue;
        }

        if (p[1] == 0x80) {
            if (*p >= 0x96 && *p <= 0xA4) {
                font = *p + 0x6A;
            } else if (*p >= 0xAA && *p <= 0xAE) {
                font = *p + 0x65;
            } else if (*p >= 0xD6 && *p <= 0xE4) {
                font = *p + 0x43;
            } else if (*p >= 0xEA && *p <= 0xEE) {
                font = *p + 0x3E;
            }
        } else if (p[1] == 0x81) {
            if (*p >= 0xAA && *p <= 0xAE) {
                font = *p + 0x6A;
            } else if (*p >= 0xEA && *p <= 0xEE) {
                font = *p + 0x43;
            }
        } else {
            font = *p;
        }

        animTbl[count] = mgNameAnim[font];
        xTbl[count] = width;

        if (*p >= 0x61 && *p <= 0x7A) {
            wTbl[count] = 2;
            width += 0x12;
        } else if (*p == 0xC2 || *p == 0xC3) {
            wTbl[count] = 0;
            width += 0x12;
        } else if (*p == 0x5C) {
            wTbl[count] = 0;
            width += 0x8;
        } else if (*p >= 0x87 && *p <= 0x8F) {
            wTbl[count] = 4;
            width += 0x18;
        } else if (*p >= 0xC7 && *p <= 0xCF) {
            wTbl[count] = 4;
            width += 0x18;
        } else {
            wTbl[count] = 0;
            width += 0x1C;
        }

        count++;
    }

    grpId = HuSprGrpCreate(count);
    grp = &HuSprGrpData[grpId];
    grp->work[0] = width;
    width = width / 2 - 0xE;

    for (i = 0; i < count; i++) {
        sprId = HuSprCreate(animTbl[i], 0, 0);
        HuSprGrpMemberSet(grpId, i, sprId);
        HuSprPosSet(grpId, i, (float)(xTbl[i] - width), (float)wTbl[i]);
    }

    HuMemDirectFree(animTbl);
    HuMemDirectFree(xTbl);
    HuMemDirectFree(wTbl);

    return grpId;
}