#include "game/board/main.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/opening.h"
#include "game/board/capsule.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/coin.h"
#include "game/board/star.h"
#include "game/board/com.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/pad.h"
#include "game/main.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/hsfex.h"

#include <math.h>

/*
 * w07Dll - "Koopa's Tycoon Town" board.
 *
 * NOTE (port): like w03Dll, mb7_Work is one flat scratch buffer (0x338
 * bytes = 0xCE 32-bit words) reused as several unrelated sub-structures,
 * addressed by raw word/byte offset in the original -O0 code. m2c's draft
 * mistypes it as `ANIMDATA *mb7_Work[0xCE]` (an unrelated struct that just
 * happens to share a size) - that typing is wrong and is not used here.
 * The layout below is being reconstructed from
 * config/GP5E01_00/rels/w07Dll/symbols.txt (mb7_Work = .bss:0x4 size:0x338)
 * and build/GP5E01_00/w07Dll/asm/REL/w07Dll/world07.s, word offset by word
 * offset, as each function that touches it is decompiled - see the
 * MB7W_* macros below, extended incrementally.
 */

/* forward declarations - definitions land in later commits as each
 * function is decompiled, in increasing order of risk (boilerplate ->
 * leaf helpers -> effect hooks -> setup/teardown -> masu event handlers
 * -> Koopa/Dokan/Hou state machines). */
static void MB7_Create(void);
static void MB7_Kill(void);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    GWPartyFSet(TRUE);
    MBObjectSetup(6, MB7_Create, MB7_Kill);
}

/* ----------------------------- leaf helpers ----------------------------- */

static f32 MB7_DistRatioGet(Point3d *a, Point3d *b, Point3d *dir) {
    f32 mag = VECMag(dir);
    if (mag > 0.0f) {
        mag = ((dir->x * (a->x - b->x)) + (dir->y * (a->y - b->y)) + (dir->z * (a->z - b->z))) / mag;
    }
    return mag;
}

/* NONMATCHING: extra callee-save float temporary (f30/f31 split) not
 * reproduced; logic is otherwise identical instruction-for-instruction */
static f32 MB7_AngleDiff(f32 from, f32 to) {
    f32 mod = (f32) fmod((f64) (to - from), 360.0);
    f32 diff = mod;
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

/* NONMATCHING: register allocation order differs (73% match) */
static s32 MB7Ev_BitGet(s32 flag, u32 mask) {
    s32 result;
    u32 inBit;
    u32 outBit;
    s32 i;

    flag = flag & mask;
    result = 0;
    inBit = 1;
    outBit = inBit;
    i = 0;
    for (; i < 0x20; i++) {
        if (mask & inBit) {
            if (flag & inBit) {
                result |= outBit;
            }
            outBit *= 2;
        }
        inBit *= 2;
    }
    return result;
}

/* NONMATCHING: register allocation order differs */
/* NONMATCHING: register allocation order differs (65.52% match) */
static u32 MB7Ev_MasuFlagGet(s32 bits, s32 mask) {
    u32 result;
    s32 inBit;
    s32 outBit;
    s32 i;

    result = 0;
    inBit = 1;
    outBit = 1;
    i = 0;
    for (; i < 0x20; i++) {
        if (mask & outBit) {
            if (bits & inBit) {
                result |= outBit;
            }
            inBit *= 2;
        }
        outBit *= 2;
    }
    return result;
}

static void MB7_LightReset(void) {
}

static void MB7_MasuStub(void) {
}

static void MB7Ev_MasuWalkPre(s32 playerNo) {
}
