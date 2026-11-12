#include "REL/m502dll.h"

M502MAIN *lbl_1_data_0 = &lbl_1_bss_0;

static s32 lbl_1_data_3C[0x8D] = {
    0x3C0000, 0x3C0001, 0x3C0002, 0x3C0003, 0x3C0004, 0x3C0005,
    0x3C0006, 0x3C0007, 0x3C0008, 0x3C0009, 0x3C000A, 0x3C000B,
    0x3C000C, 0x3C000D, 0x3C000E, 0x3C000F, 0x3C0010, 0x3C0011,
    0x3C0012, 0x3C0013, 0x3C0014, 0x3C0015, 0x3C0016, 0x3C0017,
    0x3C0018, 0x3C0019, 0x3C001A, 0x3C001B, 0x3C001C, 0x3C001D,
    0x3C001E, 0x3C001F, 0x3C0020, 0x3C0021, 0x3C0022, 0x3C0023,
    0x3C0024, 0x3C0025, 0x3C0026, 0x3C0027, 0x3C0028, 0x3C0029,
    0x3C002A, 0x3C002B, 0x3C002C, 0x3C002D, 0x3C002E, 0x3C002F,
    0x3C0030, 0x3C0031, 0x3C0032, 0x3C0033, 0x3C0034, 0x3C0035,
    0x3C0036, 0x3C0037, 0x3C0038, 0x3C0039, 0x3C003A, 0x3C003B,
    0x3C003C, 0x3C003D, 0x3C003E, 0x3C003F, 0x3C0040, 0x3C0041,
    0x3C0042, 0x3C0043, 0x3C0044, 0x3C0045, 0x3C0046, 0x3C0047,
    0x3C0048, 0x3C0049, 0x3C004A, 0x3C004B, 0x3C004C, 0x3C004D,
    0x3C004E, 0x3C004F, 0x3C0050, 0x3C0051, 0x3C0052, 0x3C0053,
    0x3C0054, 0x3C0055, 0x3C0056, 0x3C0057, 0x3C0058, 0x3C0059,
    0x3C005A, 0x3C005B, 0x3C005C, 0x3C005D, 0x3C005E, 0x3C005F,
    0x3C0060, 0x3C0061, 0x3C0062, 0x3C0063, 0x3C0064, 0x3C0065,
    0x3C0066, 0x3C0067, 0x3C0068, 0x3C0069, 0x3C006A, 0x3C006B,
    0x3C006C, 0x3C006D, 0x3C006E, 0x3C006F, 0x3C0070, 0x3C0071,
    0x3C0072, 0x3C0073, 0x3C0074, 0x3C0075, 0x3C0075, 0x3C0076,
    0x3C0077, 0x3C0078, 0x3C0078, 0x3C0078, 0x3C0078, 0x3C0078,
    0x3C0078, 0x3C0078, 0x3C0078, 0x3C0079, 0x3C007A, 0x3C007B,
    0x3C007C, 0x3C007D, 0x3C007E, 0x3C007F, 0x3C0080, 0x3C0081,
    0x3C0082, 0x3C0083, 0x3C0084,
};

static char *lbl_1_data_8B8[0xA2] = {
    "model-sky",
    "model-tenzyou",
    "model-dokan",
    "model-kabe03",
    "model-hasira",
    "model-kabe00",
    "model-kabe01",
    "model-kabe02",
    "model-kabe04",
    "model-yuka00",
    "model-yuka01",
    "model-yuka02",
    "layer1-b100",
    "layer1-b101",
    "layer1-b110",
    "layer1-b111",
    "layer1-b120",
    "layer1-b121",
    "layer1-b130",
    "layer1-b131",
    "layer1-b132",
    "layer1-b133",
    "layer1-b134",
    "layer1-b135",
    "layer1-b140",
    "layer1-b141",
    "layer1-b142",
    "layer1-b143",
    "layer1-b144",
    "layer1-b145",
    "layer2-b200",
    "layer2-b201",
    "layer2-b210",
    "layer2-b211",
    "layer2-b220",
    "layer2-b221",
    "layer2-b230",
    "layer2-b231",
    "layer2-b232",
    "layer2-b233",
    "layer2-b234",
    "layer2-b235",
    "layer2-b240",
    "layer2-b241",
    "layer2-b242",
    "layer2-b243",
    "layer2-b244",
    "layer2-b245",
    "layer3-b300",
    "layer3-b301",
    "layer3-b310",
    "layer3-b311",
    "layer3-b320",
    "layer3-b321",
    "layer3-b330",
    "layer3-b331",
    "layer3-b332",
    "layer3-b333",
    "layer3-b334",
    "layer3-b335",
    "layer3-b340",
    "layer3-b341",
    "layer3-b342",
    "layer3-b343",
    "layer3-b344",
    "layer3-b345",
    "m502te00-00",
    "m502te00-01",
    "m502te00-02",
    "m502te00-03",
    "m502te00-04",
    "m502te00-05",
    "m502te00-06",
    "m502te00-07",
    "m502te10-00",
    "m502te10-01",
    "m502te10-02",
    "m502te10-03",
    "m502te10-04",
    "m502te10-05",
    "m502te10-06",
    "m502te10-07",
    "m502te20-00",
    "m502te20-01",
    "m502te20-02",
    "m502te20-03",
    "m502te20-04",
    "m502te20-05",
    "m502te20-06",
    "m502te20-07",
    "m502ie00-00",
    "m502ie00-01",
    "m502ie00-02",
    "m502ie00-03",
    "m502ie00-04",
    "m502ie00-05",
    "m502ie00-06",
    "m502ie00-07",
    "m502ie10-00",
    "m502ie10-01",
    "m502ie10-02",
    "m502ie10-03",
    "m502ie10-04",
    "m502ie10-05",
    "m502ie10-06",
    "m502ie10-07",
    "m502ie20-00",
    "m502ie20-01",
    "m502ie20-02",
    "m502ie20-03",
    "m502ie20-04",
    "m502ie20-05",
    "m502ie20-06",
    "m502ie20-07",
    "m502we00_2-cyl3_1",
    "m502we15-cyl2_1_2_2_2",
    "m502we00_2-cyl3_1",
    "m502we02_3-cube1",
    "m502we02_3-cube1",
    "m502dbg-obj5",
    "m502bg-obj6",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "m502ke00-poly1_1",
    "layer1-b451",
    "layer1-b452",
    "layer1-b453",
    "layer1-b454",
    "layer1-b455",
    "m502we06-m502we06",
    "m502we07-m502we07",
    "m502we08-m502we08",
    "m502we12-m502we06",
    "m502we12-m502we06",
    "m502we12-m502we06",
    "m502we12-m502we06",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

static Vec lbl_1_data_B40 = { 0.0f, 4000.0f, 500.0f };
static Vec lbl_1_data_B4C = { 0.0f, -1.0f, -1.0f };
static GXColor lbl_1_data_B58 = { 0xFF, 0xFF, 0xFF, 0xFF };

M502MAIN lbl_1_bss_0;
s32 lbl_1_bss_5028;
s16 lbl_1_bss_502C;
s32 lbl_1_bss_5030;

s32 _prolog(void)
{
    const M502_VOIDFUNC *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M502_VOIDFUNC *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJMAN *objman;
    OMOBJ *obj;

    objman = omInitObjMan(0x200, 0x2000);
    omGameSysInit(objman);
    obj = omAddObjEx(objman, 0x64, 0, 0, -1, fn_1_134);
    obj->data = &lbl_1_bss_0;
    fn_1_398(&lbl_1_bss_0, objman, obj);
    Hu3DNoSyncSet(1);
}

void fn_1_134(OMOBJ *obj)
{
    M502MAIN *m = obj->data;

    if (fn_1_5DC(m) != 0) {
        fn_1_504(m);
        omDelObjEx(HuPrcCurrentGet(), obj);
        omOvlReturnEx(1, 1);
    }
}

int fn_1_198(int seId)
{
    return HuAudFXPlay(seId);
}

void fn_1_1C0(int seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_1E8(int seNo, s32 speed)
{
    HuAudFXFadeOut(seNo, speed);
}

void fn_1_218(int seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void fn_1_248(Vec *pos, Vec *heading, f32 sndDist, f32 sndSpeed, f32 startDis, f32 frontSurDis, f32 backSurDis)
{
    HuAudFXListnerSetEX(pos, heading, sndDist, sndSpeed, startDis, frontSurDis, backSurDis);
}

int fn_1_2A0(s16 streamId)
{
    return HuAudSStreamPlay(streamId);
}

void fn_1_2C8(int streamNo, s32 speed)
{
    HuAudSStreamFadeOut(streamNo, speed);
}

void fn_1_2F8(void)
{
    HuAudAllStop();
}

int fn_1_318(s16 streamId)
{
    return HuAudBGMPlay(streamId);
}

int fn_1_340(s16 streamId)
{
    return HuAudJinglePlay(streamId);
}

int fn_1_368(s16 streamId, s16 chanNo)
{
    return HuAudSStreamChanPlay(streamId, chanNo);
}

void fn_1_398(M502MAIN *m, OMOBJMAN *objman, OMOBJ *obj)
{
    u16 i;

    memset(m, 0, offsetof(M502MAIN, unk5024));
    m->objman = objman;
    m->obj = obj;
    m->unk5008 = 0;
    m->versus = _CheckFlag(0x30002);
    m->mesId = -1;
    m->bgmId = -1;
    m->seId = -1;
    m->unk5020 = -1;
    fn_1_1350(m);
    fn_1_15C0(&m->light, 0);
    for (i = 0; i < 4; i++) {
        fn_1_1690(&m->player[i], i, m);
    }
    Hu3DLayerHookSet(12, (HU3DLAYERHOOK)fn_1_27AC);
    fn_1_735C(&m->player[0].block);
    fn_1_7290(&m->player[1].block, &m->player[0].block);
    fn_1_7290(&m->player[2].block, &m->player[0].block);
    fn_1_7290(&m->player[3].block, &m->player[0].block);
    fn_1_9468(&m->opening, m);
    Hu3DBGColorSet(0, 0, 0);
    WipeCreate(1, 5, 60);
    m->gameTime = 0x4650;
    m->mgTimer = NULL;
    lbl_1_bss_5028 = 0;
    lbl_1_bss_502C = 0;
}

void fn_1_504(M502MAIN *m)
{
    u16 i;

    fn_1_95B4(&m->opening);
    for (i = 0; i < 4; i++) {
        fn_1_1740(&m->player[i]);
    }
    fn_1_1638(&m->light);
    fn_1_13A0(m);
    if (m->mesId >= 0) {
        GameMesKill(m->mesId);
    }
    GameMesClose();
    if (lbl_1_data_0->versus == 0 && m->mgTimer != NULL) {
        MgTimerKill(m->mgTimer);
        m->mgTimer = NULL;
    }
    memset(m, 0, offsetof(M502MAIN, unk5024));
    fn_1_2F8();
}

/* the whole game: returns 1 when the module should return to the overlay manager */
s32 fn_1_5DC(M502MAIN *m)
{
    s32 found;
    s32 allDone;
    s32 anyDead;
    u16 j;
    u16 k;
    M502PLAYER *p;
    s32 val;

    if (omSysExitReq != 0 && m->state < 7 && lbl_1_bss_5028 == 0) {
        WipeCreate(2, 6, 60);
        HuAudFadeOut(1000);
        if (m->state == 1) {
            m->state = 8;
            lbl_1_bss_5030 = 1;
        } else {
            lbl_1_bss_5028 = 1;
            lbl_1_bss_502C = 0;
        }
    }
    if (lbl_1_bss_5028 != 0) {
        lbl_1_bss_502C++;
        if (lbl_1_bss_502C >= 60 && WipeCheckEnd() == 0) {
            m->state = 7;
        }
    }
    switch (m->state) {
    case 0:
        m->state = 1;
    case 1:
        if (fn_1_9644(m) != 0) {
            m->state = 2;
        }
        break;
    case 2:
        if (omUPauseFlag + omPauseChk() == 0) {
            if (lbl_1_data_0->versus == 0) {
                if (m->mgTimer != NULL) {
                    if (MgTimerDoneCheck(m->mgTimer) != 0) {
                        fn_1_FD8(m);
                        m->state = 9;
                    }
                } else if (m->gameTime <= 0x708) {
                    m->mgTimer = MgTimerCreate(0);
                    MgTimerParamSet(m->mgTimer, 0x708, 0, 0);
                    MgTimerModeOnSet(m->mgTimer, 1);
                }
            } else if (m->mgTimer == NULL) {
                for (j = 0; j < 4; j++) {
                    p = &m->player[j];
                    MgTimerParamSet(p->timer, 0, 0x1518, 0);
                    MgTimerModeOnSet(p->timer, 2);
                }
                m->mgTimer = (MGTIMER *)-1;
            }
            if (m->gameTime != 0) {
                m->gameTime--;
            }
        }
        break;
    case 3:
        if (m->mesId < 0) {
            m->mesId = GameMesCreate(2, 1);
        } else {
            if (m->bgmId >= 0 && (GameMesStatGet(m->mesId) & 0x10) != 0) {
                fn_1_2C8(m->bgmId, 100);
                m->bgmId = -1;
            }
            if (GameMesStatGet(m->mesId) == 0) {
                m->mesId = -1;
                m->timer = 0;
                m->state = 4;
            }
        }
        break;
    case 4:
        val = m->timer++;
        if (val > 120) {
            WipeCreate(2, 0, 60);
            WipeColorSet(0xFF, 0xFF, 0xFF);
            m->timer = 0;
            m->state = 5;
        }
        break;
    case 5:
        if (WipeCheckEnd() == 0) {
            WipeCreate(1, 0, 60);
            for (j = 0; j < 4; j++) {
                fn_1_8F04(&m->player[j].block);
            }
            for (j = 0; j < 4; j++) {
                for (k = 0; k < 4; k++) {
                    Hu3DModelAttrSet(m->player[j].stage.model[k], 1);
                }
                fn_1_CAC8(&m->player[j].bat);
            }
            m->state = 6;
        }
        break;
    case 6:
        if (fn_1_A914(m) != 0) {
            s16 first = m->firstPlayer;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[first].mgCoinBonus = 10;
            }
            m->state = 7;
        }
        break;
    case 9:
        if (m->mesId < 0) {
            m->mesId = GameMesCreate(2, 1);
        } else {
            if (m->bgmId >= 0 && (GameMesStatGet(m->mesId) & 0x10) != 0) {
                fn_1_2C8(m->bgmId, 100);
                m->bgmId = -1;
            }
            if (GameMesStatGet(m->mesId) == 0) {
                m->mesId = -1;
                found = 0;
                j = 0;
                while (j < 4) {
                    if (m->player[j].plr.phase == 2) {
                        found = 1;
                        break;
                    }
                    j++;
                }
                if (found != 0) {
                    fn_1_1018(m);
                    m->timer = 0;
                    m->state = 4;
                } else {
                    for (j = 0; j < 4; j++) {
                        m->player[j].plr.phase = 3;
                    }
                    m->state = 10;
                }
            }
        }
        break;
    case 10:
        if (m->mesId < 0) {
            m->mesId = GameMesCreate(2, 2);
            m->timer = 0;
        } else {
            if (m->bgmId < 0) {
                m->bgmId = fn_1_340(0x7B);
            }
            if (m->timer == 0) {
                CharLoseVoicePlay(m->player[0].plr.charNo, m->player[1].plr.charNo, m->player[2].plr.charNo, m->player[3].plr.charNo);
            }
            val = m->timer + 1;
            m->timer = val;
            if (val >= 210) {
                m->mesId = -1;
                WipeCreate(2, 6, 60);
                m->state = 7;
            }
        }
        break;
    case 11:
        if (m->mesId < 0) {
            m->mesId = GameMesCreate(2, 1);
        } else {
            if (m->bgmId >= 0 && (GameMesStatGet(m->mesId) & 0x10) != 0) {
                fn_1_2C8(m->bgmId, 100);
                m->bgmId = -1;
            }
            if (GameMesStatGet(m->mesId) == 0) {
                m->mesId = -1;
                m->timer = 0;
                m->state = 12;
            }
        }
        break;
    case 12:
        WipeCreate(2, 6, 60);
        m->timer = 0;
        m->state = 7;
        for (j = 0; j < 4; j++) {
            p = &m->player[j];
            val = MgTimerValueGet(p->timer);
            if (p->plr.phase == 2) {
                GwPlayer[p->plr.playerNo].mgScore = val;
            } else {
                GwPlayer[p->plr.playerNo].mgScore = 0;
            }
        }
        break;
    case 7:
        if (WipeCheckEnd() == 0) {
            for (j = 0; j < 4; j++) {
                fn_1_8F04(&m->player[j].block);
            }
            return 1;
        }
        break;
    case 8:
        if (lbl_1_bss_5030 != 0 && fn_1_9644(m) != 0) {
            lbl_1_bss_5030 = 0;
        }
        if (WipeCheckEnd() == 0) {
            return 1;
        }
        break;
    }
    anyDead = 0;
    for (j = 0; j < 4; j++) {
        p = &m->player[j];
        if (fn_1_17C4(p) != 0) {
            anyDead = 1;
            if (lbl_1_data_0->versus != 0 && MgTimerModeGet(p->timer) != 2 && p->timer->stopF == 0) {
                p->timer->stopF = 1;
                p->timer->mode = 1;
            }
        }
    }
    if (lbl_1_data_0->versus == 0) {
        if (anyDead != 0 && m->state == 2) {
            m->state = 3;
            m->timer = 0;
            if (m->mgTimer != NULL) {
                MgTimerModeOffSet(m->mgTimer);
                MgTimerDispOff(m->mgTimer);
            }
            fn_1_FD8(m);
            fn_1_1018(m);
        }
    } else {
        allDone = 1;
        for (j = 0; j < 4; j++) {
            if (m->mgTimer != NULL) {
                if (MgTimerModeGet(m->player[j].timer) != 0) {
                    allDone = 0;
                }
            } else {
                allDone = 0;
            }
        }
        if (allDone != 0 && m->state == 2) {
            fn_1_FD8(m);
            m->state = 11;
            m->timer = 0;
        }
    }
    if (lbl_1_data_0->versus == 0 && m->unk5008 != 0 && m->state <= 5) {
        if (m->seId < 0) {
            m->seId = fn_1_198(0x631);
        }
        fn_1_1104(m);
    }
    fn_1_9604(&m->opening);
    return 0;
}

void fn_1_FD8(M502MAIN *m)
{
    u16 i;

    for (i = 0; i < 4; i++) {
        m->player[i].plr.active = 0;
    }
}

void fn_1_1018(M502MAIN *m)
{
    u16 order[4] = { 0, 0, 0, 0 };
    s16 cnt = 0;
    u16 i;

    for (i = 0; i < 4; i++) {
        if (m->player[i].plr.phase == 2) {
            order[cnt++] = i;
        }
    }
    if (cnt > 1) {
        m->firstPlayer = order[rand8() % cnt];
        return;
    }
    m->firstPlayer = order[0];
}

/* pad rumble and camera shake once the countdown runs low */
void fn_1_1104(M502MAIN *m)
{
    f32 shake;
    u16 tier;
    u16 i;
    M502PLAYER *p;

    if (m->vibTimer < 30) {
        tier = 0;
    } else if (m->vibTimer < 150) {
        tier = 1;
    } else {
        tier = 2;
    }
    if (tier == 0) {
        shake = 0.0f;
    } else if (tier == 1) {
        shake = 4.0f;
    } else {
        shake = 8.0f;
    }
    for (i = 0; i < 4; i++) {
        p = &m->player[i];
        if (p->plr.phase == 2) {
            if (tier == 1 && (m->vibTimer % 6) == 0) {
                omVibrate(i, 2, 6, 6);
            }
            if (tier == 2 && (m->vibTimer % 6) == 0) {
                omVibrate(i, 4, 4, 2);
            }
            if ((((m->vibTimer + ((u32)m->vibTimer >> 31)) >> 1) & 1) != 0) {
                p->plr.player->camera.tgtGoal.y += shake;
                p->plr.player->camera.posGoal.y += shake;
                p->plr.player->camera.tgt.y += shake;
                p->plr.player->camera.pos.y += shake;
            } else {
                p->plr.player->camera.tgtGoal.y -= shake;
                p->plr.player->camera.posGoal.y -= shake;
                p->plr.player->camera.tgt.y -= shake;
                p->plr.player->camera.pos.y -= shake;
            }
        }
    }
    m->vibTimer++;
}

void fn_1_1350(M502MAIN *m)
{
    M502MODELS *models;

    models = &m->models;
    memset(models, 0, sizeof(M502MODELS));
    fn_1_13F0(models);
    fn_1_14C0(models);
}

void fn_1_13A0(M502MAIN *m)
{
    M502MODELS *models;

    models = &m->models;
    fn_1_147C(models);
    fn_1_1564(models);
    memset(models, 0, sizeof(M502MODELS));
}

void fn_1_13F0(M502MODELS *models)
{
    models->texBuf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    models->anim1 = HuSprAnimDataRead(0x21000A);
    models->anim2 = HuSprAnimDataRead(0x21000C);
}

void fn_1_147C(M502MODELS *models)
{
    if (models->texBuf != NULL) {
        HuMemDirectFree(models->texBuf);
        models->texBuf = NULL;
    }
}

void fn_1_14C0(M502MODELS *models)
{
    s32 i;
    s16 id;

    for (i = 0; i < 0x8D; i++) {
        id = Hu3DModelCreateData(lbl_1_data_3C[i]);
        models->model[i] = id;
        models->name[i] = lbl_1_data_8B8[i];
        Hu3DModelAttrSet(models->model[i], 1);
    }
}

void fn_1_1564(M502MODELS *models)
{
    s32 i;

    for (i = 0; i < 0x8D; i++) {
        Hu3DModelKill(models->model[i]);
        models->model[i] = 0;
    }
}

void fn_1_15C0(M502LIGHT *light, s32 unk)
{
    memset(light, 0, sizeof(M502LIGHT));
    light->unk0 = unk;
    light->id = Hu3DGLightCreateV(&lbl_1_data_B40, &lbl_1_data_B4C, &lbl_1_data_B58);
    Hu3DGLightStaticSet(light->id, 1);
    Hu3DGLightInfinitytSet(light->id);
}

void fn_1_1638(M502LIGHT *light)
{
    if (light->id != 0) {
        Hu3DGLightKill(light->id);
        light->id = 0;
    }
    memset(light, 0, sizeof(M502LIGHT));
}

void fn_1_168C(void)
{
}

void fn_1_1690(M502PLAYER *p, s32 no, M502MAIN *m)
{
    memset(p, 0, sizeof(M502PLAYER));
    p->main = m;
    p->no = no;
    fn_1_38E0(&p->plr, no, p);
    fn_1_21B8(&p->stage, no, p);
    fn_1_6664(&p->block, no, p);
    fn_1_1830(&p->camera, no, p);
    fn_1_C468(&p->bat, p);
    p->timer = NULL;
}

void fn_1_1740(M502PLAYER *p)
{
    if (p->main->versus != 0 && p->timer != NULL) {
        MgTimerKill(p->timer);
    }
    fn_1_C4F0(&p->bat);
    fn_1_1A60(&p->camera);
    fn_1_2704(&p->stage);
    fn_1_3B88(&p->plr);
    fn_1_6778(&p->block);
    memset(p, 0, sizeof(M502PLAYER));
}

s32 fn_1_17C4(M502PLAYER *p)
{
    s32 dead;

    fn_1_6808(&p->block);
    dead = 0;
    if (fn_1_3BF4(&p->plr) != 0) {
        dead = 1;
    }
    fn_1_C554(&p->bat);
    fn_1_27A8(&p->stage);
    fn_1_1A90(&p->camera);
    return dead;
}
