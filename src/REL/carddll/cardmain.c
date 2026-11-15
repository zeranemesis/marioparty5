// carddll/cardmain.c - Card Party main loop: setup, turn state machine, cursor/pawn control and the
// screen-space helpers (_prolog/_epilog, ObjectSetup, fn_1_2E8 ...).

#include "REL/carddll/cardmain.h"

typedef void (*CardVoidFunc)(void);

extern const CardVoidFunc _ctors[];
extern const CardVoidFunc _dtors[];

/* ---- .data ---- */
static char lbl_1_data_1C[8] = "s_mario";
static char lbl_1_data_24[8] = "s_luiji";
static char lbl_1_data_2C[8] = "s_peach";

/* ---- .bss (non-static where other carddll units reference them) ---- */
s16 lbl_1_bss_0[4];
s32 lbl_1_bss_8[3];
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20[10];
s16 lbl_1_bss_48;
s16 lbl_1_bss_4A;
s32 lbl_1_bss_4C[4];
s16 lbl_1_bss_5C;
u8 lbl_1_bss_5E;
s32 lbl_1_bss_60;
f32 lbl_1_bss_64;
s32 lbl_1_bss_68;
s32 lbl_1_bss_6C;
s16 lbl_1_bss_70;
s16 lbl_1_bss_72;
s32 lbl_1_bss_74[3];
f32 lbl_1_bss_80;
s32 lbl_1_bss_84;
Vec lbl_1_bss_88;
s32 lbl_1_bss_94;
s32 lbl_1_bss_98[2];
f32 lbl_1_bss_A0[2];
s16 lbl_1_bss_A8[2];
s32 lbl_1_bss_AC;
s32 lbl_1_bss_B0;
s32 lbl_1_bss_B4;
s32 lbl_1_bss_B8;
f32 lbl_1_bss_BC;
f32 lbl_1_bss_C0;
f32 lbl_1_bss_C4;
f32 lbl_1_bss_C8;
Vec lbl_1_bss_CC;
Vec lbl_1_bss_D8;
CardGame lbl_1_bss_E4;
f32 lbl_1_bss_2BC[2];
s32 lbl_1_bss_2C4;
Vec lbl_1_bss_2C8[4];
s16 lbl_1_bss_2F8[4][2];
s16 lbl_1_bss_308[6];
s32 lbl_1_bss_314;
OMOBJ *lbl_1_bss_318;
HUPROCESS *lbl_1_bss_31C;

int _prolog(void)
{
    const CardVoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const CardVoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void ObjectSetup(void)
{
    Vec shadowPos = { 0.0f, 3000.0f, 500.0f };
    Vec shadowUp = { 0.0f, 1.0f, 0.0f };
    Vec shadowTarget = { 0.0f, 0.0f, 0.0f };
    Vec lightPos = { 0.0f, 8000.0f, 500.0f };
    Vec lightDir = { 0.0f, -1.0f, 0.0f };
    GXColor lightCol = { 0xFF, 0xFF, 0xFF, 0xFF };

    lbl_1_bss_31C = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(lbl_1_bss_31C);
    fn_1_30E08();
    lbl_1_bss_314 = Hu3DGLightCreateV(&lightPos, &lightDir, &lightCol);
    Hu3DGLightStaticSet(lbl_1_bss_314, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_314);
    Hu3DShadowCreate(60.0f, 20.0f, 10000.0f);
    Hu3DShadowColSet(0, 0, 0);
    Hu3DShadowTPLvlSet(0.4f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    fn_1_8C78();
    lbl_1_bss_318 = omAddObjEx(lbl_1_bss_31C, 0, 0x7A, 0, -1, fn_1_2E8);
    lbl_1_bss_318->work[0] = 0x64;
    lbl_1_bss_5E = 0x80;
    fn_1_850C(&lbl_1_bss_5E, &lbl_1_bss_318);
    Hu3DModelAttrSet(lbl_1_bss_5C, 1);
    HuWinInit(1);
    fn_1_501B4(0);
    lbl_1_bss_60 = 0;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_2E8(OMOBJ *obj) {
    s32 var_r31;
    s32 var_r31_2;

    var_r31 = 0;
    while (var_r31 < 4) {
        lbl_1_bss_E4.pl[var_r31].charNo = (s32) GwPlayerConf[var_r31].charNo;
        lbl_1_bss_E4.pl[var_r31].f54 = -1;
        lbl_1_bss_E4.pl[var_r31].f56 = -1;
        lbl_1_bss_E4.pl[var_r31].f58 = -1;
        lbl_1_bss_0[var_r31] = -1;
        var_r31 += 1;
    }
    lbl_1_bss_E4.boardSize = GwCardCommon.boardSize + 7;
    lbl_1_bss_E4.pl[0].f28 = (s32) (u8) GwCardCommon.pedestalF[0];
    lbl_1_bss_E4.pl[1].f28 = (s32) (u8) GwCardCommon.pedestalF[1];
    lbl_1_bss_E4.pl[2].f28 = (s32) (u8) GwCardCommon.pedestalF[2];
    lbl_1_bss_E4.pl[3].f28 = (s32) (u8) GwCardCommon.pedestalF[3];
    fn_1_8EC8(&lbl_1_bss_E4);
    fn_1_392B8();
    fn_1_7304();
    var_r31_2 = 0;
    while (var_r31_2 < 6) {
        lbl_1_bss_308[var_r31_2] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[var_r31_2], 0x10000000, HEAP_MODEL));
        Hu3DModelCameraSet(lbl_1_bss_308[var_r31_2], 1U);
        Hu3DModelPosSet(lbl_1_bss_308[var_r31_2], -50.0f, -10.0f, 0.0f);
        Hu3DModelRotSet(lbl_1_bss_308[var_r31_2], 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(lbl_1_bss_308[var_r31_2], 1);
        var_r31_2 += 1;
    }
    Hu3DModelShadowMapSet(*lbl_1_bss_308);
    lbl_1_bss_C8 = 0.0f;
    lbl_1_bss_C4 = 0.0f;
    lbl_1_bss_C0 = 0.0f;
    lbl_1_bss_BC = 0.0f;
    lbl_1_bss_B8 = 0;
    lbl_1_bss_B4 = 0;
    lbl_1_bss_AC = 0;
    lbl_1_bss_A8[1] = -1;
    lbl_1_bss_A8[0] = -1;
    lbl_1_bss_A0[1] = 0.0f;
    lbl_1_bss_A0[0] = 0.0f;
    lbl_1_bss_98[1] = -1;
    lbl_1_bss_98[0] = -1;
    lbl_1_bss_94 = 0;
    lbl_1_bss_6C = -1;
    lbl_1_bss_E4.curPlayer = 0;
    lbl_1_bss_E4.f188 = 0xC8;
    lbl_1_bss_E4.f18C = 0x12C;
    lbl_1_bss_E4.f190 = 0;
    lbl_1_bss_E4.f19C = 2;
    lbl_1_bss_E4.f1D4 = 0;
    WipeColorSet(0xFFU, 0xFFU, 0xFFU);
    WipeCreate(1, 0, 0x3C);
    obj->objFunc = fn_1_70C;
    fn_1_FA24();
    fn_1_2FEF4(&lbl_1_bss_E4);
    fn_1_30B00(&lbl_1_bss_E4, 0);
    fn_1_3EAA0();
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_70C(OMOBJ *obj) {
    s32 temp_r3;
    s32 var_r30;
    u32 temp_r0;

    if ((s16) omSysExitReq != 0) {
        WipeCreate(2, 0, 0x3C);
        obj->objFunc = fn_1_934;
    }
    temp_r0 = obj->work[0];
    switch ((s32) temp_r0) {                        /* irregular */
    case 0x64:
        if ((s32) lbl_1_bss_14 == -1) {
            lbl_1_bss_14 = HuAudBGMPlay(0x27);
        }
        if (WipeCheckEnd() == 0) {
            obj->work[0] += 1;
        }
        break;
    case 0x65:
        if (fn_1_FB88(&lbl_1_bss_E4) != 0) {
            obj->work[0] += 1;
        }
        break;
    case 0x66:
        temp_r3 = fn_1_AB4(&lbl_1_bss_E4);
        lbl_1_bss_68 = temp_r3;
        if (temp_r3 != 0) {
            if ((s32) lbl_1_bss_68 == -1) {
                obj->work[0] = 0x68;
            } else {
                obj->work[0] += 1;
            }
        }
        break;
    case 0x67:
        if (fn_1_12E04() != 0) {
            obj->work[0] += 1;
            var_r30 = 0;
            while (var_r30 < 4) {
                GwCardCommon.starNum[var_r30] = (s8) (u8) (lbl_1_bss_E4.pl[var_r30].f44 + (lbl_1_bss_E4.pl[var_r30].f48 * 2));
                GwCardCommon.ztarNum[var_r30] = (s8) (u8) lbl_1_bss_E4.pl[var_r30].f4C;
                var_r30 += 1;
            }
            lbl_1_bss_60 = 1;
        }
        break;
    case 0x68:
        WipeColorSet(0xFFU, 0xFFU, 0xFFU);
        WipeCreate(2, 0, 0x3C);
        obj->objFunc = fn_1_934;
        break;
    }
    fn_1_3F73C();
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_934(OMOBJ *obj) {
    s32 var_r31;

    if (((s32) lbl_1_bss_20[0] == 0) && ((s32) lbl_1_bss_60 != 0)) {
        HuAudFXFadeOut(lbl_1_bss_20[9], 0x3E8);
    }
    lbl_1_bss_20[0] += 1;
    if (WipeCheckEnd() == 0) {
        if ((s32) lbl_1_bss_20[0] > 0x64) {
            HuAudAllStop();
            var_r31 = 0;
            while (var_r31 < 4) {
                if ((s16) lbl_1_bss_0[var_r31] != -1) {
                    Hu3DLLightKill(lbl_1_bss_468[var_r31], lbl_1_bss_0[var_r31]);
                }
                var_r31 += 1;
            }
            fn_1_34CC8(&lbl_1_bss_E4);
            fn_1_3678C(&lbl_1_bss_E4);
            Hu3DModelKill(lbl_1_bss_5C);
            fn_1_F24C();
            Hu3DModelAllKill();
            fn_1_30910();
            Hu3DLightAllKill();
            HuDataDirClose(0x110000);
            if ((s32) lbl_1_bss_60 != 0) {
                omOvlGotoEx(DLL_rescarddll, 1, 0, 0);
                return;
            }
            omOvlReturnEx(1, 1);
        }
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_AB4(CardGame *game) {
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_r30;

    var_r30 = 0;
    temp_r3 = game->f188;
    switch (temp_r3) {
    case 0xC8:
        fn_1_CA4(game);
        break;
    case 0xC9:
        if (fn_1_2930C(game) != 0) {
            if ((s32) game->cnt1D0 == 0) {
                var_r30 = 1;
            } else {
                game->f188 = 0xC8;
            }
        }
        break;
    case 0xCA:
        temp_r3_2 = fn_1_187FC(game, lbl_1_bss_6C);
        lbl_1_bss_68 = temp_r3_2;
        if (temp_r3_2 != 0) {
            if ((s32) lbl_1_bss_68 != -1) {
                game->f188 = 0xC8;
                lbl_1_bss_6C = -1;
                espDrawNoSet(lbl_1_bss_F9E[0], 0);
                espDrawNoSet(lbl_1_bss_F9E[1], 0);
            } else {
                var_r30 = -1;
            }
        }
        break;
    case 0xCB:
        if (fn_1_1B2F0(game) != 0) {
            game->f188 = 0xC8;
            game->f18C = 0x12C;
            game->f19C = 2;
        }
        break;
    case 0xCC:
        if (fn_1_198F4(game) != 0) {
            game->f188 = 0xC8;
        }
        break;
    case 0xCD:
        if (fn_1_1AB2C(game) != 0) {
            game->f188 = 0xC8;
        }
        break;
    case 0xCE:
        if (fn_1_32D70(game) != 0) {
            if ((s32) (game->pl[game->order[game->curPlayer]].f50 & 0x1000) == 0) {
                fn_1_4FBC8(game, 0x3B);
            }
            game->f188 = 0xC8;
        }
        break;
    }
    fn_1_8894();
    fn_1_8B84(game);
    fn_1_358F8(game);
    return var_r30;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_CA4(CardGame *game)
{
    Vec spD0;
    Vec spC4;
    Vec tmp;
    s16 sp8;
    s32 *sp24;
    s32 *sp20;
    s32 *sp1C;
    f32 *sp18;
    f32 *sp14;
    s32 *sp10;
    s32 *spC;
    CardPlayer *temp_r30;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 var_f30;
    f32 var_f31;
    s16 temp_r17;
    s16 var_r25;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 var_r23;
    s32 var_r24;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;
    s32 var_r29_2;
    s32 var_r29_3;
    s32 var_r29_4;
    s32 var_r29_5;
    s32 var_r29_6;

    sp8 = 0;
    var_r25 = 0;
    temp_r30 = &game->pl[game->order[game->curPlayer]];
    fn_1_5A00(game);
    temp_r5 = game->f18C;
    switch (temp_r5) {                              /* switch 1 */
    case 0x12C:                                     /* switch 1 */
        if (fn_1_30FA4(game->f19C, game) != 0) {
            if ((s32) game->f19C == 2) {
                lbl_1_bss_B0 = fn_1_5710(game, game->curPlayer, 0);
                if (((s32) lbl_1_bss_B0 != 0) && ((s32) temp_r30->f00 == 0)) {
                    fn_1_9DA8(&lbl_1_bss_88, game);
                    lbl_1_bss_84 = lbl_1_bss_B0;
                    lbl_1_bss_80 = 1.0f;
                    lbl_1_bss_74[0] = game->f1B0[0];
                    if ((s32) lbl_1_bss_B0 >= 2) {
                        lbl_1_bss_74[1] = game->f1B0[1];
                        if ((s32) lbl_1_bss_B0 >= 3) {
                            lbl_1_bss_74[2] = game->f1B0[2];
                        }
                    }
                    temp_r30->f50 |= 0x100;
                }
                if ((s32) (temp_r30->f50 & 0x80) != 0) {
                    game->f18C = 0x138;
                    fn_1_9DA8(&spD0, game);
                    spD0.x += -50.0f;
                    spD0.y += 150.0f;
                    tmp.x = 5.0f;
                    tmp.y = 5.0f;
                    tmp.z = 5.0f;
                    PSVECSubtract(&Hu3DCamera->pos, &Hu3DCamera->target, &spC4);
                    PSVECNormalize(&spC4, &spC4);
                    PSVECScale(&spC4, &spC4, 100.0f);
                    PSVECAdd(&spC4, &spD0, &spD0);
                    temp_r17 = fn_1_3ED3C(1, &spD0);
                    fn_1_3F6B0(1, temp_r17, &tmp);
                    game->f1A4 = 0;
                } else {
                    game->f18C = 0x12D;
                }
            } else if ((s32) game->f19C == 3) {
                game->f18C = 0x12F;
            }
        }
        break;
    case 0x12D:                                     /* switch 1 */
        if ((s32) lbl_1_bss_B4 == 0) {
            if (fn_1_182E0(game) != 0) {
                lbl_1_bss_B4 = 1;
                game->f18C = 0x134;
                lbl_1_bss_B8 = 0;
            }
        } else {
            game->f18C = 0x134;
        }
        break;
    case 0x12E:                                     /* switch 1 */
        if (fn_1_393C4(game) != 0) {
            lbl_1_bss_2C4 = 0;
            lbl_1_bss_2BC[0] = 0.0f;
            game->f18C = 0x12C;
            game->f19C = 3;
            game->f1A0 = 1;
            temp_r30->f50 &= 0xFFFFFBFF;
            fn_1_30B00(game, 1);
            lbl_1_bss_B8 = 0;
            lbl_1_bss_48 = 0;
        }
        break;
    case 0x12F:                                     /* switch 1 */
        fn_1_9DA8(&lbl_1_bss_D8, game);
        if ((s32) temp_r30->f00 != 0) {
            lbl_1_bss_BC = 300.0f;
            game->f184 -= 1;
            game->f18C = 0x131;
            temp_r30->f00 = 0;
            lbl_1_bss_CC.x = lbl_1_bss_D8.x;
            lbl_1_bss_CC.z = lbl_1_bss_D8.z;
            break;
        }
        if ((s16) GwPlayerConf[game->order[game->curPlayer]].type == 0) {
            fn_1_502E0(0x11);
        }
        if ((s32) (HuPadBtnDown[game->order[game->curPlayer]] & 0x800) != 0) {
            fn_1_50460();
            game->f188 = 0xCD;
            fn_1_7E28();
            fn_1_30B00(game, -1);
            temp_r30->f50 |= 0x40;
            HuAudFXPlay(0x3FB);
            HuAudSStreamVolSet(0, 0x60U, 0x1F4U);
        } else if ((s32) (HuPadBtnDown[game->order[game->curPlayer]] & 0x20) != 0) {
            fn_1_50460();
            game->f188 = 0xCC;
            fn_1_7E28();
            fn_1_30B00(game, -1);
            temp_r30->f50 |= 0x40;
            HuAudFXPlay(0x3FB);
            HuAudSStreamVolSet(0, 0x60U, 0x1F4U);
        } else {
            if ((s32) (temp_r30->f50 & 0x40) != 0) {
                fn_1_30B00(game, 1);
                temp_r30->f50 &= 0xFFFFFFBF;
            }
            fn_1_7E98(temp_r30);
            fn_1_77D0(&lbl_1_bss_D8, game);
            if (((s32) (CARD_WALL(temp_r30) & 1) != 0) && ((s32) (HuPadDStkRep[game->order[game->curPlayer]] & 8) != 0)) {
                if (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r30->rotY2 != 0.0f))) {
                    if (temp_r30->rotY != 180.0f) {
                        HuAudFXPlay(0);
                    }
                    temp_r30->rotY = 180.0f;
                }
            } else if (((s32) (CARD_WALL(temp_r30) & 2) != 0) && ((s32) (HuPadDStkRep[game->order[game->curPlayer]] & 4) != 0)) {
                if (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r30->rotY2 != 180.0f))) {
                    if (temp_r30->rotY != 0.0f) {
                        HuAudFXPlay(0);
                    }
                    temp_r30->rotY = 0.0f;
                }
            } else if (((s32) (CARD_WALL(temp_r30) & 4) != 0) && ((s32) (HuPadDStkRep[game->order[game->curPlayer]] & 1) != 0)) {
                if (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r30->rotY2 != 90.0f))) {
                    if (temp_r30->rotY != -90.0f) {
                        HuAudFXPlay(0);
                    }
                    temp_r30->rotY = -90.0f;
                }
            } else if (((s32) (CARD_WALL(temp_r30) & 8) != 0) && ((s32) (HuPadDStkRep[game->order[game->curPlayer]] & 2) != 0) && (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r30->rotY2 != -90.0f)))) {
                if (temp_r30->rotY != 90.0f) {
                    HuAudFXPlay(0);
                }
                temp_r30->rotY = 90.0f;
            }
            Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], 0.0f, temp_r30->rotY, 0.0f);
            if ((s32) (HuPadBtnDown[game->order[game->curPlayer]] & 0x100) != 0) {
                fn_1_50460();
                game->f184 -= 1;
                fn_1_41EC(game);
                temp_r30->rotY2 = temp_r30->rotY;
                game->f1A0 = 0;
                fn_1_7E28();
            }
            lbl_1_bss_CC.x = lbl_1_bss_D8.x;
            lbl_1_bss_CC.z = lbl_1_bss_D8.z;
        }
        break;
    case 0x130:                                     /* switch 1 */
        fn_1_9DA8(&lbl_1_bss_D8, game);
        if (temp_r30->rotY == 180.0f) {
            if (CARD_WALL(temp_r30) == 3) {
                temp_r30->rotY = 180.0f;
                temp_r30->rotY2 = 180.0f;
            } else if (CARD_WALL(temp_r30) == 6) {
                temp_r30->rotY = -90.0f;
                temp_r30->rotY2 = -90.0f;
            } else if (CARD_WALL(temp_r30) == 0xA) {
                temp_r30->rotY = 90.0f;
                temp_r30->rotY2 = 90.0f;
            } else {
                game->f18C = 0x12F;
            }
        } else if (temp_r30->rotY == 0.0f) {
            if (CARD_WALL(temp_r30) == 3) {
                temp_r30->rotY = 0.0f;
                temp_r30->rotY2 = 0.0f;
            } else if (CARD_WALL(temp_r30) == 5) {
                temp_r30->rotY = -90.0f;
                temp_r30->rotY2 = -90.0f;
            } else if (CARD_WALL(temp_r30) == 9) {
                temp_r30->rotY = 90.0f;
                temp_r30->rotY2 = 90.0f;
            } else {
                game->f18C = 0x12F;
            }
        } else if (temp_r30->rotY == -90.0f) {
            if (CARD_WALL(temp_r30) == 0xC) {
                temp_r30->rotY = -90.0f;
                temp_r30->rotY2 = -90.0f;
            } else if (CARD_WALL(temp_r30) == 9) {
                temp_r30->rotY = 180.0f;
                temp_r30->rotY2 = 180.0f;
            } else if (CARD_WALL(temp_r30) == 0xA) {
                temp_r30->rotY = 0.0f;
                temp_r30->rotY2 = 0.0f;
            } else {
                game->f18C = 0x12F;
            }
        } else if (temp_r30->rotY == 90.0f) {
            if (CARD_WALL(temp_r30) == 0xC) {
                temp_r30->rotY = 90.0f;
                temp_r30->rotY2 = 90.0f;
            } else if (CARD_WALL(temp_r30) == 5) {
                temp_r30->rotY = 180.0f;
                temp_r30->rotY2 = 180.0f;
            } else if (CARD_WALL(temp_r30) == 6) {
                temp_r30->rotY = 0.0f;
                temp_r30->rotY2 = 0.0f;
            } else {
                game->f18C = 0x12F;
            }
        }
        if ((s32) game->f18C != 0x12F) {
            game->f184 -= 1;
            fn_1_41EC(game);
        }
        lbl_1_bss_CC.x = lbl_1_bss_D8.x;
        lbl_1_bss_CC.z = lbl_1_bss_D8.z;
        break;
    case 0x131:                                     /* switch 1 */
        if ((s32) (lbl_1_bss_8C4[temp_r30->row][temp_r30->col] & 0x10) != 0) {
            game->f18C = 0x132;
        } else {
            game->f188 = 0xC9;
        }
        break;
    case 0x132:                                     /* switch 1 */
        if ((s16) lbl_1_bss_4A == 0) {
            if ((s16) lbl_1_bss_48 == 0) {
                var_r24 = 0x3EF;
            } else {
                var_r24 = 0x3F0;
            }
            lbl_1_bss_20[0] = var_r24;
            fn_1_8D04(*lbl_1_bss_20, lbl_1_bss_468[game->order[game->curPlayer]], 1);
            lbl_1_bss_20[0] = 0;
            lbl_1_bss_48 ^= 1;
            lbl_1_bss_4A = 1;
        }
        fn_1_30B00(game, 1);
        spD0 = lbl_1_bss_CC;
        spD0.x += 100.0f;
        fn_1_31788(&spD0);
        if (lbl_1_bss_C8 <= lbl_1_bss_BC) {
            spD0 = lbl_1_bss_D8;
            if (temp_r30->rotY == 180.0f) {
                spD0.z -= lbl_1_bss_C8;
                Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], lbl_1_bss_C0, temp_r30->rotY, 0.0f);
            } else if (temp_r30->rotY == 0.0f) {
                spD0.z += lbl_1_bss_C8;
                Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], lbl_1_bss_C0, temp_r30->rotY, 0.0f);
            } else if (temp_r30->rotY == -90.0f) {
                spD0.x -= lbl_1_bss_C8;
                Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], 0.0f, temp_r30->rotY, lbl_1_bss_C0);
            } else if (temp_r30->rotY == 90.0f) {
                spD0.x += lbl_1_bss_C8;
                Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], 0.0f, temp_r30->rotY, lbl_1_bss_C0);
            }
            fn_1_5248(game);
            temp_r3 = fn_1_5710(game, game->curPlayer, 0);
            lbl_1_bss_B0 = temp_r3;
            if (temp_r3 == 0) {
                if (lbl_1_bss_C8 < (lbl_1_bss_BC * 0.5f)) {
                    if ((s32) lbl_1_bss_94 != 0) {
                        var_f31 = 15.0f;
                    } else {
                        var_f31 = 10.0f;
                    }
                    lbl_1_bss_C4 += var_f31;
                    if (temp_r30->rotY == 90.0f) {
                        lbl_1_bss_C0 += 2.0f;
                    } else {
                        lbl_1_bss_C0 -= 2.0f;
                    }
                } else if (lbl_1_bss_C8 > (lbl_1_bss_BC * 0.5f)) {
                    if ((s32) lbl_1_bss_94 != 0) {
                        var_f30 = 15.0f;
                    } else {
                        var_f30 = 10.0f;
                    }
                    lbl_1_bss_C4 -= var_f30;
                    if (temp_r30->rotY == 90.0f) {
                        lbl_1_bss_C0 -= 2.0f;
                    } else {
                        lbl_1_bss_C0 += 2.0f;
                    }
                }
                spD0.y += lbl_1_bss_C4;
            } else {
                if (lbl_1_bss_C8 < (lbl_1_bss_BC * 0.5f)) {
                    lbl_1_bss_C4 += 15.0f;
                    if (temp_r30->rotY == 90.0f) {
                        lbl_1_bss_C0 += 2.0f;
                    } else {
                        lbl_1_bss_C0 -= 2.0f;
                    }
                } else if (lbl_1_bss_C8 > (lbl_1_bss_BC * 0.5f)) {
                    lbl_1_bss_C4 -= 15.0f;
                    if (temp_r30->rotY == 90.0f) {
                        lbl_1_bss_C0 -= 2.0f;
                    } else {
                        lbl_1_bss_C0 += 2.0f;
                    }
                    if (lbl_1_bss_A0[1] == 0.0f) {
                        fn_1_8D04(0x3F4, lbl_1_bss_468[game->order[game->curPlayer]], 1);
                    }
                    for (var_r27 = 0; var_r27 < (s32) lbl_1_bss_B0; var_r27++) {
                        if (Hu3DData[lbl_1_bss_468[game->f1B0[var_r27]]].scale.y != 0.0f) {
                            var_r25 = lbl_1_bss_468[game->f1B0[var_r27]];
                            lbl_1_bss_98[1] = game->f1B0[var_r27];
                            break;
                        }
                    }
                    lbl_1_bss_A0[1] += 0.1f;
                    if (lbl_1_bss_A0[1] > 1.0f) {
                        lbl_1_bss_A0[1] = 1.0f;
                    }
                    Hu3DModelScaleSet(var_r25, 1.0f, 1.0f - lbl_1_bss_A0[1], 1.0f);
                    lbl_1_bss_A8[1] = var_r25;
                    if ((s32) (game->pl[lbl_1_bss_98[1]].f50 & 4) != 0) {
                        tmp.x = 1.0f;
                        tmp.y = 1.0f;
                        tmp.z = 1.0f;
                        tmp.y -= lbl_1_bss_A0[1];
                        fn_1_3F6B0(4, game->pl[lbl_1_bss_98[1]].f56, &tmp);
                    }
                    if ((s16) game->pl[lbl_1_bss_98[1]].f58 != -1) {
                        Hu3DModelScaleSet(game->pl[lbl_1_bss_98[1]].f58, 1.0f, 1.0f - lbl_1_bss_A0[1], 1.0f);
                    }
                    game->pl[lbl_1_bss_98[1]].f50 |= 0x200;
                }
                spD0.y += lbl_1_bss_C4;
            }
            Hu3DModelPosSet(lbl_1_bss_468[game->order[game->curPlayer]], -50.0f + spD0.x, spD0.y, spD0.z);
            lbl_1_bss_C8 += lbl_1_bss_BC / 20.0f;
            if ((s16) lbl_1_bss_0[game->order[game->curPlayer]] != -1) {
                Hu3DLLightPosSet(lbl_1_bss_468[game->order[game->curPlayer]], lbl_1_bss_0[game->order[game->curPlayer]], -50.0f + spD0.x, 80.0f + spD0.y, 100.0f + spD0.z, 0.0f, 0.0f, -1.0f);
            }
            lbl_1_bss_CC.x = spD0.x;
            lbl_1_bss_CC.z = spD0.z;
            if ((s32) (temp_r30->f50 & 2) != 0) {
                spD0.x += -50.0f;
                fn_1_3F3D8(3, temp_r30->f56, &spD0);
            } else if ((s32) (temp_r30->f50 & 4) != 0) {
                spD0.x += -50.0f;
                fn_1_3F3D8(4, temp_r30->f56, &spD0);
            }
        } else {
            var_r23 = 0x3F1;
            lbl_1_bss_20[0] = var_r23;
            fn_1_8D04(*lbl_1_bss_20, lbl_1_bss_468[game->order[game->curPlayer]], 1);
            lbl_1_bss_20[0] = 0;
            lbl_1_bss_4A = 0;
            if ((s32) game->f184 == 0) {
                if ((s16) lbl_1_bss_A8[1] != -1) {
                    game->f18C = 0x137;
                    game->f1A4 = 0;
                } else {
                    game->f18C = 0x133;
                }
            } else {
                game->f18C = 0x130;
                if ((s16) lbl_1_bss_A8[1] != -1) {
                    lbl_1_bss_A8[0] = lbl_1_bss_A8[1];
                    lbl_1_bss_A8[1] = -1;
                    lbl_1_bss_A0[0] = lbl_1_bss_A0[1];
                    lbl_1_bss_A0[1] = 0.0f;
                    lbl_1_bss_98[0] = lbl_1_bss_98[1];
                    lbl_1_bss_98[1] = -1;
                    lbl_1_bss_AC = 1;
                }
                lbl_1_bss_94 = 0;
            }
            lbl_1_bss_C8 = 0.0f;
            lbl_1_bss_C4 = 0.0f;
            lbl_1_bss_C0 = 0.0f;
            Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], lbl_1_bss_C0, temp_r30->rotY, 0.0f);
        }
        if (((s32) game->f18C == 0x132) && ((s16) GwPlayerConf[game->order[game->curPlayer]].type != 0)) {
            for (var_r29 = 0; var_r29 < 4; var_r29++) {
                if (((s16) GwPlayerConf[var_r29].type == 0) && ((s32) (HuPadBtnDown[var_r29] & 0x1000) != 0)) {
                    lbl_1_bss_6C = var_r29;
                    game->f188 = 0xCA;
                    espDrawNoSet(lbl_1_bss_F9E[0], 0x40);
                    espDrawNoSet(lbl_1_bss_F9E[1], 0x40);
                    HuAudFXPlay(0x3FA);
                    HuAudSStreamVolSet(0, 0x60U, 0x1F4U);
                    break;
                }
            }
        }
        break;
    case 0x133:                                     /* switch 1 */
        if ((s32) (temp_r30->f50 & 0x1000) == 0) {
            game->f18C = 0x12C;
            game->f19C = 2;
            game->curPlayer += 1;
            if ((s32) game->curPlayer == 4) {
                game->curPlayer = 0;
            }
        } else {
            game->f18C = 0x136;
        }
        if ((s32) (temp_r30->f50 & 2) != 0) {
            fn_1_3F278(3, temp_r30->f56);
            temp_r30->f56 = -1;
        } else if (((s32) (temp_r30->f50 & 4) != 0) && ((s32) game->f1D4 != 0x14)) {
            fn_1_3F278(4, temp_r30->f56);
            temp_r30->f56 = -1;
        }
        temp_r30->f50 &= 0xFFC0;
        if ((s32) game->f1D4 == 0x14) {
            temp_r30->f50 |= 4;
            game->f1D4 = -1;
        }
        lbl_1_bss_B4 = 0;
        fn_1_30B00(game, -1);
        break;
    case 0x134:                                     /* switch 1 */
        if (((s32) (temp_r30->f50 & 0x8000) == 0) && ((s32) game->cnt1D0 == 1) && ((s32) temp_r30->f30 == 4)) {
            fn_1_36828();
            spD0.x = 288.0f;
            spD0.y = 240.0f;
            spD0.z = 1500.0f;
            lbl_1_bss_64 = 400.0f;
            spD0.y = spD0.y - lbl_1_bss_64;
            fn_1_36A70(&spD0);
            game->f18C = 0x135;
        } else {
            if ((s32) lbl_1_bss_B4 == 1) {
                if ((s32) (temp_r30->f50 & 4) != 0) {
                    fn_1_4FBC8(game, 0x25);
                }
                lbl_1_bss_B4 = -1;
            }
            game->f18C = 0x12E;
        }
        break;
    case 0x135:                                     /* switch 1 */
        lbl_1_bss_64 -= 10.0f;
        if (lbl_1_bss_64 < 0.0f) {
            lbl_1_bss_64 = 0.0f;
        }
        spD0.x = 288.0f;
        spD0.y = 240.0f;
        spD0.z = 1500.0f;
        spD0.y = spD0.y - lbl_1_bss_64;
        fn_1_36A70(&spD0);
        if (lbl_1_bss_64 == 240.0f) {
            HuAudFXPlay(0x42A);
        }
        if (lbl_1_bss_64 <= 6.0f) {
            fn_1_36B10(1);
        }
        if (lbl_1_bss_64 == 0.0f) {
            fn_1_4FBC8(game, 0x34);
            game->f188 = 0xCE;
            game->f18C = 0x134;
        }
        break;
    case 0x136:                                     /* switch 1 */
        temp_r3_2 = fn_1_345B4(game);
        if (temp_r3_2 != 0) {
            game->f18C = 0x12C;
            game->f19C = 2;
            if (temp_r3_2 == 1) {
                fn_1_4FBC8(game, 0x3B);
                game->curPlayer += 1;
                if ((s32) game->curPlayer == 4) {
                    game->curPlayer = 0;
                }
            }
        }
        break;
    case 0x137:                                     /* switch 1 */
        fn_1_9DA8(&spD0, game);
        if ((s32) game->f1A4 == 0) {
            switch ((s32) lbl_1_bss_B0) {           /* switch 5; irregular */
            case 1:                                 /* switch 5 */
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x69], 0x10000000, HEAP_MODEL));
                game->pl[game->f1B0[0]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + spD0.x, spD0.y, spD0.z);
                Hu3DModelScaleSet(lbl_1_bss_72, 1.0f, 0.0f, 1.0f);
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[0]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DModelShadowSet(lbl_1_bss_72);
                lbl_1_bss_A0[0] = 0.0f;
                game->f1A4 = 1;
                break;
            case 2:                                 /* switch 5 */
                lbl_1_bss_72 = game->pl[game->f1B0[0]].f54;
                lbl_1_bss_70 = (s16) game->f1B0[0];
                if ((s16) lbl_1_bss_72 == -1) {
                    lbl_1_bss_72 = game->pl[game->f1B0[1]].f54;
                    lbl_1_bss_70 = (s16) game->f1B0[1];
                }
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    game->pl[lbl_1_bss_70].f54 = -1;
                    var_r29_2 = 0;
                    while (var_r29_2 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r29_2].f54) {
                            game->pl[var_r29_2].f54 = -1;
                        }
                        var_r29_2 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x6A], 0x10000000, HEAP_MODEL));
                game->pl[game->f1B0[0]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + spD0.x, spD0.y, spD0.z);
                game->pl[game->f1B0[1]].f54 = lbl_1_bss_72;
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[0]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[1]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_luiji");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            case 3:                                 /* switch 5 */
                lbl_1_bss_72 = game->pl[game->f1B0[0]].f54;
                lbl_1_bss_70 = (s16) game->f1B0[0];
                if ((s16) lbl_1_bss_72 == -1) {
                    lbl_1_bss_72 = game->pl[game->f1B0[1]].f54;
                    lbl_1_bss_70 = (s16) game->f1B0[1];
                    if ((s16) lbl_1_bss_72 == -1) {
                        lbl_1_bss_72 = game->pl[game->f1B0[2]].f54;
                        lbl_1_bss_70 = (s16) game->f1B0[2];
                    }
                }
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    game->pl[lbl_1_bss_70].f54 = -1;
                    var_r29_3 = 0;
                    while (var_r29_3 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r29_3].f54) {
                            game->pl[var_r29_3].f54 = -1;
                        }
                        var_r29_3 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x6B], 0x10000000, HEAP_MODEL));
                game->pl[game->f1B0[0]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + spD0.x, spD0.y, spD0.z);
                game->pl[game->f1B0[1]].f54 = lbl_1_bss_72;
                game->pl[game->f1B0[2]].f54 = lbl_1_bss_72;
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[0]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[1]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_luiji");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[2]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_peach");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            }
            if ((s32) lbl_1_bss_B0 != 1) {
                lbl_1_bss_AC = 0;
                lbl_1_bss_A8[1] = -1;
                lbl_1_bss_A8[0] = -1;
                lbl_1_bss_A0[1] = 0.0f;
                lbl_1_bss_A0[0] = 0.0f;
                lbl_1_bss_94 = 0;
                temp_r5_2 = game->f1B0[0];
                game->pl[temp_r5_2].f50 |= 0x80;
                game->f18C = 0x133;
            }
        } else {
            lbl_1_bss_A0[0] += 0.1f;
            if (lbl_1_bss_A0[0] > 1.0f) {
                lbl_1_bss_A0[0] = 1.0f;
            }
            lbl_1_bss_72 = game->pl[game->f1B0[0]].f54;
            Hu3DModelScaleSet(lbl_1_bss_72, 1.0f, lbl_1_bss_A0[0], 1.0f);
            if (lbl_1_bss_A0[0] == 1.0f) {
                lbl_1_bss_AC = 0;
                lbl_1_bss_A8[1] = -1;
                lbl_1_bss_A8[0] = -1;
                lbl_1_bss_A0[1] = 0.0f;
                lbl_1_bss_A0[0] = 0.0f;
                lbl_1_bss_94 = 0;
                temp_r4 = game->f1B0[0];
                game->pl[temp_r4].f50 |= 0x80;
                game->f18C = 0x133;
            }
        }
        break;
    case 0x138:                                     /* switch 1 */
        fn_1_9DA8(&spD0, game);
        game->f1A4 += 1;
        if ((s32) game->f1A4 == 0xA) {
            for (var_r28 = 0; var_r28 < (s32) lbl_1_bss_B0; var_r28++) {
                if (Hu3DData[lbl_1_bss_468[game->f1B0[var_r28]]].scale.y != 0.0f) {
                    sp8 = lbl_1_bss_468[game->f1B0[var_r28]];
                    temp_r4_2 = game->f1B0[var_r28];
                    game->pl[temp_r4_2].f50 |= 0x200;
                    if ((s32) (game->pl[game->f1B0[var_r28]].f50 & 4) != 0) {
                        tmp.x = 1.0f;
                        tmp.y = 0.0f;
                        tmp.z = 1.0f;
                        fn_1_3F6B0(4, game->pl[game->f1B0[var_r28]].f56, &tmp);
                    }
                    break;
                }
            }
            Hu3DModelScaleSet(sp8, 1.0f, 0.0f, 1.0f);
            switch ((s32) lbl_1_bss_B0) {           /* switch 6; irregular */
            case 1:                                 /* switch 6 */
                lbl_1_bss_72 = temp_r30->f54;
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    temp_r30->f54 = -1;
                    var_r29_4 = 0;
                    while (var_r29_4 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r29_4].f54) {
                            game->pl[var_r29_4].f54 = -1;
                        }
                        var_r29_4 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x69], 0x10000000, HEAP_MODEL));
                game->pl[game->f1B0[0]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + spD0.x, spD0.y, spD0.z);
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[0]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            case 2:                                 /* switch 6 */
                lbl_1_bss_72 = temp_r30->f54;
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    temp_r30->f54 = -1;
                    var_r29_5 = 0;
                    while (var_r29_5 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r29_5].f54) {
                            game->pl[var_r29_5].f54 = -1;
                        }
                        var_r29_5 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x6A], 0x10000000, HEAP_MODEL));
                game->pl[game->f1B0[0]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + spD0.x, spD0.y, spD0.z);
                game->pl[game->f1B0[1]].f54 = lbl_1_bss_72;
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[0]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[1]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_luiji");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            case 3:                                 /* switch 6 */
                lbl_1_bss_72 = temp_r30->f54;
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    temp_r30->f54 = -1;
                    var_r29_6 = 0;
                    while (var_r29_6 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r29_6].f54) {
                            game->pl[var_r29_6].f54 = -1;
                        }
                        var_r29_6 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x6B], 0x10000000, HEAP_MODEL));
                game->pl[game->f1B0[0]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + spD0.x, spD0.y, spD0.z);
                game->pl[game->f1B0[1]].f54 = lbl_1_bss_72;
                game->pl[game->f1B0[2]].f54 = lbl_1_bss_72;
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[0]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[1]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_luiji");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[game->f1B0[2]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_peach");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            }
        }
        if ((s32) game->f1A4 >= 0x14) {
            lbl_1_bss_A0[0] += 0.1f;
            if (lbl_1_bss_A0[0] > 1.0f) {
                lbl_1_bss_A0[0] = 1.0f;
            }
            Hu3DModelScaleSet(lbl_1_bss_468[game->order[game->curPlayer]], 1.0f, lbl_1_bss_A0[0], 1.0f);
            if ((s32) (temp_r30->f50 & 4) != 0) {
                tmp.x = 1.0f;
                tmp.y = lbl_1_bss_A0[0];
                tmp.z = 1.0f;
                fn_1_3F6B0(4, temp_r30->f56, &tmp);
            }
            if (lbl_1_bss_A0[0] == 1.0f) {
                temp_r30->f50 &= 0xFFFFFDFF;
                lbl_1_bss_A0[0] = 0.0f;
                temp_r30->f50 &= 0xFFFFFF7F;
                game->f18C = 0x12D;
            }
        }
        break;
    }
    return 0;
}

void fn_1_41EC(CardGame *game) {
    CardPlayer *temp_r31;

    temp_r31 = &game->pl[game->order[game->curPlayer]];
    if (temp_r31->rotY == 180.0f) {
        if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col] & 0x40) != 0) {
            if ((s32) (lbl_1_bss_8C4[temp_r31->row - 1][temp_r31->col] & 0x40) != 0) {
                temp_r31->row -= 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 300.0f;
            } else {
                temp_r31->row -= 1;
                temp_r31->f20 = 2;
                game->f18C = 0x131;
                lbl_1_bss_BC = 201.5625f;
            }
        } else {
            temp_r31->f20 -= 1;
            if ((s32) temp_r31->f20 >= 0) {
                game->f18C = 0x132;
                lbl_1_bss_BC = 98.4375f;
            } else if ((s32) (lbl_1_bss_8C4[temp_r31->row - 1][temp_r31->col] & 0x40) != 0) {
                temp_r31->row -= 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 201.5625f;
            } else {
                temp_r31->row -= 1;
                temp_r31->f20 = 2;
                game->f18C = 0x131;
                lbl_1_bss_BC = 103.125f;
            }
        }
    } else if (temp_r31->rotY == 0.0f) {
        if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col] & 0x40) != 0) {
            if ((s32) (lbl_1_bss_8C4[temp_r31->row + 1][temp_r31->col] & 0x40) != 0) {
                temp_r31->row += 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 300.0f;
            } else {
                temp_r31->row += 1;
                temp_r31->f20 = 0;
                game->f18C = 0x131;
                lbl_1_bss_BC = 201.5625f;
            }
        } else {
            temp_r31->f20 += 1;
            if ((s32) temp_r31->f20 <= 2) {
                game->f18C = 0x132;
                lbl_1_bss_BC = 98.4375f;
            } else if ((s32) (lbl_1_bss_8C4[temp_r31->row + 1][temp_r31->col] & 0x40) != 0) {
                temp_r31->row += 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 201.5625f;
            } else {
                temp_r31->row += 1;
                temp_r31->f20 = 0;
                game->f18C = 0x131;
                lbl_1_bss_BC = 103.125f;
            }
        }
    } else if (temp_r31->rotY == -90.0f) {
        if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col] & 0x40) != 0) {
            if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col - 1] & 0x40) != 0) {
                temp_r31->col -= 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 300.0f;
            } else {
                temp_r31->col -= 1;
                temp_r31->f24 = 2;
                game->f18C = 0x131;
                lbl_1_bss_BC = 201.5625f;
            }
        } else {
            temp_r31->f24 -= 1;
            if ((s32) temp_r31->f24 >= 0) {
                game->f18C = 0x132;
                lbl_1_bss_BC = 98.4375f;
            } else if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col - 1] & 0x40) != 0) {
                temp_r31->col -= 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 201.5625f;
            } else {
                temp_r31->col -= 1;
                temp_r31->f24 = 2;
                game->f18C = 0x131;
                lbl_1_bss_BC = 103.125f;
            }
        }
    } else if (temp_r31->rotY == 90.0f) {
        if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col] & 0x40) != 0) {
            if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col + 1] & 0x40) != 0) {
                temp_r31->col += 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 300.0f;
            } else {
                temp_r31->col += 1;
                temp_r31->f24 = 0;
                game->f18C = 0x131;
                lbl_1_bss_BC = 201.5625f;
            }
        } else {
            temp_r31->f24 += 1;
            if ((s32) temp_r31->f24 <= 2) {
                game->f18C = 0x132;
                lbl_1_bss_BC = 98.4375f;
            } else if ((s32) (lbl_1_bss_8C4[temp_r31->row][temp_r31->col + 1] & 0x40) != 0) {
                temp_r31->col += 1;
                temp_r31->f20 = 1;
                temp_r31->f24 = 1;
                game->f18C = 0x132;
                lbl_1_bss_BC = 201.5625f;
            } else {
                temp_r31->col += 1;
                temp_r31->f24 = 0;
                game->f18C = 0x131;
                lbl_1_bss_BC = 103.125f;
            }
        }
    }
    Hu3DModelRotSet(lbl_1_bss_468[game->order[game->curPlayer]], 0.0f, temp_r31->rotY, 0.0f);
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_49E4(CardGame *game) {
    Vec tmp;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    CardPlayer *temp_r29;
    s32 var_r28;
    s32 var_r30;
    s32 var_r30_2;
    s32 var_r30_3;

    var_r28 = 0;
    temp_r29 = &game->pl[game->order[game->curPlayer]];
    sp20 = lbl_1_bss_88.x;
    sp24 = lbl_1_bss_88.y;
    sp28 = lbl_1_bss_88.z;
    if ((s32) (temp_r29->f50 & 0x100) != 0) {
        lbl_1_bss_80 -= 0.1f;
        if (lbl_1_bss_80 < 0.0f) {
            lbl_1_bss_80 = 0.0f;
        }
        Hu3DModelScaleSet(lbl_1_bss_468[lbl_1_bss_74[0]], 1.0f, 1.0f - lbl_1_bss_80, 1.0f);
        if ((s32) (game->pl[lbl_1_bss_74[0]].f50 & 4) != 0) {
            tmp.x = 1.0f;
            tmp.y = 1.0f;
            tmp.z = 1.0f;
            tmp.y -= lbl_1_bss_80;
            fn_1_3F6B0(4, game->pl[lbl_1_bss_74[0]].f56, &tmp);
        }
        if ((s16) game->pl[lbl_1_bss_74[0]].f58 != -1) {
            Hu3DModelScaleSet(game->pl[lbl_1_bss_74[0]].f58, 1.0f, 1.0f - lbl_1_bss_80, 1.0f);
            game->pl[lbl_1_bss_74[0]].f50 &= 0xFFFFFF7F;
        }
        lbl_1_bss_94 = 1;
        if ((s32) lbl_1_bss_84 == 1) {
            lbl_1_bss_72 = game->pl[lbl_1_bss_74[0]].f54;
            Hu3DModelScaleSet(lbl_1_bss_72, 1.0f, lbl_1_bss_80, 1.0f);
        }
        if (lbl_1_bss_80 == 0.0f) {
            game->pl[lbl_1_bss_74[0]].f50 &= 0xFFFFFDFF;
            switch ((s32) lbl_1_bss_84) {           /* irregular */
            case 1:
                lbl_1_bss_72 = game->pl[lbl_1_bss_74[0]].f54;
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    game->pl[lbl_1_bss_74[0]].f54 = -1;
                    var_r30 = 0;
                    while (var_r30 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r30].f54) {
                            game->pl[var_r30].f54 = -1;
                        }
                        var_r30 += 1;
                    }
                }
                break;
            case 2:
                lbl_1_bss_72 = game->pl[lbl_1_bss_74[0]].f54;
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    game->pl[lbl_1_bss_74[0]].f54 = -1;
                    var_r30_2 = 0;
                    while (var_r30_2 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r30_2].f54) {
                            game->pl[var_r30_2].f54 = -1;
                        }
                        var_r30_2 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x69], 0x10000000, HEAP_MODEL));
                game->pl[lbl_1_bss_74[1]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + sp20, sp24, sp28);
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[lbl_1_bss_74[1]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            case 3:
                lbl_1_bss_72 = game->pl[lbl_1_bss_74[0]].f54;
                if ((s16) lbl_1_bss_72 != -1) {
                    Hu3DModelShadowReset(lbl_1_bss_72);
                    Hu3DAnimModelKill(lbl_1_bss_72);
                    Hu3DModelKill(lbl_1_bss_72);
                    game->pl[lbl_1_bss_74[0]].f54 = -1;
                    var_r30_3 = 0;
                    while (var_r30_3 < 4) {
                        if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r30_3].f54) {
                            game->pl[var_r30_3].f54 = -1;
                        }
                        var_r30_3 += 1;
                    }
                }
                lbl_1_bss_72 = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x6A], 0x10000000, HEAP_MODEL));
                game->pl[lbl_1_bss_74[1]].f54 = lbl_1_bss_72;
                Hu3DModelCameraSet(lbl_1_bss_72, 1U);
                Hu3DModelLayerSet(lbl_1_bss_72, 2);
                Hu3DModelPosSet(lbl_1_bss_72, -50.0f + sp20, sp24, sp28);
                game->pl[lbl_1_bss_74[2]].f54 = lbl_1_bss_72;
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[lbl_1_bss_74[1]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_mario");
                Hu3DAnimCreate(HuDataReadNum(lbl_1_data_B0[game->pl[lbl_1_bss_74[2]].charNo + 0x6D], 0x10000000), lbl_1_bss_72, "s_luiji");
                Hu3DModelShadowSet(lbl_1_bss_72);
                break;
            }
            temp_r29->f50 &= 0xFFFFFEFF;
            var_r28 = 1;
        }
    } else {
        var_r28 = 1;
    }
    return var_r28;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_5248(CardGame *game) {
    Vec tmp;
    s32 temp_r29;
    s32 var_r30;

    var_r30 = 0;
    temp_r29 = fn_1_49E4(game);
    if ((s32) lbl_1_bss_AC != 0) {
        *lbl_1_bss_A0 -= 0.1f;
        if (*lbl_1_bss_A0 < 0.0f) {
            *lbl_1_bss_A0 = 0.0f;
        }
        Hu3DModelScaleSet(*lbl_1_bss_A8, 1.0f, 1.0f - *lbl_1_bss_A0, 1.0f);
        if ((s32) (game->pl[*lbl_1_bss_98].f50 & 4) != 0) {
            tmp.x = 1.0f;
            tmp.y = 1.0f;
            tmp.z = 1.0f;
            tmp.y -= *lbl_1_bss_A0;
            fn_1_3F6B0(4, game->pl[*lbl_1_bss_98].f56, &tmp);
        }
        if ((s16) game->pl[*lbl_1_bss_98].f58 != -1) {
            Hu3DModelScaleSet(game->pl[*lbl_1_bss_98].f58, 1.0f, 1.0f - *lbl_1_bss_A0, 1.0f);
        }
        if (*lbl_1_bss_A0 == 0.0f) {
            game->pl[*lbl_1_bss_98].f50 &= 0xFFFFFDFF;
            lbl_1_bss_AC = 0;
            *lbl_1_bss_A8 = -1;
            *lbl_1_bss_98 = -1;
        }
        lbl_1_bss_94 = 1;
    }
    if (((s32) lbl_1_bss_AC == 0) && (temp_r29 != 0)) {
        var_r30 = 1;
    }
    return var_r30;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_5500(CardGame *game) {
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r3_3;
    s32 var_r29;
    s32 var_r30;

    lbl_1_bss_AC = 0;
    lbl_1_bss_A8[1] = -1;
    lbl_1_bss_A8[0] = -1;
    lbl_1_bss_A0[1] = 0.0f;
    lbl_1_bss_A0[0] = 0.0f;
    lbl_1_bss_98[1] = -1;
    lbl_1_bss_98[0] = -1;
    lbl_1_bss_94 = 0;
    var_r30 = 0;
    while (var_r30 < 4) {
        Hu3DModelScaleSet(lbl_1_bss_468[var_r30], 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrReset(lbl_1_bss_468[var_r30], 1U);
        lbl_1_bss_72 = game->pl[var_r30].f54;
        if ((s16) lbl_1_bss_72 != -1) {
            Hu3DModelShadowReset(lbl_1_bss_72);
            Hu3DAnimModelKill(lbl_1_bss_72);
            Hu3DModelKill(lbl_1_bss_72);
            game->pl[var_r30].f54 = -1;
            var_r29 = 0;
            while (var_r29 < 4) {
                if ((s16) lbl_1_bss_72 == (s16) game->pl[var_r29].f54) {
                    game->pl[var_r29].f54 = -1;
                }
                var_r29 += 1;
            }
        }
        game->pl[var_r30].f50 &= 0xFFFFFF7F;
        game->pl[var_r30].f50 &= 0xFFFFFEFF;
        game->pl[var_r30].f50 &= 0xFFFFFDFF;
        var_r30 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_5710(CardGame *game, s32 arg1, s32 arg2) {
    CardPlayer *temp_r30;
    CardPlayer *temp_r31;
    s32 temp_r21;
    s32 temp_r21_2;
    s32 temp_r21_3;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r25;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;

    temp_r31 = &game->pl[game->order[arg1]];
    var_r29 = temp_r31->row;
    var_r28 = temp_r31->col;
    var_r24 = temp_r31->f20;
    var_r23 = temp_r31->f24;
    if (arg2 > 0) {
        if (temp_r31->rotY == 180.0f) {
            var_r29 += 1;
            if ((s32) (lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r24 = 1;
            } else {
                var_r24 = 0;
            }
        } else if (temp_r31->rotY == 0.0f) {
            var_r29 -= 1;
            if ((s32) (lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r24 = 1;
            } else {
                var_r24 = 2;
            }
        } else if (temp_r31->rotY == -90.0f) {
            var_r28 += 1;
            if ((s32) (lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r23 = 1;
            } else {
                var_r23 = 0;
            }
        } else if (temp_r31->rotY == 90.0f) {
            var_r28 -= 1;
            if ((s32) (lbl_1_bss_8C4[var_r29][var_r28] & 0x40) != 0) {
                var_r23 = 1;
            } else {
                var_r23 = 2;
            }
        }
    }
    var_r22 = 0;
    var_r26 = 0;
    var_r25 = 0;
    var_r27 = 0;
    while (var_r27 < 4) {
        if (var_r27 != arg1) {
            temp_r30 = &game->pl[game->order[var_r27]];
            if ((var_r29 == (s32) temp_r30->row) && (var_r28 == (s32) temp_r30->col) && (var_r24 == (s32) temp_r30->f20) && (var_r23 == (s32) temp_r30->f24) && ((s32) temp_r30->f00 == 0)) {
                game->f1B0[var_r22] = game->order[var_r27];
                var_r22 += 1;
                if (var_r27 < arg1) {
                    var_r26 += 1;
                } else if (var_r27 > arg1) {
                    var_r25 += 1;
                }
            }
        }
        var_r27 += 1;
    }
    if (arg2 == 2) {
        return var_r22;
    }
    if ((var_r26 > 0) && (var_r25 > 0)) {
        if ((var_r26 == 1) && (var_r25 == 1)) {
            temp_r21 = game->f1B0[0];
            game->f1B0[0] = game->f1B0[1];
            game->f1B0[1] = temp_r21;
        } else if ((var_r26 == 2) && (var_r25 == 1)) {
            temp_r21_2 = game->f1B0[2];
            game->f1B0[2] = game->f1B0[1];
            game->f1B0[1] = game->f1B0[0];
            game->f1B0[0] = temp_r21_2;
        } else if ((var_r26 == 1) && (var_r25 == 2)) {
            temp_r21_3 = game->f1B0[0];
            game->f1B0[0] = game->f1B0[1];
            game->f1B0[1] = game->f1B0[2];
            game->f1B0[2] = temp_r21_3;
        }
    }
    return var_r22;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_5A00(CardGame *game) {
    CardPlayer *temp_r30;
    s32 temp_r0;
    s32 temp_r3;
    s32 temp_r4;

    temp_r30 = &game->pl[game->order[game->curPlayer]];
    if ((s16) GwPlayerConf[game->order[game->curPlayer]].type != 0) {
        HuPadBtnDown[game->order[game->curPlayer]] = 0;
        HuPadDStkRep[game->order[game->curPlayer]] = 0;
        temp_r3 = game->f18C;
        switch (temp_r3) {                          /* switch 1; irregular */
        case 0x12D:                                 /* switch 1 */
            temp_r4 = lbl_1_bss_B8 + 1;
            lbl_1_bss_B8 = temp_r4;
            if (temp_r4 >= 0x64) {
                HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                lbl_1_bss_B8 = 0;
                return;
            }
            break;
        case 0x12E:                                 /* switch 1 */
            temp_r0 = lbl_1_bss_B8 + 1;
            lbl_1_bss_B8 = temp_r0;
            if (temp_r0 >= 0x1E) {
                if ((s32) (temp_r30->f50 & 0x20) != 0) {
                    HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                    lbl_1_bss_B8 = 0;
                    return;
                }
                if ((s32) temp_r30->f34 != -1) {
                    if (fn_1_25624(game) != 0) {
                        if ((s32) temp_r30->f38 != -1) {
                            if ((s32) temp_r30->f3C != -1) {
                                if (((s32) temp_r30->f34 == 0) && ((s32) temp_r30->f38 == 0) && ((s32) temp_r30->f3C == 0)) {
                                    HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                                } else if (((s32) temp_r30->f34 == 0) || ((s32) temp_r30->f38 == 0) || ((s32) temp_r30->f3C == 0)) {
                                    HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                                } else {
                                    HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                                }
                            } else if (((s32) temp_r30->f34 == 0) && ((s32) temp_r30->f38 == 0)) {
                                HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                            } else if (((s32) temp_r30->f34 == 0) || ((s32) temp_r30->f38 == 0)) {
                                HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                            } else {
                                HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                            }
                        } else if ((s32) temp_r30->f34 == 0) {
                            HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                        } else {
                            HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                        }
                    } else {
                        HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                        if ((s32) temp_r30->f38 != -1) {
                            if ((s32) temp_r30->f3C != -1) {
                                if (((s32) temp_r30->f34 == 0) && ((s32) temp_r30->f38 == 0) && ((s32) temp_r30->f3C == 0)) {
                                    if (fn_1_21028(game) == 0) {
                                        HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                                    } else {
                                        HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                                    }
                                } else {
                                    HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                                }
                            } else if (((s32) temp_r30->f34 == 0) && ((s32) temp_r30->f38 == 0)) {
                                if (fn_1_21028(game) == 0) {
                                    HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                                } else {
                                    HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                                }
                            } else {
                                HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                            }
                        } else if ((s32) temp_r30->f34 == 0) {
                            if (fn_1_21028(game) == 0) {
                                HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                            } else {
                                HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                            }
                        } else {
                            HuPadBtnDown[game->order[game->curPlayer]] = 0x200;
                        }
                    }
                } else {
                    HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                }
                lbl_1_bss_B8 = 0;
                return;
            }
            break;
        case 0x12F:                                 /* switch 1 */
            lbl_1_bss_B8 += 1;
            lbl_1_bss_B8 %= 0x12C;
            if ((s32) temp_r30->f00 != 0) {
                if (((s32) game->order[game->curPlayer] == 0) && ((s32) lbl_1_bss_B8 >= 0x1E)) {
                    HuPadDStkRep[0] = 4;
                    lbl_1_bss_B8 = 0;
                    return;
                }
                if (((s32) game->order[game->curPlayer] == 1) && ((s32) lbl_1_bss_B8 >= 0x1E)) {
                    HuPadDStkRep[1] = 8;
                    lbl_1_bss_B8 = 0;
                    return;
                }
                if (((s32) game->order[game->curPlayer] == 2) && ((s32) lbl_1_bss_B8 >= 0x1E)) {
                    HuPadDStkRep[2] = 2;
                    lbl_1_bss_B8 = 0;
                    return;
                }
                if (((s32) game->order[game->curPlayer] == 3) && ((s32) lbl_1_bss_B8 >= 0x1E)) {
                    HuPadDStkRep[3] = 1;
                    lbl_1_bss_B8 = 0;
                    return;
                }
            } else {
                switch ((s32) lbl_1_bss_B8) {       /* switch 2; irregular */
                case 0x1E:                          /* switch 2 */
                    HuPadDStkRep[game->order[game->curPlayer]] = fn_1_62A4(game);
                    return;
                case 0x3C:                          /* switch 2 */
                    HuPadBtnDown[game->order[game->curPlayer]] = 0x100;
                    lbl_1_bss_B8 = 0;
                    lbl_1_bss_C3C = 0;
                    break;
                }
            }
            break;
        }
    }
}

/* COM: choose a D-stick direction (8 up, 4 down, 1 left, 2 right) toward an interesting card. */
// NONMATCHING: the original repeats the per-direction scoring inline (same result)
u8 fn_1_62A4(CardGame *game)
{
    CardPlayer *pl;
    s32 t;
    s32 score;
    s32 i;
    s32 r;
    s32 c;
    u8 dir;

    dir = 0;
    score = 0;
    pl = &game->pl[game->order[game->curPlayer]];
    if (lbl_1_bss_C3C != 0) {
        t = fn_1_21028(game);
        if (t != 0) {
            if (t == 1) {
                return 8;
            }
            if (t == 2) {
                return 4;
            }
            if (t == 3) {
                return 1;
            }
            return 2;
        }
    }
    for (i = 0; i < 2; i++) {
        /* up */
        if ((CARD_WALL(pl) & 1) && (pl->row - i) > 0 && (lbl_1_bss_8C4[pl->row - (i + 1)][pl->col] & 0xF00) != 0
            && (lbl_1_bss_8C4[pl->row - (i + 1)][pl->col] & 0x800) == 0 && (lbl_1_bss_8C4[pl->row][pl->col] & 1) != 0) {
            if ((lbl_1_bss_8C4[pl->row - (i + 1)][pl->col] & 0x400) != 0) {
                dir = 8;
                score = 1;
                if (frandmod(2) != 0) {
                    break;
                }
            } else {
                if (score == 0) {
                    dir = 8;
                    score = -1;
                }
            }
        }
        /* down */
        if ((CARD_WALL(pl) & 2) && (pl->row + i) < (game->boardSize - 1) && (lbl_1_bss_8C4[i + pl->row + 1][pl->col] & 0xF00) != 0
            && (lbl_1_bss_8C4[i + pl->row + 1][pl->col] & 0x800) == 0 && (lbl_1_bss_8C4[pl->row][pl->col] & 2) != 0) {
            if ((lbl_1_bss_8C4[i + pl->row + 1][pl->col] & 0x400) != 0) {
                dir = 4;
                score = 1;
                if (frandmod(2) != 0) {
                    break;
                }
            } else {
                if (score == 0) {
                    dir = 4;
                    score = -1;
                }
            }
        }
        /* left */
        if ((CARD_WALL(pl) & 4) && (pl->col - i) > 0 && (lbl_1_bss_8C4[pl->row][pl->col - (i + 1)] & 0xF00) != 0
            && (lbl_1_bss_8C4[pl->row][pl->col - (i + 1)] & 0x800) == 0 && (lbl_1_bss_8C4[pl->row][pl->col] & 4) != 0) {
            if ((lbl_1_bss_8C4[pl->row][pl->col - (i + 1)] & 0x400) != 0) {
                dir = 1;
                score = 1;
                if (frandmod(2) != 0) {
                    break;
                }
            } else {
                if (score == 0) {
                    dir = 1;
                    score = -1;
                }
            }
        }
        /* right */
        if ((CARD_WALL(pl) & 8) && (pl->col + i) < (game->boardSize - 1) && (lbl_1_bss_8C4[pl->row][i + pl->col + 1] & 0xF00) != 0
            && (lbl_1_bss_8C4[pl->row][i + pl->col + 1] & 0x800) == 0 && (lbl_1_bss_8C4[pl->row][pl->col] & 8) != 0) {
            if ((lbl_1_bss_8C4[pl->row][i + pl->col + 1] & 0x400) != 0) {
                dir = 2;
                score = 1;
                if (frandmod(2) != 0) {
                    break;
                }
            } else {
                if (score == 0) {
                    dir = 2;
                    score = -1;
                }
            }
        }
        if (score == 1 || (score == -1 && frandmod(2) != 0)) {
            break;
        }
    }
    if (score == 0) {
        for (r = 0; r < (game->boardSize - 1); r++) {
            for (c = 0; c < (game->boardSize - 1); c++) {
                if ((lbl_1_bss_8C4[r][c] & 0xF00) == 0) {
                    continue;
                }
                if (frandmod(2) != 0) {
                    if (r < pl->row) {
                        dir = (CARD_WALL(pl) & 1) ? 8 : fn_1_7098(game, r, c, 0);
                    } else if (r > pl->row) {
                        dir = (CARD_WALL(pl) & 2) ? 4 : fn_1_7098(game, r, c, 0);
                    } else if (c < pl->col) {
                        dir = (CARD_WALL(pl) & 4) ? 1 : fn_1_7098(game, r, c, 1);
                    } else {
                        dir = (CARD_WALL(pl) & 8) ? 2 : fn_1_7098(game, r, c, 1);
                    }
                } else {
                    if (c < pl->col) {
                        dir = (CARD_WALL(pl) & 4) ? 1 : fn_1_7098(game, r, c, 1);
                    } else if (c > pl->col) {
                        dir = (CARD_WALL(pl) & 8) ? 2 : fn_1_7098(game, r, c, 1);
                    } else if (r < pl->row) {
                        dir = (CARD_WALL(pl) & 1) ? 8 : fn_1_7098(game, r, c, 0);
                    } else {
                        dir = (CARD_WALL(pl) & 2) ? 4 : fn_1_7098(game, r, c, 0);
                    }
                }
                if (frandmod(2) != 0) {
                    if ((lbl_1_bss_8C4[r][c] & 0x400) != 0) {
                        r = game->boardSize - 1;
                        c = game->boardSize - 1;
                    } else if (frandmod(2) != 0) {
                        r = game->boardSize - 1;
                        c = game->boardSize - 1;
                    }
                }
            }
        }
    }
    return dir;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
u8 fn_1_7098(CardGame *game, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp8;
    CardPlayer *temp_r31;
    u16 var_r23;
    u16 var_r24;
    u16 var_r25;
    u16 var_r26;
    u16 var_r27;
    u16 var_r28;
    u16 var_r30;

    sp8 = arg3;
    var_r30 = 0;
    temp_r31 = &game->pl[game->order[game->curPlayer]];
    switch (sp8) {                                  /* irregular */
    case 0:
        if ((s32) (CARD_WALL(temp_r31) & 4) != 0) {
            var_r30 = 1;
        }
        if ((s32) (CARD_WALL(temp_r31) & 8) != 0) {
            if (var_r30 == 1) {
                if (arg2 < (s32) temp_r31->col) {
                    var_r28 = 1;
                } else {
                    if (arg2 > (s32) temp_r31->col) {
                        var_r27 = 2;
                    } else {
                        if (frandmod(2U) != 0) {
                            var_r26 = 1;
                        } else {
                            var_r26 = 2;
                        }
                        var_r27 = var_r26;
                    }
                    var_r28 = var_r27;
                }
                var_r30 = var_r28;
            } else {
                var_r30 = 2;
            }
        }
        break;
    case 1:
        if ((s32) (CARD_WALL(temp_r31) & 1) != 0) {
            var_r30 = 8;
        }
        if ((s32) (CARD_WALL(temp_r31) & 2) != 0) {
            if (var_r30 == 8) {
                if (arg1 < (s32) temp_r31->row) {
                    var_r25 = 8;
                } else {
                    if (arg1 > (s32) temp_r31->row) {
                        var_r24 = 4;
                    } else {
                        if (frandmod(2U) != 0) {
                            var_r23 = 8;
                        } else {
                            var_r23 = 4;
                        }
                        var_r24 = var_r23;
                    }
                    var_r25 = var_r24;
                }
                var_r30 = var_r25;
            } else {
                var_r30 = 4;
            }
        }
        break;
    }
    return (u8) var_r30;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_7304(void) {
    Vec v;
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 4) {
        lbl_1_bss_2F8[var_r31][0] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x40], 0x10000000, HEAP_MODEL));
        lbl_1_bss_2F8[var_r31][1] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_B0[0x41], 0x10000000, HEAP_MODEL));
        Hu3DModelCameraSet(lbl_1_bss_2F8[var_r31][0], 1U);
        Hu3DModelCameraSet(lbl_1_bss_2F8[var_r31][1], 1U);
        switch (var_r31) {                          /* irregular */
        case 0:
            v.x = -50.0f;
            v.y = 180.0f;
            v.z = -55.0f;
            lbl_1_bss_2C8[var_r31].x = 0.0f;
            lbl_1_bss_2C8[var_r31].y = 0.0f;
            lbl_1_bss_2C8[var_r31].z = 0.0f;
            break;
        case 1:
            v.x = -50.0f;
            v.y = 180.0f;
            v.z = 55.0f;
            lbl_1_bss_2C8[var_r31].x = 0.0f;
            lbl_1_bss_2C8[var_r31].y = 180.0f;
            lbl_1_bss_2C8[var_r31].z = 0.0f;
            break;
        case 2:
            v.x = -105.0f;
            v.y = 180.0f;
            v.z = 0.0f;
            lbl_1_bss_2C8[var_r31].x = 0.0f;
            lbl_1_bss_2C8[var_r31].y = 90.0f;
            lbl_1_bss_2C8[var_r31].z = 0.0f;
            break;
        default:
            v.x = 5.0f;
            v.y = 180.0f;
            v.z = 0.0f;
            lbl_1_bss_2C8[var_r31].x = 0.0f;
            lbl_1_bss_2C8[var_r31].y = -90.0f;
            lbl_1_bss_2C8[var_r31].z = 0.0f;
            break;
        }
        Hu3DModelPosSetV(lbl_1_bss_2F8[var_r31][0], &v);
        Hu3DModelPosSetV(lbl_1_bss_2F8[var_r31][1], &v);
        Hu3DModelRotSetV(lbl_1_bss_2F8[var_r31][0], &lbl_1_bss_2C8[var_r31]);
        Hu3DModelRotSetV(lbl_1_bss_2F8[var_r31][1], &lbl_1_bss_2C8[var_r31]);
        Hu3DModelAttrSet(lbl_1_bss_2F8[var_r31][0], 1U);
        Hu3DModelAttrSet(lbl_1_bss_2F8[var_r31][1], 1U);
        Hu3DModelLayerSet(lbl_1_bss_2F8[var_r31][0], 3);
        Hu3DModelLayerSet(lbl_1_bss_2F8[var_r31][1], 3);
        Hu3DModelScaleSet(lbl_1_bss_2F8[var_r31][0], 0.7f, 0.7f, 0.7f);
        Hu3DModelScaleSet(lbl_1_bss_2F8[var_r31][1], 0.7f, 0.7f, 0.7f);
        var_r31 += 1;
    }
    lbl_1_bss_2C4 = 0;
    lbl_1_bss_2BC[0] = 0.0f;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_77D0(Vec *pos, CardGame *game) {
    Vec v;
    CardPlayer *temp_r31;
    s32 var_r29;

    temp_r31 = &game->pl[game->order[game->curPlayer]];
    if ((s32) lbl_1_bss_2C4 == 0) {
        lbl_1_bss_2BC[0] += 2.0f;
        if (lbl_1_bss_2BC[0] >= 20.0f) {
            lbl_1_bss_2C4 = 1;
        }
    } else {
        lbl_1_bss_2BC[0] -= 2.0f;
        if (lbl_1_bss_2BC[0] <= 0.0f) {
            lbl_1_bss_2C4 = 0;
        }
    }
    var_r29 = 0;
    while (var_r29 < 4) {
        v.x = pos->x;
        v.y = pos->y;
        v.z = pos->z;
        switch (var_r29) {                          /* irregular */
        case 0:
            v.x += -50.0f;
            v.y += 180.0f;
            v.z += -55.0f - lbl_1_bss_2BC[0];
            break;
        case 1:
            v.x += -50.0f;
            v.y += 180.0f;
            v.z += 55.0f + lbl_1_bss_2BC[0];
            break;
        case 2:
            v.x += -50.0f + (-55.0f - lbl_1_bss_2BC[0]);
            v.y += 180.0f;
            break;
        default:
            v.x += -50.0f + (55.0f + lbl_1_bss_2BC[0]);
            v.y += 180.0f;
            break;
        }
        Hu3DModelPosSetV(lbl_1_bss_2F8[var_r29][0], &v);
        Hu3DModelPosSetV(lbl_1_bss_2F8[var_r29][1], &v);
        Hu3DModelAttrSet(lbl_1_bss_2F8[var_r29][0], 1U);
        Hu3DModelAttrSet(lbl_1_bss_2F8[var_r29][1], 1U);
        var_r29 += 1;
    }
    if (((s32) (CARD_WALL(temp_r31) & 1) != 0) && (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r31->rotY2 != 0.0f)))) {
        if (temp_r31->rotY == 180.0f) {
            Hu3DModelAttrReset(lbl_1_bss_2F8[0][0], 1U);
        } else {
            Hu3DModelAttrReset(lbl_1_bss_2F8[0][1], 1U);
        }
    }
    if (((s32) (CARD_WALL(temp_r31) & 2) != 0) && (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r31->rotY2 != 180.0f)))) {
        if (temp_r31->rotY == 0.0f) {
            Hu3DModelAttrReset(lbl_1_bss_2F8[1][0], 1U);
        } else {
            Hu3DModelAttrReset(lbl_1_bss_2F8[1][1], 1U);
        }
    }
    if (((s32) (CARD_WALL(temp_r31) & 4) != 0) && (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r31->rotY2 != 90.0f)))) {
        if (temp_r31->rotY == -90.0f) {
            Hu3DModelAttrReset(lbl_1_bss_2F8[2][0], 1U);
        } else {
            Hu3DModelAttrReset(lbl_1_bss_2F8[2][1], 1U);
        }
    }
    if (((s32) (CARD_WALL(temp_r31) & 8) != 0) && (((s32) game->f1A0 != 0) || (((s32) game->f1A0 == 0) && (temp_r31->rotY2 != -90.0f)))) {
        if (temp_r31->rotY == 90.0f) {
            Hu3DModelAttrReset(lbl_1_bss_2F8[3][0], 1U);
            return;
        }
        Hu3DModelAttrReset(lbl_1_bss_2F8[3][1], 1U);
    }
}

void fn_1_7DFC(void) {
    lbl_1_bss_2C4 = 0;
    lbl_1_bss_2BC[0] = 0.0f;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_7E28(void) {
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 4) {
        Hu3DModelAttrSet(lbl_1_bss_2F8[var_r31][0], 1U);
        Hu3DModelAttrSet(lbl_1_bss_2F8[var_r31][1], 1U);
        var_r31 += 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_7E98(CardPlayer *pl) {
    f32 var_f28;
    f32 var_f29;
    f32 var_f30;
    f32 var_f31;

    if (pl->rotY == 180.0f) {
        if ((CARD_WALL(pl) == 0xE) || (CARD_WALL(pl) == 0xC)) {
            if (frandmod(2U) != 0) {
                var_f31 = -90.0f;
            } else {
                var_f31 = 90.0f;
            }
            pl->rotY = var_f31;
            return;
        }
        if (CARD_WALL(pl) == 6) {
            pl->rotY = -90.0f;
            return;
        }
        if (CARD_WALL(pl) == 0xA) {
            pl->rotY = 90.0f;
            return;
        }
        return;
    } else if (pl->rotY == 0.0f) {
        if ((CARD_WALL(pl) == 0xD) || (CARD_WALL(pl) == 0xC)) {
            if (frandmod(2U) != 0) {
                var_f30 = -90.0f;
            } else {
                var_f30 = 90.0f;
            }
            pl->rotY = var_f30;
            return;
        }
        if (CARD_WALL(pl) == 5) {
            pl->rotY = -90.0f;
            return;
        }
        if (CARD_WALL(pl) == 9) {
            pl->rotY = 90.0f;
            return;
        }
    } else if (pl->rotY == -90.0f) {
        if ((CARD_WALL(pl) == 0xB) || (CARD_WALL(pl) == 3)) {
            if (frandmod(2U) != 0) {
                var_f29 = 180.0f;
            } else {
                var_f29 = 0.0f;
            }
            pl->rotY = var_f29;
            return;
        }
        if (CARD_WALL(pl) == 9) {
            pl->rotY = 180.0f;
            return;
        }
        if (CARD_WALL(pl) == 0xA) {
            pl->rotY = 0.0f;
            return;
        }
    } else if (pl->rotY == 90.0f) {
        if ((CARD_WALL(pl) == 7) || (CARD_WALL(pl) == 3)) {
            if (frandmod(2U) != 0) {
                var_f28 = 180.0f;
            } else {
                var_f28 = 0.0f;
            }
            pl->rotY = var_f28;
            return;
        }
        if (CARD_WALL(pl) == 5) {
            pl->rotY = 180.0f;
            return;
        }
        if (CARD_WALL(pl) == 6) {
            pl->rotY = 0.0f;
        }
    }
}

void fn_1_850C(u8 *alpha, OMOBJ **obj) {
    lbl_1_bss_5C = Hu3DHookFuncCreate(fn_1_8568);
    Hu3DModelLayerSet(lbl_1_bss_5C, 5);
    Hu3DModelCameraSet(lbl_1_bss_5C, 1U);
}

/* Model hook: draws the black full-screen fade quad (alpha = lbl_1_bss_5E) in screen space. */
// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_8568(HU3DMODEL *model, Mtx *mtx)
{
    Mtx44 proj;
    GXColor color;

    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = lbl_1_bss_5E;
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    C_MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -1.6777215e7f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(*mtx);
    GXLoadPosMtxImm(*mtx, 0);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(0);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
    GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(0, 0, 0);
    GXPosition3s16(640, 0, 0);
    GXPosition3s16(640, 480, 0);
    GXPosition3s16(0, 480, 0);
}

/* Show the compass arrow (model 2..5) that matches where the camera looks; lbl_1_bss_4C tracks which are shown. */
void fn_1_8894(void)
{
    Vec dir;
    Mtx rot;
    f32 yaw;
    f32 pitch;

    PSVECSubtract(&Hu3DCamera->target, &Hu3DCamera->pos, &dir);
    PSVECNormalize(&dir, &dir);
    yaw = (f32)(180.0 * (atan2((f64)dir.x, (f64)dir.z) / 3.141592653589793));
    Hu3DModelAttrReset(lbl_1_bss_308[2], 1);
    lbl_1_bss_4C[0] = 0;
    Hu3DModelAttrReset(lbl_1_bss_308[3], 1);
    lbl_1_bss_4C[1] = 0;
    Hu3DModelAttrReset(lbl_1_bss_308[4], 1);
    lbl_1_bss_4C[2] = 0;
    Hu3DModelAttrReset(lbl_1_bss_308[5], 1);
    lbl_1_bss_4C[3] = 0;
    PSMTXRotRad(rot, 'y', 0.017453292f * -yaw);
    PSMTXMultVec(rot, &dir, &dir);
    pitch = (f32)(180.0 * (atan2((f64)dir.y, (f64)dir.z) / 3.141592653589793));
    if (pitch <= -30.0f) {
        return;
    }
    if (yaw >= 0.0f && yaw <= 90.0f) {
        Hu3DModelAttrSet(lbl_1_bss_308[2], 1);
        lbl_1_bss_4C[0] = 1;
    }
    if (yaw <= 0.0f && yaw >= -90.0f) {
        Hu3DModelAttrSet(lbl_1_bss_308[3], 1);
        lbl_1_bss_4C[1] = 1;
    }
    if (yaw >= 90.0f && yaw <= 180.0f) {
        Hu3DModelAttrSet(lbl_1_bss_308[4], 1);
        lbl_1_bss_4C[2] = 1;
    }
    if (yaw <= -90.0f && yaw >= -180.0f) {
        Hu3DModelAttrSet(lbl_1_bss_308[5], 1);
        lbl_1_bss_4C[3] = 1;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_8B84(CardGame *game) {
    s32 temp_r0;

    temp_r0 = game->f188;
    switch (temp_r0) {                              /* irregular */
    case 0xCC:
    case 0xC8:
        Hu3DModelAttrReset(lbl_1_bss_308[2], 1U);
        Hu3DModelAttrReset(lbl_1_bss_308[3], 1U);
        Hu3DModelAttrReset(lbl_1_bss_308[4], 1U);
        Hu3DModelAttrReset(lbl_1_bss_308[5], 1U);
        return;
    case 0xCD:
        if ((s32) lbl_1_bss_4C[2] == 0) {
            Hu3DModelAttrReset(lbl_1_bss_308[4], 1U);
        }
        if ((s32) lbl_1_bss_4C[3] == 0) {
            Hu3DModelAttrReset(lbl_1_bss_308[5], 1U);
        }
        return;
    }
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
void fn_1_8C78(void)
{
    s32 i;

    lbl_1_bss_4A = 0;
    lbl_1_bss_48 = 0;
    for (i = 0; i < 10; i++) {
        lbl_1_bss_20[i] = 0;
    }
    lbl_1_bss_8[1] = 0;
    lbl_1_bss_8[0] = 0;
    lbl_1_bss_14 = -1;
}

/* Play sound arg0 for model arg1 panned by its screen position (camera arg2); returns the voice, -1 if off screen. */
// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_8D04(s32 arg0, s16 arg1, s32 arg2)
{
    Vec pos;
    Vec scr;
    s16 camNo;
    s16 model;
    HU3DMODEL *m;
    s32 snd;

    model = arg1;
    camNo = (s16)arg2;
    m = &Hu3DData[model];
    pos.x = m->pos.x;
    pos.y = m->pos.y;
    pos.z = m->pos.z;
    Hu3D3Dto2D(&pos, camNo, &scr);
    if (scr.x < 0.0f || scr.x > 576.0f) {
        return -1;
    }
    if (scr.y < 0.0f || scr.y > 480.0f) {
        return -1;
    }
    if ((arg0 & 0x8000) == 0) {
        snd = HuAudFXPlay(arg0 & 0x7FF);
        if (arg0 == 0x40B || arg0 == 0x40C || arg0 == 0x353 || arg0 == 0x43A || arg0 == 0x446 || arg0 == 0x456) {
            lbl_1_bss_18 = snd;
        }
    } else {
        snd = lbl_1_bss_18;
    }
    HuAudFXPanning(snd, (s32)(scr.x / 9.0f) + 0x20);
    return snd;
}

// NONMATCHING: register allocation / instruction scheduling differs from the original
s32 fn_1_8E88(void) {
    return lbl_1_bss_E4.pl[lbl_1_bss_E4.order[lbl_1_bss_E4.curPlayer]].f50;
}

