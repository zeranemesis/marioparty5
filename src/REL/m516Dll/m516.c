#include "game/audio.h"
#include "game/charman.h"
#include "game/gamemes.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/timer.h"
#include "game/object.h"
#include "game/wipe.h"
#include "dolphin/mtx.h"
#include "string.h"

void fn_1_330(OMOBJ *obj);
void fn_1_410(OMOBJ *obj);
void fn_1_524(OMOBJ *obj);
void fn_1_654(OMOBJ *obj);
void fn_1_970(OMOBJ *obj);
void fn_1_CA8(OMOBJ *obj);
void fn_1_EF8(OMOBJ *obj);
void fn_1_11EC(OMOBJ *obj);
void fn_1_14B0(OMOBJ *obj);
OMOBJFUNC fn_1_1F04(OMOBJMAN *objman);
OMOBJFUNC fn_1_2108(OMOBJMAN *objman, OMOBJ *obj);
extern OMOBJMAN *lbl_1_bss_1C;
extern s32 lbl_1_bss_8;
extern MGTIMER *lbl_1_bss_0;
extern s16 lbl_1_bss_4;
extern s16 lbl_1_bss_6;
extern s16 lbl_1_bss_C;
extern s32 lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_18;
extern OMOBJ *lbl_1_bss_14;
extern Vec lbl_1_rodata_10;
extern Vec lbl_1_rodata_1C;
extern f32 lbl_1_rodata_2C;
extern f32 lbl_1_rodata_30;
extern f32 lbl_1_rodata_34;
extern f32 lbl_1_rodata_38;
extern Vec lbl_1_data_0;
extern Vec lbl_1_data_C;
extern Vec lbl_1_data_18;
extern GXColor lbl_1_data_24;
void fn_1_79F8(OMOBJMAN *objman);
void fn_1_80C0(OMOBJMAN *objman);
void fn_1_6F24(OMOBJMAN *objman);
void fn_1_48CC(OMOBJMAN *objman);
void fn_1_254C(OMOBJMAN *objman);
extern f32 lbl_1_rodata_3C;
extern f32 lbl_1_rodata_40;
extern f32 lbl_1_rodata_28;
extern double lbl_1_rodata_48;
extern f32 lbl_1_rodata_50;
extern f32 lbl_1_rodata_54;
f32 fn_1_1D34(f32 start, f32 end, f32 t);
f32 fn_1_1E50(f32 start, f32 end);
typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
void ObjectSetup(void);

s32 _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    Vec rot10;
    Vec rot1c;
    Vec aim;
    Vec pos;
    Mtx rot;
    OMOBJMAN *objman;
    HU3DLIGHT *light;

    rot10 = lbl_1_rodata_10;
    rot1c = lbl_1_rodata_1C;

    lbl_1_bss_8 = -1;
    lbl_1_bss_0 = 0;

    lbl_1_bss_C = Hu3DGLightCreateV(&lbl_1_data_0, &lbl_1_data_C, &lbl_1_data_24);
    Hu3DGLightInfinitytSet(lbl_1_bss_C);
    light = &Hu3DGlobalLight[lbl_1_bss_C];
    light->type |= 0x8000;

    aim.x = aim.y = aim.z = lbl_1_rodata_28;
    pos.x = pos.z = lbl_1_rodata_28;
    pos.y = lbl_1_data_18.z;

    mtxRot(rot, lbl_1_data_18.x, lbl_1_data_18.y, lbl_1_rodata_28);
    PSMTXMultVec(rot, &pos, &pos);
    Hu3DGLightPosAimSetV(lbl_1_bss_C, &pos, &aim);

    objman = omInitObjMan(0x32, 0x2000);
    lbl_1_bss_1C = objman;
    omGameSysInit(objman);
    lbl_1_bss_10 = 0;
    lbl_1_bss_6 = lbl_1_bss_4 = -1;

    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, lbl_1_rodata_2C, lbl_1_rodata_30, lbl_1_rodata_34,
        lbl_1_rodata_38);

    lbl_1_bss_14 = omAddObjEx(objman, 0x7FDA, 0, 0, OM_GRP_NONE, omOutView);
    lbl_1_bss_18 = omAddObjEx(objman, 0xA, 0, 0, OM_GRP_NONE, fn_1_330);

    fn_1_79F8(objman);
    fn_1_80C0(objman);
    fn_1_6F24(objman);
    fn_1_48CC(objman);
    fn_1_254C(objman);
}

void fn_1_330(OMOBJ *obj)
{
    u32 *work;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x64, HU_MEMNUM_OVL);
    work = (u32 *)obj->data;
    memset(work, 0, 0x64);

    work[0x00 / 4] = 0;
    work[0x04 / 4] = 0;
    work[0x10 / 4] = 0;
    work[0x14 / 4] = 0;
    work[0x18 / 4] = 0;
    work[0x1C / 4] = 0;
    work[0x20 / 4] = 0;
    work[0x24 / 4] = 0;
    work[0x08 / 4] = 0;
    work[0x0C / 4] = 0;
    work[0x58 / 4] = 1;
    work[0x48 / 4] = work[0x4C / 4] = work[0x50 / 4] = work[0x54 / 4] = 0xFFFFFFFF;
    work[0x5C / 4] = 0;
    work[0x60 / 4] = 0;

    obj->objFunc = fn_1_654;
}

s32 fn_1_1820(void);
s32 fn_1_1838(void);
s32 fn_1_1850(void);
s32 fn_1_1868(void);
s32 fn_1_1880(void);
void fn_1_1898(s32 value);
s32 fn_1_18B0(s32 index);
void fn_1_1A20(void);
void fn_1_18E0(s32 index);
void fn_1_1910(s32 index);
s32 fn_1_1940(void);
s32 fn_1_1978(void);
void fn_1_19E0(s32 player, s32 value1, s32 value2);
s32 fn_1_1A9C(f32 value);
s32 fn_1_1AF8(Vec *pos);
s32 fn_1_1B8C(s32 soundId, Vec *pos);
void fn_1_1C60(s32 voiceId, Vec *pos);

void fn_1_1A20(void)
{
    u32 *work = (u32 *)lbl_1_bss_18->data;

    if (++lbl_1_bss_10 == 4) {
        CharLoseVoicePlay((s16)work[0x38 / 4], (s16)work[0x3C / 4],
                          (s16)work[0x40 / 4], (s16)work[0x44 / 4]);
    }
}

void fn_1_410(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;

    work[0x04 / 4]++;
    work[0x14 / 4]++;
    work[0x1C / 4]++;
    work[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && work[0x00 / 4] == 1 && work[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && work[0x00 / 4] != 5) {
        work[0x00 / 4] = 4;
        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 1000);
            lbl_1_bss_8 = -1;
        }
        HuAudFadeOut(1000);
        fn_1_524(obj);
    }
}

s32 fn_1_1820(void)
{
    return *(u32 *)lbl_1_bss_18->data;
}

s32 fn_1_1838(void)
{
    return *(u32 *)((u8 *)lbl_1_bss_18->data + 0x10);
}

s32 fn_1_1850(void)
{
    return *(u32 *)((u8 *)lbl_1_bss_18->data + 0x18);
}

s32 fn_1_1868(void)
{
    return *(u32 *)((u8 *)lbl_1_bss_18->data + 0x20);
}

s32 fn_1_1880(void)
{
    return *(u32 *)((u8 *)lbl_1_bss_18->data + 0x58);
}

void fn_1_1898(s32 value)
{
    *(u32 *)((u8 *)lbl_1_bss_18->data + 0x58) = value;
}

// NONMATCHING: MWCC spills the data pointer to r31 for the indexed load.
s32 fn_1_18B0(s32 index)
{
    u32 *work = *(u32 **)((u8 *)lbl_1_bss_18 + 0x5c);
    return ((u32 *)((u8 *)work + index * 4 + 0x48))[0];
}

void fn_1_18E0(s32 index)
{
    *(u32 *)((u8 *)lbl_1_bss_18->data + 0x5C) |= 1 << ((index & 3) * 4);
}

void fn_1_1910(s32 index)
{
    *(u32 *)((u8 *)lbl_1_bss_18->data + 0x60) |= 1 << ((index & 3) * 4);
}

// NONMATCHING: the compare uses cmplw against a register while MWCC emits cmplwi.
s32 fn_1_1978(void)
{
    if (lbl_1_bss_0 == 0) {
        return 30;
    }
    return MgTimerValueGet(lbl_1_bss_0) / 60;
}

// NONMATCHING: MWCC preserves the constructor-adjusted field pointers through r30/r31.
void fn_1_19E0(s32 player, s32 value1, s32 value2)
{
    u8 *work = *(u8 **)((u8 *)lbl_1_bss_18 + 0x5c);
    player &= 3;
    *(s32 *)(work + 0x28 + player * 4) = value1;
    *(s32 *)(work + 0x38 + player * 4) = value2;
}

s32 fn_1_1A9C(f32 value)
{
    s32 result;
    value *= lbl_1_rodata_3C;
    result = (s32)(lbl_1_rodata_40 + value);
    if (result < 32) {
        result = 32;
    }
    if (result > 96) {
        result = 96;
    }
    return result;
}

s32 fn_1_1AF8(Vec *pos)
{
    HuVecF screen;
    Hu3D3Dto2D(pos, TRUE, &screen);
    return fn_1_1A9C(screen.x);
}

// NONMATCHING: MWCC's repeated integer aliases and sound ID check differ from this cleanup.
s32 fn_1_1B8C(s32 soundId, Vec *pos)
{
    HuVecF screen;
    s32 pan;

    Hu3D3Dto2D(pos, TRUE, &screen);
    pan = fn_1_1A9C(screen.x);
    if (pan == -1) {
        return -1;
    }
    return HuAudFXPlayPan((s32)(s16)soundId, (s16)pan);
}

// NONMATCHING: The intermediate register copies used for the voice ID stay visible in MWCC.
void fn_1_1C60(s32 voiceId, Vec *pos)
{
    HuVecF screen;
    s32 pan;

    if (voiceId == -1 || HuAudFXStatusGet(voiceId) != 2) {
        return;
    }
    Hu3D3Dto2D(pos, TRUE, &screen);
    pan = fn_1_1A9C(screen.x);
    if (pan != -1) {
        HuAudFXPanning(voiceId, (s16)pan);
    }
}

f32 fn_1_1D34(f32 start, f32 end, f32 t)
{
    f32 delta;
    f32 tmp1;
    f32 tmp2;

    tmp1 = fmod((f32)(end - start), lbl_1_rodata_48);
    delta = tmp1;

    if (delta < lbl_1_rodata_28) {
        delta += lbl_1_rodata_50;
    }
    if (delta > lbl_1_rodata_54) {
        delta -= lbl_1_rodata_50;
    }

    tmp2 = fmod(start + t * delta, lbl_1_rodata_48);
    delta = tmp2;

    if (delta < lbl_1_rodata_28) {
        delta += lbl_1_rodata_50;
    }
    return delta;
}

f32 fn_1_1E50(f32 start, f32 end)
{
    f32 delta;
    f32 tmp;

    tmp = fmod((f32)(end - start), lbl_1_rodata_48);
    delta = tmp;

    if (delta < lbl_1_rodata_28) {
        delta += lbl_1_rodata_50;
    }
    if (delta > lbl_1_rodata_54) {
        delta -= lbl_1_rodata_50;
    }
    return delta;
}

void fn_1_524(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;

    if ((s32)work[0x08 / 4] != 0) {
        return;
    }

    switch ((s32)work[0x00 / 4]) {
    case 0:
        WipeCreate(1, 5, 60);
        work[0x00 / 4] = 1;
        work[0x14 / 4] = 0;
        obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
        break;
    case 1:
        work[0x00 / 4] = 2;
        work[0x1C / 4] = 0;
        obj->objFunc = fn_1_EF8;
        break;
    case 2:
        work[0x00 / 4] = 3;
        work[0x24 / 4] = 0;
        obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
        break;
    case 3:
    case 4:
        work[0x08 / 4] = 1;
        work[0x00 / 4] = 5;
        obj->objFunc = fn_1_970;
        break;
    case 5:
    default:
        work[0x00 / 4] = 5;
        obj->objFunc = fn_1_CA8;
        break;
    }

    work[0x04 / 4] = 0;
}

void fn_1_654(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    u32 *workB;
    u32 *workC;

    work[0x04 / 4]++;
    work[0x14 / 4]++;
    work[0x1C / 4]++;
    work[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && work[0x00 / 4] == 1 && work[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && work[0x00 / 4] != 5) {
        work[0x00 / 4] = 4;

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
            lbl_1_bss_8 = -1;
        }

        HuAudFadeOut(0x3E8);

        work = (u32 *)obj->data;

        if (work[0x08 / 4] == 0) {
            switch (work[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                work[0x00 / 4] = 1;
                work[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                work[0x00 / 4] = 2;
                work[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                work[0x00 / 4] = 3;
                work[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                work[0x08 / 4] = 1;
                work[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                work[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            work[0x04 / 4] = 0;
        }
    }

    work = (u32 *)obj->data;

    if (work[0x08 / 4] == 0) {
        switch (work[0x00 / 4]) {
        case 0:
            WipeCreate(1, 5, 60);
            work[0x00 / 4] = 1;
            work[0x14 / 4] = 0;
            obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
            break;
        case 1:
            work[0x00 / 4] = 2;
            work[0x1C / 4] = 0;
            obj->objFunc = fn_1_EF8;
            break;
        case 2:
            work[0x00 / 4] = 3;
            work[0x24 / 4] = 0;
            obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
            break;
        case 3:
        case 4:
            work[0x08 / 4] = 1;
            work[0x00 / 4] = 5;
            obj->objFunc = fn_1_970;
            break;
        case 5:
        default:
            work[0x00 / 4] = 5;
            obj->objFunc = fn_1_CA8;
            break;
        }

        work[0x04 / 4] = 0;
    }
}

void fn_1_970(OMOBJ *obj)
{
    u32 *workA;
    u32 *workB;
    u32 *workD;
    u32 *workC;

    workA = (u32 *)obj->data;
    workB = (u32 *)obj->data;

    workB[0x04 / 4]++;
    workB[0x14 / 4]++;
    workB[0x1C / 4]++;
    workB[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && workB[0x00 / 4] == 1 && workB[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && workB[0x00 / 4] != 5) {
        workB[0x00 / 4] = 4;

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
            lbl_1_bss_8 = -1;
        }

        HuAudFadeOut(0x3E8);

        workC = (u32 *)obj->data;

        if ((s32)workC[0x08 / 4] == 0) {
            switch (workC[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workC[0x00 / 4] = 1;
                workC[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workC[0x00 / 4] = 2;
                workC[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workC[0x00 / 4] = 3;
                workC[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workC[0x08 / 4] = 1;
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workC[0x04 / 4] = 0;
        }
    }

    if (WipeCheckEnd() == 0) {
        WipeCreate(2, 6, 60);
        workA[0x08 / 4] = 0;

        workD = (u32 *)obj->data;

        if ((s32)workD[0x08 / 4] == 0) {
            switch (workD[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workD[0x00 / 4] = 1;
                workD[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workD[0x00 / 4] = 2;
                workD[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workD[0x00 / 4] = 3;
                workD[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workD[0x08 / 4] = 1;
                workD[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workD[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workD[0x04 / 4] = 0;
        }
    }
}

void fn_1_6F80(void);
void fn_1_495C(void);
void fn_1_2600(void);
void fn_1_8148(void);
void fn_1_7A80(void);

void fn_1_CA8(OMOBJ *obj)
{
    u32 *work;
    u32 *workB;

    work = (u32 *)obj->data;

    work[0x04 / 4]++;
    work[0x14 / 4]++;
    work[0x1C / 4]++;
    work[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && work[0x00 / 4] == 1 && work[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && work[0x00 / 4] != 5) {
        work[0x00 / 4] = 4;

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
            lbl_1_bss_8 = -1;
        }

        HuAudFadeOut(0x3E8);

        workB = (u32 *)obj->data;

        if ((s32)workB[0x08 / 4] == 0) {
            switch (workB[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workB[0x00 / 4] = 1;
                workB[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workB[0x00 / 4] = 2;
                workB[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workB[0x00 / 4] = 3;
                workB[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workB[0x08 / 4] = 1;
                workB[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workB[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workB[0x04 / 4] = 0;
        }
    }

    if (WipeCheckEnd() == 0) {
        fn_1_6F80();
        fn_1_495C();
        fn_1_2600();
        fn_1_8148();
        fn_1_7A80();
        GameMesClose();
        HuAudFadeOut(1);
        omOvlReturnEx(1, 1);
    }
}

void fn_1_EF8(OMOBJ *obj)
{
    u32 *workA;
    u32 *workB;
    u32 *workC;

    workA = (u32 *)obj->data;
    workB = (u32 *)obj->data;

    workB[0x04 / 4]++;
    workB[0x14 / 4]++;
    workB[0x1C / 4]++;
    workB[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && workB[0x00 / 4] == 1 && workB[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && workB[0x00 / 4] != 5) {
        workB[0x00 / 4] = 4;

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
            lbl_1_bss_8 = -1;
        }

        HuAudFadeOut(0x3E8);

        workC = (u32 *)obj->data;

        if ((s32)workC[0x08 / 4] == 0) {
            switch (workC[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workC[0x00 / 4] = 1;
                workC[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workC[0x00 / 4] = 2;
                workC[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workC[0x00 / 4] = 3;
                workC[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workC[0x08 / 4] = 1;
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workC[0x04 / 4] = 0;
        }
    }

    switch (workA[0x18 / 4]) {
    case 0:
        lbl_1_bss_6 = GameMesCreate(2, 0);
        workA[0x18 / 4] = 1;
        workA[0x1C / 4] = 0;
        break;
    case 1:
        if (GameMesStatGet(lbl_1_bss_6) == 0 && (s32)workA[0x08 / 4] == 0) {
            lbl_1_bss_0 = MgTimerCreate(0);
            MgTimerParamSet(lbl_1_bss_0, 0x708, 0, 0);
            MgTimerModeOnSet(lbl_1_bss_0, 1);
            workA[0x18 / 4] = 2;
            workA[0x1C / 4] = 0;
            obj->objFunc = fn_1_11EC;
        }
        break;
    }
}


void fn_1_11EC(OMOBJ *obj)
{
    u32 *workA;
    s32 flag;
    u32 *workB;
    u32 *workC;

    workA = (u32 *)obj->data;
    flag = 0;
    workB = (u32 *)obj->data;

    workB[0x04 / 4]++;
    workB[0x14 / 4]++;
    workB[0x1C / 4]++;
    workB[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && workB[0x00 / 4] == 1 && workB[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && workB[0x00 / 4] != 5) {
        workB[0x00 / 4] = 4;

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
            lbl_1_bss_8 = -1;
        }

        HuAudFadeOut(0x3E8);

        workC = (u32 *)obj->data;

        if ((s32)workC[0x08 / 4] == 0) {
            switch (workC[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workC[0x00 / 4] = 1;
                workC[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workC[0x00 / 4] = 2;
                workC[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workC[0x00 / 4] = 3;
                workC[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workC[0x08 / 4] = 1;
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workC[0x04 / 4] = 0;
        }
    }

    if (MgTimerDoneCheck(lbl_1_bss_0) != 0) {
        flag = 1;
    }

    if (fn_1_6D78() <= 0) {
        flag = 1;
    }

    if (flag != 0) {
        lbl_1_bss_4 = GameMesCreate(2, 1);

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x64);
            lbl_1_bss_8 = -1;
        }

        workA[0x18 / 4] = 3;
        workA[0x1C / 4] = 0;

        if ((s32)workA[0x08 / 4] == 0) {
            obj->objFunc = fn_1_14B0;
        }
    }
}

void fn_1_14B0(OMOBJ *obj)
{
    u32 *workA;
    u32 *workB;
    u32 *workD;
    u32 *workC;

    workA = (u32 *)obj->data;

    if (lbl_1_bss_0 != (MGTIMER *)0) {
        MgTimerKill(lbl_1_bss_0);
        lbl_1_bss_0 = 0;
    }

    workB = (u32 *)obj->data;

    workB[0x04 / 4]++;
    workB[0x14 / 4]++;
    workB[0x1C / 4]++;
    workB[0x24 / 4]++;

    if (lbl_1_bss_8 == -1 && workB[0x00 / 4] == 1 && workB[0x14 / 4] == 0x12) {
        lbl_1_bss_8 = HuAudBGMPlay(0x54);
    }

    if (omSysExitReq != 0 && workB[0x00 / 4] != 5) {
        workB[0x00 / 4] = 4;

        if (lbl_1_bss_8 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_8, 0x3E8);
            lbl_1_bss_8 = -1;
        }

        HuAudFadeOut(0x3E8);

        workC = (u32 *)obj->data;

        if ((s32)workC[0x08 / 4] == 0) {
            switch (workC[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workC[0x00 / 4] = 1;
                workC[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workC[0x00 / 4] = 2;
                workC[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workC[0x00 / 4] = 3;
                workC[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workC[0x08 / 4] = 1;
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workC[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workC[0x04 / 4] = 0;
        }
    }

    if (fn_1_1940() != 0 && workA[0x5C / 4] == 0x1111) {
        workA[0x18 / 4] = 4;
        workA[0x1C / 4] = 0;

        workD = (u32 *)obj->data;

        if ((s32)workD[0x08 / 4] == 0) {
            switch (workD[0x00 / 4]) {
            case 0:
                WipeCreate(1, 5, 60);
                workD[0x00 / 4] = 1;
                workD[0x14 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_1F04(lbl_1_bss_1C);
                break;
            case 1:
                workD[0x00 / 4] = 2;
                workD[0x1C / 4] = 0;
                obj->objFunc = fn_1_EF8;
                break;
            case 2:
                workD[0x00 / 4] = 3;
                workD[0x24 / 4] = 0;
                obj->objFunc = (OMOBJFUNC)fn_1_2108(lbl_1_bss_1C, obj);
                break;
            case 3:
            case 4:
                workD[0x08 / 4] = 1;
                workD[0x00 / 4] = 5;
                obj->objFunc = fn_1_970;
                break;
            case 5:
            default:
                workD[0x00 / 4] = 5;
                obj->objFunc = fn_1_CA8;
                break;
            }

            workD[0x04 / 4] = 0;
        }
    }
}

s32 fn_1_1940(void)
{
    return GameMesStatGet(lbl_1_bss_4) == 0;
}
