#include "REL/m502dll.h"

#define M502_PAN(no) ((no) == 0 ? 0x28 : (no) == 1 ? 0x38 : (no) == 2 ? 0x48 : 0x58)

s32 fn_1_3D50(M502PLRSUB *plr);
s32 fn_1_3FA0(M502PLRSUB *plr);
s32 fn_1_402C(M502PLRSUB *plr);
void fn_1_40D8(M502PLRSUB *plr);
void fn_1_44B8(M502PLRSUB *plr);
void fn_1_46CC(M502PLRSUB *plr);
void fn_1_4874(M502PLRSUB *plr);
void fn_1_4A1C(M502PLRSUB *plr);
s32 fn_1_4E70(M502PLRSUB *plr);
void fn_1_5384(M502PLRSUB *plr);
void fn_1_5510(M502PLRSUB *plr);
void fn_1_561C(M502PLRSUB *plr, s16 *lvl, s16 *row, s16 *col);

static s16 lbl_1_data_C38[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0 };

/* motion data numbers per character (11 motions each) */
static u32 lbl_1_data_C54[13][11] = {
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C0003, 0x8C0004, 0x870055, 0x8C0008, 0x8C0009, 0x8C000A, 0x8C001D, 0x8C0007 },
    { 0x330000, 0x330001, 0x330002, 0x330003, 0x330004, 0x2E0055, 0x330008, 0x330009, 0x33000A, 0x33001D, 0x330007 },
    { 0xB70000, 0xB70001, 0xB70002, 0xB70003, 0xB70004, 0xB20055, 0xB70008, 0xB70009, 0xB7000A, 0xB7001D, 0xB70007 },
    { 0xED0000, 0xED0001, 0xED0002, 0xED0003, 0xED0004, 0xE80055, 0xED0008, 0xED0009, 0xED000A, 0xED001D, 0xED0007 },
    { 0xE60000, 0xE60001, 0xE60002, 0xE60003, 0xE60004, 0xE10055, 0xE60008, 0xE60009, 0xE6000A, 0xE6001D, 0xE60007 },
    { 0x170000, 0x170001, 0x170002, 0x170003, 0x170004, 0x120055, 0x170008, 0x170009, 0x17000A, 0x17001D, 0x170007 },
    { 0xE00000, 0xE00001, 0xE00002, 0xE00003, 0xE00004, 0xDB0055, 0xE00008, 0xE00009, 0xE0000A, 0xE0001D, 0xE00007 },
    { 0x2D0000, 0x2D0001, 0x2D0002, 0x2D0003, 0x2D0004, 0x280055, 0x2D0008, 0x2D0009, 0x2D000A, 0x2D001D, 0x2D0007 },
    { 0xC90000, 0xC90001, 0xC90002, 0xC90003, 0xC90004, 0xC40055, 0xC90008, 0xC90009, 0xC9000A, 0xC9001D, 0xC90007 },
    { 0xB00000, 0xB00001, 0xB00002, 0xB00003, 0xB00004, 0x9C0055, 0xB00008, 0xB00009, 0xB0000A, 0xB0001D, 0xB00007 },
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C0003, 0x8C0004, 0x870055, 0x8C0008, 0x8C0009, 0x8C000A, 0x8C001D, 0x8C0007 },
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C0003, 0x8C0004, 0x870055, 0x8C0008, 0x8C0009, 0x8C000A, 0x8C001D, 0x8C0007 },
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C0003, 0x8C0004, 0x870055, 0x8C0008, 0x8C0009, 0x8C000A, 0x8C001D, 0x8C0007 },
};
static f32 lbl_1_data_E90 = -9300.0f;
static f32 lbl_1_data_E94 = -8850.0f;

void fn_1_38E0(M502PLRSUB *plr, s32 no, M502PLAYER *player)
{
    M502MAIN *m = player->main;
    OMOBJ *obj;
    s16 mtn;
    u16 i;

    memset(plr, 0, sizeof(M502PLRSUB));
    plr->player = player;
    plr->playerNo = no;
    plr->grp = GwPlayerConf[plr->playerNo].grp;
    plr->charNo = GwPlayerConf[plr->playerNo].charNo;
    obj = omAddObjEx(m->objman, 0x64, 1, 0xB, -1, NULL);
    plr->obj = obj;
    plr->active = 0;
    *obj->mdlId = CharModelCreate((s16)plr->charNo, 2);
    CharModelStepFxSet((s16)plr->charNo, 0);
    CharEffectLayerSet(4);
    for (i = 0; i < 0xB; i++) {
        mtn = CharMotionCreate((s16)plr->charNo, lbl_1_data_C54[plr->charNo][i]);
        obj->mtnId[i] = mtn;
        CharMotionSet((s16)plr->charNo, obj->mtnId[i]);
    }
    Hu3DModelAttrSet(*obj->mdlId, 1);
    Hu3DModelAttrSet(*obj->mdlId, 0x40000001);
    Hu3DModelLayerSet(*obj->mdlId, 3);
    Hu3DMotionSet(*obj->mdlId, *obj->mtnId);
    Hu3DModelCameraSet(*obj->mdlId, (u16)lbl_1_data_B60[no]);
    omSetTra(obj, 0.0f, 0.0f, 0.0f);
    omSetRot(obj, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(*obj->mdlId, 1);
    Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
    fn_1_6400(&plr->input, no, plr);
    plr->lane = 1;
    CharMotionUpdateSet((s16)plr->charNo, lbl_1_data_C54[0][4], 0);
    CharMotionUpdateSet((s16)plr->charNo, lbl_1_data_C54[0][3], 0);
}

void fn_1_3B88(M502PLRSUB *plr)
{
    fn_1_6478(&plr->input);
    if (plr->obj != NULL) {
        CharModelKill((s16)plr->charNo);
        omDelObjEx(HuPrcCurrentGet(), plr->obj);
        plr->obj = NULL;
    }
    memset(plr, 0, sizeof(M502PLRSUB));
}

s32 fn_1_3BF4(M502PLRSUB *plr)
{
    M502MAIN *m = plr->player->main;
    s32 ret;

    if (m->state == 7) {
        plr->active = 0;
    }
    ret = 0;
    if (plr->active != 0) {
        fn_1_64B8(&plr->input);
    }
    else {
        plr->input.stkX = 0;
        plr->input.btnDown = 0;
        plr->input.btn = 0;
    }
    switch (plr->mode) {
    case 0:
        CharModelVoicePanSet((s16)plr->charNo, 0x40);
        if (m->state >= 2 && m->state != 8) {
            plr->active = 1;
            plr->mode = 1;
        }
        break;
    case 1:
        CharModelVoicePanSet((s16)plr->charNo, M502_PAN(plr->playerNo));
        if (fn_1_3D50(plr) != 0) {
            ret = 1;
        }
        break;
    case 2:
        fn_1_3D50(plr);
        break;
    }
    return ret;
}

s32 fn_1_3D50(M502PLRSUB *plr)
{
    M502MAIN *m = plr->player->main;
    M502STAGE *stage = &plr->player->stage;
    s16 lvl;
    s16 row;
    s16 col;
    s32 ret;
    s32 st;

    ret = 0;
    fn_1_561C(plr, &lvl, &row, &col);
    switch (plr->phase) {
    case 4:
        break;
    case 0:
        if (fn_1_402C(plr) != 0 && row > 0) {
            plr->phase = 1;
        }
        break;
    case 1:
        if (fn_1_3FA0(plr) != 0 && row <= 0) {
            plr->phase = 0;
        }
        break;
    case 2:
        fn_1_40D8(plr);
        break;
    case 3:
        Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[10], 0.0f, 4.0f, 0);
        plr->phase = 4;
        break;
    }
    if (plr->phase < 2 && plr->obj->trans.y < -9420.0f) {
        fn_1_218(fn_1_198(0x630), M502_PAN(plr->playerNo));
        fn_1_3298(stage);
        plr->phase = 2;
        plr->step = 0;
        plr->unkC = 0;
        plr->vel = -36.5f;
        plr->flag74 = 0;
        ret = 1;
        if (m->unk5008 == 0) {
            m->vibTimer = 0;
            m->unk5008 = 1;
        }
    }
    if (plr->phase < 2) {
        st = m->state;
        if ((st >= 0 && st < 4) || (st >= 7 && st < 10) || st == 11 || st == 12) {
            fn_1_5510(plr);
        }
    }
    return ret;
}

s32 fn_1_3FA0(M502PLRSUB *plr)
{
    s32 ret = 0;

    switch (plr->action) {
    case 0:
        fn_1_44B8(plr);
        break;
    case 3:
        fn_1_4A1C(plr);
        break;
    case 4:
        if (fn_1_4E70(plr)) {
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_402C(M502PLRSUB *plr)
{
    s32 ret = 0;

    switch (plr->action) {
    case 0:
        fn_1_44B8(plr);
        break;
    case 1:
        fn_1_46CC(plr);
        break;
    case 2:
        fn_1_4874(plr);
        break;
    case 3:
        fn_1_4A1C(plr);
        break;
    case 4:
        if (fn_1_4E70(plr)) {
            ret = 1;
        }
        break;
    }
    return ret;
}

void fn_1_40D8(M502PLRSUB *plr)
{
    M502MAIN *m = plr->player->main;
    f32 d;
    f32 t;
    f32 c;
    f32 f;

    switch (plr->step) {
    case 0:
        d = lbl_1_data_E90 - plr->obj->trans.y;
        if (d >= 0.0f) {
            f = d;
        }
        else {
            f = -d;
        }
        t = f / 150.0f;
        if (t > 1.0f) {
            c = 1.0f;
        }
        else if (t < 0.0f) {
            c = 0.0f;
        }
        else {
            c = t;
        }
        if (plr->obj->trans.y < lbl_1_data_E90) {
            plr->vel = plr->vel - (-2.4333334f * c);
            plr->vel = plr->vel * 0.9f;
        }
        else {
            plr->vel = plr->vel + (-4.866667f * c);
            plr->vel = plr->vel * 0.8f;
        }
        plr->vel = plr->vel - (-0.24333334f * c);
        plr->obj->trans.y = plr->obj->trans.y + plr->vel;
        if (plr->vel > 0.0f && plr->flag74 == 0) {
            Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[9], 0.0f, 4.0f, 0x40000001);
            plr->flag74 = 1;
        }
        if (lbl_1_data_0->versus == 0) {
            if (plr->unkC++ > 0xB4) {
                plr->obj->trans.y = lbl_1_data_E90;
                plr->unkC = 0;
                plr->step++;
                fn_1_218(fn_1_198(0x632), M502_PAN(plr->playerNo));
                return;
            }
        }
        return;
    case 1:
        plr->unkC++;
        t = 0.8f * ((f32)plr->unkC / 60.0f);
        fn_1_2E08(&plr->player->stage, t);
        plr->obj->trans.y = plr->obj->trans.y + 280.0f * t;
        if (plr->unkC > 0x3C) {
            plr->unkC = 0;
            plr->step++;
        }
        break;
    }
}

void fn_1_44B8(M502PLRSUB *plr)
{
    OMOBJ *obj = plr->obj;

    Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
    if (plr->phase == 0) {
        if (plr->input.stkX < 0 && plr->lane > 0) {
            Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[2], 0.0f, 4.0f, 0x40000001);
            plr->lane--;
            plr->action = 1;
        }
        else if (plr->input.stkX > 0 && plr->lane < 2) {
            Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[2], 0.0f, 4.0f, 0x40000001);
            plr->lane++;
            plr->action = 2;
        }
    }
    if ((plr->input.btnDown & 0x100) != 0) {
        Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[3], 0.0f, 2.0f, 0);
        plr->jumpVel = 1300.0f;
        plr->unkC = 0;
        plr->action = 3;
        plr->hold = 0;
        plr->landing = 0;
    }
    if (plr->obj->rot.y < 0.0f) {
        plr->obj->rot.y = plr->obj->rot.y + 15.0f;
    }
    if (plr->obj->rot.y > 0.0f) {
        plr->obj->rot.y = plr->obj->rot.y - 15.0f;
    }
}

void fn_1_46CC(M502PLRSUB *plr)
{
    f32 v;

    plr->obj->trans.x = plr->obj->trans.x - 10.0f;
    plr->obj->rot.y = plr->obj->rot.y - 15.0f;
    if (plr->obj->rot.y < -90.0f) {
        v = -90.0f;
    }
    else {
        v = plr->obj->rot.y;
    }
    plr->obj->rot.y = v;
    if (plr->input.stkX > 0 && plr->lane < 2) {
        plr->lane++;
        plr->action = 2;
        return;
    }
    if (plr->obj->trans.x < -300.0f + 300.0f * (f32)plr->lane) {
        if (plr->input.stkX < 0 && plr->lane > 0) {
            plr->lane--;
            return;
        }
        Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[0], 0.0f, 4.0f, 0x40000001);
        plr->action = 0;
    }
}

void fn_1_4874(M502PLRSUB *plr)
{
    f32 v;

    plr->obj->trans.x = plr->obj->trans.x + 10.0f;
    plr->obj->rot.y = plr->obj->rot.y + 15.0f;
    if (plr->obj->rot.y > 90.0f) {
        v = 90.0f;
    }
    else {
        v = plr->obj->rot.y;
    }
    plr->obj->rot.y = v;
    if (plr->input.stkX < 0 && plr->lane > 0) {
        plr->lane--;
        plr->action = 1;
        return;
    }
    if (plr->obj->trans.x > -300.0f + 300.0f * (f32)plr->lane) {
        if (plr->input.stkX > 0 && plr->lane < 2) {
            plr->lane++;
            return;
        }
        Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[0], 0.0f, 4.0f, 0x40000001);
        plr->action = 0;
    }
}

void fn_1_4A1C(M502PLRSUB *plr)
{
    Vec p = { 0.0f, 0.0f, 0.0f };
    Vec q;
    f32 v;

    if (plr->unkC == 6) {
        if ((plr->input.btn & 0x100) != 0) {
            plr->hold = 1;
        }
        else {
            plr->hold = 0;
        }
    }
    if (plr->hold != 0) {
        plr->jumpVel = plr->jumpVel + -26.66666805744171;
    }
    else {
        plr->jumpVel = plr->jumpVel + -120.0000062584877;
    }
    if (plr->jumpVel < -750.0f) {
        v = -750.0f;
    }
    else {
        v = plr->jumpVel;
    }
    plr->jumpVel = v;
    plr->obj->trans.y = plr->obj->trans.y + 1.35f * (0.016666668f * plr->jumpVel);
    fn_1_5384(plr);
    if (plr->unkC >= 1 && (plr->input.btnDown & 0x140) != 0 && plr->obj->trans.y > plr->landY) {
        Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[6], 0.0f, 8.0f, 0);
        plr->unkC = 0;
        plr->action = 4;
        plr->step = 0;
        plr->startY = plr->obj->trans.y;
    }
    if (plr->landing != 0) {
        if (plr->obj->trans.y <= plr->landY) {
            plr->obj->trans.y = plr->landY;
            if (plr->input.stkX < 0 && plr->lane > 0 && plr->phase == 0) {
                Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[2], 0.0f, 4.0f, 0x40000001);
                plr->lane--;
                plr->landing = 0;
                plr->action = 1;
            }
            else if (plr->input.stkX > 0 && plr->lane < 2 && plr->phase == 0) {
                Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[2], 0.0f, 4.0f, 0x40000001);
                plr->lane++;
                plr->landing = 0;
                plr->action = 2;
            }
            else {
                if (plr->obj->trans.y <= plr->landY) {
                    Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[0], 0.0f, 4.0f, 0x40000001);
                    p.x = plr->obj->trans.x;
                    p.y = plr->obj->trans.y - 10.0f;
                    p.z = plr->obj->trans.z;
                    q = p;
                    CharModelLandDustCreate((s16)plr->charNo, &q);
                    CharFXPlay((s16)plr->charNo, 0x96);
                    plr->landing = 0;
                    plr->action = 0;
                }
            }
        }
    }
    if (plr->obj->rot.y < 0.0f) {
        plr->obj->rot.y = plr->obj->rot.y + 15.0f;
    }
    if (plr->obj->rot.y > 0.0f) {
        plr->obj->rot.y = plr->obj->rot.y - 15.0f;
    }
    plr->unkC++;
}

s32 fn_1_4E70(M502PLRSUB *plr)
{
    s16 lvl;
    s16 row;
    s16 col;
    f32 d;
    f32 f;
    s16 n;
    s32 ret;

    ret = 0;
    switch (plr->step) {
    case 0:
        plr->unkC = plr->unkC + 1;
        if (plr->unkC >= 0xF && Hu3DMotionEndCheck(*plr->obj->mdlId) != 0) {
            plr->step = 1;
        }
        break;
    case 1:
        if (plr->unk70 != 0) {
            plr->unk70 = plr->unk70 - 1;
        }
        else {
            plr->obj->trans.y = plr->obj->trans.y + -36.5f;
        }
        fn_1_5384(plr);
        if (plr->landing != 0) {
            if (plr->obj->trans.y <= plr->landY) {
                plr->obj->trans.y = plr->landY;
                plr->step = 2;
                fn_1_561C(plr, &lvl, &row, &col);
                d = plr->startY - plr->obj->trans.y;
                if (d >= 0.0f) {
                    f = d;
                }
                else {
                    f = -d;
                }
                if (f > 220.0f) {
                    n = 3;
                }
                else if (f > 145.0f) {
                    n = 2;
                }
                else {
                    n = 1;
                }
                plr->unk78 = n;
                if (fn_1_70B0(&plr->player->block, lvl, row, col, n) != 0) {
                    plr->landing = 0;
                    plr->step = 3;
                }
                else {
                    Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[8], 0.0f, 0.0f, 0);
                }
            }
        }
        break;
    case 2:
        if (Hu3DMotionEndCheck(*plr->obj->mdlId) != 0) {
            if (plr->input.stkX < 0 && plr->lane > 0 && plr->phase == 0) {
                Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[2], 0.0f, 4.0f, 0x40000001);
                plr->lane--;
                plr->landing = 0;
                plr->action = 1;
            }
            else if (plr->input.stkX > 0 && plr->lane < 2 && plr->phase == 0) {
                Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[2], 0.0f, 4.0f, 0x40000001);
                plr->lane++;
                plr->landing = 0;
                plr->action = 2;
            }
            else {
                if (plr->obj->trans.y <= plr->landY) {
                    Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[0], 0.0f, 4.0f, 0x40000001);
                    plr->landing = 0;
                    plr->action = 0;
                }
            }
        }
        break;
    case 3:
        if (plr->unk70 != 0) {
            plr->unk70 = plr->unk70 - 1;
        }
        else {
            plr->obj->trans.y = plr->obj->trans.y + -36.5f;
        }
        fn_1_5384(plr);
        if (plr->landing != 0) {
            if (plr->obj->trans.y <= plr->landY) {
                plr->obj->trans.y = plr->landY;
                Hu3DMotionShiftSet(*plr->obj->mdlId, plr->obj->mtnId[8], 0.0f, 0.0f, 0);
                ret = 1;
                plr->step = 2;
            }
        }
        break;
    }
    if (plr->obj->rot.y < 0.0f) {
        plr->obj->rot.y = plr->obj->rot.y + 15.0f;
    }
    if (plr->obj->rot.y > 0.0f) {
        plr->obj->rot.y = plr->obj->rot.y - 15.0f;
    }
    plr->unkC = plr->unkC + 1;
    return ret;
}

void fn_1_5384(M502PLRSUB *plr)
{
    Vec zero = { 0.0f, 0.0f, 0.0f };
    Vec out;
    s16 lvl;
    s16 row;
    s16 col;
    M502BLOCK *b = &plr->player->block;
    M502CELL *c;
    s32 idx;

    out = zero;
    if (plr->landing == 0) {
        fn_1_561C(plr, &lvl, &row, &col);
        if (lvl < 3) {
            idx = col + row * 3;
#ifdef TARGET_PC
            if (idx < 0 || idx >= 27) {
                return;
            }
#endif
            c = &b->cell[lvl][idx];
            if (c->kind != 0 && c->timerC >= 10000) {
                fn_1_6CFC(lvl, row, col, &out);
                plr->landing = 1;
                plr->landY = out.y;
            }
        }
        else if (lvl == 7) {
            plr->landing = 1;
            plr->landY = 300.0f * (f32)(s16)(plr->obj->trans.y / 300.0f);
        }
    }
}

void fn_1_5510(M502PLRSUB *plr)
{
    M502CAMERA *cam = &plr->player->camera;
    f32 x;

    x = plr->obj->trans.x;
    cam->tgtGoal.x = x;
    cam->posGoal.x = x;
    cam->tgt.x = x;
    cam->pos.x = x;
    if (plr->obj->trans.y > lbl_1_data_E94) {
        cam->tgtGoal.y = plr->obj->trans.y - 70.0f;
        cam->posGoal.y = 300.0f + plr->obj->trans.y;
    }
    else {
        cam->tgtGoal.y = lbl_1_data_E94 - 70.0f;
        cam->posGoal.y = 300.0f + lbl_1_data_E94;
    }
    if (plr->phase == 0) {
        cam->posGoal.z = 4550.0f;
        return;
    }
    cam->posGoal.z = 3500.0f;
}

void fn_1_561C(M502PLRSUB *plr, s16 *lvl, s16 *row, s16 *col)
{
    Vec p;

    p.x = plr->obj->trans.x;
    p.y = plr->obj->trans.y;
    p.z = plr->obj->trans.z;
    p.y -= 150.0f;
    p.x = 300.0f * (f32)(s16)(p.x / 300.0f);
    p.y = 300.0f * (f32)(s16)(p.y / 300.0f);
    p.z = 300.0f * (f32)(s16)(p.z / 300.0f);
    fn_1_6E1C(&p, lvl, row, col);
}

void fn_1_57B4(M502AI *ai, s32 playerNo, M502INPUT *in)
{
    memset(ai, 0, sizeof(M502AI));
    ai->input = in;
    ai->enabled = GwPlayerConf[playerNo].type;
}

void fn_1_5818(M502AI *ai)
{
    memset(ai, 0, sizeof(M502AI));
}

void fn_1_5848(M502AI *ai)
{
    M502INPUT *in = ai->input;

    if (ai->enabled != 0 && in->plr->mode == 1) {
        in->stkX = 0;
        in->btnDown = 0;
        in->btn = 0;
        switch (ai->step) {
        case 0:
            fn_1_58E0(ai);
            return;
        case 1:
            fn_1_5EB8(ai);
            break;
        }
    }
}
