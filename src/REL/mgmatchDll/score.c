#include "dolphin.h"

#include "game/animdata.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/sprite.h"

#include "math.h"
#include "string.h"

extern void OSReport(const char *msg, ...);
extern int HuAudFXPlay(int seId);

extern s8 fn_1_588(void);
extern s8 fn_1_59C(void);
extern void fn_1_5D0(s32 no, s32 arg);
extern s8 fn_1_624(void);
extern void fn_1_87C(u32 arg0);
extern void fn_1_898(u32 arg0);
extern u32 fn_1_8B4(u32 arg0);

void fn_1_12934(void);
void fn_1_12B60(void);
void fn_1_14350(void);
void fn_1_15460(s16 layer);

/* Character slot of the match module: only the fields this unit reads. */
typedef struct ScorePlayer_s {
    u8 pad0[0x18];
    s16 playerNo;  //0x18
    s16 charNo;    //0x1A
    u8 pad1C[0x34 - 0x1C];
} SCOREPLAYER;

/* lbl_1_bss_EC0: score board state. */
typedef struct ScoreBoard_s {
    s16 grp;           //0x00
    f32 x;             //0x04
    f32 y;             //0x08
    s32 cnt[4];        //0x0C
    struct {
        HU3DMODELID id; //0x00
        Vec pos;        //0x04
    } mdl[2];          //0x1C
    s8 cols;           //0x3C
    s8 rows;           //0x3D
    s8 state;          //0x3E
    s8 sub;            //0x3F
    s8 type;           //0x40
    u8 result;         //0x41
    HU3DMODELID *mdlList; //0x44
} SCOREBOARD;

/* lbl_1_bss_E74: result sprite/model sequence state. */
typedef struct ScoreSeq_s {
    s16 grp[3];        //0x00
    s8 init;           //0x06
    s32 jointType[4];  //0x08
    s16 mdl[4];        //0x18
    s16 order[4];      //0x20
    s16 state;         //0x28
    s16 timer;         //0x2A
    s16 gameType;      //0x2C
    s16 team;          //0x2E
    s16 count;         //0x30
    f32 ampA;          //0x34
    f32 ampB;          //0x38
    ANIMDATA *anim[4]; //0x3C
} SCORESEQ;

typedef struct ScorePos2_s {
    f32 x;
    f32 y;
    f32 pad;
} SCOREPOS2;

SCOREPOS2 lbl_1_bss_E38[4];
s16 lbl_1_bss_E68[6];
SCORESEQ lbl_1_bss_E74;
SCOREBOARD lbl_1_bss_EC0;

f32 lbl_1_data_6C8[3][4][2] = {
    -138.0f, -39.0f, -138.0f, 3.0f, -138.0f, 45.0f, -138.0f, 87.0f,
    -96.0f, -39.0f, -96.0f, 3.0f, -96.0f, 45.0f, -96.0f, 87.0f,
    -54.0f, -39.0f, -54.0f, 3.0f, -54.0f, 45.0f, -54.0f, 87.0f,
};

f32 lbl_1_data_728[3][8][2] = {
    -96.0f, -39.0f, -96.0f, 3.0f, -96.0f, 45.0f, -96.0f, 87.0f, -96.0f, -18.0f, -96.0f, 66.0f, -96.0f, -39.0f, -96.0f, 45.0f,
    -54.0f, -39.0f, -54.0f, 3.0f, -54.0f, 45.0f, -54.0f, 87.0f, -54.0f, -18.0f, -54.0f, 66.0f, -54.0f, -39.0f, -54.0f, 45.0f,
    -12.0f, -39.0f, -12.0f, 3.0f, -12.0f, 45.0f, -12.0f, 87.0f, -12.0f, -18.0f, -12.0f, 66.0f, -12.0f, -39.0f, -12.0f, 45.0f,
};

u32 lbl_1_data_7E8[10] = {
    0x980040, 0x980041, 0x980042, 0x980043, 0x980044,
    0x980045, 0x980046, 0x980047, 0x980048, 0x980049,
};

u32 lbl_1_data_810[3][3] = {
    0x980020, 0x980021, 0x980022,
    0x980023, 0x980024, 0x980025,
    0x980026, 0x980027, 0x980028,
};

f32 lbl_1_data_834[4] = {
    288.0f, 600.0f, 288.0f, 150.0f,
};

f32 lbl_1_data_844[3][2][6] = {
    -600.0f, -630.0f, -500.0f, -665.0f, 800.0f, -500.0f,
    600.0f, -630.0f, -500.0f, 665.0f, 800.0f, -500.0f,
    -478.0f, -630.0f, -500.0f, -528.799988f, 800.0f, -500.0f,
    478.0f, -630.0f, -500.0f, 528.799988f, 800.0f, -500.0f,
    -356.100006f, -630.0f, -500.0f, -400.0f, 800.0f, -500.0f,
    356.100006f, -630.0f, -500.0f, 400.0f, 800.0f, -500.0f,
};

u32 lbl_1_data_8D4[3][2][3] = {
    0x980029, 0x98002A, 0x98002B,
    0x0, 0x0, 0x0,
    0x98002C, 0x98002E, 0x980030,
    0x98002D, 0x98002F, 0x980031,
    0x980032, 0x980034, 0x980036,
    0x980033, 0x980035, 0x980037,
};

f32 lbl_1_data_91C[108] = {
    -96.0f, -81.0f, -96.0f, -27.0f, -96.0f, 27.0f, -96.0f, 81.0f, -96.0f, -54.0f,
    -96.0f, 54.0f, -96.0f, -81.0f, -96.0f, 27.0f, -96.0f, -27.0f, -96.0f, 81.0f,
    -42.0f, -81.0f, -42.0f, -27.0f, -42.0f, 27.0f, -42.0f, 81.0f, -42.0f, -54.0f,
    -42.0f, 54.0f, -42.0f, -81.0f, -42.0f, 27.0f, -42.0f, -27.0f, -42.0f, 81.0f,
    12.0f, -81.0f, 12.0f, -27.0f, 12.0f, 27.0f, 12.0f, 81.0f, 12.0f, -54.0f,
    12.0f, 54.0f, 12.0f, -81.0f, 12.0f, 27.0f, 12.0f, -27.0f, 12.0f, 81.0f,
    -150.0f, -81.0f, -150.0f, -27.0f, -150.0f, 27.0f, -150.0f, 81.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -96.0f, -81.0f, -96.0f, -27.0f,
    -96.0f, 27.0f, -96.0f, 81.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -42.0f, -81.0f, -42.0f, -27.0f, -42.0f, 27.0f, -42.0f, 81.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

f32 lbl_1_data_ACC[60] = {
    -217.0f, -81.0f, -217.0f, -27.0f, -217.0f, 27.0f, -217.0f, 81.0f, -217.0f, -54.0f,
    -217.0f, 54.0f, -217.0f, -81.0f, -217.0f, 27.0f, -217.0f, -27.0f, -217.0f, 81.0f,
    -163.0f, -81.0f, -163.0f, -27.0f, -163.0f, 27.0f, -163.0f, 81.0f, -163.0f, -54.0f,
    -163.0f, 54.0f, -163.0f, -81.0f, -163.0f, 27.0f, -163.0f, -27.0f, -163.0f, 81.0f,
    -109.0f, -81.0f, -109.0f, -27.0f, -109.0f, 27.0f, -109.0f, 81.0f, -109.0f, -54.0f,
    -109.0f, 54.0f, -109.0f, -81.0f, -109.0f, 27.0f, -109.0f, -27.0f, -109.0f, 81.0f,
};

f32 lbl_1_data_BBC[3][4][2] = {
    -150.0f, -81.0f, -150.0f, -27.0f, -150.0f, 27.0f, -150.0f, 81.0f,
    -96.0f, -81.0f, -96.0f, -27.0f, -96.0f, 27.0f, -96.0f, 81.0f,
    -42.0f, -81.0f, -42.0f, -27.0f, -42.0f, 27.0f, -42.0f, 81.0f,
};

f32 lbl_1_data_C1C[4] = {
    288.0f, 800.0f, 288.0f, 240.0f,
};

f32 lbl_1_data_C2C[4] = {
    288.0f, 800.0f, 288.0f, 240.0f,
};

f32 lbl_1_data_C3C[4] = {
    288.0f, 800.0f, 288.0f, 240.0f,
};

Vec lbl_1_data_C4C[5] = {
    0.0f, 0.0f, 0.0f,
    -300.0f, 0.0f, 0.0f,
    -100.0f, 0.0f, 0.0f,
    100.0f, 0.0f, 0.0f,
    300.0f, 0.0f, 0.0f,
};
char lbl_1_data_C88[10] = "root_head";
char lbl_1_data_C92[10] = "ske_head";
char *lbl_1_data_C9C[15] = {
    lbl_1_data_C88, lbl_1_data_C88, lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92,
    lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92,
    lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92, lbl_1_data_C92,
};
f32 lbl_1_data_CD8[13] = {
    15.0f, 18.0f, 12.0f, 20.0f, 15.0f, 6.0f, 0.0f, 20.0f, 90.0f, 15.0f, 15.0f, 15.0f, 15.0f,
};
f32 lbl_1_data_D0C[62] = {
    -300.0f, 400.0f, 0.0f, -295.0f, 290.0f, 0.0f,
    -290.0f, 185.0f, 0.0f, -285.0f, 85.0f, 0.0f,
    -190.0f, 400.0f, 0.0f, -187.0f, 290.0f, 0.0f,
    -185.0f, 185.0f, 0.0f, -183.0f, 84.0f, 0.0f,
    -87.0f, 400.0f, 0.0f, -86.0f, 290.0f, 0.0f,
    -85.0f, 185.0f, 0.0f, -84.0f, 84.0f, 0.0f,
    -35.0f, -35.0f, -35.0f, -45.0f, -50.0f, -35.0f,
    -40.0f, -35.0f, -60.0f, -80.0f, -80.0f, -80.0f,
    -80.0f, 550.0f, 600.0f, 500.0f, 600.0f, 700.0f,
    550.0f, 500.0f, 600.0f, 900.0f, 800.0f, 800.0f,
    800.0f, 800.0f,
};
u32 lbl_1_data_E04[2] = { 0x980005, 0x980006 };
f32 lbl_1_data_E0C[2] = { 0.0f, -5.0f };
char lbl_1_data_E14[14] = "resrts_window";
char lbl_1_data_E22[15] = "\x83\x58\x83\x52\x83\x41 %d \x95\x8E\xA6\n";
char lbl_1_data_E31[11] = "S3TC_touei";

void fn_1_12404(s8 gameType, s8 maxScore, SCOREPLAYER *player, s8 *score, HU3DMODELID *mdl)
{
    s32 typeIdx = 0;
    s32 rowOfs = 0;
    s8 rows = 0;
    s8 cols = 0;
    s8 type = 0;
    ANIMDATA *anim;
    s16 grp;
    s32 i;
    s32 j;
    s8 r;
    f32 x;
    f32 y;
    Vec mdlPos;
    HU3DMODELID model;
    f32 px;
    f32 py;

    switch (gameType) {
        case 0:
            typeIdx = 0;
            rows = 4;
            rowOfs = 0;
            break;
        case 2:
            typeIdx = 1;
            rows = 2;
            rowOfs = 4;
            break;
        case 1:
            typeIdx = 2;
            rows = 2;
            rowOfs = 6;
            break;
    }
    switch (maxScore) {
        case 7:
            type = 0;
            cols = 7;
            break;
        case 5:
            type = 1;
            cols = 5;
            break;
        case 3:
            type = 2;
            cols = 3;
            break;
    }
    lbl_1_bss_EC0.rows = rows;
    lbl_1_bss_EC0.cols = cols;
    lbl_1_bss_EC0.type = type;
    anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_810[typeIdx][type], HU_MEMNUM_OVL, HEAP_MODEL));
    grp = HuSprGrpCreate((s16)(cols * rows + 5));
    lbl_1_bss_EC0.grp = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(anim, 100, 0));
    i = 4;
    while (i-- != 0) {
        HuSprGrpMemberSet(grp, (s16)(i + 1),
            HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(
                lbl_1_data_7E8[GwPlayerConf[player[i].playerNo].charNo], HU_MEMNUM_OVL, HEAP_MODEL)),
                50, 0));
        HuSprPosSet(grp, (s16)(i + 1), lbl_1_data_6C8[type][i][0], lbl_1_data_6C8[type][i][1]);
        lbl_1_bss_EC0.cnt[i] = score[i];
    }
    r = rows;
    while (r-- != 0) {
        f32 *e = lbl_1_data_728[lbl_1_bss_EC0.type][rowOfs + r];
        x = e[0];
        y = e[1];
        for (j = 0; j < cols; j++) {
            s16 idx = (s16)(r * lbl_1_bss_EC0.cols + j + 5);
            HuSprGrpMemberSet(grp, idx,
                HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x98003B, HU_MEMNUM_OVL, HEAP_MODEL)), 10, 0));
            HuSprPosSet(grp, idx, x, y);
            HuSprTPLvlSet(grp, idx, 0.0f);
            x += 42.0f;
        }
    }
    HuSprGrpDrawNoSet(grp, 0x40);
    HuSprExecLayerSet(0x40, 1);
    for (i = 0; i < cols * rows + 5; i++) {
        HuSprAttrSet(grp, (s16)i, 1);
        HuSprScaleSet(grp, (s16)i, 1.0f, 1.0f);
    }
    i = 2;
    while (i-- != 0) {
        f32 *e = lbl_1_data_844[lbl_1_bss_EC0.type][i];
        mdlPos.x = e[0];
        mdlPos.y = e[1];
        mdlPos.z = e[2];
        model = Hu3DModelCreate(HuDataSelHeapReadNum(0x980002, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_EC0.mdl[i].id = model;
        lbl_1_bss_EC0.mdl[i].pos = mdlPos;
        Hu3DMotionSet(model, Hu3DJointMotion(model, HuDataSelHeapReadNum(lbl_1_data_E04[i], HU_MEMNUM_OVL, HEAP_MODEL)));
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DModelLayerSet(model, 2);
        Hu3DModelCameraSet(model, 0x10);
        Hu3DModelPosSet(model, mdlPos.x, mdlPos.y, mdlPos.z);
    }
    lbl_1_bss_EC0.mdlList = mdl;
    fn_1_12934();
    lbl_1_bss_E74.init = 0;
    HuPrcChildCreate(fn_1_12B60, 100, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_12934(void)
{
    s8 r;
    s32 n;
    s16 i;
    s32 k;

    r = lbl_1_bss_EC0.rows;
    while (r-- != 0) {
        n = lbl_1_bss_EC0.cnt[r];
        while (n-- != 0) {
            HuSprTPLvlSet(lbl_1_bss_EC0.grp, (s16)(r * lbl_1_bss_EC0.cols + n + 5), 1.0f);
        }
    }
    HuSprGrpPosSet(lbl_1_bss_EC0.grp, lbl_1_data_834[0], lbl_1_data_834[1]);
    lbl_1_bss_EC0.x = lbl_1_data_834[0];
    lbl_1_bss_EC0.y = lbl_1_data_834[1];
    for (i = 0; i < lbl_1_bss_EC0.cols * lbl_1_bss_EC0.rows + 5; i++) {
        HuSprAttrSet(lbl_1_bss_EC0.grp, i, 4);
    }
    for (k = 0; k < 2; k++) {
        Hu3DModelAttrSet(lbl_1_bss_EC0.mdl[k].id, 1);
    }
    lbl_1_bss_EC0.state = 3;
    lbl_1_bss_EC0.sub = 0;
    lbl_1_bss_EC0.result = 0;
}

void fn_1_12AE4(s8 no)
{
    lbl_1_bss_EC0.state = no;
    lbl_1_bss_EC0.sub = 0;
}

s32 fn_1_12B08(void)
{
    switch (lbl_1_bss_EC0.state) {
        case 3:
            return 1;
        default:
            if (lbl_1_bss_EC0.state >= 1 && lbl_1_bss_EC0.state < 6) {
                return 0;
            }
            return 1;
    }
}

s8 fn_1_12B4C(void)
{
    return (s8)lbl_1_bss_EC0.result;
}

void fn_1_12B60(void)
{
    Vec diff[2];
    Vec pos;
    f32 xy[2];
    f32 dy;
    f32 f;
    f32 s;
    f32 off;
    f32 sc;
    s32 t = 0;
    s32 frame;
    s32 baseIdx;
    s32 n;
    s16 i;
    s32 k;
    s8 p;
    s16 idx;

#ifdef TARGET_PC
    frame = 0;
#endif
    while (TRUE) {
        switch (lbl_1_bss_EC0.state) {
            case 3:
                break;
            case 1:
                dy = lbl_1_data_834[3] - lbl_1_data_834[1];
                switch (lbl_1_bss_EC0.sub) {
                    case 0:
                        for (i = 0; i < lbl_1_bss_EC0.cols * lbl_1_bss_EC0.rows + 5; i++) {
                            HuSprAttrReset(lbl_1_bss_EC0.grp, i, 4);
                        }
                        for (k = 0; k < 2; k++) {
                            Hu3DModelAttrReset(lbl_1_bss_EC0.mdl[k].id, 1);
                        }
                        frame = 0;
                        lbl_1_bss_EC0.sub = 1;
                        break;
                    case 1:
                        k = 2;
                        while (k-- != 0) {
                            f32 *e = lbl_1_data_844[lbl_1_bss_EC0.type][k];
                            PSVECSubtract((Vec *)(e + 3), (Vec *)e, &diff[k]);
                        }
                        if (frame <= 60) {
                            f = (f32)sin(3.141592653589793 * (f64)(f32)sin(3.141592653589793 * (f64)((f32)frame / 60.0f) * 0.5) * 0.5);
                            xy[0] = lbl_1_data_834[0];
                            xy[1] = lbl_1_data_834[1];
                            xy[1] += dy * f;
                            HuSprGrpPosSet(lbl_1_bss_EC0.grp, xy[0], xy[1]);
                            lbl_1_bss_EC0.x = xy[0];
                            lbl_1_bss_EC0.y = xy[1];
                            k = 2;
                            while (k-- != 0) {
                                PSVECScale(&diff[k], &lbl_1_bss_EC0.mdl[k].pos, f);
                                PSVECAdd((Vec *)lbl_1_data_844[lbl_1_bss_EC0.type][k], &lbl_1_bss_EC0.mdl[k].pos,
                                    &lbl_1_bss_EC0.mdl[k].pos);
                                Hu3DModelPosSet(lbl_1_bss_EC0.mdl[k].id, lbl_1_bss_EC0.mdl[k].pos.x,
                                    lbl_1_bss_EC0.mdl[k].pos.y, lbl_1_bss_EC0.mdl[k].pos.z);
                            }
                            frame++;
                        } else {
                            lbl_1_bss_EC0.sub = 2;
                        }
                        break;
                    case 2:
                        lbl_1_bss_EC0.state = 3;
                        lbl_1_bss_EC0.sub = 0;
                        lbl_1_bss_EC0.result = 1;
                        break;
                }
                break;
            case 2:
                dy = lbl_1_data_834[1] - lbl_1_data_834[3];
                switch (lbl_1_bss_EC0.sub) {
                    case 0:
                        frame = 0;
                        lbl_1_bss_EC0.sub = 1;
                        break;
                    case 1:
                        k = 2;
                        while (k-- != 0) {
                            f32 *e = lbl_1_data_844[lbl_1_bss_EC0.type][k];
                            PSVECSubtract((Vec *)e, (Vec *)(e + 3), &diff[k]);
                        }
                        if (frame <= 60) {
                            f = (f32)cos(3.141592653589793 * (f64)(f32)cos(3.141592653589793 * (f64)((f32)frame / 60.0f) * 0.5) * 0.5);
                            xy[0] = lbl_1_data_834[2];
                            xy[1] = lbl_1_data_834[3];
                            xy[1] += dy * f;
                            HuSprGrpPosSet(lbl_1_bss_EC0.grp, xy[0], xy[1]);
                            lbl_1_bss_EC0.x = xy[0];
                            lbl_1_bss_EC0.y = xy[1];
                            k = 2;
                            while (k-- != 0) {
                                PSVECScale(&diff[k], &lbl_1_bss_EC0.mdl[k].pos, f);
                                PSVECAdd((Vec *)(lbl_1_data_844[lbl_1_bss_EC0.type][k] + 3), &lbl_1_bss_EC0.mdl[k].pos,
                                    &lbl_1_bss_EC0.mdl[k].pos);
                                Hu3DModelPosSet(lbl_1_bss_EC0.mdl[k].id, lbl_1_bss_EC0.mdl[k].pos.x,
                                    lbl_1_bss_EC0.mdl[k].pos.y, lbl_1_bss_EC0.mdl[k].pos.z);
                            }
                            frame++;
                        } else {
                            lbl_1_bss_EC0.sub = 2;
                        }
                        break;
                    case 2:
                        for (i = 0; i < lbl_1_bss_EC0.cols * lbl_1_bss_EC0.rows + 5; i++) {
                            HuSprAttrSet(lbl_1_bss_EC0.grp, i, 4);
                        }
                        for (k = 0; k < 2; k++) {
                            Hu3DModelAttrSet(lbl_1_bss_EC0.mdl[k].id, 1);
                        }
                        lbl_1_bss_EC0.state = 3;
                        lbl_1_bss_EC0.sub = 0;
                        lbl_1_bss_EC0.result = 0;
                        break;
                }
                break;
            case 4:
                p = fn_1_588();
                idx = (s16)(lbl_1_bss_EC0.cols * p + lbl_1_bss_EC0.cnt[p] + 5);
                switch (lbl_1_bss_EC0.sub) {
                    case 0:
                        HuAudFXPlay(0x48F);
                        frame = 0;
                        lbl_1_bss_EC0.sub = 1;
                        break;
                    case 1:
                        if (frame <= 45) {
                            s = (f32)frame / 45.0f;
                            f = (f32)sin(3.141592653589793 * (f64)(f32)sin(3.141592653589793 * (f64)s * 0.5) * 0.5);
                            sc = 1.0f + 40.0f * (1.0f - f);
                            HuSprTPLvlSet(lbl_1_bss_EC0.grp, idx,
                                (f32)cos(3.141592653589793 * (f64)(f32)cos(3.141592653589793 * (f64)s * 0.5) * 0.5));
                            HuSprScaleSet(lbl_1_bss_EC0.grp, idx, sc, sc);
                            frame++;
                        } else {
                            lbl_1_bss_EC0.sub = 2;
                        }
                        break;
                    case 2:
                        switch (fn_1_59C()) {
                            case 0:
                                fn_1_5D0(p, 0xC);
                                break;
                            case 2:
                                if (p == 0) {
                                    fn_1_5D0(0, 0xC);
                                    fn_1_5D0(1, 0xC);
                                } else {
                                    fn_1_5D0(2, 0xC);
                                    fn_1_5D0(3, 0xC);
                                }
                                break;
                            case 1:
                                if (p == 0) {
                                    fn_1_5D0(0, 0xC);
                                } else {
                                    fn_1_5D0(1, 0xC);
                                    fn_1_5D0(2, 0xC);
                                    fn_1_5D0(3, 0xC);
                                }
                                break;
                        }
                        HuAudFXPlay(0x490);
                        lbl_1_bss_EC0.cnt[fn_1_588()]++;
                        lbl_1_bss_EC0.state = 3;
                        lbl_1_bss_EC0.sub = 0;
                        break;
                }
                break;
            case 5:
                baseIdx = lbl_1_bss_EC0.cols * fn_1_624() + 5;
                switch (lbl_1_bss_EC0.sub) {
                    case 0:
                        frame = 0;
                        lbl_1_bss_EC0.sub = 1;
                        break;
                    case 1:
                        if ((f32)frame <= 60.0f) {
                            f = (f32)frame / 60.0f;
                            n = lbl_1_bss_EC0.cnt[fn_1_624()];
                            while (n-- != 0) {
                                Hu3DModelTPLvlSet(lbl_1_bss_EC0.mdlList[n + 9], f);
                                HuSprTPLvlSet(lbl_1_bss_EC0.grp, (s16)(baseIdx + n), 1.0f - f);
                            }
                            frame++;
                        } else {
                            lbl_1_bss_EC0.state = 3;
                            lbl_1_bss_EC0.sub = 0;
                        }
                        break;
                }
                break;
        }
        s = (f32)sin(2.0 * (3.141592653589793 * (f64)((f32)t / 120.0f)));
        off = 30.0f * s;
        k = 2;
        while (k-- != 0) {
            pos = lbl_1_bss_EC0.mdl[k].pos;
            pos.y += off;
            Hu3DModelPosSet(lbl_1_bss_EC0.mdl[k].id, pos.x, pos.y, pos.z);
        }
        xy[0] = lbl_1_bss_EC0.x;
        xy[1] = lbl_1_bss_EC0.y;
        xy[1] -= 10.0f * s;
        HuSprGrpPosSet(lbl_1_bss_EC0.grp, xy[0], xy[1]);
        t = (t + 1) % 120;
        HuPrcVSleep();
    }
}

void fn_1_138B8(Vec *dst, Vec *scale, s32 row, s32 col, s32 gameType, f32 div, f32 z)
{
    Vec p;
    Vec lo;
    Vec hi;
    s32 ofs = 0;

    switch (gameType) {
        case 0:
            ofs = 0;
            break;
        case 2:
            ofs = 4;
            break;
        case 1:
            ofs = 6;
            break;
    }
    p.x = 288.0f + ((f32)(col * 0x2A) + lbl_1_data_728[lbl_1_bss_EC0.type][ofs + row][0]);
    p.y = lbl_1_data_834[3] + lbl_1_data_728[lbl_1_bss_EC0.type][ofs + row][1];
    p.z = z;
    lo.x = (288.0f + ((f32)(col * 0x2A) + lbl_1_data_728[lbl_1_bss_EC0.type][ofs + row][0])) - 21.0f;
    lo.y = (lbl_1_data_834[3] + lbl_1_data_728[lbl_1_bss_EC0.type][ofs + row][1]) - 21.0f;
    lo.z = z;
    hi.x = 21.0f + (288.0f + ((f32)(col * 0x2A) + lbl_1_data_728[lbl_1_bss_EC0.type][ofs + row][0]));
    hi.y = 21.0f + (lbl_1_data_834[3] + lbl_1_data_728[lbl_1_bss_EC0.type][ofs + row][1]);
    hi.z = z;
    scale->x = (f32)((hi.x - lo.x) / div);
    scale->y = (f32)((hi.y - lo.y) / div);
    scale->z = 1.0f;
    Hu3D2Dto3D(&p, 0x10, dst);
}

void fn_1_13C1C(s16 team, s32 gameType, s32 maxScore, s32 *order, s32 *rank)
{
    s16 grpC;
    s16 grpA;
    s16 grpB;
    s32 ctype = 0;
    s32 rowN = 0;
    s32 rowOfs = 0;
    s32 mtype = 0;
    s32 cols = 0;
    s32 sel;
    s16 i;
    s32 j;
    f32 x;
    f32 y;
    ANIMDATA *anim;
    s16 mdl;

    sel = (team != 0) ? 1 : 0;
    lbl_1_bss_E74.gameType = gameType;
    lbl_1_bss_E74.team = team;
    switch (gameType) {
        case 0:
            ctype = 0;
            rowN = 4;
            rowOfs = 0;
            sel = 0;
            break;
        case 2:
            ctype = 1;
            rowN = 2;
            rowOfs = 4;
            break;
        case 1:
            ctype = 2;
            rowN = 2;
            rowOfs = (team != 0) ? 8 : 6;
            break;
    }
    switch (maxScore) {
        case 7:
            mtype = 0;
            cols = 7;
            break;
        case 5:
            mtype = 1;
            cols = 5;
            break;
        case 3:
            mtype = 2;
            cols = 3;
            break;
    }
    if (lbl_1_bss_E74.init == 0) {
        lbl_1_bss_E74.init = 1;
        anim = HuSprAnimRead(HuDataSelHeapReadNum(0x98003A, HU_MEMNUM_OVL, HEAP_MODEL));
        grpA = HuSprGrpCreate(2);
        HuSprGrpMemberSet(grpA, 0, HuSprCreate(anim, 0x50, 0));
        HuSprPosSet(grpA, 0, 0.0f, -160.0f);
        HuSprGrpMemberSet(grpA, 1,
            HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x98004C, HU_MEMNUM_OVL, HEAP_MODEL)), 0x7F, 0));
        HuSprScaleSet(grpA, 1, 72.0f, 60.0f);
        HuSprTPLvlSet(grpA, 1, 0.5f);
        grpB = HuSprGrpCreate(4);
        i = 4;
        while (i-- != 0) {
            lbl_1_data_BBC[mtype][i][1] -= 23.0f;
            x = lbl_1_data_BBC[mtype][i][0];
            y = lbl_1_data_BBC[mtype][i][1];
            HuSprGrpMemberSet(grpB, i,
                HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x98004B, HU_MEMNUM_OVL, HEAP_MODEL)), 10, 0));
            HuSprPosSet(grpB, i, x, y);
            HuSprAttrSet(grpB, i, 4);
            HuSprScaleSet(grpB, i, 0.0f, 0.0f);
        }
        anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_8D4[ctype][sel][mtype], HU_MEMNUM_OVL, HEAP_MODEL));
        grpC = HuSprGrpCreate((s16)(cols * rowN + 6));
        HuSprGrpMemberSet(grpC, 0, HuSprCreate(anim, 0x64, 0));
        i = rowN;
        while (i-- != 0) {
            f32 *e = &lbl_1_data_ACC[mtype * 20 + (rowOfs + i) * 2];
            x = e[0];
            y = e[1];
            HuSprGrpMemberSet(grpC, (s16)(i + 1),
                HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x980038, HU_MEMNUM_OVL, HEAP_MODEL)), 10, i));
            HuSprPosSet(grpC, (s16)(i + 1), x, y);
        }
        i = rowN;
        while (i-- != 0) {
            f32 *e = &lbl_1_data_91C[mtype * 20 + (rowOfs + i) * 2];
            x = e[0];
            y = e[1];
            for (j = 0; j < cols; j++) {
                s16 idx = (s16)(i * cols + j + 6);
                HuSprGrpMemberSet(grpC, idx,
                    HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x98003C, HU_MEMNUM_OVL, HEAP_MODEL)), 10, 0));
                HuSprPosSet(grpC, idx, x, y);
                x += 54.0f;
            }
        }
        HuSprGrpDrawNoSet(grpC, 0x40);
        HuSprGrpDrawNoSet(grpA, 0x40);
        HuSprGrpDrawNoSet(grpB, 0);
        HuSprExecLayerCameraSet(0x40, 0x10, 0xC);
        lbl_1_bss_E74.grp[0] = grpC;
        lbl_1_bss_E74.grp[1] = grpA;
        lbl_1_bss_E74.grp[2] = grpB;
    } else {
        grpC = lbl_1_bss_E74.grp[0];
        grpA = lbl_1_bss_E74.grp[1];
        grpB = lbl_1_bss_E74.grp[2];
    }
    HuSprGrpPosSet(grpC, 550.0f + lbl_1_data_C1C[2], lbl_1_data_C1C[3]);
    HuSprGrpPosSet(grpA, lbl_1_data_C2C[2], lbl_1_data_C2C[3]);
    HuSprGrpPosSet(grpB, lbl_1_data_C3C[2], lbl_1_data_C3C[3]);
    i = 4;
    while (i-- != 0) {
        lbl_1_bss_E74.order[i] = (s16)order[i];
    }
    i = rowN;
    while (i-- != 0) {
        for (j = 0; j < cols; j++) {
            HuSprTPLvlSet(grpC, (s16)(i * cols + j + 6), 0.0f);
        }
    }
    for (j = 0; j < 4; j++) {
        mdl = lbl_1_bss_EC0.mdlList[order[j] + 0x1A];
        lbl_1_bss_E74.jointType[j] = rank[order[j]];
        lbl_1_bss_E74.mdl[j] = mdl;
    }
    fn_1_898(0x20000);
    fn_1_87C(0x400000);
    lbl_1_bss_E74.state = 0;
    HuPrcChildCreate(fn_1_14350, 100, 0x1000, 0, HuPrcCurrentGet());
}

/* Plays the three-step "win" scale-in of one head model of the result screen. */
void fn_1_14350(void)
{
    Vec scl;
    s32 se[7] = { 0x498, 0x499, 0x49A, 0x49B, 0x49C, 0x49E, 0x49F };
    f32 f;
    f32 g;
    f64 ang;
    s32 idx;
    s16 m;
    s16 anim;
    s16 i;
    s16 k;
    s16 q;

#ifdef TARGET_PC
    ang = 0.0;
#endif
    while (fn_1_8B4(0x400000) != 0) {
        switch (lbl_1_bss_E74.state) {
            case 0x0:
                lbl_1_bss_E74.state = 1;
                lbl_1_bss_E74.timer = 0x78;
                break;
            case 0x1:
                if (lbl_1_bss_E74.timer == 0) {
                    lbl_1_bss_E74.timer = 0x3C;
                    lbl_1_bss_E74.state = 2;
                } else {
                    lbl_1_bss_E74.timer--;
                }
                break;
            case 0x2:
                HuSprGrpPosSet(lbl_1_bss_E74.grp[0],
                    lbl_1_data_C1C[2] + (550.0f * (f32)sin(3.141592653589793 * (f64)((f32)lbl_1_bss_E74.timer / 60.0f) * 0.5)),
                    lbl_1_data_C1C[3]);
                if (lbl_1_bss_E74.timer == 0) {
                    lbl_1_bss_E74.state = 0x64;
                } else {
                    lbl_1_bss_E74.timer--;
                }
                break;
            case 0x64:
                scl.x = 0.0f;
                scl.y = 0.0f;
                scl.z = 0.01f;
                i = 4;
                while (i-- != 0) {
                    m = lbl_1_bss_EC0.mdlList[i + 0x1E];
                    switch (lbl_1_bss_E74.gameType) {
                        case 0:
                            if (i == 0) {
                                HuSprAttrReset(lbl_1_bss_E74.grp[2], i, 4);
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003D, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            } else if (i == 1) {
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003E, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            } else {
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003F, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            }
                            break;
                        case 2:
                            if (i <= 1) {
                                HuSprAttrReset(lbl_1_bss_E74.grp[2], i, 4);
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003D, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            } else {
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003F, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            }
                            break;
                        case 1:
                            if (lbl_1_bss_E74.team == 0) {
                                if (i == 0) {
                                    HuSprAttrReset(lbl_1_bss_E74.grp[2], i, 4);
                                    anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003D, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                    Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                                } else {
                                    anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003F, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                    Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                                }
                            } else if (i <= 2) {
                                HuSprAttrReset(lbl_1_bss_E74.grp[2], i, 4);
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003D, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            } else {
                                anim = Hu3DAnimCreate(HuDataSelHeapReadNum(0x98003F, HU_MEMNUM_OVL, HEAP_MODEL), m, lbl_1_data_E14);
                                Hu3DAnimBankSet(anim, (u16)lbl_1_bss_E74.jointType[i]);
                            }
                            break;
                    }
                    Hu3DModelPosSetV(m, (HuVecF *)&lbl_1_data_D0C[lbl_1_bss_EC0.type * 12 + i * 3]);
                    Hu3DModelAttrReset(m, 1);
                    Hu3DModelScaleSetV(m, &scl);
                }
                i = 4;
                while (i-- != 0) {
                    Hu3DModelAttrReset(lbl_1_bss_EC0.mdlList[i + 0x1E], 1);
                }
                HuAudFXPlay(0x497);
                lbl_1_bss_E74.state = 0x65;
                lbl_1_bss_E74.timer = 0;
                lbl_1_bss_E74.ampA = 0.8f;
                lbl_1_bss_E74.ampB = (f32)(3.141592653589793 - (f64)(f32)asin((f64)lbl_1_bss_E74.ampA));
                break;
            case 0x65:
                f = (f32)sin((f64)((f32)sin(3.141592653589793 * (f64)((f32)lbl_1_bss_E74.timer / 30.0f) * 0.5)
                                * lbl_1_bss_E74.ampB))
                    * (1.0f / lbl_1_bss_E74.ampA);
                scl.x = 0.45f * f;
                scl.y = 0.45f * f;
                scl.z = 0.01f;
                i = 4;
                while (i-- != 0) {
                    Hu3DModelScaleSetV(lbl_1_bss_EC0.mdlList[i + 0x1E], &scl);
                }
                if ((f32)lbl_1_bss_E74.timer >= 30.0f) {
                    lbl_1_bss_E74.state = 0x66;
                    lbl_1_bss_E74.timer = 0x1E;
                } else {
                    lbl_1_bss_E74.timer++;
                }
                break;
            case 0x66:
                if (lbl_1_bss_E74.timer == 0) {
                    lbl_1_bss_E74.ampA = 0.9f;
                    lbl_1_bss_E74.ampB = (f32)(3.141592653589793 - (f64)(f32)asin((f64)lbl_1_bss_E74.ampA));
                    lbl_1_bss_E74.count = 1;
                    lbl_1_bss_E74.timer = 0;
                    lbl_1_bss_E74.state = 0xC8;
                } else {
                    lbl_1_bss_E74.timer--;
                }
                break;
            case 0xC8:
                idx = lbl_1_bss_E74.count - 1;
                f = (f32)sin(3.141592653589793 * (f64)((f32)lbl_1_bss_E74.timer / 20.0f) * 0.5);
                g = (f32)sin((f64)(f * lbl_1_bss_E74.ampB)) * (1.0f / lbl_1_bss_E74.ampA);
                if (lbl_1_bss_E74.gameType != 0) {
                    if (lbl_1_bss_E74.gameType >= 0 && lbl_1_bss_E74.gameType < 3) {
                        if (lbl_1_bss_E74.team != 0) {
                            if (lbl_1_bss_EC0.cnt[0] >= lbl_1_bss_E74.count) {
                                k = lbl_1_bss_EC0.cols + idx + 6;
                                HuSprTPLvlSet(lbl_1_bss_E74.grp[0], k, f);
                                HuSprScaleSet(lbl_1_bss_E74.grp[0], k, g, g);
                            }
                            if (lbl_1_bss_EC0.cnt[1] >= lbl_1_bss_E74.count) {
                                q = idx + 6;
                                HuSprTPLvlSet(lbl_1_bss_E74.grp[0], q, f);
                                HuSprScaleSet(lbl_1_bss_E74.grp[0], q, g, g);
                            }
                        } else {
                            if (lbl_1_bss_EC0.cnt[0] >= lbl_1_bss_E74.count) {
                                k = idx + 6;
                                HuSprTPLvlSet(lbl_1_bss_E74.grp[0], k, f);
                                HuSprScaleSet(lbl_1_bss_E74.grp[0], k, g, g);
                            }
                            if (lbl_1_bss_EC0.cnt[1] >= lbl_1_bss_E74.count) {
                                q = lbl_1_bss_EC0.cols + idx + 6;
                                HuSprTPLvlSet(lbl_1_bss_E74.grp[0], q, f);
                                HuSprScaleSet(lbl_1_bss_E74.grp[0], q, g, g);
                            }
                        }
                    }
                } else {
                    i = 4;
                    while (i-- != 0) {
                        if (lbl_1_bss_EC0.cnt[lbl_1_bss_E74.order[i]] >= lbl_1_bss_E74.count) {
                            k = i * lbl_1_bss_EC0.cols + idx + 6;
                            HuSprTPLvlSet(lbl_1_bss_E74.grp[0], k, f);
                            HuSprScaleSet(lbl_1_bss_E74.grp[0], k, g, g);
                        }
                    }
                }
                if (lbl_1_bss_E74.timer == 0) {
                    HuAudFXPlay(se[idx]);
                }
                if ((f32)lbl_1_bss_E74.timer >= 20.0f) {
                    OSReport(lbl_1_data_E22, lbl_1_bss_E74.count);
                    if (lbl_1_bss_E74.count == lbl_1_bss_EC0.cols) {
                        HuAudFXPlay(0x4A0);
                        lbl_1_bss_E74.state = 0x12C;
                        lbl_1_bss_E74.timer = 0;
                        lbl_1_bss_E74.ampA = 0.8f;
                        lbl_1_bss_E74.ampB = (f32)(3.141592653589793 - (f64)(f32)asin((f64)lbl_1_bss_E74.ampA));
                    } else {
                        lbl_1_bss_E74.timer = 0;
                    }
                    lbl_1_bss_E74.count++;
                } else {
                    lbl_1_bss_E74.timer++;
                }
                break;
            case 0x12C:
                f = (f32)sin((f64)((f32)sin(3.141592653589793 * (f64)((f32)lbl_1_bss_E74.timer / 30.0f) * 0.5)
                                * lbl_1_bss_E74.ampB))
                    * (1.0f / lbl_1_bss_E74.ampA);
                i = 4;
                while (i-- != 0) {
                    HuSprScaleSet(lbl_1_bss_E74.grp[2], i, (f32)(0.7 * (f64)f), (f32)(0.7 * (f64)f));
                }
                if ((f32)lbl_1_bss_E74.timer >= 30.0f) {
                    ang = 0.0;
                    lbl_1_bss_E74.state = 0x12D;
                    fn_1_87C(0x20000);
                } else {
                    lbl_1_bss_E74.timer++;
                }
                break;
            case 0x12D:
                f = (f32)(0.05000000074505806 * sin((3.141592653589793 * ang) / 180.0));
                ang = (f64)(f32)fmod((f64)((f32)ang + 3.0f), 360.0);
                i = 4;
                while (i-- != 0) {
                    HuSprScaleSet(lbl_1_bss_E74.grp[2], i, (f32)(0.7 + (f64)f), (f32)(0.7 + (f64)f));
                }
                break;
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_15460(s16 layer)
{
    Vec p;
    f32 xy[2];
    s32 i;

    if (Hu3DCameraBit == 1) {
        for (i = 0; i < 4; i++) {
            Hu3DModelObjPosGet(lbl_1_bss_E74.mdl[i], lbl_1_data_C9C[lbl_1_bss_E74.jointType[i]], &p);
            p.y += lbl_1_data_CD8[lbl_1_bss_E74.jointType[i]];
            Hu3D3Dto2D(&p, 1, (HuVecF *)xy);
            lbl_1_bss_E38[i].x = (f32)(s16)((s32)(1.1111111111111112 * (f64)xy[0]) & 0xFFFE);
            lbl_1_bss_E38[i].y = (f32)(s16)((s32)xy[1] & 0xFFFE);
            Hu3DFbCopyExec((s16)(s32)(lbl_1_bss_E38[i].x - 40.0f), (s16)(s32)(lbl_1_bss_E38[i].y - 40.0f), 0x50, 0x50,
                GX_TF_RGB565, 0, lbl_1_bss_E74.anim[i]->bmp->data);
        }
        Hu3DZClear();
    }
}

void fn_1_15690(s32 type, s32 *order, s32 *rank)
{
    Vec p;
    f32 xy[2];
    Vec scl;
    HU3DMODELID model;
    s32 i;

    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x980018, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(model, 3);
    Hu3DModelCameraSet(model, 1);
    Hu3DModelPosSet(model, lbl_1_data_C4C[0].x, lbl_1_data_C4C[0].y, lbl_1_data_C4C[0].z);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x280, 0x1E0);
    Hu3DCameraPosSet(1, 0.0f, 100.0f, 8000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f);
    for (i = 0; i < 4; i++) {
        model = lbl_1_bss_EC0.mdlList[order[i] + 0x1A];
        Hu3DModelPosSet(model, (f32)(i * 0x96 - 0x12C), 0.0f, 0.0f);
        Hu3DModelLayerSet(model, 3);
        Hu3DModelCameraSet(model, 1);
        Hu3DModelAttrReset(model, 1);
        lbl_1_bss_E74.jointType[i] = rank[order[i]];
        lbl_1_bss_E74.mdl[i] = model;
        Hu3DModelObjPosGet(model, lbl_1_data_C9C[rank[order[i]]], &p);
        p.y += lbl_1_data_CD8[rank[order[i]]];
        Hu3D3Dto2D(&p, 1, (HuVecF *)xy);
        lbl_1_bss_E38[i].x = (f32)(s16)((s32)(1.1111111111111112 * (f64)xy[0]) & 0xFFFE);
        lbl_1_bss_E38[i].y = (f32)(s16)((s32)xy[1] & 0xFFFE);
        lbl_1_bss_E74.anim[i] = HuSprAnimMake(0x50, 0x50, 2);
        lbl_1_bss_E74.anim[i]->bmp->data = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_E74.anim[i]->bmp->dataSize, HU_MEMNUM_OVL);
        memset(lbl_1_bss_E74.anim[i]->bmp->data, 0, lbl_1_bss_E74.anim[i]->bmp->dataSize);
    }
    Hu3DLayerHookSet(6, fn_1_15460);
    HuPrcVSleep();
    scl.x = 0.45f;
    scl.y = 0.45f;
    scl.z = 0.01f;
    i = 4;
    while (i-- != 0) {
        model = lbl_1_bss_EC0.mdlList[i + 0x1E];
        Hu3DModelPosSetV(model, (HuVecF *)&lbl_1_data_D0C[type * 12 + i * 3]);
        Hu3DModelAttrReset(model, 1);
        Hu3DAnimCreate(lbl_1_bss_E74.anim[i], model, lbl_1_data_E31);
        Hu3DModelScaleSetV(model, &scl);
    }
}

void fn_1_15BC8(void)
{
    if (lbl_1_bss_E74.init != 0) {
        HuSprGrpPosSet(lbl_1_bss_E74.grp[0], lbl_1_data_C1C[0], lbl_1_data_C1C[1]);
        HuSprGrpPosSet(lbl_1_bss_E74.grp[1], lbl_1_data_C2C[0], lbl_1_data_C2C[1]);
        HuSprGrpPosSet(lbl_1_bss_E74.grp[2], lbl_1_data_C3C[0], lbl_1_data_C3C[1]);
    }
}

void fn_1_15C74(SCOREPLAYER *player)
{
    s16 grp;
    s16 idx;
    s32 i;

    grp = HuSprGrpCreate(5);
    lbl_1_bss_E68[0] = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x98004E, HU_MEMNUM_OVL, HEAP_MODEL)), 10, 0));
    i = 4;
    while (i-- != 0) {
        idx = i + 1;
        HuSprGrpMemberSet(grp, idx,
            HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_7E8[player[i].charNo], HU_MEMNUM_OVL, HEAP_MODEL)),
                10, 0));
        HuSprPosSet(grp, idx, lbl_1_data_E0C[0], lbl_1_data_E0C[1]);
        HuSprAttrSet(grp, idx, 4);
    }
    HuSprGrpDrawNoSet(grp, 0);
    HuSprGrpTPLvlSet(grp, 0.0f);
}

void fn_1_15DB8(SCOREPLAYER *player, Vec *pos)
{
    Vec p;
    ANIMDATA *anim;
    s32 i;

    if (player == NULL || pos == NULL) {
        HuSprGrpTPLvlSet(lbl_1_bss_E68[0], 0.0f);
        return;
    }
    HuSprGrpTPLvlSet(lbl_1_bss_E68[0], 1.0f);
    p = *pos;
    p.y += 160.0f;
    Hu3D3Dto2D(&p, 0x10, &p);
    HuSprGrpPosSet(lbl_1_bss_E68[0], p.x, p.y);
    i = 4;
    while (i-- != 0) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_7E8[player[i].charNo], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAttrSet(lbl_1_bss_E68[0], (s16)(i + 1), 4);
    }
    HuSprAttrReset(lbl_1_bss_E68[0], (s16)(player->playerNo + 1), 4);
}
