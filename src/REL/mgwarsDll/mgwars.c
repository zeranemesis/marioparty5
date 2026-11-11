#include "game/gamework.h"
#include "game/process.h"
#include "game/wipe.h"
#include "game/window.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/charman.h"
#include "game/audio.h"
#include "game/object.h"
#include "game/mgdata.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/sprite.h"
#include "string.h"
#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);
extern float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

void fn_1_498(void);
void fn_1_4BC4(void);
void fn_1_4EE8(void);
void fn_1_DA8(void);
void fn_1_7034(void);
s32 fn_1_C04(s32, s32, s32);
void fn_1_64C(u32 mask);
void fn_1_6068(void);
void fn_1_6444(void);
void fn_1_6A14(void);
void fn_1_71C0(void);
void fn_1_328(void);
void fn_1_182C(void);
void fn_1_8C80(void);
void fn_1_668(u32 mask);
u32 fn_1_684(u32 mask);
void fn_1_8C8(void);
void fn_1_F14(void);
void fn_1_FD0(void);
void fn_1_108C(void);
void fn_1_1148(void);
void fn_1_728C(void);
void fn_1_7E30(void);
void fn_1_8DF0(void);
void fn_1_9BC8(void);
void fn_1_C620(void);
void fn_1_796C(void);
void fn_1_B614(void);
void fn_1_C19C(void);
void fn_1_7CC8(void);
void fn_1_1A80(void);
void fn_1_57C0(s16, HuVecF *);
void fn_1_F068(void);
void fn_1_F848(s32, s32);

typedef struct FbNode_s {
	u8 pad0[0x10];
	struct FbNode_s *next;
} FBNODE;

typedef struct MgWarsGame_s {
	u16 ovl;
	u16 unlock;
	s8 mgIdx;
	u8 is3D;
} MGWARSGAME;

typedef struct MgWarsDataEnt_s {
	u16 ovl;
	u16 unlock;
	s8 mgIdx;
	u8 pad;
} MGWARSDATAENT;

typedef struct MgWarsPlayer_s {
	s16 modelId;
	s16 motId[4];
	s16 unkA;
	s16 charNo;
	s16 padNo;
	s32 unk10;
	f32 unk14;
	s32 unk18;
} MGWARSPLAYER;

/* Minigame work (lbl_1_bss_8) */
typedef struct MgWarsWork_s {
	OMOBJMAN *objman;
	s16 unk4;
	s16 unk6;
	u8 unk8;
	u8 pad9;
	s16 unkA;
	s32 unkC;
	u32 unk10;
	u8 unk14[4];
	u8 unk18;
	u8 unk19;
	MGWARSGAME games[26];
	u8 padB6[2];
	ANIMDATA *unkB8[26];
	s32 gameCnt;
	s32 unk124;
	MGWARSPLAYER player[4];
	s16 unk198;
	u8 pad19A[0x3E];
	s16 models[75];
	s16 hexModel[19][5];
	s16 hexState[19];
	s16 unk352;
	s32 unk354;
	s16 unk358;
	f32 unk35C;
	s16 unk360;
	f32 unk364;
	s16 unk368;
	s16 unk36A;
	s32 unk36C;
	s32 unk370;
	s32 unk374;
	s32 unk378;
	s32 unk37C;
	s32 unk380;
	s32 unk384;
	s32 unk388;
	HUPROCESS *unk38C;
	ANIMDATA *unk390;
	f32 unk394;
	f32 unk398;
	f32 unk39C;
	f32 unk3A0;
	HuVecF unk3A4;
	HuVecF camTarget[4];
	HuVecF camPos[4];
} MGWARSWORK;

/* Common/minigame-manager work (mgModeWork) */

#define MDL(off) (lbl_1_bss_0->models[((off) - 0x1D8) / 2])

typedef struct MgWarsCom_s {
	u8 pad0[0x3];
	u8 unk3;
} MGWARSCOM;

typedef struct MgWarsObj1778_s {
	u8 pad0[0xA];
	s16 unkA;
	s16 unkC;
} MGWARSOBJ1778;

MGWARSWORK *lbl_1_bss_0;
MGWARSCOM *lbl_1_bss_4;
MGWARSWORK lbl_1_bss_8;

MGWARSDATAENT lbl_1_data_0[26] = {
	{ 0x000F, 0x01F5, 0 }, { 0x0010, 0x01F6, 0 }, { 0x0011, 0x01F7, 0 }, { 0x0012, 0x01F8, 0 },
	{ 0x004A, 0x0233, 0 }, { 0x004B, 0x0234, 0 }, { 0x004C, 0x0235, 0 }, { 0x0057, 0x0240, 0 },
	{ 0x0013, 0x01F9, 0 }, { 0x0014, 0x01FA, 0 }, { 0x0015, 0x01FB, 0 }, { 0x0017, 0x01FD, 0 },
	{ 0x0018, 0x01FE, 0 }, { 0x0059, 0x0243, 0 }, { 0x0019, 0x01FF, 0 }, { 0x001A, 0x0200, 0 },
	{ 0x001B, 0x0201, 0 }, { 0x001C, 0x0202, 0 }, { 0x001D, 0x0203, 0 }, { 0x004D, 0x0236, 0 },
	{ 0x0031, 0x0217, 0 }, { 0x0032, 0x0218, 0 }, { 0x0033, 0x0219, 0 }, { 0x0034, 0x021A, 0 },
	{ 0x0035, 0x021B, 0 }, { 0x0036, 0x021C, 0 },
};

u32 lbl_1_data_9C[5] = { 0x8C0000, 0x8C0006, 0x8C0007, 0x8C0001, 0 };

HuVecF lbl_1_data_B0[4] = {
    { -340.0f, 50.0f, -290.0f },
    { 340.0f, 50.0f, -290.0f },
    { -340.0f, 50.0f, 290.0f },
    { 340.0f, 50.0f, 290.0f },
};

HuVecF lbl_1_data_E0[18] = {
    { 0.0f, 120.0f, -290.0f },
    { -440.0f, 120.0f, -220.0f },
    { 240.0f, 120.0f, -220.0f },
    { -440.0f, 120.0f, 350.0f },
    { 240.0f, 120.0f, 350.0f },
    { -230.0f, 120.0f, -150.0f },
    { 180.0f, 120.0f, -150.0f },
    { -440.0f, 120.0f, 200.0f },
    { 130.0f, 120.0f, 380.0f },
    { -200.0f, 120.0f, -220.0f },
    { 200.0f, 120.0f, -220.0f },
    { -200.0f, 120.0f, 350.0f },
    { 200.0f, 120.0f, 350.0f },
    { 0.0f, 930.0f, 0.0f },
    { -440.0f, 930.0f, -220.0f },
    { 240.0f, 930.0f, -220.0f },
    { -440.0f, 930.0f, 350.0f },
    { 240.0f, 930.0f, 350.0f },
};

HuVecF lbl_1_data_1B8[4] = {
    { -340.0f, 0.0f, -290.0f },
    { 340.0f, 0.0f, -290.0f },
    { -340.0f, 0.0f, 290.0f },
    { 340.0f, 0.0f, 290.0f },
};

HuVecF lbl_1_data_1E8[19] = {
    { -160.0f, 0.0f, -280.0f },
    { 0.0f, 0.0f, -280.0f },
    { 160.0f, 0.0f, -280.0f },
    { -240.0f, 0.0f, -140.0f },
    { -80.0f, 0.0f, -140.0f },
    { 80.0f, 0.0f, -140.0f },
    { 240.0f, 0.0f, -140.0f },
    { -320.0f, 0.0f, 0.0f },
    { -160.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 160.0f, 0.0f, 0.0f },
    { 320.0f, 0.0f, 0.0f },
    { -240.0f, 0.0f, 140.0f },
    { -80.0f, 0.0f, 140.0f },
    { 80.0f, 0.0f, 140.0f },
    { 240.0f, 0.0f, 140.0f },
    { -160.0f, 0.0f, 280.0f },
    { 0.0f, 0.0f, 280.0f },
    { 160.0f, 0.0f, 280.0f },
};

HuVecF lbl_1_data_2CC = { 0.0f, 300.0f, -530.0f };

s8 lbl_1_data_2D8[19][10] = {
	{ -1, 1, 4, 3, -1, -1, -1, -1, 0, 0 },
	{ -1, 2, 5, 4, 0, -1, -1, -1, 0, 0 },
	{ -1, -1, 6, 5, 1, -1, -1, -1, 0, 0 },
	{ 0, 4, 8, 7, -1, -1, -1, -1, 0, 0 },
	{ 1, 5, 9, 8, 3, 0, -1, -1, 0, 0 },
	{ 2, 6, 10, 9, 4, 1, -1, -1, 0, 0 },
	{ -1, -1, 11, 10, 5, 2, -1, -1, 0, 0 },
	{ 3, 8, 12, -1, -1, -1, -1, -1, 0, 0 },
	{ 4, 9, 13, 12, 7, 3, -1, -1, 0, 0 },
	{ 5, 10, 14, 13, 8, 4, -1, -1, 0, 0 },
	{ 6, 11, 15, 14, 9, 5, -1, -1, 0, 0 },
	{ -1, -1, -1, 15, 10, 6, -1, -1, 0, 0 },
	{ 8, 13, 16, -1, -1, 7, -1, -1, 0, 0 },
	{ 9, 14, 17, 16, 12, 8, -1, -1, 0, 0 },
	{ 10, 15, 18, 17, 13, 9, -1, -1, 0, 0 },
	{ 11, -1, -1, 18, 14, 10, -1, -1, 0, 0 },
	{ 13, 17, -1, -1, -1, 12, -1, -1, 0, 0 },
	{ 14, 18, -1, -1, 16, 13, -1, -1, 0, 0 },
	{ 15, -1, -1, -1, 17, 14, -1, -1, 0, 0 },
};

HuVecF lbl_1_data_398[4] = {
    { -340.0f, 50.0f, 300.0f },
    { 340.0f, 50.0f, 300.0f },
    { -340.0f, 50.0f, 880.0f },
    { 340.0f, 50.0f, 880.0f },
};

f32 lbl_1_data_3C8[13] = { -5.0f, -5.0f, 0.0f, -10.0f, -20.0f, -5.0f, -10.0f, 0.0f, -15.0f, -40.0f, -40.0f, -40.0f, -40.0f };

s32 lbl_1_data_3FC[10][7] = {
	0x000001F8, 0x000001FB, 0x00000200, 0x000001F6, 0x00000200, 0x000001FA, 0x000001F8,
	0x000001E4, 0x000001E7, 0x000001EC, 0x000001E2, 0x000001EC, 0x000001E6, 0x000001E4,
	0x0000025C, 0x0000025F, 0x00000264, 0x0000025A, 0x00000264, 0x0000025E, 0x0000025C,
	0x000002AC, 0x000002AF, 0x000002B4, 0x000002AA, 0x000002B4, 0x000002AE, 0x000002AC,
	0x00000284, 0x00000287, 0x0000028C, 0x00000282, 0x0000028C, 0x00000286, 0x00000284,
	0x000001BC, 0x000001BF, 0x000001C4, 0x000001BA, 0x000001C4, 0x000001BE, 0x000001BC,
	0x00000298, 0x0000029B, 0x000002A0, 0x00000296, 0x000002A0, 0x0000029A, 0x00000298,
	0x000001D0, 0x000001D3, 0x000001D8, 0x000001CE, 0x000001D8, 0x000001D2, 0x000001D0,
	0x00000270, 0x00000273, 0x00000278, 0x0000026E, 0x00000278, 0x00000272, 0x00000270,
	0x00000234, 0x00000237, 0x0000023C, 0x00000232, 0x0000023C, 0x00000236, 0x00000234
};

int _prolog(void)
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

void fn_1_A0(u8 *arr, s32 n)
{
	u8 tmp[0x100];
	s32 i;
	s32 j;
	s32 k;

	i = n;
	while (i--) {
		tmp[i] = arr[i];
	}
	i = n;
	while (i--) {
		if (i != 0) {
			k = frandmod(i + 1);
		} else {
			k = 0;
		}
		arr[i] = tmp[k];
		for (j = 0; j <= i; j++) {
			if (j != k && j > k) {
				tmp[j - 1] = tmp[j];
			}
		}
	}
}

void fn_1_164(s32 *a, s32 *b, s32 n)
{
	s32 i;
	s32 j;
	s32 t;
	s32 t2;

	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			if (a[i] > a[j]) {
				t = a[i];
				a[i] = a[j];
				a[j] = t;
				if (b != 0) {
					t2 = b[i];
					b[i] = b[j];
					b[j] = t2;
				}
			}
		}
	}
}

s32 fn_1_21C(f32 *val, f32 target, f32 step)
{
	f32 rem = fmodf(*val - target, 180.0f);

	if (fabsf(rem) < step) {
		*val = target;
		return 1;
	}
	if (rem > 0.0f) {
		rem -= step;
	} else {
		rem += step;
	}
	*val = target + rem;
	return 0;
}

void fn_1_328(void)
{
	u32 start;
	s32 asyncId;

	lbl_1_bss_0->unk370 = 0;
	start = OSGetTick();
	asyncId = HuDataDirReadAsync(mgDataTbl[lbl_1_bss_0->unk374].dataDir);
	while (HuDataGetAsyncStat(asyncId) == 0) {
		HuPrcVSleep();
	}
	lbl_1_bss_0->unk370 = 1;
	OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick() - start));
	OSReport("Finished!\n");
	HuPrcEnd();
	while (1) {
		HuPrcVSleep();
	}
}

void fn_1_404(s32 arg0)
{
	lbl_1_bss_0->unk374 = arg0;
	HuDataDirClose(0x9B0000);
	HuDataDirClose(0x270000);
	CharModelDataClose(-1);
	if (fn_1_684(4) == 0) {
		HuPrcChildCreate(fn_1_328, 0xc8, 0x3000, 0, HuPrcCurrentGet());
	} else {
		lbl_1_bss_0->unk370 = 1;
	}
}

void fn_1_498(void)
{
	fn_1_C04(lbl_1_bss_0->unk360, (s32)lbl_1_bss_0->unk364, lbl_1_bss_0->unk368);
	lbl_1_bss_0->unk360 = -1;
	HuPrcEnd();
}

s16 fn_1_50C(void)
{
	return lbl_1_bss_0->unk6;
}

void fn_1_520(void)
{
	if (fn_1_684(0x40) == 0) {
		fn_1_64C(0x40);
		HuWinExOpen(lbl_1_bss_0->unk6);
	}
}

void fn_1_568(void)
{
	if (fn_1_684(0x40)) {
		fn_1_668(0x40);
		HuWinExClose(lbl_1_bss_0->unk6);
		HuWinHomeClear(lbl_1_bss_0->unk6);
	}
}

u8 fn_1_5C4(void)
{
	return lbl_1_bss_0->unk8;
}

s16 fn_1_5D8(void)
{
	return lbl_1_bss_0->unkA;
}

u8 fn_1_5EC(void *dst)
{
	memcpy(dst, lbl_1_bss_0->unk14, 4);
	return lbl_1_bss_0->unk18;
}

u8 fn_1_638(void)
{
	return lbl_1_bss_0->unk19;
}

void fn_1_64C(u32 mask)
{
	lbl_1_bss_0->unk10 |= mask;
}

void fn_1_668(u32 mask)
{
	lbl_1_bss_0->unk10 &= ~mask;
}

u32 fn_1_684(u32 mask)
{
	return lbl_1_bss_0->unk10 & mask;
}

void fn_1_69C(u32 mask)
{
	lbl_1_bss_4->unk3 |= mask;
}

void fn_1_6BC(u32 mask)
{
	lbl_1_bss_4->unk3 &= ~mask;
}

u32 fn_1_6DC(u32 mask)
{
	return lbl_1_bss_4->unk3 & mask;
}

void fn_1_6F4(u32 mask)
{
	lbl_1_bss_0->unk10 &= ~0x10000;
	lbl_1_bss_0->unkC |= mask;
}

void fn_1_728(u32 mask)
{
	while ((u32)lbl_1_bss_0->unk10 & 0x10000) {
		HuPrcVSleep();
	}
	lbl_1_bss_0->unk10 |= 0x10000;
	lbl_1_bss_0->unkC &= ~mask;
}

u32 fn_1_7A0(u32 mask)
{
	return lbl_1_bss_0->unkC & mask;
}

void ObjectSetup(void)
{
	_SetFlag(0xb);
	HuPrcChildCreate(fn_1_8C8, 0xc8, 0x3000, 0, HuPrcCurrentGet());
}

void fn_1_7FC(void (*func)(void))
{
	HuPrcChildCreate(func, 0x64, 0x1000, 0, HuPrcCurrentGet());
	HuPrcVSleep();
}

void fn_1_83C(s16 a, f32 b, s32 c)
{
	lbl_1_bss_0->unk360 = a;
	lbl_1_bss_0->unk364 = b;
	lbl_1_bss_0->unk368 = (s16)c;
	HuPrcChildCreate(fn_1_498, 0x64, 0x1000, 0, HuPrcCurrentGet());
	HuPrcVSleep();
}

void fn_1_8C8(void)
{
	s32 cont;
	s32 inited;
	s32 running;
	OMOVLHIS *hist;

	hist = omOvlHisGet(0);
	fn_1_728C();
	running = 1;
	inited = 0;
	while (running) {
		cont = 1;
		switch (hist->evtno) {
		case 0:
			fn_1_7E30();
			inited = 0;
			break;
		case 1:
			fn_1_8DF0();
			if ((lbl_1_bss_0->unk10 & 8) == 0) {
				fn_1_9BC8();
				if (lbl_1_bss_4->unk3 & 4U) {
					cont = 0;
				}
			}
			break;
		case 2:
			fn_1_C620();
			cont = 0;
			break;
		case 3:
			cont = 0;
			running = 0;
			break;
		}
		if (inited == 0) {
			fn_1_796C();
			inited = 1;
		}
		if (cont) {
			fn_1_B614();
			fn_1_C19C();
		}
	}
	fn_1_7CC8();
	omOvlReturnEx(1, 1);
	HuPrcEnd();
}

void fn_1_9F4(s32 frames)
{
	while (frames--) {
		HuPrcVSleep();
	}
}

void fn_1_A30(s32 a, s32 b)
{
	WipeCreate(1, a, b);
	while (WipeCheckEnd()) {
		HuPrcVSleep();
	}
}

void fn_1_A84(s32 a, s32 b)
{
	WipeCreate(2, a, b);
	while (WipeCheckEnd()) {
		HuPrcVSleep();
	}
}

void fn_1_AD8(s32 modelId, f32 start, f32 end)
{
	Hu3DMotionStartEndSet(modelId, start, end);
	Hu3DMotionTimeSet(modelId, start);
}

s16 fn_1_B34(s32 arg0, s32 arg1)
{
	Hu3DModelAttrSet(lbl_1_bss_0->models[arg0 + 6], 1);
	lbl_1_bss_0->models[arg0 + 6] = lbl_1_bss_0->hexModel[arg0][arg1];
	Hu3DModelAttrReset(lbl_1_bss_0->models[arg0 + 6], 1);
	return lbl_1_bss_0->models[arg0 + 6];
}

s32 fn_1_C04(s32 camNo, s32 maxTime, s32 mode)
{
	HSFMOTION *motion;
	HU3DMODEL *model;
	HU3DMOTION *hu3dMot;
	HSFDATA *hsf;

	if (lbl_1_bss_0->unk4 == camNo) {
		return 0;
	}
	lbl_1_bss_0->unk360 = camNo;
	if (maxTime != 0) {
		CamMotionEx2(lbl_1_bss_0->models[camNo], 2, (f32)maxTime, mode);
	} else {
		model = &Hu3DData[lbl_1_bss_0->models[camNo]];
		hu3dMot = &Hu3DMotion[model->motId];
		hsf = hu3dMot->hsf;
		motion = hsf->motion;
		if (motion->maxTime < 9.0f) {
			CamMotionEx2(lbl_1_bss_0->models[camNo], 2, motion->maxTime, mode);
		} else {
			CamMotionEx2(lbl_1_bss_0->models[camNo], 2, 1.0f, mode);
		}
	}
	lbl_1_bss_0->unk360 = -1;
	lbl_1_bss_0->unk4 = camNo;
	return 1;
}

void fn_1_DA8(void)
{
	HuVecF target;
	HuVecF up;
	HuVecF pos;

	Hu3DCameraPosGet(2, &pos, &up, &target);
	pos.z -= 40.0f;
	Hu3DModelPosSetV(MDL(0x24E), &pos);
	while (lbl_1_bss_0->unk10 & 0x100) {
		if (lbl_1_bss_0->unk360 >= 0) {
			Hu3DCameraPosGet(2, &pos, &up, &target);
			pos.z -= 40.0f;
			Hu3DModelPosSetV(MDL(0x24E), &pos);
		}
		HuPrcVSleep();
	}
	HuPrcEnd();
}

void fn_1_E8C(void)
{
	lbl_1_bss_0->unk10 |= 0x100;
	Hu3DModelAttrReset(MDL(0x24E), 0x40000002);
	Hu3DModelAttrReset(MDL(0x24E), 1);
	HuPrcChildCreate(fn_1_DA8, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_F14(void)
{
	s32 i;
	s32 j;

	i = 0x78;
	while (i--) {
		HuPrcVSleep();
	}
	HuAudJinglePlay(0x86);
	j = 0x96;
	while (j--) {
		HuPrcVSleep();
	}
	lbl_1_bss_0->unk378 = HuAudBGMPlay(0x52);
	HuPrcEnd();
}

void fn_1_F94(void)
{
	HuPrcChildCreate(fn_1_F14, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_FD0(void)
{
	s32 i;
	s32 j;

	i = 0x78;
	while (i--) {
		HuPrcVSleep();
	}
	HuAudJinglePlay(0x80);
	j = 0x96;
	while (j--) {
		HuPrcVSleep();
	}
	lbl_1_bss_0->unk378 = HuAudBGMPlay(0x52);
	HuPrcEnd();
}

void fn_1_1050(void)
{
	HuPrcChildCreate(fn_1_FD0, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_108C(void)
{
	s32 i;

	HuAudSStreamPauseFadeOut(lbl_1_bss_0->unk378, 1, 0x64);
	HuAudJinglePlay(0x88);
	i = 0x118;
	while (i--) {
		HuPrcVSleep();
	}
	HuAudSStreamPauseFadeOut(lbl_1_bss_0->unk378, 0, 0x3e8);
	HuPrcEnd();
}

void fn_1_110C(void)
{
	HuPrcChildCreate(fn_1_108C, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_1148(void)
{
	s32 i;

	HuAudJinglePlay(0x81);
	i = 0x12c;
	while (i--) {
		HuPrcVSleep();
	}
	HuAudSStreamPauseFadeOut(lbl_1_bss_0->unk378, 0, 0x3e8);
	HuPrcEnd();
}

void fn_1_11AC(void)
{
	HuPrcChildCreate(fn_1_1148, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_11E8(s16 layerNo)
{
	if (Hu3DCameraBit == 1) {
		Hu3DFbCopyExec((s16)lbl_1_bss_0->unk394, (s16)lbl_1_bss_0->unk398,
			(s16)lbl_1_bss_0->unk39C, (s16)lbl_1_bss_0->unk3A0, 5, 0,
			lbl_1_bss_0->unk390->bmp->data);
	}
}

void fn_1_12AC(s32 camNo)
{
	s32 i;

	Hu3DCameraPosSet(1, lbl_1_bss_0->camPos[camNo].x, lbl_1_bss_0->camPos[camNo].y,
		lbl_1_bss_0->camPos[camNo].z, 0.0f, 1.0f, 0.0f, lbl_1_bss_0->camTarget[camNo].x,
		lbl_1_bss_0->camTarget[camNo].y, lbl_1_bss_0->camTarget[camNo].z);
	i = 4;
	while (i--) {
		if (i == camNo) {
			Hu3DModelCameraSet(lbl_1_bss_0->player[i].modelId, 3);
		} else {
			Hu3DModelCameraSet(lbl_1_bss_0->player[i].modelId, 2);
		}
	}
}

void fn_1_13FC(void)
{
	s32 i;

	Hu3DCameraCreate(1);
	Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 6000.0f, 1.2f);
	Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
	Hu3DCameraScissorSet(1, 0, 0, 0x280, 0x1e0);
	lbl_1_bss_0->unk394 = 192.0f;
	lbl_1_bss_0->unk398 = 100.0f;
	lbl_1_bss_0->unk39C = 256.0f;
	lbl_1_bss_0->unk3A0 = 256.0f;
	Hu3DZClearLayerSet(0xF);
	if (lbl_1_bss_0->unk390 == 0) {
		lbl_1_bss_0->unk390 = HuSprAnimMake(0x100, 0x100, 2);
		lbl_1_bss_0->unk390->bmp->data = HuMemDirectMallocNum(HEAP_MODEL, lbl_1_bss_0->unk390->bmp->dataSize, 0x10000000);
		memset(lbl_1_bss_0->unk390->bmp->data, 0, lbl_1_bss_0->unk390->bmp->dataSize);
	}
	Hu3DLayerHookSet(6, fn_1_11E8);
	Hu3DCameraPosSet(1, lbl_1_bss_0->camPos[0].x, lbl_1_bss_0->camPos[0].y,
		lbl_1_bss_0->camPos[0].z, 0.0f, 1.0f, 0.0f, lbl_1_bss_0->camTarget[0].x,
		lbl_1_bss_0->camTarget[0].y, lbl_1_bss_0->camTarget[0].z);
	i = 4;
	while (i--) {
		if (i == 0) {
			Hu3DModelCameraSet(lbl_1_bss_0->player[i].modelId, 3);
		} else {
			Hu3DModelCameraSet(lbl_1_bss_0->player[i].modelId, 2);
		}
	}
	HuPrcVSleep();
	Hu3DModelAttrReset(MDL(0x24A), 1);
	if (lbl_1_bss_0->unk124 >= 0) {
		Hu3DAnimKill(lbl_1_bss_0->unk124);
	}
	lbl_1_bss_0->unk124 = Hu3DAnimCreate(lbl_1_bss_0->unk390, MDL(0x24A), "mg_shot");
}

void fn_1_1778(MGWARSOBJ1778 *obj, s32 arg)
{
	switch (obj->unkA) {
	case 0:
	case 2:
		HuAudFXPlayPan(lbl_1_data_3FC[obj->unkC][arg], 0x28);
		break;
	case 1:
	case 3:
		HuAudFXPlayPan(lbl_1_data_3FC[obj->unkC][arg], 0x58);
		break;
	}
}

void fn_1_182C(void)
{
	while (lbl_1_bss_0->unk388-- != 0) {
		HuPrcVSleep();
	}
	HuAudFXStop(lbl_1_bss_0->unk384);
	lbl_1_bss_0->unk384 = -1;
	HuPrcEnd();
}

void fn_1_189C(s32 arg0, s32 arg1)
{
	if (lbl_1_bss_0->unk384 >= 0) {
		HuAudFXStop(lbl_1_bss_0->unk384);
		HuPrcKill(lbl_1_bss_0->unk38C);
	}
	lbl_1_bss_0->unk384 = HuAudFXPlay(arg0);
	lbl_1_bss_0->unk388 = arg1;
	lbl_1_bss_0->unk38C = HuPrcChildCreate(fn_1_182C, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_195C(void)
{
	Hu3DCameraKill(1);
	Hu3DLayerHookSet(0xF, NULL);
	Hu3DLayerHookSet(6, NULL);
	Hu3DModelAttrSet(MDL(0x24A), 1);
	if (lbl_1_bss_0->unkB8[lbl_1_bss_0->gameCnt / 2] != 0) {
		if (lbl_1_bss_0->unk124 >= 0) {
			Hu3DAnimKill(lbl_1_bss_0->unk124);
		}
		lbl_1_bss_0->unk124 = Hu3DAnimCreate(lbl_1_bss_0->unkB8[lbl_1_bss_0->gameCnt / 2], MDL(0x24A), "mg_shot");
	}
}

void fn_1_1A80(void)
{
	s16 model;
	s32 i;
	s32 j;

	MDL(0x1D8) = Hu3DModelCreateData(0x9B0002);
	MDL(0x1DA) = Hu3DModelCreateData(0x9B0003);
	MDL(0x1DC) = Hu3DModelCreateData(0x9B0004);
	MDL(0x1DE) = Hu3DModelCreateData(0x9B0005);
	MDL(0x240) = Hu3DModelCreateData(0x9B000B);
	MDL(0x242) = Hu3DModelCreateData(0x9B000C);
	MDL(0x244) = Hu3DModelCreateData(0x9B000D);
	MDL(0x246) = Hu3DModelCreateData(0x9B000E);
	MDL(0x248) = Hu3DModelCreateData(0x9B000F);
	MDL(0x24A) = Hu3DModelCreateData(0x9B000A);
	MDL(0x258) = Hu3DModelCreateData(0x9B0017);
	MDL(0x24E) = Hu3DModelCreateData(0x9B0012);
	MDL(0x250) = Hu3DModelCreateData(0x9B0013);
	MDL(0x252) = Hu3DModelCreateData(0x9B0014);
	MDL(0x254) = Hu3DModelCreateData(0x9B0015);
	MDL(0x256) = Hu3DModelCreateData(0x9B0016);
	MDL(0x1E2) = Hu3DModelCreateData(0x9B0008);
	MDL(0x230) = Hu3DModelCreateData(0x9B001D);
	MDL(0x232) = Hu3DModelCreateData(0x9B001E);
	MDL(0x234) = Hu3DModelCreateData(0x9B001F);
	MDL(0x236) = Hu3DModelCreateData(0x9B0020);
	switch (GwCommon.languageNo) {
	case 0:
		MDL(0x24C) = Hu3DModelCreateData(0x9B0010);
		MDL(0x1E0) = Hu3DModelCreateData(0x9B0006);
		MDL(0x25A) = Hu3DModelCreateData(0x9B0007);
		break;
	case 1:
		MDL(0x24C) = Hu3DModelCreateData(0x9B003D);
		MDL(0x1E0) = Hu3DModelCreateData(0x9B003E);
		MDL(0x25A) = Hu3DModelCreateData(0x9B003F);
		break;
	}
	Hu3DModelLayerSet(MDL(0x1D8), 0);
	Hu3DModelLayerSet(MDL(0x1DA), 0);
	Hu3DModelLayerSet(MDL(0x1DC), 2);
	Hu3DModelLayerSet(MDL(0x1DE), 0);
	Hu3DModelLayerSet(MDL(0x1E0), 0);
	Hu3DModelLayerSet(MDL(0x1E2), 2);
	Hu3DModelLayerSet(MDL(0x24A), 0);
	Hu3DModelLayerSet(MDL(0x24C), 0);
	Hu3DModelLayerSet(MDL(0x258), 2);
	Hu3DModelLayerSet(MDL(0x25A), 0);
	Hu3DModelLayerSet(MDL(0x24E), 1);
	Hu3DModelLayerSet(MDL(0x250), 7);
	Hu3DModelLayerSet(MDL(0x252), 4);
	Hu3DModelLayerSet(MDL(0x254), 4);
	Hu3DModelLayerSet(MDL(0x256), 4);
	Hu3DModelLayerSet(MDL(0x230), 2);
	Hu3DModelLayerSet(MDL(0x232), 2);
	Hu3DModelLayerSet(MDL(0x234), 2);
	Hu3DModelLayerSet(MDL(0x236), 2);
	Hu3DModelLayerSet(MDL(0x248), 3);
	Hu3DModelLightInfoSet(MDL(0x1D8), 1);
	lbl_1_bss_0->unk3A4.x = lbl_1_data_1E8[9].x;
	lbl_1_bss_0->unk3A4.y = lbl_1_data_1E8[9].y;
	lbl_1_bss_0->unk3A4.z = lbl_1_data_1E8[9].z;
	Hu3DModelPosSet(MDL(0x24A), lbl_1_data_2CC.x, lbl_1_data_2CC.y, lbl_1_data_2CC.z);
	Hu3DModelPosSet(MDL(0x248), lbl_1_bss_0->unk3A4.x, 20.0f + lbl_1_bss_0->unk3A4.y, lbl_1_bss_0->unk3A4.z);
	Hu3DModelAttrSet(MDL(0x1D8), 0x40000001);
	Hu3DModelAttrSet(MDL(0x1DA), 0x40000001);
	Hu3DModelAttrSet(MDL(0x1DC), 0x40000001);
	Hu3DModelAttrSet(MDL(0x1DE), 0x40000001);
	Hu3DModelAttrSet(MDL(0x1E0), 0x40000003);
	Hu3DModelAttrSet(MDL(0x1E0), 1);
	Hu3DModelAttrSet(MDL(0x1E2), 0x40000002);
	Hu3DModelAttrSet(MDL(0x1E2), 1);
	Hu3DModelAttrSet(MDL(0x248), 0x40000001);
	Hu3DModelAttrSet(MDL(0x248), 1);
	Hu3DModelAttrSet(MDL(0x24A), 0x40000001);
	Hu3DModelAttrSet(MDL(0x24A), 1);
	Hu3DModelAttrSet(MDL(0x24C), 0x40000002);
	Hu3DModelAttrSet(MDL(0x24C), 1);
	Hu3DModelAttrSet(MDL(0x258), 0x40000002);
	Hu3DModelAttrSet(MDL(0x258), 1);
	Hu3DModelAttrSet(MDL(0x25A), 0x40000002);
	Hu3DModelAttrSet(MDL(0x25A), 1);
	Hu3DModelAttrSet(MDL(0x24E), 0x40000002);
	Hu3DModelAttrSet(MDL(0x24E), 1);
	Hu3DModelAttrSet(MDL(0x250), 0x40000001);
	Hu3DModelAttrSet(MDL(0x250), 1);
	Hu3DModelAttrSet(MDL(0x252), 0x40000002);
	Hu3DModelAttrSet(MDL(0x252), 1);
	Hu3DModelAttrSet(MDL(0x254), 0x40000002);
	Hu3DModelAttrSet(MDL(0x254), 1);
	Hu3DModelAttrSet(MDL(0x256), 0x40000002);
	Hu3DModelAttrSet(MDL(0x256), 1);
	Hu3DModelAttrSet(MDL(0x230), 0x40000003);
	Hu3DModelAttrSet(MDL(0x232), 0x40000003);
	Hu3DModelAttrSet(MDL(0x234), 0x40000003);
	Hu3DModelAttrSet(MDL(0x236), 0x40000003);
	model = MDL(0x1DE);
	Hu3DMotionStartEndSet(model, 0.0f, 180.0f);
	Hu3DMotionTimeSet(model, 0.0f);
	MDL(0x25C) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0021), 2);
	MDL(0x25E) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0022), 2);
	MDL(0x260) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0023), 2);
	MDL(0x262) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0024), 2);
	MDL(0x264) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0025), 2);
	MDL(0x266) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0026), 2);
	MDL(0x268) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0027), 2);
	MDL(0x26A) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0028), 2);
	MDL(0x26C) = Hu3DModelCameraCreate(Hu3DMotionCreateData(0x9B0029), 2);
	Hu3DModelAttrSet(MDL(0x25C), 0x40000002);
	Hu3DModelAttrSet(MDL(0x25E), 0x40000002);
	Hu3DModelAttrSet(MDL(0x260), 0x40000002);
	Hu3DModelAttrSet(MDL(0x262), 0x40000002);
	Hu3DModelAttrSet(MDL(0x264), 0x40000002);
	Hu3DModelAttrSet(MDL(0x266), 0x40000002);
	Hu3DModelAttrSet(MDL(0x268), 0x40000002);
	Hu3DModelAttrSet(MDL(0x26A), 0x40000002);
	Hu3DModelAttrSet(MDL(0x26C), 0x40000002);
	Hu3DCameraMotionOff(MDL(0x25C));
	Hu3DCameraMotionOff(MDL(0x25E));
	Hu3DCameraMotionOff(MDL(0x260));
	Hu3DCameraMotionOff(MDL(0x262));
	Hu3DCameraMotionOff(MDL(0x264));
	Hu3DCameraMotionOff(MDL(0x266));
	Hu3DCameraMotionOff(MDL(0x268));
	Hu3DCameraMotionOff(MDL(0x26A));
	Hu3DCameraMotionOff(MDL(0x26C));
	fn_1_C04(0x42, 0, 2);
	i = 4;
	while (i--) {
		model = lbl_1_bss_0->models[i + 0x34];
		Hu3DModelPosSet(model, lbl_1_bss_0->unk3A4.x, lbl_1_bss_0->unk3A4.y, lbl_1_bss_0->unk3A4.z);
		Hu3DModelLayerSet(model, 1);
		Hu3DModelAttrSet(model, 0x40000001);
		Hu3DModelAttrSet(model, 1);
	}
	for (i = 0; i < 0x13; i++) {
		lbl_1_bss_0->hexModel[i][0] = Hu3DModelCreateData(0x9B0019);
		lbl_1_bss_0->hexModel[i][1] = Hu3DModelCreateData(0x9B001A);
		lbl_1_bss_0->hexModel[i][2] = Hu3DModelCreateData(0x9B001B);
		lbl_1_bss_0->hexModel[i][3] = Hu3DModelCreateData(0x9B001C);
		lbl_1_bss_0->hexModel[i][4] = Hu3DModelCreateData(0x9B0018);
		lbl_1_bss_0->models[i + 6] = lbl_1_bss_0->hexModel[i][0];
		j = 5;
		while (j--) {
			model = lbl_1_bss_0->hexModel[i][j];
			Hu3DModelPosSet(model, lbl_1_data_1E8[i].x, lbl_1_data_1E8[i].y, lbl_1_data_1E8[i].z);
			Hu3DModelLayerSet(model, 2);
			Hu3DModelAttrSet(model, 1);
			Hu3DModelAttrSet(model, 0x40000001);
		}
		Hu3DModelAttrReset(lbl_1_bss_0->models[i + 6], 1);
	}
	for (i = 0; i < 0x13; i++) {
		model = Hu3DModelCreateData(0x9B0011);
		lbl_1_bss_0->models[i + 0x19] = model;
		Hu3DModelPosSet(model, lbl_1_data_1E8[i].x, lbl_1_data_1E8[i].y, lbl_1_data_1E8[i].z);
		Hu3DModelLayerSet(model, 2);
		Hu3DModelAttrSet(model, 1);
		Hu3DModelAttrSet(model, 0x40000001);
	}
	for (i = 0; i < 4; i++) {
		model = Hu3DModelCreateData(0x9B0009);
		lbl_1_bss_0->models[i + 0x30] = model;
		Hu3DModelPosSet(model, lbl_1_data_1B8[i].x, lbl_1_data_1B8[i].y, lbl_1_data_1B8[i].z);
		Hu3DModelLayerSet(model, 4);
		Hu3DModelAttrSet(model, 1);
		Hu3DModelAttrSet(model, 0x40000001);
	}
	i = 0x4B;
	while (i--) {
		if (i < 4) {
			if (i < 2) {
				Hu3DModelCameraSet(lbl_1_bss_0->models[i], 2);
			} else {
				Hu3DModelCameraSet(lbl_1_bss_0->models[i], 3);
			}
		} else {
			Hu3DModelCameraSet(lbl_1_bss_0->models[i], 2);
		}
	}
}
