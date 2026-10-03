// m512Dll/REL/m512Dll/score
// CMinigame score window / record timer helpers.
//
// The unit pulls in game/mg/score.h and game/mg/timer.h for MgScoreWin* and
// MgTimer*, but both reach humath.h, and MWCC emits two local statics with
// MSL's `extern inline sqrtf` (_half$localstatic3 / _three$localstatic4, 16
// bytes of .rodata) for every object that does so. The module .rodata
// already holds one copy, so the REL would grow by 16 bytes. Only the API
// this unit actually touches is declared here instead.

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
typedef signed short HUSPRID;
typedef signed short HUSPRGRPID;

struct ANIMDATA;

// ---------------------------------------------------------------- external

extern "C" {
void *fn_1_1264(void *self);                                    // CGameObject ctor
void fn_1_1270(void *self, int flag);                            // CGameObject dtor
void fn_1_1330(void *self, void *arg);                           // base member init
void fn_1_1D4(char *msg, char *file, int line);                 // __assert_fail

// base class vtable, lives in the module's .rodata (another object)
void *lbl_1_data_1FC[4];

// singletons and the shared base-class pointer, all in the module's .bss
void *lbl_1_bss_C;
void *lbl_1_bss_9FC;
void *lbl_1_bss_A00;

void __dl__FPv(void *obj);

ANIMDATA *HuDataSelHeapReadNum(u32 dataNum, u32 memNum, s32 heap);
ANIMDATA *HuSprAnimRead(void *data);
HUSPRID HuSprCreate(ANIMDATA *anim, s16 prio, s16 bank);
HUSPRGRPID HuSprGrpCreate(s16 sprNum);
void HuSprGrpMemberSet(HUSPRGRPID grpId, s16 memberNo, HUSPRID sprId);
void HuSprGrpKill(HUSPRGRPID grpId);
void HuSprAttrSet(HUSPRGRPID grpId, s16 memberNo, s32 attr);
void HuSprAttrReset(HUSPRGRPID grpId, s16 memberNo, s32 attr);
void HuSprPosSet(HUSPRGRPID grpId, s16 memberNo, f32 posX, f32 posY);
void HuSprScaleSet(HUSPRGRPID grpId, s16 memberNo, f32 scaleX, f32 scaleY);
void HuSprTPLvlSet(HUSPRGRPID grpId, s16 memberNo, f32 tpLvl);
void HuSprGrpPosSet(HUSPRGRPID grpId, f32 posX, f32 posY);

HUSPRGRPID MgScoreWinCreate(s16 sizeX, s16 sizeY);
void MgScoreWinColorSet(HUSPRGRPID gid, u8 r, u8 g, u8 b);
void MgScoreWinTPLvlSet(HUSPRGRPID gid, f32 tpLvl);
void MgScoreWinPosSet(HUSPRGRPID gid, f32 posX, f32 posY);
void MgScoreWinDispSet(HUSPRGRPID gid, s32 disp);

// MgTimer_s, trimmed after winGrpId; only the fields this unit reads are named
typedef struct {
    s32 mode;             // 0x00
    s32 prevMode;         // 0x04
    s32 type;             // 0x08
    s32 offType;          // 0x0C
    s32 dispOnF;          // 0x10
    s32 stopF;            // 0x14
    s32 scaleDir;         // 0x18
    s32 winDispF;         // 0x1C
    u8 flashFlag;         // 0x20
    s32 maxTime;          // 0x24
    s32 endTime;          // 0x28
    s32 speed;            // 0x2C
    s32 time;             // 0x30
    s32 recordTime;       // 0x34
    s32 fadeOutTime;      // 0x38
    f32 posX;             // 0x3C
    f32 posY;             // 0x40
    s16 timeUnit;         // 0x44
    s16 gameMesId;        // 0x46
    HUSPRGRPID winGrpId;  // 0x48
} M512MGTIMER;

M512MGTIMER *MgTimerCreate(int type);
void MgTimerKill(M512MGTIMER *timer);
void MgTimerParamSet(M512MGTIMER *timer, s32 maxTime, s32 endTime, s32 recordTime);
s32 MgTimerValueGet(M512MGTIMER *timer);
void MgTimerPosSet(M512MGTIMER *timer, f32 posX, f32 posY);
void MgTimerModeOnSet(M512MGTIMER *timer, int offType);
s32 MgTimerDoneCheck(M512MGTIMER *timer);
void MgTimerWinColorSet(M512MGTIMER *timer, u8 r, u8 g, u8 b);
void MgTimerRecordDispOn(M512MGTIMER *timer);
}

// ---------------------------------------------------------- unit functions

// ------------------------------------------------------------------- types

typedef struct {
    u32 unk0;
    u32 unk4;
    void *vtable;         // 0x08
    s32 bitmask;          // 0x0C
    M512MGTIMER *timer;   // 0x10
    s32 param;            // 0x14
    u8 timerOnF;          // 0x18
    u8 wideF;             // 0x19
} M512Score;

typedef struct {
    HUSPRGRPID winGrpId;  // 0x00
    HUSPRGRPID sprGrpId;  // 0x02
    s32 kind;             // 0x04
    s32 idx;              // 0x08
    s32 digitNum;         // 0x0C
    s32 digit[5];         // 0x10
    s32 maxTime;          // 0x24
    f32 posX;             // 0x28
    f32 posY;             // 0x2C
    u8 timerOnF;          // 0x30
    M512MGTIMER *timer;   // 0x34
} M512ScoreItem;

typedef struct {
    u32 unk0;
    u32 unk4;
    void *vtable;         // 0x08
    s32 bitmask;          // 0x0C
    u8 animF;             // 0x10
    u8 recordF;           // 0x11
    M512ScoreItem item[4];// 0x14
} M512ScoreList;

extern "C" {
void fn_1_1DDC(void *self);                                     // base vtable slot
void fn_1_D3E8(M512Score *self);
void fn_1_D178(M512Score *self, s32 param, u8 wideF);
void *fn_1_D27C(M512Score *self, s16 flag);
void *fn_1_D308(M512Score *self, s16 flag);
void fn_1_D4C4(M512Score *self);
void fn_1_D694(M512Score *self, s32 idx);
void *fn_1_D804(M512ScoreList *self, u8 recordF, s32 arg2);
void *fn_1_DAF0(M512ScoreList *self, s16 flag);
void *fn_1_DB7C(M512ScoreList *self, s16 flag);
void fn_1_E23C(M512ScoreList *self);
void fn_1_E65C(M512ScoreList *self, s32 idx);
}

// -------------------------------------------------------------------- data

char lbl_1_data_458[] = "ms_pointer == 0";
char lbl_1_data_468[] = "singleton.h";
char lbl_1_data_474[] = "ms_pointer != 0";

// DATANUM(DATA_mgconst, 0x20) .. 0x2D, then 0x2A twice
s32 lbl_1_data_484[15] = {
    0x00960020, 0x00960021, 0x00960022, 0x00960023, 0x00960024,
    0x00960026, 0x00960027, 0x00960028, 0x00960029, 0x0096002A,
    0x0096002B, 0x0096002C, 0x0096002D, 0x0096002A, 0x0096002A
};

typedef struct {
    void *unk0;
    void *unk4;
    void *dtor;
    void *vfunc1;
    void *vfunc2;
} M512Vtbl;

M512Vtbl lbl_1_data_4C0 = { 0, 0, (void *)fn_1_DB7C, (void *)fn_1_E65C, (void *)fn_1_E23C };
M512Vtbl lbl_1_data_4D4 = { 0, 0, (void *)fn_1_D308, (void *)fn_1_D694, (void *)fn_1_1DDC };

typedef struct {
    f32 x;
    f32 y;
} M512Pos;

static const M512Pos lbl_1_rodata_3B0[4] = {
    { 76.0f, 54.0f },
    { 500.0f, 54.0f },
    { 76.0f, 424.0f },
    { 500.0f, 424.0f }
};

#define MGSCORE_DIGIT_MAX 5
#define MGSCORE_VTBL  (&lbl_1_data_4D4)
#define MGSCOREL_VTBL (&lbl_1_data_4C0)

extern "C" {

void fn_1_D3E8(M512Score *self)
{
    if (self->wideF != 0) {
        self->timer = MgTimerCreate(2);
        MgTimerParamSet(self->timer, 0, 0x1518, self->param);
        MgTimerPosSet(self->timer, 292.0f, 432.0f);
    } else {
        self->timer = MgTimerCreate(1);
        MgTimerParamSet(self->timer, 0, 0x4650, self->param);
        MgTimerPosSet(self->timer, 292.0f, 424.0f);
    }
    MgTimerWinColorSet(self->timer, 0, 0, 0);
    MgTimerRecordDispOn(self->timer);
    self->timerOnF = 1;
}

void fn_1_D178(M512Score *self, s32 param, u8 wideF)
{
    void *singleton;

    fn_1_1264(self);
    self->vtable = lbl_1_data_1FC;
    self->bitmask = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A00 != 0) {
        fn_1_1D4(lbl_1_data_458, lbl_1_data_468, 0x35);
    }
    singleton = (void *)&self->timer;
    if (singleton != 0) {
        singleton = (void *)((u8 *)singleton - 0x10);
    }
    lbl_1_bss_A00 = singleton;
    self->vtable = MGSCORE_VTBL;
    self->param = param;
    self->timerOnF = 0;
    self->wideF = wideF;
    if (self->wideF == 0) {
        fn_1_D3E8(self);
    }
}

void *fn_1_D27C(M512Score *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A00 != 0) {
        } else {
            fn_1_1D4(lbl_1_data_474, lbl_1_data_468, 0x3D);
        }
        lbl_1_bss_A00 = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void *fn_1_D308(M512Score *self, s16 flag)
{
    void *singleton;

    if (self != 0) {
        self->vtable = MGSCORE_VTBL;
        if (self->timerOnF != 0) {
            MgTimerKill(self->timer);
        }
        singleton = (void *)&self->timer;
        if (singleton != 0) {
            if (lbl_1_bss_A00 != 0) {
            } else {
                fn_1_1D4(lbl_1_data_474, lbl_1_data_468, 0x3D);
            }
            lbl_1_bss_A00 = 0;
        }
        if (self != 0) {
            self->vtable = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_D4C4(M512Score *self)
{
    if (self->wideF != 0) {
        fn_1_D3E8(self);
    }
}

void fn_1_D5AC(M512Score *self)
{
    MgTimerModeOnSet(self->timer, 0);
}

void fn_1_D5E0(M512Score *self)
{
    if (self->timer->stopF == 0) {
        self->timer->stopF = 1;
        self->timer->mode = 1;
    }
}

s32 fn_1_D60C(M512Score *self)
{
    s32 result = 0;

    if (MgTimerDoneCheck(self->timer) != 0 && self->timerOnF != 0) {
        result = 1;
    }
    return result;
}

void fn_1_D664(M512Score *self)
{
    MgTimerValueGet(self->timer);
}

void fn_1_D694(M512Score *self, s32 idx)
{
    self->bitmask &= ~(1 << idx);
    switch (idx) {
    case 1:
        if (self->wideF != 0) {
            fn_1_D3E8(self);
        }
        break;
    case 4:
    case 0:
        return;
    case 2:
        MgTimerModeOnSet(self->timer, 0);
        return;
    case 3:
        if (self->timer->stopF == 0) {
            self->timer->stopF = 1;
            self->timer->mode = 1;
        }
        break;
    }
}

void *fn_1_D804(M512ScoreList *self, u8 recordF, s32 arg2)
{
    ANIMDATA *digitAnim;
    ANIMDATA *unitAnim;
    void *singleton;
    HUSPRGRPID grpId;
    s32 i;
    s32 j;

    fn_1_1264(self);
    self->vtable = lbl_1_data_1FC;
    self->bitmask = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_9FC != 0) {
        fn_1_1D4(lbl_1_data_458, lbl_1_data_468, 0x35);
    }
    singleton = (void *)&self->animF;
    if (singleton != 0) {
        singleton = (void *)((u8 *)singleton - 0x10);
    }
    lbl_1_bss_9FC = singleton;
    self->vtable = MGSCOREL_VTBL;
    self->animF = 0;
    self->recordF = recordF;
    digitAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x00460013, 0x10000000, 2));
    unitAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x00460014, 0x10000000, 2));
    for (i = 0; i < 4; i++) {
        M512ScoreItem *item = &self->item[i];

        item->timerOnF = 0;
        item->winGrpId = MgScoreWinCreate(0x78, 0x20);
        MgScoreWinColorSet(item->winGrpId, 0x40, 0x40, 0x40);
        MgScoreWinTPLvlSet(item->winGrpId, 0.8f);
        MgScoreWinDispSet(item->winGrpId, 0);
        grpId = HuSprGrpCreate(0xB);
        item->sprGrpId = grpId;
        for (j = 0; j < 5; j++) {
            HuSprGrpMemberSet(grpId, j + 1, HuSprCreate(digitAnim, 0xD, 0));
            HuSprPosSet(grpId, j + 1, -22.0f + 17.0f * (f32)(f64)j, 0.0f);
        }
        for (j = 0; j < 5; j++) {
            HuSprGrpMemberSet(grpId, j + 6, HuSprCreate(unitAnim, 0xB, 0));
            HuSprPosSet(grpId, j + 6, -22.0f + 17.0f * (f32)(f64)j, 0.0f);
        }
        for (j = 0; j < 0xB; j++) {
            HuSprAttrSet(grpId, j, 1);
            HuSprAttrSet(grpId, j, 4);
        }
    }
    return self;
}

void *fn_1_DAF0(M512ScoreList *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_9FC != 0) {
        } else {
            fn_1_1D4(lbl_1_data_474, lbl_1_data_468, 0x3D);
        }
        lbl_1_bss_9FC = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void *fn_1_DB7C(M512ScoreList *self, s16 flag)
{
    s32 i;
    void *singleton;

    if (self != 0) {
        self->vtable = MGSCOREL_VTBL;
        for (i = 0; i < 4; i++) {
            M512ScoreItem *item = &self->item[i];

            if (item->timerOnF != 0) {
                MgTimerKill(item->timer);
            }
            HuSprGrpKill(item->sprGrpId);
        }
        singleton = (void *)&self->animF;
        if (singleton != 0) {
            if (lbl_1_bss_9FC != 0) {
            } else {
                fn_1_1D4(lbl_1_data_474, lbl_1_data_468, 0x3D);
            }
            lbl_1_bss_9FC = 0;
        }
        if (self != 0) {
            self->vtable = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_DC94(M512ScoreList *self, s32 idx, s32 kind)
{
    M512ScoreItem *item = &self->item[idx];
    ANIMDATA *anim;
    s32 j;

    item->idx = idx;
    item->kind = kind;
    item->digitNum = 0;
    item->posX = lbl_1_rodata_3B0[idx].x;
    item->posY = lbl_1_rodata_3B0[idx].y;
    MgScoreWinPosSet(item->winGrpId, item->posX, item->posY);
    anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_484[kind], 0x10000000, 2));
    HuSprGrpMemberSet(item->sprGrpId, 0, HuSprCreate(anim, 0xC, 0));
    HuSprPosSet(item->sprGrpId, 0, -42.0f, 0.0f);
    HuSprAttrSet(item->sprGrpId, 0, 4);
    HuSprGrpPosSet(item->sprGrpId, item->posX, item->posY);
    for (j = 0; j < MGSCORE_DIGIT_MAX; j++) {
        item->digit[j] = 0x12;
    }
}

void fn_1_DDDC(M512ScoreList *self, s32 idx, s32 digitNum, s32 maxTime)
{
    self->item[idx].digitNum = digitNum;
    self->item[idx].maxTime = maxTime;
}

void fn_1_DDF8(void *arg1, M512ScoreItem *item)
{
    s32 i;

    for (i = 0; i < MGSCORE_DIGIT_MAX; i++) {
        if (i < item->digitNum) {
            if ((item->digit[i] -= 1) < 0) {
                item->digit[i] = 0;
            }
        } else {
            if ((f64)(item->digit[i] += 1) > 18.0) {
                item->digit[i] = 0x12;
            }
        }
    }
}

void fn_1_DEB8(void *arg1, M512ScoreItem *item)
{
    s32 i;
    f32 scale;
    f32 tpLvl;

    MgScoreWinDispSet(item->winGrpId, 1);
    for (i = 0; i < 6; i++) {
        HuSprAttrReset(item->sprGrpId, i, 4);
    }
    for (i = 0; i < MGSCORE_DIGIT_MAX; i++) {
        if ((f64)item->digit[i] < 18.0) {
            HuSprAttrReset(item->sprGrpId, i + 6, 4);
            scale = (f32)((f64)item->digit[i] + 18.0) / 18.0;
            HuSprScaleSet(item->sprGrpId, i + 6, scale, scale);
            tpLvl = (f32)(18.0 - (f64)item->digit[i]) / 18.0;
            HuSprTPLvlSet(item->sprGrpId, i + 6, tpLvl);
        } else {
            HuSprAttrSet(item->sprGrpId, i + 6, 4);
        }
    }
}

void fn_1_E098(void *arg1, M512ScoreItem *item)
{
    f32 posX;
    f32 posY;

    if (item->timerOnF == 0) {
        item->timer = MgTimerCreate(2);
        MgTimerParamSet(item->timer, item->maxTime, item->maxTime, item->maxTime);
        MgTimerModeOnSet(item->timer, 2);
        MgTimerRecordDispOn(item->timer);
        if (item->timer->stopF == 0) {
            item->timer->stopF = 1;
            item->timer->mode = 1;
        }
        posX = 20.0f + lbl_1_rodata_3B0[item->idx].x;
        posY = 8.0f + lbl_1_rodata_3B0[item->idx].y;
        MgTimerPosSet(item->timer, posX, posY);
        MgScoreWinDispSet(item->timer->winGrpId, 0);
        item->timer->winDispF = 0;
        item->timerOnF = 1;
    }
}

void fn_1_E1CC(void *arg1, M512ScoreItem *item)
{
    s32 i;

    MgScoreWinDispSet(item->winGrpId, 1);
    for (i = 1; i < 0xB; i++) {
        HuSprAttrSet(item->sprGrpId, i, 1);
        HuSprAttrSet(item->sprGrpId, i, 4);
    }
}

void fn_1_E23C(M512ScoreList *self)
{
    f32 posX;
    f32 posY;
    f32 scale;
    f32 tpLvl;
    s32 i;
    s32 j;

    if (self->animF != 0) {
        for (i = 0; i < 4; i++) {
            M512ScoreItem *item = &self->item[i];

            for (j = 0; j < MGSCORE_DIGIT_MAX; j++) {
                if (j < item->digitNum) {
                    if ((item->digit[j] -= 1) < 0) {
                        item->digit[j] = 0;
                    }
                } else {
                    if ((f64)(item->digit[j] += 1) > 18.0) {
                        item->digit[j] = 0x12;
                    }
                }
            }
            if (self->recordF != 0 && item->digitNum >= 5) {
                if (item->timerOnF == 0) {
                    item->timer = MgTimerCreate(2);
                    MgTimerParamSet(item->timer, item->maxTime, item->maxTime, item->maxTime);
                    MgTimerModeOnSet(item->timer, 2);
                    MgTimerRecordDispOn(item->timer);
                    if (item->timer->stopF == 0) {
                        item->timer->stopF = 1;
                        item->timer->mode = 1;
                    }
                    posX = 20.0f + lbl_1_rodata_3B0[item->idx].x;
                    posY = 8.0f + lbl_1_rodata_3B0[item->idx].y;
                    MgTimerPosSet(item->timer, posX, posY);
                    MgScoreWinDispSet(item->timer->winGrpId, 0);
                    item->timer->winDispF = 0;
                    item->timerOnF = 1;
                }
                MgScoreWinDispSet(item->winGrpId, 1);
                for (j = 1; j < 0xB; j++) {
                    HuSprAttrSet(item->sprGrpId, j, 1);
                    HuSprAttrSet(item->sprGrpId, j, 4);
                }
            } else {
                MgScoreWinDispSet(item->winGrpId, 1);
                for (j = 0; j < 6; j++) {
                    HuSprAttrReset(item->sprGrpId, j, 4);
                }
                for (j = 0; j < MGSCORE_DIGIT_MAX; j++) {
                    if ((f64)item->digit[j] < 18.0) {
                        HuSprAttrReset(item->sprGrpId, j + 6, 4);
                        scale = (f32)((f64)item->digit[j] + 18.0) / 18.0;
                        HuSprScaleSet(item->sprGrpId, j + 6, scale, scale);
                        tpLvl = (f32)(18.0 - (f64)item->digit[j]) / 18.0;
                        HuSprTPLvlSet(item->sprGrpId, j + 6, tpLvl);
                    } else {
                        HuSprAttrSet(item->sprGrpId, j + 6, 4);
                    }
                }
            }
        }
    }
}

void fn_1_E65C(M512ScoreList *self, s32 idx)
{
    self->bitmask &= ~(1 << idx);
    switch (idx) {
    case 2:
        self->animF = 1;
        break;
    case 4:
    case 3:
    case 1:
    case 0:
        break;
    }
}

}
