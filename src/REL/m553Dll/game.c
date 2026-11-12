#include "REL/m553Dll.h"
#include "game/mg/actman.h"

typedef struct M553GModel_s {
    HU3DMODELID id;
    HuVecF pos;
    HuVecF rot;
} M553GMODEL;

typedef struct M553GPlayer_s {
    s16 idx;
    s16 charNo;
    u8 pad04[0x18];
} M553GPLAYER;

typedef struct M553GMotCtl_s {
    s16 motNo;
    u8 pad02[2];
    f32 speed;
    u8 pad08[4];
    f32 time;
    u8 pad10[8];
    s32 attr;
    u8 pad1C[4];
    u32 attrReset;
    u8 pad24[0x30];
} M553GMOTCTL;

typedef struct M553GWork_s {
    u8 pad00[4];
    M553GPLAYER player[2];
    u8 pad3C[0x1C];
    M553GMODEL *model;
    u8 pad5C[0x18];
    M553GMOTCTL motCtl[2];
    u8 pad11C[0x54];
    s32 *data;
    u8 pad174[4];
    s32 unk178;
    u8 pad17C[0x688];
} M553GWORK;

typedef struct M553GState_s {
    s16 mode;
    s16 sub;
    s16 unk04;
    s16 unk06;
    u8 pad08[0x18];
    s16 winner;
    u8 pad22[0x18];
    s16 unk3A;
    u8 pad3C[0xA0];
} M553GSTATE;

typedef struct M553GPlayerState_s {
    s16 unk00;
    s16 unk02;
    u8 pad04[0x12];
    s16 unk16;
    u8 pad18[4];
    HuVecF pos;
    u8 pad28[0x118];
} M553GPLAYERSTATE;

typedef struct M553GCam_s {
    s32 no;
    HuVecF pos;
} M553GCAM;

extern MGPLAYER *lbl_1_bss_0[3];
extern M553GCAM *lbl_1_bss_14;
extern M553GPLAYERSTATE lbl_1_bss_48C[2];
extern M553GSTATE lbl_1_bss_70C;

M553GWORK *lbl_1_bss_1EF8;

extern void fn_1_94C(s16 no);
extern void fn_1_10F4(void);
extern void fn_1_14F4(void);
extern void fn_1_3AE4(void *arg);
extern void fn_1_84C8(void);
extern s16 fn_1_984C(void);
extern s16 fn_1_9A70(void);
extern void fn_1_B668(void);
extern void fn_1_3AC4(void);
void fn_1_6F30(void);

s32 fn_1_6064(void *arg0)
{
    HU3DMODEL *model1;
    HU3DMODEL *model2;

    fn_1_3AE4(arg0);
    MgPlayerPadSet(lbl_1_bss_0[1], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0[2], 0, 0, 0, 0);
    MgActorExec();
    model1 = &Hu3DData[lbl_1_bss_1EF8->model[0x86].id];
    lbl_1_bss_48C[0].pos.x = model1->pos.x;
    lbl_1_bss_48C[0].pos.y = model1->pos.y;
    lbl_1_bss_48C[0].pos.z = model1->pos.z;
    model2 = &Hu3DData[lbl_1_bss_1EF8->model[0x87].id];
    lbl_1_bss_48C[1].pos.x = model2->pos.x;
    lbl_1_bss_48C[1].pos.y = model2->pos.y;
    lbl_1_bss_48C[1].pos.z = model2->pos.z;
    return 1;
}

s16 fn_1_6170(void)
{
    s16 ret = 0;

    ret = fn_1_984C();
    fn_1_6F30();
    return ret;
}

s32 fn_1_61A8(void)
{
    s32 ret = 0;

    lbl_1_bss_70C.mode = 0;
    lbl_1_bss_70C.sub = 0;
    lbl_1_bss_70C.unk04 = 0;
    lbl_1_bss_70C.unk06 = 0;
    fn_1_6F30();
    return ret;
}

s32 fn_1_6850(void)
{
    s32 ret = 0;

    fn_1_84C8();
    lbl_1_bss_70C.mode = 0;
    lbl_1_bss_70C.sub = 0;
    lbl_1_bss_70C.unk04 = 0;
    lbl_1_bss_70C.unk06 = 0;
    lbl_1_bss_48C[0].unk02 = 0;
    lbl_1_bss_48C[1].unk02 = 0;
    MgPlayerPadSet(lbl_1_bss_0[1], 0, 0, 0, 0);
    MgPlayerPadSet(lbl_1_bss_0[2], 0, 0, 0, 0);
    MgActorExec();
    fn_1_6F30();
    return ret;
}

s16 fn_1_6928(void)
{
    s16 ret = 0;

    ret = fn_1_9A70();
    fn_1_B668();
    fn_1_6F30();
    return ret;
}

s32 fn_1_6964(void)
{
    M553GMOTCTL *ctl;

    if (lbl_1_bss_1EF8->unk178 == 0) {
        if (lbl_1_bss_70C.winner != 0) {
            ctl = &lbl_1_bss_1EF8->motCtl[lbl_1_bss_70C.winner & 1];
            ctl->motNo = 3;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
            ctl = &lbl_1_bss_1EF8->motCtl[(lbl_1_bss_70C.winner & 2) >> 1];
            ctl->motNo = 4;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
        } else if (lbl_1_bss_48C[0].unk00 != 0 || lbl_1_bss_48C[1].unk00 != 0) {
            ctl = &lbl_1_bss_1EF8->motCtl[0];
            ctl->motNo = 4;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
            ctl = &lbl_1_bss_1EF8->motCtl[1];
            ctl->motNo = 4;
            ctl->speed = 1.0f;
            ctl->time = 0.0f;
            ctl->attr = 0;
            ctl->attrReset = 0x40000001;
            CharLoseVoicePlay(lbl_1_bss_1EF8->player[0].charNo, lbl_1_bss_1EF8->player[1].charNo, -1, -1);
        }
        lbl_1_bss_1EF8->unk178 = 1;
    }
    fn_1_6F30();
    if (lbl_1_bss_70C.winner != 0) {
        CharModelVoicePanSet(lbl_1_bss_1EF8->player[lbl_1_bss_70C.winner & 1].charNo, 0x40);
    }
    return 0;
}

s32 fn_1_6BDC(void)
{
    s32 ret = 1;

    fn_1_6F30();
    return ret;
}

s32 fn_1_6C0C(void)
{
    s32 ret = 1;

    fn_1_3AC4();
    MgActorKill(lbl_1_bss_0[1]->actor);
    MgActorKill(lbl_1_bss_0[2]->actor);
    return ret;
}

void fn_1_6C64(void)
{
    HU3DMODEL *model1;
    HU3DMODEL *model2;
    s16 pan;
    s16 i;

    model1 = &Hu3DData[lbl_1_bss_1EF8->model[0x86].id];
    model2 = &Hu3DData[lbl_1_bss_1EF8->model[0x87].id];
    for (i = 0; i < 0x86; i++) {
        Hu3DModelPosSet(lbl_1_bss_1EF8->model[i].id, lbl_1_bss_1EF8->model[i].pos.x, lbl_1_bss_1EF8->model[i].pos.y, lbl_1_bss_1EF8->model[i].pos.z);
        Hu3DModelRotSet(lbl_1_bss_1EF8->model[i].id, lbl_1_bss_1EF8->model[i].rot.x, lbl_1_bss_1EF8->model[i].rot.y, lbl_1_bss_1EF8->model[i].rot.z);
    }
    pan = 64.0f + (0.042666666f * model1->pos.x);
    CharModelVoiceVolSet(lbl_1_bss_1EF8->player[0].charNo, 111.0f + (0.020666666f * model1->pos.z));
    CharModelVoicePanSet(lbl_1_bss_1EF8->player[0].charNo, pan);
    pan = 64.0f + (0.042666666f * model2->pos.x);
    CharModelVoiceVolSet(lbl_1_bss_1EF8->player[1].charNo, 111.0f + (0.020666666f * model2->pos.z));
    CharModelVoicePanSet(lbl_1_bss_1EF8->player[1].charNo, pan);
}

void fn_1_6F30(void)
{
    HU3DMODEL *model1;
    HU3DMODEL *model2;
    s16 pan;
    s16 i;

    Hu3DModelAttrSet(lbl_1_bss_1EF8->model[0x75].id, 0x400000);
    Hu3DModelLayerSet(lbl_1_bss_1EF8->model[0x75].id, 7);
    lbl_1_bss_1EF8->model[0x75].pos.x = -1100.0f + lbl_1_bss_14->pos.x;
    lbl_1_bss_1EF8->model[0x75].pos.y = 1150.0f + (lbl_1_bss_14->pos.y - 953.0f);
    lbl_1_bss_1EF8->model[0x75].pos.z = -800.0f + (lbl_1_bss_14->pos.z - 1556.0f);
    model1 = &Hu3DData[lbl_1_bss_1EF8->model[0x86].id];
    model2 = &Hu3DData[lbl_1_bss_1EF8->model[0x87].id];
    for (i = 0; i < 0x86; i++) {
        Hu3DModelPosSet(lbl_1_bss_1EF8->model[i].id, lbl_1_bss_1EF8->model[i].pos.x, lbl_1_bss_1EF8->model[i].pos.y, lbl_1_bss_1EF8->model[i].pos.z);
        Hu3DModelRotSet(lbl_1_bss_1EF8->model[i].id, lbl_1_bss_1EF8->model[i].rot.x, lbl_1_bss_1EF8->model[i].rot.y, lbl_1_bss_1EF8->model[i].rot.z);
    }
    pan = 64.0f + (0.042666666f * model1->pos.x);
    CharModelVoiceVolSet(lbl_1_bss_1EF8->player[0].charNo, 111.0f + (0.020666666f * model1->pos.z));
    CharModelVoicePanSet(lbl_1_bss_1EF8->player[0].charNo, pan);
    pan = 64.0f + (0.042666666f * model2->pos.x);
    CharModelVoiceVolSet(lbl_1_bss_1EF8->player[1].charNo, 111.0f + (0.020666666f * model2->pos.z));
    CharModelVoicePanSet(lbl_1_bss_1EF8->player[1].charNo, pan);
    fn_1_94C(0);
    fn_1_10F4();
    fn_1_14F4();
}

s32 fn_1_7300(s16 no)
{
    M553GMOTCTL *ctl = &lbl_1_bss_1EF8->motCtl[no];
    s16 i = 0;
    s32 ret = 0;

    if (lbl_1_bss_48C[no].unk00 != 0) {
    }
    return ret;
}

typedef struct M553GEvent_s {
    s32 unk00;
    s32 index;
    s32 type;
} M553GEVENT;

s32 fn_1_B5F4(void *work, M553GEVENT *ev)
{
    s16 i = 0;

    switch (ev->type) {
        case 0:
            lbl_1_bss_48C[ev->index].unk16++;
            lbl_1_bss_70C.unk3A = 1;
            break;
        case 6:
            break;
    }
    return 0;
}

typedef struct M553GVec4_s {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} M553GVEC4;

M553GVEC4 *fn_1_B654(M553GVEC4 *p)
{
    s32 tmp;

    tmp = p->unk0C;
    return p;
}
