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
 * w04Dll - "Deep Bloober Sea" board.
 *
 * NOTE (port): mb4_Work is one flat scratch buffer (0x7F7C bytes) reused as
 * several different sub-structures, addressed with raw byte offsets in the
 * original code (typical of MWCC -O0 code that never got a single named
 * struct). Only the regions actually touched by the functions decompiled so
 * far are named below; the rest stays a raw byte array until reached.
 */

extern void ObjectSetup(void);

static void MB4_Create(void);
static void MB4_Kill(void);
static void MB4_CoinClose(void);
static void MB4_CoinEffClose(void);
static void MB4_BubbleEffClose(void);
static void MB4_SakanaClose(void);
static void MB4Ev_Kihon(s32 playerNo);
static void MB4Ev_Iruka(s32 playerNo);
static void MB4Ev_Blow(s32 playerNo);
static void MB4Ev_Kaisoo(s32 playerNo);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

static OMOBJ *mb4_MainObj;

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
    MBObjectSetup(3, MB4_Create, MB4_Kill);
}

/* ----------------------------- leaf helpers ----------------------------- */

void MB4_Kill(void) {
    if (mb4_MainObj != NULL) {
        mb4_MainObj = NULL;
    }
    MB4_CoinClose();
    MB4_CoinEffClose();
    MB4_BubbleEffClose();
    MB4_SakanaClose();
}

void MB4_LightReset(void) {
}

/* masu->flag bits used by this board's event dispatch; not part of the
 * shared MASU_FLAG_* set in game/board/masu.h (those start at bit 16). */
#define MB4_MASU_FLAG_HATENA 0x1
#define MB4_MASU_FLAG_IRUKA 0x10
#define MB4_MASU_FLAG_BLOW 0x40
#define MB4_MASU_FLAG_KAISOO 0x100

void MB4Ev_MasuHatena(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    if (masuP->flag & MB4_MASU_FLAG_HATENA) {
        MB4Ev_Kihon(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_IRUKA) {
        MB4Ev_Iruka(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_BLOW) {
        MB4Ev_Blow(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_KAISOO) {
        MB4Ev_Kaisoo(playerNo);
    }
}

s32 MB4_MasuStub(void) {
    return 0;
}

s32 MB4Ev_MasuWalkPre(s32 playerNo) {
    return 0;
}
