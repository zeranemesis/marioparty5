#include "REL/m569Dll.h"

#include "game/charman.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/pad.h"
#include "game/memory.h"

#include "math.h"
#include "string.h"

/* ---------------------------------------------------------------- data */

static unsigned int lbl_1_data_170[9] = {
    0x8C0000, 0x8C0001, 0x8C0002, 0x8C000B, 0x8C0030, 0x8C0031, 0x8C002D, 0x8C002E, 0
};
static f32 lbl_1_data_194[4] = { -600.0f, 600.0f, 750.0f, 900.0f };
static HuVecF lbl_1_data_1A4[4] = {
    { -400.0f, 0.0f, 0.0f },
    { 400.0f, 0.0f, 0.0f },
    { 400.0f, 0.0f, 0.0f },
    { 400.0f, 0.0f, 0.0f }
};
static HuVecF lbl_1_data_1D4[4] = {
    { -400.0f, 0.0f, -300.0f },
    { 260.0f, 0.0f, -450.0f },
    { 330.0f, 0.0f, -300.0f },
    { 400.0f, 0.0f, -150.0f }
};
static HuVecF lbl_1_data_204[4] = {
    { -300.0f, 0.0f, -50.0f },
    { -100.0f, 0.0f, -50.0f },
    { 100.0f, 0.0f, -50.0f },
    { 300.0f, 0.0f, -50.0f }
};

/* ------------------------------------------------------------- functions */

static void fn_1_3F50(OMOBJ *obj);

void fn_1_3A48(void)
{
    s16 i;
    s32 j;
    OMOBJ *obj;
    M569PLAYER *player;
    s16 mdl;
    s16 mdl2;
    unsigned int dataNum;

    omMakeGroupEx(lbl_1_bss_D4, 0, 4);
    lbl_1_bss_F8 = omGetGroupMemberListEx(lbl_1_bss_D4, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 2, 8, 0, fn_1_3F50);
        lbl_1_bss_F8[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569PLAYER), HU_MEMNUM_OVL);
        player = (M569PLAYER *)obj->data;
        fn_1_32AC(player, i);
        player->action = 0;
        player->state = 0;
        player->voiceF = 1;
        mdl = CharModelMotListCreate(player->charNo, 4, lbl_1_data_170, obj->mtnId);
        obj->mdlId[0] = mdl;
        Hu3DModelShadowSet(mdl);
        CharModelAttrSet(player->charNo, 0x40000001);
        player->motNo = -1;
        dataNum = charDataDirTbl[player->charNo][3];
        mdl2 = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
        HuDataDirClose(dataNum);
        for (j = 0; j < 8; j++) {
            player->jointMot[j] = Hu3DJointMotion(mdl2, HuAR_ARAMtoMRAMFileRead((u16)lbl_1_data_170[j] | charDataDirTbl[player->charNo][4], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        Hu3DModelScaleSet(mdl2, 1.0f, -1.0f, 1.0f);
        Hu3DModelAttrSet(mdl2, 0x800000);
        Hu3DMotionSet(mdl2, player->jointMot[0]);
        obj->mdlId[1] = mdl2;
        fn_1_4B8C(obj, 0);
    }
    CharEffectLayerSet(3);
    fn_1_4D74();
    for (j = 0; j < 4; j++) {
        player = (M569PLAYER *)lbl_1_bss_F8[j]->data;
        player->pos.x = lbl_1_data_194[player->slot];
        player->pos.z = 0.0f;
        omSetTra(lbl_1_bss_F8[j], player->pos.x, player->pos.y, player->pos.z);
        if (player->slot == 0) {
            player->row = 0;
            player->aiRow = frandmod(3);
            player->aiWait = 0;
        } else {
            player->row = player->slot - 1;
            player->aiRow = player->row;
            player->aiWait = frandmod(0x5A) + 0x1E;
        }
    }
}

s32 fn_1_3D68(OMOBJ *obj)
{
    M569PLAYER *player;
    M569PLAYER *other;
    HuVecF pos;
    HuVecF ang;
    s32 ok;
    s32 i;

    player = (M569PLAYER *)obj->data;
    pos = player->pos;
    ok = fn_1_3378(&pos, &player->goal, &ang, 10.0f);
    for (i = 0; i < 4; i++) {
        other = (M569PLAYER *)lbl_1_bss_F8[i]->data;
        if (player != other && fn_1_360C(&player->pos, &pos, &other->pos, 100.0f) != 0) {
            ok = 0;
        }
    }
    player->rot.y = fn_1_349C(player->rot.y, ang.y, 15.0f);
    player->pos = pos;
    return ok;
}

void fn_1_3E68(OMOBJ *obj)
{
    M569PLAYER *player;

    player = (M569PLAYER *)obj->data;
    switch (obj->work[0]) {
    case 0:
        obj->work[0] = 1;
        player->goal = lbl_1_data_1A4[player->slot];
        CharModelAttrSet(player->charNo, 0x40000001);
        fn_1_4B8C(obj, 2);
        break;
    case 1:
        if (fn_1_3378(&player->pos, &player->goal, &player->rot, 10.0f) != 0) {
            if (player->slot == 0) {
                fn_1_5138(player->objId);
            } else {
                fn_1_4F14(player->objId);
            }
        }
        break;
    }
}

static void fn_1_3F50(OMOBJ *obj)
{
    M569PLAYER *player;

    player = (M569PLAYER *)obj->data;
    CharModelVoicePanSet(player->charNo, fn_1_1014(&player->pos));
    switch (player->state) {
    case 1:
        fn_1_3E68(obj);
        break;
    case 2:
        fn_1_454C(obj);
        break;
    case 3:
        if (fn_1_3D68(obj) != 0) {
            fn_1_4B8C(obj, 0);
            player->state = 0;
        }
        break;
    case 4:
        if (fn_1_3D68(obj) != 0) {
            fn_1_4B8C(obj, 0);
            player->state = 5;
        }
        break;
    case 5:
        player->rot.y = fn_1_349C(player->rot.y, player->ang.y, 15.0f);
        if (fabs(player->rot.y - player->ang.y) <= 0.1f) {
            player->rot.y = player->ang.y;
            player->state = 0;
        }
        break;
    }
    Hu3DModelPosSetV(obj->mdlId[1], &player->pos);
    Hu3DModelRotSetV(obj->mdlId[1], &player->rot);
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
}

s32 fn_1_433C(M569PLAYER *player)
{
    if (player->action != 0) {
        return 0;
    }
    if (fn_1_848C(player->row) != 1) {
        return 0;
    }
    if (player->type != 0) {
        if (player->aiWait == 0) {
            if (player->row == player->aiRow) {
                return 1;
            }
        } else {
            player->aiWait--;
        }
        return 0;
    }
    if (HuPadBtnDown[player->padNo] & 0x100) {
        return 1;
    }
    return 0;
}

s32 fn_1_4404(M569PLAYER *player)
{
    if (player->type != 0) {
        if (player->action > 1) {
            return 0;
        }
        if (player->aiRow == -1) {
            return 0;
        }
        if (player->row < player->aiRow) {
            player->action = 5;
            return 1;
        }
        if (player->row > player->aiRow) {
            player->action = 4;
            return 1;
        }
        return 0;
    }
    if (player->grp != 0) {
        return 0;
    }
    if ((HuPadDStk[player->padNo] & 2) && (player->action <= 1 || player->action == 4) && player->row != 2) {
        player->action = 5;
        return 1;
    }
    if ((HuPadDStk[player->padNo] & 1) && (player->action <= 1 || player->action == 5) && player->row != 0) {
        player->action = 4;
        return 1;
    }
    return 0;
}

void fn_1_454C(OMOBJ *obj)
{
    M569PLAYER *player;
    s32 busy;
    s32 press;
    s32 moved;
    s32 mot;

    player = (M569PLAYER *)obj->data;
    busy = fn_1_85C0();
    if (busy != 0 && player->action == 0) {
        player->state = 0;
        player->aiRow = -1;
        player->action = 0;
        fn_1_4B8C(obj, 0);
        player->coin = fn_1_86C4();
        return;
    }
    if (player->action == 2) {
        if (fn_1_848C(player->row) == 1 && CharModelTimingHookNoGet(player->charNo) != 0) {
            omVibrate(player->objId, 12, 12, 0);
            fn_1_839C(player->row);
            if (player->slot == 0) {
                player->aiRow = fn_1_84A8();
            } else {
                player->aiRow = -1;
            }
        }
        if (CharMotionEndCheck(player->charNo) != 0) {
            fn_1_4B8C(obj, 0);
            player->action = 0;
        }
        return;
    }
    press = fn_1_433C(player);
    if (press != 0) {
        fn_1_4B8C(obj, 3);
        player->action = 2;
        return;
    }
    mot = -1;
    if ((player->action == 4 || player->action == 5) && fn_1_3378(&player->pos, &player->goal, &player->ang, 10.0f) != 0) {
        player->action = 1;
        mot = 0;
    }
    moved = fn_1_4404(player);
    if (moved != 0 && busy == 0) {
        if (player->action == 5) {
            player->row++;
        } else {
            player->row--;
        }
        player->goal = player->pos;
        player->goal.x = lbl_1_data_32C[player->row].x;
        mot = 2;
    }
    if (mot != -1) {
        fn_1_4B8C(obj, mot);
    }
    if (player->action == 1) {
        player->ang.y = 0.0f;
        if (fabs(player->rot.y - player->ang.y) <= 0.1f) {
            player->rot.y = player->ang.y;
            player->action = 0;
        }
    }
    player->rot.y = fn_1_349C(player->rot.y, player->ang.y, 30.0f);
}

static void fn_1_49DC(OMOBJ *obj)
{
    M569PLAYER *player;
    s32 se;
    s16 time;
    s32 pan;

    player = (M569PLAYER *)lbl_1_bss_F8[obj->work[0]]->data;
#ifdef TARGET_PC
    se = 0;
    time = 0;
#endif
    switch (player->motNo) {
    case 4:
        se = 0x1F8;
        time = CharMotionTotalTimeGet(player->charNo, lbl_1_data_170[player->motNo]);
        break;
    case 6:
        se = 0x1FB;
        time = CharMotionTotalTimeGet(player->charNo, lbl_1_data_170[player->motNo]);
        break;
    case 7:
        se = 0x200;
        time = CharMotionExtraTimeGet(player->charNo, lbl_1_data_170[player->motNo]);
        break;
    }
    pan = fn_1_1014(&player->pos);
    if (Hu3DMotionTimeGet(lbl_1_bss_F8[obj->work[0]]->mdlId[0]) >= (f32)time
        && Hu3DMotionShiftIDGet(lbl_1_bss_F8[obj->work[0]]->mdlId[0]) < 0) {
        CharFXPlayPan(player->charNo, se, pan);
        omDelObjEx(lbl_1_bss_D4, obj);
    }
}

void fn_1_4B8C(OMOBJ *obj, s32 motNo)
{
    M569PLAYER *player;
    s16 charNo;
    f32 shift;
    f32 start;
    u32 attr;
    OMOBJ *hook;

    player = (M569PLAYER *)obj->data;
    charNo = player->charNo;
    start = 0.0f;
    shift = 10.0f;
    attr = 0x40000001;
    if (player->motNo != motNo) {
        player->motNo = motNo;
        switch (motNo) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            attr = 0;
            shift = 0.0f;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            CharMotionVoiceOnSet(player->charNo, (s16)lbl_1_data_170[motNo], 0);
            attr = 0;
            shift = 0.0f;
            if (player->voiceF != 0) {
                hook = omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, -1, fn_1_49DC);
                hook->work[0] = player->objId;
            }
            break;
        }
        if (shift != 0.0f) {
            CharMotionShiftSet(charNo, obj->mtnId[motNo], start, shift, attr);
            Hu3DMotionShiftSet(obj->mdlId[1], player->jointMot[motNo], start, shift, attr);
            return;
        }
        CharModelAttrReset(charNo, 0x40000003);
        CharMotionSet(charNo, obj->mtnId[motNo]);
        Hu3DModelAttrReset(obj->mdlId[1], 0x40000003);
        Hu3DMotionSet(obj->mdlId[1], player->jointMot[motNo]);
    }
}

void fn_1_4D74(void)
{
    s32 i;
    s32 j;
    s32 r;
    M569PLAYER *player;

    for (i = 0; i < 2; i++) {
        memset(&lbl_1_bss_D8[i], 0, sizeof(M569TEAM));
        for (j = 0; j < 3; j++) {
            lbl_1_bss_D8[i].member[j] = -1;
        }
    }
    for (i = 0; i < 4; i++) {
        player = (M569PLAYER *)lbl_1_bss_F8[i]->data;
        if (player->grp == 0) {
            lbl_1_bss_D8[0].member[0] = player->objId;
            player->slot = 0;
            player->row = 0;
        } else {
            do {
                r = frandmod(3);
            } while (lbl_1_bss_D8[1].member[r] != -1);
            lbl_1_bss_D8[1].member[r] = player->objId;
            player->slot = r + 1;
            player->row = r;
        }
    }
}

void fn_1_4EB4(s32 idx, s32 state)
{
    M569PLAYER *player = (M569PLAYER *)lbl_1_bss_F8[idx]->data;

    player->state = state;
}

s32 fn_1_4EE4(s32 idx)
{
    M569PLAYER *player = (M569PLAYER *)lbl_1_bss_F8[idx]->data;

    return player->state;
}

void fn_1_4F14(s32 idx)
{
    M569PLAYER *player;

    player = (M569PLAYER *)lbl_1_bss_F8[idx]->data;
    player->goal = lbl_1_data_1D4[player->slot];
    player->ang.x = 0.0f;
    player->ang.y = (f32)(180.0 * (atan2(lbl_1_bss_34.pos.x - player->goal.x, lbl_1_bss_34.pos.z - player->goal.z) / 3.141592653589793));
    if (player->ang.y < 0.0f) {
        player->ang.y += 360.0f;
    }
    player->ang.z = 0.0f;
    player->state = 4;
    fn_1_4B8C(lbl_1_bss_F8[idx], 2);
}

void fn_1_5138(s32 idx)
{
    M569PLAYER *player;

    player = (M569PLAYER *)lbl_1_bss_F8[idx]->data;
    player->goal = lbl_1_data_32C[player->row];
    player->goal.y = 0.0f;
    player->ang.x = 0.0f;
    player->ang.y = 0.0f;
    player->ang.z = 0.0f;
    player->state = 4;
    fn_1_4B8C(lbl_1_bss_F8[idx], 2);
}

void fn_1_52F8(s32 idx)
{
    M569PLAYER *player;

    player = (M569PLAYER *)lbl_1_bss_F8[idx]->data;
    player->goal = lbl_1_data_204[player->slot];
    player->ang.x = 0.0f;
    player->ang.y = 0.0f;
    player->ang.z = 0.0f;
    player->state = 4;
    fn_1_4B8C(lbl_1_bss_F8[idx], 2);
}

void fn_1_54A8(s32 team)
{
    s32 i;
    s32 score;
    s32 id;
    M569PLAYER *player;

    score = lbl_1_bss_D8[team].score;
    for (i = 0; i < 3; i++) {
        id = lbl_1_bss_D8[team].member[i];
        if (id != -1) {
            if (score > 0) {
                fn_1_4B8C(lbl_1_bss_F8[id], 6);
            } else {
                player = (M569PLAYER *)lbl_1_bss_F8[id]->data;
                fn_1_4B8C(lbl_1_bss_F8[id], 7);
            }
        }
    }
}
