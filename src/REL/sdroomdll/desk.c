#include "types.h"

extern int sprintf();

typedef struct {
    u8 pad[0x428];
    u32 sdPoint[4];
} DESK_GWCOMMON;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2[10];
    u16 partFlag[4];
} DESK_GWSDCOMMON;

typedef struct {
    u8 pad[4];
    s16 mdlId[4];
} DESK_MDLTBL;

extern DESK_GWCOMMON GwCommon;
extern DESK_GWSDCOMMON GwSdCommon;
extern DESK_MDLTBL gDeskMdlTbl;

extern void *gDeskPrc;
extern s16 gDeskSprGrpId;
extern s16 gDeskSpinMdlId[2];
extern s16 gDeskWinId[7];
extern s16 gDeskWinId2[10];
extern s16 gDeskWinId3[10];
extern char gDeskMesBuf[6];
extern char gDeskNumBuf[4];
extern s16 gDeskPicMax[];

extern void HuWinKill(s16 winId);
extern void HuWinMesSet(s16 winId, u32 messNum);
extern void HuWinInsertMesSet(s16 winId, char *messStr, s16 insertMesNo);
extern void HuWinScaleSet(s16 winId, f32 scaleX, f32 scaleY);
extern void Hu3DModelKill(s16 modelId);
extern void Hu3DModelAttrReset(s16 modelId, u32 attr);
extern void Hu3DModelRotSet(s16 modelId, f32 rotX, f64 rotY, f32 rotZ);
extern void HuSprGrpKill(s16 grpId);
extern s32 HuPrcKill(void *process);
extern void HuPrcVSleep(void);

static const char gDeskFmt02[] = "%02d";

static const u32 gDeskMessTbl[4] = {
    0x00190022,
    0x00190024,
    0x00190023,
    0x00190025,
};

void fn_1_26DAC(void)
{
    s16 i;

    for (i = 0; i < 7; i++) {
        HuWinKill(gDeskWinId[i]);
    }

    for (i = 0; i < 10; i++) {
        HuWinKill(gDeskWinId2[i]);
        HuWinKill(gDeskWinId3[i]);
    }

    HuPrcKill(gDeskPrc);

    Hu3DModelAttrReset(gDeskMdlTbl.mdlId[1], 1);
    Hu3DModelAttrReset(gDeskMdlTbl.mdlId[2], 1);
    Hu3DModelAttrReset(gDeskMdlTbl.mdlId[3], 1);
    Hu3DModelAttrReset(gDeskMdlTbl.mdlId[0], 1);

    HuSprGrpKill(gDeskSprGrpId);

    if (gDeskSpinMdlId[0] != -1) {
        Hu3DModelKill(gDeskSpinMdlId[0]);
    }

    if (gDeskSpinMdlId[1] != -1) {
        Hu3DModelKill(gDeskSpinMdlId[1]);
    }
}

// NONMATCHING: two instructions differ from the original - mwcceppc 1.3.2
// emits `fsub f2, f0, f2` where CodeWarrior emitted the single-precision
// form `fsubs f2, f0, f2` for the s16 -> f64 magic conversion. With an f32
// middle parameter mwcceppc additionally inserts an `frsp`, so neither the
// f32 nor the f64 prototype reproduces the target's 62-instruction body.
void fn_1_26F0C(void)
{
    s16 i = 0;

    while (1) {
        if (gDeskSpinMdlId[0] >= 0) {
            Hu3DModelRotSet(gDeskSpinMdlId[0], 0.0f, (f64)i, 0.0f);

            if (gDeskSpinMdlId[1] >= 0) {
                Hu3DModelRotSet(gDeskSpinMdlId[1], 0.0f, (f64)i, 0.0f);
            }

            i += 2;

            if (i > 360) {
                i -= 360;
            }
        }

        HuPrcVSleep();
    }
}

void fn_1_27004(s16 no)
{
    sprintf(gDeskNumBuf, "%03d", (s32)GwCommon.sdPoint[no]);
    HuWinInsertMesSet(gDeskWinId[1], gDeskNumBuf, 0);
    HuWinMesSet(gDeskWinId[1], gDeskMessTbl[no]);
}

void fn_1_2709C(s16 winId, s16 picNo)
{
    s16 i;
    s16 count = 0;

    for (i = count; i < gDeskPicMax[picNo]; i++) {
        if (GwSdCommon.partFlag[picNo] & (1 << i)) {
            count++;
        }
    }

    sprintf(gDeskMesBuf, gDeskFmt02, count);
    sprintf(gDeskMesBuf + 3, gDeskFmt02, gDeskPicMax[picNo]);
    HuWinInsertMesSet(winId, gDeskMesBuf, 0);
    HuWinInsertMesSet(winId, gDeskMesBuf + 3, 1);
    HuWinMesSet(winId, 0x0019002C);
    HuWinScaleSet(winId, 0.8f, 0.8f);
}