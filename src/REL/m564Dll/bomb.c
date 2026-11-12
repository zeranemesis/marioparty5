#include "REL/m564Dll.h"

void fn_1_64B8(s16 layerNo);
void fn_1_6A14(M564_BOMB *bomb);
void fn_1_7430(M564_BOMB *bomb);
void fn_1_74C4(M564_BOMB *bomb, s16 mot);
void fn_1_76D8(OMOBJ *obj);
void fn_1_9344(OMOBJ *obj);
void fn_1_95D4(void);
s16 fn_1_9810(void);
void fn_1_98AC(s16 spot, s16 bombNo);
int fn_1_75A4(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_7674(MGACTOR *actor, int param);

MGACTOR_PARAM lbl_1_data_200 = {
    30.0f, 30.0f, 4, 5, fn_1_75A4, 0, 0, fn_1_7674,
};

s32 lbl_1_data_220[3] = { 0x770011, 0x770012, 0x770013 };

Vec lbl_1_data_22C = { 0.0f, -10.0f, 0.0f };

M564_BOMB *lbl_1_bss_1E8[15];
s16 lbl_1_bss_1E6;
s16 lbl_1_bss_1E4;
M564_SPOT lbl_1_bss_11C[10];
s16 lbl_1_bss_118;

void fn_1_60E8(void) {
    s16 i;
    s32 j;
    OMOBJ *obj;
    M564_BOMB *bomb;

    lbl_1_bss_118 = Hu3DModelCreate(HuDataSelHeapReadNum(0x770015, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_118, 1);
    Hu3DModelAttrSet(lbl_1_bss_118, 0x40000002);
    for (i = 0; i < 15; i++) {
        obj = omAddObjEx(lbl_1_bss_F8, (s16)(i + 0x1100), 0, 0, -1, fn_1_76D8);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M564_BOMB), HU_MEMNUM_OVL);
        bomb = obj->data;
        lbl_1_bss_1E8[i] = bomb;
        memset(bomb, 0, sizeof(M564_BOMB));
        bomb->no = i;
        bomb->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x770010, HU_MEMNUM_OVL, HEAP_MODEL));
        for (j = 0; j < 3; j++) {
            bomb->mot[j] = Hu3DJointMotion(bomb->mdlId, HuDataSelHeapReadNum(lbl_1_data_220[j], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        bomb->curMot = -1;
        fn_1_74C4(bomb, 0);
        Hu3DModelLayerSet(bomb->mdlId, 2);
        Hu3DModelAttrSet(bomb->mdlId, 0x40000001);
        Hu3DModelAttrSet(bomb->mdlId, 1);
        Hu3DModelScaleSet(bomb->mdlId, 0.55f, 0.55f, 0.55f);
        bomb->shadowMdl = Hu3DModelLink(lbl_1_bss_118);
        Hu3DModelLayerSet(bomb->shadowMdl, 1);
        Hu3DModelScaleSet(bomb->shadowMdl, 0.55f, 0.55f, 0.55f);
        Hu3DModelAttrSet(bomb->shadowMdl, 0x40000002);
        Hu3DModelAttrSet(bomb->shadowMdl, 1);
        bomb->actor = MgActorCreate(&lbl_1_data_200, bomb->mdlId);
        MgActorPauseOn(bomb->actor);
        lbl_1_data_200.param++;
        MgActorColAttrSet(bomb->actor, 0x40);
        bomb->state = 0;
        bomb->comF = 0;
        bomb->holder = -1;
    }
    lbl_1_bss_1E4 = 0;
    lbl_1_bss_1E6 = Hu3DModelCreate(HuDataSelHeapReadNum(0x770014, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_1E6, 1);
    Hu3DModelAttrSet(lbl_1_bss_1E6, 0x40000002);
    Hu3DModelLayerSet(lbl_1_bss_1E6, 5);
    fn_1_95D4();
    Hu3DLayerHookSet(1, fn_1_64B8);
}

s32 fn_1_6404(void) {
    s32 limit;
    u32 time;

    time = MgSeqTimerValueGet();
    if (time < 0x708U) {
        limit = 15;
    } else if (time < 0x834U) {
        limit = 13;
    } else if (time < 0x960U) {
        limit = 12;
    } else if (time < 0xA8CU) {
        limit = 11;
    } else if (time < 0xBB8U) {
        limit = 10;
    } else if (time < 0xCE4U) {
        limit = 8;
    } else {
        limit = 6;
    }
    return limit <= lbl_1_bss_1E4;
}

void fn_1_64B8(s16 layerNo) {
    s32 i;
    M564_BOMB *bomb;

    for (i = 0; i < 15; i++) {
        bomb = lbl_1_bss_1E8[i];
        if (bomb->state != 0) {
            fn_1_6A14(bomb);
            Hu3DModelPosSet(bomb->shadowMdl, bomb->pos.x, 0.1f, bomb->pos.z);
        }
    }
}

void fn_1_653C(void) {
    s32 limit;
    s32 i;
    u32 time;
    M564_BOMB *bomb;

    time = MgSeqTimerValueGet();
    if (time < 0x708U) {
        limit = 15;
    } else if (time < 0x834U) {
        limit = 13;
    } else if (time < 0x960U) {
        limit = 12;
    } else if (time < 0xA8CU) {
        limit = 11;
    } else if (time < 0xBB8U) {
        limit = 10;
    } else if (time < 0xCE4U) {
        limit = 8;
    } else {
        limit = 6;
    }
    if (lbl_1_bss_1E4 < limit) {
        bomb = NULL;
        for (i = 0; i < 15; i++) {
            if (lbl_1_bss_1E8[i]->state == 0) {
                bomb = lbl_1_bss_1E8[i];
                break;
            }
        }
        if (bomb != NULL) {
            bomb->spot = fn_1_9810();
            if (bomb->spot != -1) {
                fn_1_98AC(bomb->spot, bomb->no);
                MgActorPosUpdate(bomb->actor, &bomb->pos);
                Hu3DModelAttrReset(bomb->mdlId, 1);
                Hu3DModelAttrReset(bomb->shadowMdl, 1);
                MgActorPauseOff(bomb->actor, &bomb->pos);
                MgActorColMaskSet(bomb->actor, 2);
                bomb->actor->gravity = 130.0f;
                bomb->actor->velY = 0.0f;
                fn_1_7430(bomb);
                bomb->dest.x = 0.0f;
                bomb->dest.z = 0.0f;
                bomb->state = 1;
                bomb->curMot = -1;
                fn_1_74C4(bomb, 1);
                bomb->timer = frandmod(180) + 420;
                bomb->fxMdl = -1;
                bomb->moveTime = 0;
                bomb->holder = -1;
                bomb->comF = 0;
                if (frandmod(100) < 20 && MgSeqTimerValueGet() < 40U) {
                    if (frandmod(100) < 50) {
                        bomb->kind = 1;
                    } else {
                        bomb->kind = 2;
                    }
                    bomb->speed = 2.4f + ((f32)frandmod(59) / 100.0f);
                } else {
                    bomb->kind = 0;
                    bomb->speed = 1.0f + ((f32)frandmod(200) / 100.0f);
                }
                lbl_1_bss_1E4++;
            }
        }
    }
}

void fn_1_6858(void) {
    M564_BOMB *bomb;

    bomb = lbl_1_bss_1E8[0];
    bomb->pos.x = -195.0f;
    bomb->pos.y = 0.0f;
    bomb->pos.z = -195.0f;
    MgActorPosUpdate(bomb->actor, &bomb->pos);
    Hu3DModelAttrReset(bomb->mdlId, 1);
    Hu3DModelAttrReset(bomb->shadowMdl, 1);
    MgActorPauseOff(bomb->actor, &bomb->pos);
    MgActorColMaskSet(bomb->actor, 3);
    bomb->actor->gravity = 130.0f;
    bomb->actor->velY = 0.0f;
    bomb->actor->rotY = 45.0f;
    bomb->wait = 999;
    bomb->state = 3;
    bomb->curMot = -1;
    fn_1_74C4(bomb, 0);
    bomb->timer = 999;
    bomb->fxMdl = -1;
    bomb->moveTime = 0;
    bomb->holder = -1;
    lbl_1_bss_1E4++;
}

void fn_1_6990(M564_BOMB *bomb) {
    Hu3DModelAttrSet(bomb->mdlId, 1);
    Hu3DModelAttrSet(bomb->shadowMdl, 1);
    MgActorPauseOn(bomb->actor);
    bomb->state = 0;
    bomb->pos.z = bomb->pos.y = bomb->pos.x = 0.0f;
    lbl_1_bss_1E4--;
}

void fn_1_6A14(M564_BOMB *bomb) {
    if (bomb->state == 5) {
        fn_1_5EA8(bomb->holder, &bomb->pos);
        PSVECAdd(&bomb->pos, &lbl_1_data_22C, &bomb->pos);
    } else {
        MgActorPosGet(bomb->actor, &bomb->pos);
    }
}

void fn_1_6A78(M564_BOMB *bomb) {
    if (bomb->curMot == 2) {
        bomb->state = 3;
        bomb->wait = 999;
        return;
    }
    bomb->state = 2;
    fn_1_74C4(bomb, 1);
    MgActorColMaskSet(bomb->actor, 3);
    if (bomb->kind == 0) {
        fn_1_7430(bomb);
    } else {
        bomb->target = fn_1_5FDC(&bomb->pos);
    }
}

void fn_1_6B04(M564_BOMB *bomb) {
    if (lbl_1_bss_24 != 3 && bomb->timer > 0) {
        bomb->timer--;
        if (bomb->timer == 0x78) {
            if (bomb->state == 2) {
                bomb->state = 3;
            }
            bomb->wait = 999;
            fn_1_74C4(bomb, 2);
        }
        if (bomb->timer == 0) {
            fn_1_7404(bomb);
        }
    }
}

void fn_1_6BA0(M564_BOMB *bomb) {
    Vec v;
    f32 dist;

    dist = fn_1_13F0(&bomb->pos, &bomb->dest, &v);
    bomb->actor->rotY = (f32)(180.0 * (atan2(v.x, v.z) / M564_PI));
    bomb->actor->vel.z = bomb->speed;
    if (dist < 440.0f) {
        fn_1_6A78(bomb);
    }
}

void fn_1_6CA8(M564_BOMB *bomb) {
    Vec v;
    f32 dist;
    f32 step;

    switch (bomb->kind) {
        case 2:
            bomb->target = fn_1_5FDC(&bomb->pos);
        case 1:
            if (fn_1_5E5C(bomb->target) != 0) {
                fn_1_6A78(bomb);
            } else {
                MgActorPosGet(lbl_1_bss_100[bomb->target]->mgPlayer->actor, &bomb->dest);
            }
            break;
    }
    dist = fn_1_13F0(&bomb->pos, &bomb->dest, &v);
    if (dist == 0.0f) {
        step = 0.0f;
    } else {
        PSVECNormalize(&v, &v);
        PSVECScale(&v, &v, bomb->speed);
        step = PSVECMag(&v);
    }
    if (step >= dist) {
        bomb->state = 3;
        bomb->wait = frandmod(120) + 60;
        fn_1_74C4(bomb, 0);
        return;
    }
    bomb->actor->rotY = fn_1_38D0(bomb->actor->rotY, (f32)(180.0 * (atan2(v.x, v.z) / M564_PI)), 5.0f);
    bomb->actor->vel.z = bomb->speed;
}

void fn_1_6EB4(M564_BOMB *bomb) {
    Vec target;
    Vec pos;
    Vec ang;
    f32 cur;
    f32 rot;
    f32 holderRot;

    if (fn_1_5E5C(bomb->holder) != 0) {
        bomb->holder = -1;
        fn_1_6A78(bomb);
        return;
    }
    cur = bomb->actor->rotY;
    rot = lbl_1_bss_100[bomb->holder]->mgPlayer->actor->rotY;
    if (fabs(fn_1_1338(cur, rot)) > 90.0f) {
        rot += 180.0f;
        if (rot > 180.0f) {
            rot -= 360.0f;
        }
    }
    bomb->actor->rotY = fn_1_38D0(cur, rot, 5.0f);
    MgActorPosGet(lbl_1_bss_100[bomb->holder]->mgPlayer->actor, &target);
    MgActorPosGet(bomb->actor, &pos);
    holderRot = lbl_1_bss_100[bomb->holder]->mgPlayer->actor->rotY;
    target.x = (f32)((f64)target.x + (50.0 * sin(M564_RAD(holderRot))));
    target.z = (f32)((f64)target.z + (50.0 * cos(M564_RAD(holderRot))));
    target.y = pos.y;
    fn_1_37AC(&pos, &target, &ang, 15.0f);
    MgActorPosSet(bomb->actor, &pos);
}

void fn_1_7154(M564_BOMB *bomb) {
    f32 speed;

    if (bomb->comF != 0) {
        speed = 5.0f;
    } else {
        speed = 20.0f;
    }
    if (bomb->flip == 0.0f) {
        bomb->actor->vel.z = speed;
    } else {
        bomb->actor->vel.z = -speed;
    }
    bomb->moveTime++;
    if (bomb->comF != 0 && bomb->moveTime == 10) {
        fn_1_7404(bomb);
    }
}

void fn_1_7210(M564_BOMB *bomb, s32 sound) {
    Vec p;
    s32 i;
    f32 d;

    if (bomb->state != 7) {
        if (bomb->state == 5) {
            fn_1_5F08(bomb->holder);
        }
        Hu3DModelAttrSet(bomb->mdlId, 1);
        Hu3DModelAttrSet(bomb->shadowMdl, 1);
        MgActorPauseOn(bomb->actor);
        bomb->fxMdl = Hu3DModelLink(lbl_1_bss_1E6);
        Hu3DModelAttrReset(bomb->fxMdl, 1);
        Hu3DModelAttrReset(bomb->fxMdl, 0x40000002);
        Hu3DModelLayerSet(bomb->fxMdl, 5);
        Hu3DModelPosSetV(bomb->fxMdl, &bomb->pos);
        Hu3DModelScaleSet(bomb->fxMdl, 0.55f, 0.55f, 0.55f);
        bomb->explPos = bomb->pos;
        bomb->state = 7;
        bomb->explTime = 0;
        bomb->timer = 0;
        bomb->holder = -1;
        if (sound != 0) {
            fn_1_14D0(&bomb->pos, 0x8D8);
        }
        for (i = 0; i < 4; i++) {
            if (fn_1_5E5C(i) == 0) {
                MgActorPosGet(lbl_1_bss_100[i]->mgPlayer->actor, &p);
                d = fn_1_13A4(&bomb->explPos, &p);
                if (d < 200.0f) {
                    omVibrate(i, 20, 4, 2);
                } else if (d < 400.0f) {
                    omVibrate(i, 12, 6, 6);
                }
            }
        }
    }
}

void fn_1_7404(M564_BOMB *bomb) {
    fn_1_7210(bomb, 1);
}

void fn_1_7430(M564_BOMB *bomb) {
    s32 i;

    for (i = 0; i < 10; i++) {
        fn_1_118C(&lbl_1_data_D8, &bomb->dest, 50.0f, 440.0f);
        bomb->dest.y = bomb->pos.y;
        if (fn_1_13A4(&bomb->dest, &bomb->pos) > 200.0f) {
            break;
        }
    }
}

void fn_1_74C4(M564_BOMB *bomb, s16 mot) {
    if (bomb->curMot != mot && (bomb->curMot != 2 || mot != 0)) {
        bomb->curMot = mot;
        Hu3DModelAttrReset(bomb->mdlId, 0x40000002);
        Hu3DModelAttrSet(bomb->mdlId, 0x40000001);
        Hu3DMotionSet(bomb->mdlId, bomb->mot[mot]);
        if (mot == 0) {
            Hu3DMotionTimeSet(bomb->mdlId, (f32)(frandmod(50) + 10));
        }
    }
}

int fn_1_75A4(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b) {
    M564_BOMB *bomb;

    bomb = lbl_1_bss_1E8[a->paramB - 4];
    if (bomb->holder != -1 && bomb->holder == b->paramB) {
        return 0;
    }
    if (bomb->state == 6) {
        if (b->paramB < 4) {
            MgPlayerAttrSet(lbl_1_bss_100[b->paramB]->mgPlayer, 0x300);
        }
        fn_1_7210(bomb, 1);
        return 0;
    }
    return 1;
}

void fn_1_7674(MGACTOR *actor, int param) {
    M564_BOMB *bomb;

    bomb = lbl_1_bss_1E8[actor->param - 4];
    if (bomb->state == 6) {
        fn_1_7210(bomb, 1);
    }
}

void fn_1_76D8(OMOBJ *obj) {
    M564_BOMB *bomb;
    s16 t;

    bomb = obj->data;
    if (bomb->state != 0) {
        fn_1_6A14(bomb);
        fn_1_6B04(bomb);
        switch (bomb->state) {
            case 0:
                break;
            case 1:
                fn_1_6BA0(bomb);
                break;
            case 2:
                fn_1_6CA8(bomb);
                break;
            case 3:
                t = bomb->wait;
                bomb->wait = t - 1;
                if (t == 0) {
                    fn_1_6A78(bomb);
                }
                break;
            case 4:
                fn_1_6EB4(bomb);
                break;
            case 6:
                fn_1_7154(bomb);
                break;
            case 7:
                bomb->explTime++;
                if (Hu3DMotionEndCheck(bomb->fxMdl) != 0) {
                    fn_1_6990(bomb);
                    Hu3DModelKill(bomb->fxMdl);
                    bomb->fxMdl = -1;
                }
                break;
            case 8:
                bomb->actor->rotY = fn_1_38D0(bomb->actor->rotY, 0.0f, 15.0f);
                if (fabs(bomb->actor->rotY) < 0.1f) {
                    fn_1_74C4(bomb, 0);
                }
                break;
        }
    }
}

s16 fn_1_8340(Vec *pos, f32 rotY) {
    Vec v;
    f32 d;
    f32 best;
    s16 found;
    s32 i;

    best = 9999.0f;
    found = -1;
    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1E8[i]->state != 0 && lbl_1_bss_1E8[i]->state <= 3) {
            d = fn_1_13F0(pos, &lbl_1_bss_1E8[i]->pos, &v);
            if (!(d > 120.0f) && (f32)fabs(fn_1_1338(rotY, (f32)(180.0 * (atan2(v.x, v.z) / M564_PI)))) < 40.0f && d < best) {
                best = d;
                found = i;
            }
        }
    }
    return found;
}

s32 fn_1_84E8(Vec *a, Vec *b, f32 dist) {
    f32 d = fn_1_13A4(b, a);

    if (d < 70.0f + dist) {
        return 1;
    }
    return 0;
}

s32 fn_1_8558(Vec *pos, f32 a, f32 b) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1E8[i]->state == 7 && lbl_1_bss_1E8[i]->explTime <= 20
            && !(lbl_1_bss_1E8[i]->explPos.y > 70.0f + (pos->y + b))
            && !(lbl_1_bss_1E8[i]->explPos.y < pos->y - 70.0f)) {
            if (fn_1_84E8(&lbl_1_bss_1E8[i]->explPos, pos, a) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void fn_1_86D8(s16 bombNo, s16 playerNo) {
    M564_BOMB *bomb;

    bomb = lbl_1_bss_1E8[bombNo];
    bomb->state = 4;
    bomb->holder = playerNo;
    fn_1_74C4(bomb, 0);
}

void fn_1_87B4(s16 bombNo, s16 playerNo) {
    M564_BOMB *bomb;

    bomb = lbl_1_bss_1E8[bombNo];
    MgActorPauseOn(bomb->actor);
    bomb->state = 5;
    bomb->holder = playerNo;
    Hu3DModelPosSetV(bomb->mdlId, &lbl_1_data_22C);
    if ((f32)fabs(fn_1_1338(lbl_1_bss_100[playerNo]->mgPlayer->actor->rotY, bomb->actor->rotY)) <= 90.0f) {
        bomb->flip = 0.0f;
        Hu3DModelRotSet(bomb->mdlId, 0.0f, 0.0f, 0.0f);
    } else {
        bomb->flip = 180.0f;
        Hu3DModelRotSet(bomb->mdlId, 0.0f, 180.0f, 0.0f);
    }
    fn_1_74C4(bomb, 0);
}

void fn_1_89EC(s16 bombNo) {
    Vec pos;
    M564_BOMB *bomb;
    f32 rot;
    f32 s;

    bomb = lbl_1_bss_1E8[bombNo];
    bomb->state = 6;
    if (bomb->comF != 0) {
        s = sqrtf(200001.0f);
        bomb->actor->velY = 20.0f + (20.0f * ((-1.0f + s) * 0.5f));
    } else {
        s = sqrtf(124.07692f);
        bomb->actor->velY = 130.0f + (130.0f * ((-1.0f + s) * 0.5f));
    }
    rot = lbl_1_bss_100[bomb->holder]->mgPlayer->actor->rotY;
    bomb->holdRot = rot;
    rot = rot + bomb->flip;
    if (rot > 180.0f) {
        rot -= 360.0f;
    }
    bomb->actor->rotY = rot;
    fn_1_5EA8(bomb->holder, &pos);
    PSVECAdd(&pos, &lbl_1_data_22C, &pos);
    MgActorPauseOff(bomb->actor, &pos);
    MgActorColMaskSet(bomb->actor, 6);
}

s32 fn_1_8D60(s16 bombNo) {
    if (lbl_1_bss_1E8[bombNo]->state == 2) {
        return 1;
    }
    if (lbl_1_bss_1E8[bombNo]->state == 3) {
        return 1;
    }
    if (lbl_1_bss_1E8[bombNo]->state == 1) {
        return 1;
    }
    return 0;
}

void fn_1_8DEC(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (fn_1_8D60(i) != 0) {
            lbl_1_bss_1E8[i]->state = 8;
        }
    }
}

s32 fn_1_8EC8(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1E8[i]->state == 6) {
            return 0;
        }
        if (lbl_1_bss_1E8[i]->state == 7) {
            return 0;
        }
        if (lbl_1_bss_1E8[i]->state == 4) {
            return 0;
        }
    }
    return 1;
}

void fn_1_8F6C(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1E8[i]->state == 8) {
            fn_1_74C4(lbl_1_bss_1E8[i], 2);
        }
    }
}

s32 fn_1_902C(Vec *pos, Vec *out) {
    Vec diff;
    Vec sum;
    s32 i;

    sum.z = sum.y = sum.x = 0.0f;
    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1E8[i]->state != 0 && lbl_1_bss_1E8[i]->state != 5 && lbl_1_bss_1E8[i]->timer <= 0x78) {
            if (fn_1_84E8(&lbl_1_bss_1E8[i]->pos, pos, 100.0f) != 0) {
                PSVECSubtract(pos, &lbl_1_bss_1E8[i]->pos, &diff);
                diff.y = 0.0f;
                PSVECAdd(&sum, &diff, &sum);
            }
        }
    }
    if (PSVECMag(&sum) < 1.0f) {
        return 0;
    }
    PSVECNormalize(&sum, &sum);
    PSVECScale(&sum, &sum, 100.0f);
    PSVECAdd(pos, &sum, out);
    return 1;
}

void fn_1_91FC(s32 idx, Vec *out) {
    out->x = (f32)(550.0 * sin(M564_RAD(18.0f + (36.0f * (f32)idx))));
    out->z = (f32)(550.0 * -cos(M564_RAD(18.0f + (36.0f * (f32)idx))));
    out->y = 0.0f;
}

void fn_1_9344(OMOBJ *obj) {
    Vec p;
    M564_SPOT *spot;
    s32 i;

    for (i = 0; i < 10; i++) {
        spot = &lbl_1_bss_11C[i];
        switch (spot->state) {
            case 0:
                break;
            case 1:
                if (Hu3DMotionEndCheck(spot->mdlId) != 0) {
                    spot->state = 2;
                }
                break;
            case 2:
                if (lbl_1_bss_1E8[spot->bombNo]->state == 2 || lbl_1_bss_1E8[spot->bombNo]->state == 5) {
                    fn_1_91FC(i, &p);
                    fn_1_14D0(&p, 0x8D7);
                    spot->state = 3;
                    Hu3DMotionTimeSet(spot->mdlId, 19.0f);
                    Hu3DMotionStartEndSet(spot->mdlId, 19.0f, Hu3DMotionMaxTimeGet(spot->mdlId));
                }
                break;
            case 3:
                if (Hu3DMotionEndCheck(spot->mdlId) != 0) {
                    spot->timer = 0;
                    spot->bombNo = -1;
                    spot->state = 4;
                }
                break;
            case 4:
                if (spot->timer++ == 0x2D) {
                    spot->state = 0;
                }
                break;
        }
    }
}

void fn_1_95D4(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        lbl_1_bss_11C[i].mdlId = lbl_1_bss_8C[i + 6];
        lbl_1_bss_11C[i].state = 0;
        lbl_1_bss_11C[i].bombNo = -1;
        lbl_1_bss_11C[i].pos.x = (f32)(650.0 * sin(M564_RAD(18.0f + (36.0f * (f32)i))));
        lbl_1_bss_11C[i].pos.z = (f32)(650.0 * -cos(M564_RAD(18.0f + (36.0f * (f32)i))));
        lbl_1_bss_11C[i].pos.y = 0.0f;
        lbl_1_bss_11C[i].timer = 0;
        Hu3DMotionSpeedSet(lbl_1_bss_11C[i].mdlId, 0.5f);
    }
    omAddObjEx(lbl_1_bss_F8, 0x1200, 0, 0, -1, fn_1_9344);
}

s16 fn_1_9810(void) {
    s32 list[10];
    s32 cnt;
    s32 i;

    cnt = 0;
    for (i = 0; i < 10; i++) {
        if (lbl_1_bss_11C[i].state == 0) {
            list[cnt++] = i;
        }
    }
    if (cnt == 0) {
        return -1;
    }
    return list[frandmod(cnt)];
}

void fn_1_98AC(s16 spotNo, s16 bombNo) {
    Vec p;
    M564_SPOT *spot;
    M564_BOMB *bomb;

    spot = &lbl_1_bss_11C[spotNo];
    if (spot->bombNo == -1) {
        bomb = lbl_1_bss_1E8[bombNo];
        bomb->pos.x = spot->pos.x;
        bomb->pos.y = spot->pos.y;
        bomb->pos.z = spot->pos.z;
        spot->bombNo = bombNo;
        Hu3DModelAttrReset(spot->mdlId, 0x40000001);
        Hu3DModelAttrReset(spot->mdlId, 0x40000002);
        Hu3DMotionTimeSet(spot->mdlId, 0.0f);
        Hu3DMotionStartEndSet(spot->mdlId, 0.0f, 19.0f);
        spot->state = 1;
        fn_1_91FC(spotNo, &p);
        fn_1_14D0(&p, 0x8D6);
    }
}
