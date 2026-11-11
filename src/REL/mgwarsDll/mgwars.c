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

#ifdef TARGET_PC
#define PC_INIT(v) = v
#else
#define PC_INIT(v)
#endif

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
void fn_1_57C0(s16, s32);
void fn_1_F068(void);
void fn_1_E5E8(void);
void fn_1_E674(void);
void fn_1_E6FC(void);
void fn_1_E784(void);
void fn_1_E87C(void);
void fn_1_E908(void);
void fn_1_E9A0(void);
void fn_1_E9FC(void);
void fn_1_EA84(void);
void fn_1_EB5C(void);
void fn_1_EBB8(void);
void fn_1_EC1C(void);
void fn_1_ECA0(void);
void fn_1_EED8(void);
void fn_1_E2D0(void);
void fn_1_404(s32);
void fn_1_8D9C(s32);
void fn_1_A84(s32, s32);
void fn_1_E36C(void);
void fn_1_E3C8(void);
void fn_1_E450(void);
void fn_1_E4D8(void);
void fn_1_E560(void);
void fn_1_5EB0(HuVecF *, f32);
void fn_1_4958(void);
void fn_1_6F54(void);
void fn_1_568(void);
void fn_1_189C(s32, s32);
void fn_1_A30(s32, s32);
void fn_1_9F4(s32);
void fn_1_7FC(void (*)(void));
void fn_1_83C(s16, f32, s32);
void fn_1_F848(int, int);
void fn_1_F808(int);
void fn_1_F828(int);
void fn_1_F860(void);
void fn_1_F884(void);

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

typedef struct MgWarsHex_s {
	s8 nb[6];
	s8 owner;
	s8 mark;
	s8 reach;
	s8 visited;
} MGWARSHEX;

typedef struct MgWarsPlayer_s {
	s16 modelId;
	s16 motId[4];
	s16 unkA;
	s16 charNo;
	s16 padNo;
	s8 state;
	u8 pad11[3];
	f32 angle;
	f32 targetAngle;
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
	s16 unk19A;
	HuVecF unk19C;
	HuVecF unk1A8;
	f32 unk1B4;
	s32 unk1B8;
	HuVecF unk1BC;
	HuVecF unk1C8;
	s32 unk1D4;
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
	s16 unk36C;
	u8 pad36E[2];
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
	s16 unk0;
	s8 unk2;
	u8 unk3;
	s8 list[26];
	s8 owner[19];
} MGWARSCOM;

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

unsigned int lbl_1_data_9C[5] = { 0x8C0000, 0x8C0006, 0x8C0007, 0x8C0001, 0 };

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

MGWARSHEX lbl_1_data_2D8[19] = {
	{ { -1, 1, 4, 3, -1, -1 }, -1, -1, 0, 0 },
	{ { -1, 2, 5, 4, 0, -1 }, -1, -1, 0, 0 },
	{ { -1, -1, 6, 5, 1, -1 }, -1, -1, 0, 0 },
	{ { 0, 4, 8, 7, -1, -1 }, -1, -1, 0, 0 },
	{ { 1, 5, 9, 8, 3, 0 }, -1, -1, 0, 0 },
	{ { 2, 6, 10, 9, 4, 1 }, -1, -1, 0, 0 },
	{ { -1, -1, 11, 10, 5, 2 }, -1, -1, 0, 0 },
	{ { 3, 8, 12, -1, -1, -1 }, -1, -1, 0, 0 },
	{ { 4, 9, 13, 12, 7, 3 }, -1, -1, 0, 0 },
	{ { 5, 10, 14, 13, 8, 4 }, -1, -1, 0, 0 },
	{ { 6, 11, 15, 14, 9, 5 }, -1, -1, 0, 0 },
	{ { -1, -1, -1, 15, 10, 6 }, -1, -1, 0, 0 },
	{ { 8, 13, 16, -1, -1, 7 }, -1, -1, 0, 0 },
	{ { 9, 14, 17, 16, 12, 8 }, -1, -1, 0, 0 },
	{ { 10, 15, 18, 17, 13, 9 }, -1, -1, 0, 0 },
	{ { 11, -1, -1, 18, 14, 10 }, -1, -1, 0, 0 },
	{ { 13, 17, -1, -1, -1, 12 }, -1, -1, 0, 0 },
	{ { 14, 18, -1, -1, 16, 13 }, -1, -1, 0, 0 },
	{ { 15, -1, -1, -1, 17, 14 }, -1, -1, 0, 0 },
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

void fn_1_1778(MGWARSPLAYER *obj, s32 arg)
{
	switch (obj->unkA) {
	case 0:
	case 2:
		HuAudFXPlayPan(lbl_1_data_3FC[obj->charNo][arg], 0x28);
		break;
	case 1:
	case 3:
		HuAudFXPlayPan(lbl_1_data_3FC[obj->charNo][arg], 0x58);
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

s32 fn_1_2F28(s32 hex, s32 target, s32 dir)
{
	s32 next;
	s32 ret;

	if (hex < 0) {
		return 0;
	}
	if (lbl_1_data_2D8[hex].owner < 0) {
		return 0;
	}
	if (target == lbl_1_data_2D8[hex].owner) {
		return 1;
	}
	next = lbl_1_data_2D8[hex].nb[dir];
	ret = fn_1_2F28(next, target, dir);
	if (ret != 0) {
		lbl_1_data_2D8[hex].mark = target;
		return ret + 1;
	}
	return 0;
}

s32 fn_1_3458(s32 hex, s32 target)
{
	s32 total;
	s32 i;
	s32 ret;

	lbl_1_data_2D8[hex].mark = (s8)target;
	total = 0;
	i = 6;
	while (i--) {
		ret = fn_1_2F28(lbl_1_data_2D8[hex].nb[i], target, i);
		if (ret != 0) {
			total += ret - 1;
		}
	}
	return total;
}

s32 fn_1_34FC(s32 hex)
{
	if (hex < 0) {
		return 0;
	}
	if (lbl_1_data_2D8[hex].owner >= 0) {
		return 1;
	}
	lbl_1_data_2D8[hex].reach = 1;
	return 0;
}

void fn_1_3554(s32 hex)
{
	s32 n0;
	s32 n1;
	s32 n2;
	s32 n3;
	s32 n4;
	s32 n5;

	if (hex < 0 || lbl_1_data_2D8[hex].visited != 0) {
		return;
	}
	lbl_1_data_2D8[hex].visited = 1;
	n0 = lbl_1_data_2D8[hex].nb[0];
	if (n0 >= 0 && lbl_1_data_2D8[n0].owner < 0) {
		lbl_1_data_2D8[n0].reach = 1;
	}
	n1 = lbl_1_data_2D8[hex].nb[1];
	if (n1 >= 0 && lbl_1_data_2D8[n1].owner < 0) {
		lbl_1_data_2D8[n1].reach = 1;
	}
	n2 = lbl_1_data_2D8[hex].nb[2];
	if (n2 >= 0 && lbl_1_data_2D8[n2].owner < 0) {
		lbl_1_data_2D8[n2].reach = 1;
	}
	n3 = lbl_1_data_2D8[hex].nb[3];
	if (n3 >= 0 && lbl_1_data_2D8[n3].owner < 0) {
		lbl_1_data_2D8[n3].reach = 1;
	}
	n4 = lbl_1_data_2D8[hex].nb[4];
	if (n4 >= 0 && lbl_1_data_2D8[n4].owner < 0) {
		lbl_1_data_2D8[n4].reach = 1;
	}
	n5 = lbl_1_data_2D8[hex].nb[5];
	if (n5 >= 0 && lbl_1_data_2D8[n5].owner < 0) {
		lbl_1_data_2D8[n5].reach = 1;
	}
}

void fn_1_37D4(void)
{
	s32 i;

	i = 0x13;
	while (i--) {
		lbl_1_data_2D8[i].visited = 0;
		lbl_1_data_2D8[i].reach = 0;
	}
	if (lbl_1_data_2D8[9].owner < 0) {
		lbl_1_data_2D8[9].reach = 1;
	}
	i = 0x13;
	while (i--) {
		if (lbl_1_data_2D8[i].owner >= 0) {
			fn_1_3554(i);
		}
	}
}

void fn_1_3AF0(s32 hex)
{
	HuVecF scaled;
	HuVecF delta;
	HuVecF pos;
	s32 pan;
	s16 model;
	s32 i;
	f32 t;

	model = lbl_1_bss_0->unk352;
	PSVECSubtract(&lbl_1_data_1E8[hex], &lbl_1_bss_0->unk3A4, &delta);
	pan = (s16)(64.0f + 24.0f * (lbl_1_data_1E8[hex].x / 320.0f));
	HuAudFXPlayPan(0x4BD, pan);
	for (i = 0; i < 0x14; i++) {
		t = (f32)i / 20.0f;
		t = sin(3.141592653589793 * (f64)t * 0.5);
		t = sin(3.141592653589793 * (f64)t * 0.5);
		t = sin(3.141592653589793 * (f64)t * 0.5);
		PSVECScale(&delta, &scaled, t);
		PSVECAdd(&lbl_1_bss_0->unk3A4, &scaled, &pos);
		Hu3DModelPosSet(model, pos.x, pos.y, pos.z);
		HuPrcVSleep();
	}
	PSVECAdd(&lbl_1_bss_0->unk3A4, &delta, &lbl_1_bss_0->unk3A4);
	Hu3DModelPosSet(model, lbl_1_bss_0->unk3A4.x, lbl_1_bss_0->unk3A4.y, lbl_1_bss_0->unk3A4.z);
}

void fn_1_3D38(s32 from, s32 to)
{
	HuVecF diff;
	MGWARSHEX *hex;
	f32 bestDist PC_INIT(0.0f);
	f32 dist;
	s32 found;
	s32 best PC_INIT(0);
	s32 n;
	s8 i;

	hex = &lbl_1_data_2D8[from];
	if (from == to) {
		hex->visited = 1;
		return;
	}
	found = 0;
	i = 6;
	while (i--) {
		n = hex->nb[i];
		if (n >= 0) {
			PSVECSubtract(&lbl_1_data_1E8[n], &lbl_1_data_1E8[to], &diff);
			dist = PSVECSquareMag(&diff);
			if (found == 0 || dist < bestDist) {
				bestDist = dist;
				best = n;
				found = 1;
			}
		}
	}
	fn_1_3D38(best, to);
	hex->visited = 1;
}

void fn_1_4170(s32 target)
{
	s32 i;
	MGWARSHEX *hex;
	s32 cur;
	s16 model;

	model = MDL(0x248);
	if (target != 9) {
		i = 0x13;
		while (i--) {
			lbl_1_data_2D8[i].visited = 0;
		}
		Hu3DModelAttrSet(model, 1);
		fn_1_3D38(9, target);
		cur = 9;
		while (cur != target) {
			hex = &lbl_1_data_2D8[cur];
			hex->visited = 0;
			i = 6;
			while (i--) {
				if (hex->nb[i] >= 0 && lbl_1_data_2D8[hex->nb[i]].visited != 0) {
					cur = hex->nb[i];
					break;
				}
			}
			fn_1_3AF0(cur);
		}
		HuAudFXPlayPan(0x4BE, (s16)(64.0f + 24.0f * (lbl_1_data_1E8[target].x / 320.0f)));
		Hu3DModelAttrReset(model, 1);
		Hu3DModelPosSet(model, lbl_1_bss_0->unk3A4.x, 20.0f + lbl_1_bss_0->unk3A4.y, lbl_1_bss_0->unk3A4.z);
	}
}

void fn_1_4534(s32 hex, s32 variant)
{
	s16 model;
	s16 model2;
	s32 pan;

	model = lbl_1_bss_0->models[hex + 6];
	switch (lbl_1_bss_0->hexState[hex]) {
	case 0:
		Hu3DModelAttrReset(model, 0x40000001);
		lbl_1_bss_0->hexState[hex] = 1;
		break;
	case 1:
		fn_1_AD8(model, 121.0f, 150.0f);
		Hu3DModelAttrReset(model, 0x40000001);
		lbl_1_bss_0->hexState[hex] = 2;
		break;
	case 2:
		if (Hu3DMotionEndCheck(model) != 0) {
			pan = (s16)(64.0f + 24.0f * (lbl_1_data_1E8[hex].x / 320.0f));
			HuAudFXPlayPan(0x4BF, pan);
			model2 = fn_1_B34(hex, variant);
			fn_1_AD8(model2, 151.0f, 180.0f);
			Hu3DModelAttrReset(model2, 0x40000001);
			lbl_1_bss_0->hexState[hex] = 3;
			if (lbl_1_data_2D8[hex].owner >= 0) {
				fn_1_F828(lbl_1_data_2D8[hex].owner);
			}
			if (lbl_1_data_2D8[hex].mark >= 0) {
				fn_1_F808(lbl_1_data_2D8[hex].mark);
				switch (lbl_1_data_2D8[hex].mark) {
				case 0:
				case 2:
					HuAudFXPlayPan(0x4C0, 0x28);
					break;
				case 1:
				case 3:
					HuAudFXPlayPan(0x4C1, 0x58);
					break;
				}
			}
		}
		break;
	case 3:
		if (Hu3DMotionEndCheck(model) != 0) {
			fn_1_AD8(model, 0.0f, 120.0f);
			Hu3DMotionTimeSet(model, (f32)lbl_1_bss_0->unk36C);
			Hu3DModelAttrSet(model, 0x40000001);
			lbl_1_bss_0->hexState[hex] = 4;
		}
		break;
	case 4:
		break;
	}
}

void fn_1_4958(void)
{
	s16 model;
	s32 i;
	s8 owner;

	fn_1_AD8(MDL(0x1DC), 0.0f, 660.0f);
	i = 0x13;
	while (i--) {
		if (lbl_1_bss_4->owner[i] < 0) {
			model = fn_1_B34(i, 4);
		} else {
			owner = lbl_1_bss_4->owner[i];
			model = fn_1_B34(i, owner);
		}
		fn_1_AD8(model, 0.0f, 120.0f);
		Hu3DModelAttrSet(model, 0x40000001);
		lbl_1_bss_0->hexState[i] = 4;
	}
}

void fn_1_4BC4(void)
{
	s32 i;
	s32 k;
	s8 cam;
	s8 order[4];

	i = 4;
	while (i--) {
		Hu3DModelAttrReset(lbl_1_bss_0->models[i + 0x2C], 0x40000003);
		Hu3DMotionTimeSet(lbl_1_bss_0->models[i + 0x2C], Hu3DMotionMaxTimeGet(lbl_1_bss_0->models[i + 0x2C]));
		order[i] = (s8)i;
	}
	k = 0;
	while (lbl_1_bss_0->unk10 & 0x400) {
		HuAudFXPlay(0x4B8);
		i = 5;
		while (i--) {
			if (lbl_1_bss_0->unk10 & 0x400) {
				HuPrcVSleep();
			} else {
				break;
			}
		}
		cam = order[k];
		fn_1_12AC(cam);
		if (Hu3DMotionEndCheck(lbl_1_bss_0->models[order[k] + 0x2C]) != 0) {
			Hu3DMotionTimeSet(lbl_1_bss_0->models[order[k] + 0x2C], 0.0f);
		}
		k++;
		if (k == 4) {
			k = 0;
		}
	}
	HuPrcEnd();
}

void fn_1_4E94(void)
{
	lbl_1_bss_0->unk10 |= 0x400;
	HuPrcChildCreate(fn_1_4BC4, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_4EE8(void)
{
	s32 i;
	s32 j;

	i = 0;
	while (lbl_1_bss_0->unk10 & 0x800) {
		fn_1_12AC(lbl_1_bss_0->unk14[i]);
		j = 0x78;
		while (j--) {
			if (lbl_1_bss_0->unk10 & 0x800) {
				HuPrcVSleep();
			} else {
				break;
			}
		}
		i++;
		if (i == (s8)lbl_1_bss_0->unk18) {
			i = 0;
		}
	}
	HuPrcEnd();
}

void fn_1_50DC(void)
{
	lbl_1_bss_0->unk10 |= 0x800;
	HuPrcChildCreate(fn_1_4EE8, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_5130(void)
{
	HuVecF pts[4];
	f32 col[4];
	f32 knots[4];
	HuVecF delta;
	HuVecF scaled;
	f32 t;
	f32 t2;
	s32 total;
	s32 i;
	s32 k;

	if (lbl_1_bss_0->unk1D4 != 0) {
		total = lbl_1_bss_0->unk1B8 + lbl_1_bss_0->unk1D4;
		pts[0] = lbl_1_bss_0->unk19C;
		pts[1] = lbl_1_bss_0->unk19C;
		pts[2] = lbl_1_bss_0->unk1BC;
		pts[3] = lbl_1_bss_0->unk1C8;
		knots[0] = -1.0f;
		knots[1] = 0.0f;
		knots[2] = (f32)lbl_1_bss_0->unk1B8;
		knots[3] = (f32)total;
		for (i = 0;; i++) {
			t = (f32)i;
			if (total == i) {
				lbl_1_bss_0->unk1A8 = lbl_1_bss_0->unk1C8;
				break;
			}
			if (lbl_1_bss_0->unk1B8 == i) {
				pts[0] = lbl_1_bss_0->unk19C;
				pts[1] = lbl_1_bss_0->unk1BC;
				pts[2] = lbl_1_bss_0->unk1C8;
				pts[3] = lbl_1_bss_0->unk1C8;
				knots[0] = 0.0f;
				knots[1] = (f32)lbl_1_bss_0->unk1B8;
				knots[2] = (f32)total;
				knots[3] = (f32)(total + 1);
			}
			k = 4;
			while (k--) {
				col[k] = pts[k].x;
			}
			lbl_1_bss_0->unk1A8.x = InterpolateBMLine(col, knots, t);
			k = 4;
			while (k--) {
				col[k] = pts[k].y;
			}
			lbl_1_bss_0->unk1A8.y = InterpolateBMLine(col, knots, t);
			k = 4;
			while (k--) {
				col[k] = pts[k].z;
			}
			lbl_1_bss_0->unk1A8.z = InterpolateBMLine(col, knots, t);
			Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1A8.x, lbl_1_bss_0->unk1A8.y, lbl_1_bss_0->unk1A8.z);
			HuPrcVSleep();
		}
	} else {
		for (i = 0; lbl_1_bss_0->unk1B8 != i; i++) {
			t2 = sin(3.141592653589793 * (f64)(f32)sin(3.141592653589793 * (f64)((f32)i / (f32)lbl_1_bss_0->unk1B8) * 0.5) * 0.5);
			PSVECSubtract(&lbl_1_bss_0->unk1BC, &lbl_1_bss_0->unk19C, &delta);
			PSVECScale(&delta, &scaled, t2);
			PSVECAdd(&lbl_1_bss_0->unk19C, &scaled, &lbl_1_bss_0->unk1A8);
			Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1A8.x, lbl_1_bss_0->unk1A8.y, lbl_1_bss_0->unk1A8.z);
			HuPrcVSleep();
		}
		lbl_1_bss_0->unk1A8 = lbl_1_bss_0->unk1BC;
	}
	Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1A8.x, lbl_1_bss_0->unk1A8.y, lbl_1_bss_0->unk1A8.z);
	lbl_1_bss_0->unk10 |= 0x200;
	HuPrcEnd();
}

void fn_1_57C0(s16 idx, s32 frames)
{
	HuVecF target;
	s32 n;

	if (idx < 0x12) {
		lbl_1_bss_0->unk10 &= ~0x200;
		if (frames != 0) {
			n = 0;
			switch (lbl_1_bss_0->unk19A) {
			case 1:
				target = lbl_1_data_E0[5];
				n = (frames * 2) / 3;
				break;
			case 2:
				target = lbl_1_data_E0[6];
				n = (frames * 2) / 3;
				break;
			case 3:
				target = lbl_1_data_E0[7];
				n = (frames * 2) / 3;
				break;
			}
			switch (idx) {
			case 2:
				break;
			case 1:
				target = lbl_1_data_E0[5];
				n = (frames * 2) / 3;
				break;
			case 3:
				target = lbl_1_data_E0[7];
				n = (frames * 0x46) / 105;
				break;
			}
			if (frames < n) {
				OSReport("補間が上手くいきません！！\n");
				abort();
			}
			if (n != 0) {
				lbl_1_bss_0->unk1C8 = lbl_1_data_E0[idx];
				lbl_1_bss_0->unk1BC = target;
			} else {
				lbl_1_bss_0->unk1BC = lbl_1_data_E0[idx];
			}
			lbl_1_bss_0->unk1D4 = n;
			lbl_1_bss_0->unk19C = lbl_1_bss_0->unk1A8;
			lbl_1_bss_0->unk1B8 = frames - n;
			HuPrcChildCreate(fn_1_5130, 0x64, 0x1000, 0, HuPrcCurrentGet());
		} else {
			lbl_1_bss_0->unk1A8 = lbl_1_data_E0[idx];
			Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1A8.x, lbl_1_bss_0->unk1A8.y, lbl_1_bss_0->unk1A8.z);
			lbl_1_bss_0->unk10 |= 0x200;
		}
		lbl_1_bss_0->unk19A = idx;
	}
}

void fn_1_5B7C(void)
{
	HuVecF scaled;
	HuVecF delta;
	f32 t;
	f32 ti;
	s32 i;

	for (i = 0;; i++) {
		ti = (f32)i;
		if (lbl_1_bss_0->unk1B8 == i) {
			break;
		}
		lbl_1_bss_0->unk1B4 = fmodf(lbl_1_bss_0->unk1B4 - 20.0f, 360.0f);
		t = sin(3.141592653589793 * (ti / (f32)lbl_1_bss_0->unk1B8) * 0.5);
		PSVECSubtract(&lbl_1_bss_0->unk1BC, &lbl_1_bss_0->unk19C, &delta);
		PSVECScale(&delta, &scaled, t);
		PSVECAdd(&lbl_1_bss_0->unk19C, &scaled, &lbl_1_bss_0->unk1A8);
		Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1A8.x, lbl_1_bss_0->unk1A8.y, lbl_1_bss_0->unk1A8.z);
		Hu3DModelRotSet(lbl_1_bss_0->unk198, 0.0f, lbl_1_bss_0->unk1B4, 0.0f);
		HuPrcVSleep();
	}
	lbl_1_bss_0->unk1A8.x = lbl_1_bss_0->unk1BC.x;
	lbl_1_bss_0->unk1A8.y = lbl_1_bss_0->unk1BC.y;
	lbl_1_bss_0->unk1A8.z = lbl_1_bss_0->unk1BC.z;
	lbl_1_bss_0->unk1B4 = 0.0f;
	Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1A8.x, lbl_1_bss_0->unk1A8.y, lbl_1_bss_0->unk1A8.z);
	Hu3DModelRotSet(lbl_1_bss_0->unk198, 0.0f, lbl_1_bss_0->unk1B4, 0.0f);
	lbl_1_bss_0->unk10 |= 0x200;
	HuPrcEnd();
}

void fn_1_5EB0(HuVecF *pos, f32 time)
{
	lbl_1_bss_0->unk10 &= ~0x200;
	lbl_1_bss_0->unk19C = *pos;
	lbl_1_bss_0->unk1BC = *pos;
	lbl_1_bss_0->unk1BC.y = 120.0f;
	lbl_1_bss_0->unk1B4 = fmodf(20.0f * time, 360.0f);
	lbl_1_bss_0->unk1B8 = (s32)time;
	Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_bss_0->unk1BC.x, lbl_1_bss_0->unk1BC.y, lbl_1_bss_0->unk1BC.z);
	Hu3DModelRotSet(lbl_1_bss_0->unk198, 0.0f, lbl_1_bss_0->unk1B4, 0.0f);
	HuPrcChildCreate(fn_1_5B7C, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_6068(void)
{
	while (1) {
		if (Hu3DMotionEndCheck(MDL(0x1DC))) {
			break;
		}
		HuPrcVSleep();
	}
	Hu3DModelAttrReset(MDL(0x1DC), 0x40000002);
	Hu3DModelAttrSet(MDL(0x1DC), 0x40000001);
	fn_1_AD8(MDL(0x1DC), 0.0f, 660.0f);
	HuPrcEnd();
}

void fn_1_6130(void)
{
	fn_1_AD8(MDL(0x1DC), 661.0f, 700.0f);
	Hu3DModelAttrReset(MDL(0x1DC), 0x40000001);
	HuPrcChildCreate(fn_1_6068, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_61D4(void)
{
	s32 i;
	f32 t;

	i = 0x3C;
	while (Hu3DMotionEndCheck(lbl_1_bss_0->unk352) == 0) {
		HuPrcVSleep();
	}
	while (i--) {
		t = (f32)i / 60.0f;
		Hu3DModelTPLvlSet(lbl_1_bss_0->unk352, t);
		Hu3DModelTPLvlSet(MDL(0x248), t);
		HuPrcVSleep();
	}
	Hu3DModelTPLvlSet(lbl_1_bss_0->unk352, 1.0f);
	Hu3DModelTPLvlSet(MDL(0x248), 1.0f);
	Hu3DModelAttrSet(lbl_1_bss_0->unk352, 1);
	Hu3DModelAttrSet(MDL(0x248), 1);
	HuPrcEnd();
}

void fn_1_6318(void)
{
	fn_1_AD8(lbl_1_bss_0->unk352, 241.0f, 300.0f);
	Hu3DModelAttrReset(lbl_1_bss_0->unk352, 0x40000001);
	fn_1_AD8(MDL(0x1DC), 661.0f, 700.0f);
	Hu3DModelAttrReset(MDL(0x1DC), 0x40000001);
	HuPrcChildCreate(fn_1_6068, 0x64, 0x1000, 0, HuPrcCurrentGet());
	HuPrcChildCreate(fn_1_61D4, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_6444(void)
{
	HuVecF dir;
	HuVecF norm;
	MGWARSPLAYER *p;
	s32 i;
	s32 done;
	s32 reached;
	f32 targetAngle;
	f32 rem;
	f32 step;
	f32 ang;

	p = lbl_1_bss_0->player;
	for (i = 0; i < 4; i++) {
		switch (lbl_1_bss_0->unk358) {
		case 0:
			p->targetAngle = 0.0f;
			break;
		case 1:
			PSVECScale(&lbl_1_data_B0[i], &dir, -1.0f);
			PSVECNormalize(&dir, &norm);
			if (norm.z > 1.0f) {
				norm.z = 1.0f;
			} else if (norm.z < -1.0f) {
				norm.z = -1.0f;
			}
			ang = acosf(norm.z);
			if (norm.x < 0.0f) {
				ang = -ang;
			}
			p->targetAngle = (180.0f * ang) / 3.141592653589793;
			break;
		case 2:
			PSVECSubtract(&lbl_1_data_2CC, &lbl_1_data_B0[i], &dir);
			PSVECNormalize(&dir, &norm);
			if (norm.z > 1.0f) {
				norm.z = 1.0f;
			} else if (norm.z < -1.0f) {
				norm.z = -1.0f;
			}
			ang = acosf(norm.z);
			if (norm.x < 0.0f) {
				ang = -ang;
			}
			p->targetAngle = (180.0f * ang) / 3.141592653589793;
			break;
		}
		targetAngle = p->targetAngle;
		rem = fmodf(p->angle - targetAngle, 180.0f);
		if (fabsf(rem) < 0.01f) {
			p->angle = targetAngle;
			reached = 1;
		} else {
			if (rem > 0.0f) {
				step = rem - 0.01f;
			} else {
				step = rem + 0.01f;
			}
			p->angle = targetAngle + step;
			reached = 0;
		}
		if (reached == 0 && p->state != 3) {
			CharMotionShiftSet(p->charNo, p->motId[3], 0.0f, 15.0f, 0x40000001);
			p->state = 3;
		}
		p++;
	}
	while (1) {
		done = 0;
		p = lbl_1_bss_0->player;
		i = 4;
		while (i--) {
			step = lbl_1_bss_0->unk35C;
			targetAngle = p->targetAngle;
			rem = fmodf(p->angle - targetAngle, 180.0f);
			if (fabsf(rem) < step) {
				p->angle = targetAngle;
				reached = 1;
			} else {
				if (rem > 0.0f) {
					ang = rem - step;
				} else {
					ang = rem + step;
				}
				p->angle = targetAngle + ang;
				reached = 0;
			}
			if (reached != 0) {
				if (p->state != 0) {
					CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0x40000001);
					p->state = 0;
				}
				done++;
			}
			Hu3DModelRotSet(p->modelId, 0.0f, p->angle, 0.0f);
			p++;
		}
		if (done != 4) {
			HuPrcVSleep();
			continue;
		}
		break;
	}
	lbl_1_bss_0->unk10 &= ~0x1000;
	HuPrcEnd();
}

void fn_1_6A14(void)
{
	u8 table[19][13];
	s32 i;
	s32 j;
	s32 cnt;
	s32 val;

	i = 0x13;
	while (i--) {
		j = 12;
		while (j--) {
			table[i][j] = j % 4;
		}
		table[i][12] = 4;
		fn_1_A0(table[i], 13);
		j = 5;
		while (j--) {
			Hu3DMotionTimeSet(lbl_1_bss_0->hexModel[i][j], 60.0f);
			Hu3DModelAttrSet(lbl_1_bss_0->hexModel[i][j], 0x40000002);
		}
	}
	cnt = 0;
	while (1) {
		if (cnt == 0xD) {
			i = 0x13;
			while (i--) {
				fn_1_A0(table[i], 13);
			}
			cnt = 0;
		}
		j = 5;
		while (j--) {
			if ((lbl_1_bss_0->unk10 & 0x2000) == 0) {
				HuPrcVSleep();
			} else {
				break;
			}
		}
		if ((lbl_1_bss_0->unk10 & 0x2000) == 0) {
			i = 0x13;
			while (i--) {
				val = (s8)table[i][cnt];
				fn_1_B34(i, val);
			}
			cnt++;
			continue;
		}
		break;
	}
	i = 0x13;
	while (i--) {
		j = 5;
		while (j--) {
			Hu3DMotionTimeSet(lbl_1_bss_0->hexModel[i][j], 0.0f);
			Hu3DModelAttrReset(lbl_1_bss_0->hexModel[i][j], 0x40000002);
		}
		if (lbl_1_bss_4->owner[i] < 0) {
			fn_1_B34(i, 4);
		} else {
			val = lbl_1_bss_4->owner[i];
			fn_1_B34(i, val);
		}
	}
	HuPrcEnd();
}

void fn_1_6F54(void)
{
	lbl_1_bss_0->unk10 &= ~0x2000;
	HuPrcChildCreate(fn_1_6A14, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_6FA8(s32 mode, f32 step)
{
	lbl_1_bss_0->unk10 |= 0x1000;
	lbl_1_bss_0->unk358 = (s16)mode;
	lbl_1_bss_0->unk35C = step;
	HuPrcChildCreate(fn_1_6444, 0x64, 0x1000, 0, HuPrcCurrentGet());
	HuPrcVSleep();
}

void fn_1_7034(void)
{
	MGWARSPLAYER *p;
	s32 i;

	while (lbl_1_bss_0->unk36A-- != 0) {
		if (lbl_1_bss_0->unk10 & 0x4000) {
			lbl_1_bss_0->unk36A = -1;
			HuPrcEnd();
			return;
		}
		HuPrcVSleep();
	}
	i = 4;
	while (i--) {
		p = &lbl_1_bss_0->player[i];
		if (p->state != 0) {
			CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0x40000001);
			p->state = 0;
		}
	}
	HuPrcEnd();
}

void fn_1_7138(s32 arg0)
{
	lbl_1_bss_0->unk10 &= ~0x4000;
	lbl_1_bss_0->unk36A = (s16)arg0;
	if (lbl_1_bss_0->unk36A > 0) {
		HuPrcChildCreate(fn_1_7034, 0x64, 0x1000, 0, HuPrcCurrentGet());
	}
}

void fn_1_71C0(void)
{
	while (1) {
		if (Hu3DMotionEndCheck(MDL(0x24C))) {
			break;
		}
		HuPrcVSleep();
	}
	Hu3DModelAttrSet(MDL(0x24C), 1);
	HuPrcEnd();
}

void fn_1_721C(void)
{
	Hu3DModelAttrReset(MDL(0x24C), 0x40000002);
	Hu3DModelAttrReset(MDL(0x24C), 1);
	HuPrcChildCreate(fn_1_71C0, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_728C(void)
{
	HuVec2f winSize;
	MGWARSPLAYER *p;
	s32 i;
	s32 j;
	s32 cnt;
	s8 idx;
	s16 k;
	s16 model;

	lbl_1_bss_4 = (MGWARSCOM *)mgModeWork;
	lbl_1_bss_0 = &lbl_1_bss_8;
	lbl_1_bss_0->objman = omInitObjMan(0xC8, 0x3E8);
	HuDataSelHeapReadNum(0x270000, HU_MEMNUM_OVL, HEAP_MODEL);
	Hu3DCameraCreate(2);
	Hu3DCameraPerspectiveSet(2, 40.0f, 10.0f, 5000.0f, 1.2f);
	Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
	Hu3DCameraScissorSet(2, 0, 0, 0x280, 0x1e0);
	lbl_1_bss_0->gameCnt = 0;
	for (i = 0; i < 0x1A; i++) {
		idx = 0;
		while (mgDataTbl[idx].ovl != lbl_1_data_0[i].ovl) {
			if (mgDataTbl[idx].ovl == 0xFFFF) {
				OSReport("mgwars.c : ＤＬＬが見つかりません！！\n");
				abort();
			}
			idx++;
		}
		lbl_1_data_0[i].mgIdx = idx;
		if (GWMgUnlockGet(idx + 0x1F5)) {
			((u16 *)&lbl_1_bss_0->games[lbl_1_bss_0->gameCnt])[0] = ((u16 *)&lbl_1_data_0[i])[0];
			((u16 *)&lbl_1_bss_0->games[lbl_1_bss_0->gameCnt])[1] = ((u16 *)&lbl_1_data_0[i])[1];
			((u16 *)&lbl_1_bss_0->games[lbl_1_bss_0->gameCnt])[2] = ((u16 *)&lbl_1_data_0[i])[2];
			lbl_1_bss_0->gameCnt++;
		}
	}
	for (i = 0; i < lbl_1_bss_0->gameCnt; i++) {
		lbl_1_bss_0->games[i].is3D = (mgDataTbl[lbl_1_bss_0->games[i].mgIdx].type == 3);
	}
	lbl_1_bss_0->unk6 = 0;
	lbl_1_bss_0->unk4 = -1;
	lbl_1_bss_0->unkC = 0;
	lbl_1_bss_0->unk10 = 0;
	lbl_1_bss_0->unk360 = -1;
	lbl_1_bss_0->unk364 = 0.0f;
	lbl_1_bss_0->unk368 = 0;
	lbl_1_bss_0->unk390 = 0;
	lbl_1_bss_0->unk384 = -1;
	lbl_1_bss_0->unk124 = -1;
	lbl_1_bss_0->unk378 = -1;
	i = 0x1A;
	while (i--) {
		lbl_1_bss_0->unkB8[i] = 0;
	}
	HuWinInit(1);
	HuWinMesMaxSizeBetGet(&winSize, 0x110000, 0x11001E);
	lbl_1_bss_0->unk6 = HuWinExCreate(HUWIN_POS_CENTER, 350.0f, (s16)winSize.x, (s16)winSize.y, -1);
	Hu3DZClearLayerSet(0xE);
	fn_1_1A80();
	fn_1_F068();
	i = 4;
	while (i--) {
		cnt = 0;
		j = 0x13;
		while (j--) {
			if (lbl_1_bss_4->owner[j] == i) {
				cnt++;
			}
		}
		fn_1_F848(i, cnt);
	}
	p = lbl_1_bss_0->player;
	for (k = 0; k < 4; k++) {
		p->unkA = k;
		p->charNo = GwPlayerConf[k].charNo;
		p->padNo = GwPlayerConf[k].padNo;
		p->modelId = CharModelMotListCreate(p->charNo, 2, lbl_1_data_9C, p->motId);
		p->state = -1;
		p->angle = 0.0f;
		CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[1], 0);
		CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[2], 0);
		CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[3], 0);
		if (p->state != 0) {
			CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0x40000001);
			p->state = 0;
		}
		Hu3DModelPosSet(p->modelId, lbl_1_data_B0[k].x, lbl_1_data_B0[k].y, lbl_1_data_B0[k].z);
		Hu3DModelLayerSet(p->modelId, 3);
		Hu3DModelCameraSet(p->modelId, 2);
		p++;
	}
	model = Hu3DModelCreateData(0x9B0000);
	lbl_1_bss_0->unk198 = model;
	Hu3DMotionSet(model, Hu3DJointMotionData(model, 0x9B0001));
	Hu3DModelAttrSet(model, 0x40000001);
	Hu3DModelLayerSet(model, 7);
	Hu3DModelCameraSet(model, 3);
	fn_1_57C0(0, 0);
	Hu3DNoSyncSet(1);
}

void fn_1_796C(void)
{
	s8 tmp[0x100];
	s8 prev;
	s32 i;
	s32 j;
	s32 k;
	s32 n;
	s8 *list;
	ANIMDATA *anim;

	if (lbl_1_bss_4->unk2 == lbl_1_bss_0->gameCnt) {
		prev = lbl_1_bss_4->list[lbl_1_bss_0->gameCnt - 1];
		lbl_1_bss_4->unk2 = 0;
		n = lbl_1_bss_0->gameCnt;
		list = lbl_1_bss_4->list;
		fn_1_A0((u8 *)list, n);
		if (prev == lbl_1_bss_4->list[0]) {
			if (lbl_1_bss_0->gameCnt > 1) {
				k = frandmod(lbl_1_bss_0->gameCnt - 1) + 1;
			} else {
				k = 1;
			}
			lbl_1_bss_4->list[k] = prev;
			lbl_1_bss_4->list[0] = lbl_1_bss_4->list[k];
		}
	}
	if ((lbl_1_bss_0->unk10 & 0x80) == 0) {
		i = lbl_1_bss_0->gameCnt;
		while (i--) {
			anim = HuSprAnimRead(HuDataSelHeapReadNum(mgDataTbl[lbl_1_bss_0->games[i].mgIdx].instPic[0], HU_MEMNUM_OVL, HEAP_MODEL));
			HuSprAnimLock(anim);
			lbl_1_bss_0->unkB8[i] = anim;
		}
		if (lbl_1_bss_0->gameCnt > 1) {
			lbl_1_bss_0->unk124 = Hu3DAnimCreate(lbl_1_bss_0->unkB8[frandmod(lbl_1_bss_0->gameCnt - 1) + 1], MDL(0x24A), "mg_shot");
		} else {
			lbl_1_bss_0->unk124 = Hu3DAnimCreate(lbl_1_bss_0->unkB8[0], MDL(0x24A), "mg_shot");
		}
		lbl_1_bss_0->unk10 |= 0x80;
	}
}

void fn_1_7CC8(void)
{
	OMOVLHIS *his;

	his = omOvlHisGet(0);
	omOvlHisChg(0, his->ovl, 0, his->stat);
	HuDataDirClose(0x9B0000);
	HuDataDirClose(0x270000);
	CharModelDataClose(-1);
	HuPrcVSleep();
	if (lbl_1_bss_0->unk378 >= 0) {
		HuAudSStreamFadeOut(lbl_1_bss_0->unk378, 0x3E8);
		lbl_1_bss_0->unk378 = -1;
	}
	if (lbl_1_bss_0->unk390 != 0) {
		HuSprAnimKill(lbl_1_bss_0->unk390);
	}
	GwSystem.mgInstF = 1;
	HuWinExClose(lbl_1_bss_0->unk6);
	HuWinExKill(lbl_1_bss_0->unk6);
	WipeCreate(2, 6, 0x3C);
	while (WipeCheckEnd() != 0) {
		HuPrcVSleep();
	}
	_ClearFlag(0x30003);
	_ClearFlag(0x30001);
	memset(lbl_1_bss_4, 0, 0x32);
}

void fn_1_7E30(void)
{
	void (*tbl[4])(void) = { fn_1_E3C8, fn_1_E450, fn_1_E4D8, fn_1_E560 };
	OMOVLHIS *his;
	MGWARSPLAYER *p;
	s32 i;
	s32 j;
	s32 cnt;

	_SetFlag(0x30003);
	_SetFlag(0x30001);
	lbl_1_bss_4->unk0 = 0;
	his = omOvlHisGet(0);
	omOvlHisChg(0, his->ovl, 1, his->stat);
	lbl_1_bss_0->unk378 = HuAudBGMPlay(0x26);
	lbl_1_bss_4->unk2 = (s8)lbl_1_bss_0->gameCnt;
	lbl_1_bss_4->unk3 = 0;
	i = lbl_1_bss_0->gameCnt;
	while (i--) {
		lbl_1_bss_4->list[i] = (s8)i;
	}
	i = 0x13;
	while (i--) {
		lbl_1_bss_4->owner[i] = -1;
	}
	i = 4;
	while (i--) {
		cnt = 0;
		j = 0x13;
		while (j--) {
			if (lbl_1_bss_4->owner[j] == i) {
				cnt++;
			}
		}
		fn_1_F848(i, cnt);
	}
	i = 4;
	while (i--) {
		p = &lbl_1_bss_0->player[i];
		if (p->state != 0) {
			CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0x40000001);
			p->state = 0;
		}
	}
	fn_1_4958();
	fn_1_57C0(0, 0);
	Hu3DModelPosSet(lbl_1_bss_0->unk198, lbl_1_data_E0[0xD].x, lbl_1_data_E0[0xD].y, lbl_1_data_E0[0xD].z);
	Hu3DModelAttrReset(MDL(0x1D8), 1);
	Hu3DModelAttrSet(MDL(0x1E0), 0x40000003);
	Hu3DModelAttrSet(MDL(0x1E0), 1);
	Hu3DMotionTimeSet(MDL(0x1E0), 0.0f);
	Hu3DMotionTimeSet(MDL(0x25A), 0.0f);
	Hu3DMotionTimeSet(MDL(0x1E2), 0.0f);
	Hu3DCameraMotionStart(MDL(0x26C), 2);
	Hu3DModelAttrReset(MDL(0x25A), 0x40000002);
	Hu3DModelAttrReset(MDL(0x25A), 1);
	Hu3DModelAttrReset(MDL(0x1E2), 0x40000002);
	Hu3DModelAttrReset(MDL(0x1E2), 1);
	Hu3DModelAttrReset(MDL(0x250), 1);
	fn_1_6F54();
	fn_1_A30(5, 0x3C);
	i = 0;
	p = lbl_1_bss_0->player;
	for (i = 0; i < 4; i++) {
		CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[3], 1);
		p++;
	}
	fn_1_9F4(0x10E);
	Hu3DCameraMotionOff(MDL(0x26C));
	Hu3DModelAttrSet(MDL(0x25A), 0x40000002);
	Hu3DModelAttrSet(MDL(0x25A), 1);
	Hu3DModelAttrReset(MDL(0x1E0), 0x40000002);
	Hu3DModelAttrReset(MDL(0x1E0), 1);
	fn_1_C04(0x42, 0x1E, 2);
	fn_1_189C(0x39D, 0xB4);
	fn_1_5EB0(&lbl_1_data_E0[0xD], 45.0f);
	while ((lbl_1_bss_0->unk10 & 0x200) == 0) {
		HuPrcVSleep();
	}
	fn_1_9F4(0xF);
	lbl_1_bss_0->unk10 |= 0x2000;
	fn_1_7FC(fn_1_E2D0);
	while ((lbl_1_bss_0->unkC & 1) == 0) {
		HuPrcVSleep();
	}
	fn_1_83C(0x45, 45.0f, 1);
	Hu3DModelAttrSet(MDL(0x250), 1);
	fn_1_7FC(fn_1_E36C);
	while ((lbl_1_bss_0->unkC & 2) == 0) {
		HuPrcVSleep();
	}
	for (i = 0; i < 4; i++) {
		p = &lbl_1_bss_0->player[i];
		fn_1_1778(p, 0);
		fn_1_189C(0x3AA, 0x78);
		omVibrate(p->padNo, 0xC, 4, 2);
		if (p->state != 1) {
			CharMotionShiftSet(p->charNo, p->motId[1], 0.0f, 15.0f, 0);
			p->state = 1;
		}
		Hu3DModelAttrReset(lbl_1_bss_0->models[i + 0x2C], 0x40000002);
		fn_1_57C0(i + 9, 0x3C);
		fn_1_7FC(tbl[i]);
		fn_1_9F4(0x96);
		if (p->state != 0) {
			CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0x40000001);
			p->state = 0;
		}
		Hu3DModelAttrReset(lbl_1_bss_0->models[i + 0x2C], 0x40000001);
	}
	fn_1_568();
	fn_1_64C(1);
}

void fn_1_8C80(void)
{
	s32 keys[4];
	s32 i;

	HuWinDisablePlayerSet(lbl_1_bss_0->unk6, 0xF);
	while ((lbl_1_bss_0->unkC & lbl_1_bss_0->unk354) == 0) {
		i = 4;
		while (i--) {
			keys[i] = -1;
			if (GwPlayerConf[i].type != 0) {
				keys[i] = 0x100;
			}
		}
		HuWinComKeyWait(keys[0], keys[1], keys[2], keys[3], 0xA);
		fn_1_9F4(0x1E);
	}
	HuWinDisablePlayerReset(lbl_1_bss_0->unk6, 0xF);
	HuWinComKeyReset();
	HuPrcEnd();
}

void fn_1_8D9C(s32 mask)
{
	lbl_1_bss_0->unk354 = mask;
	HuPrcChildCreate(fn_1_8C80, 0xc8, 0x3000, 0, HuPrcCurrentGet());
}

void fn_1_8DF0(void)
{
	s8 winner PC_INIT(0);
	s32 cnt;
	s8 i;
	s16 rank;
	s16 rank2;
	s16 coin;
	s16 coin2;
	s32 j;
	MGWARSPLAYER *p;

	fn_1_4958();
	cnt = 0;
	if (lbl_1_bss_4->unk3 & 1) {
		OSReport("バトルゲーム！！\n");
		i = 4;
		while (i--) {
			rank = GwPlayer[i].mgCoinBonus;
			OSReport("\t Player %d = Rank %d\n", i, rank);
			rank2 = GwPlayer[i].mgCoinBonus;
			if (rank2 == 0) {
				winner = i;
				cnt++;
			}
		}
	} else {
		OSReport("VS 4 ゲーム！！\n");
		i = 4;
		while (i--) {
			coin = GwPlayer[i].mgCoinBonus;
			OSReport("\t Player %d = Coin %d\n", i, coin);
			coin2 = GwPlayer[i].mgCoinBonus;
			if (coin2 >= 10) {
				winner = i;
				cnt++;
			}
		}
	}
	if (lbl_1_bss_4->unk3 & 2) {
		lbl_1_bss_0->unk10 |= 0x10;
	}
	if (cnt != 1) {
		lbl_1_bss_0->unk378 = HuAudBGMPlay(0x52);
		lbl_1_bss_0->unk10 |= 8;
	} else {
		if (lbl_1_bss_0->unk10 & 0x10) {
			fn_1_1050();
		} else {
			fn_1_F94();
		}
		lbl_1_bss_0->unk8 = winner;
	}
	if (lbl_1_bss_0->unk10 & 8) {
		fn_1_C04(0x42, 0x3C, 2);
		fn_1_57C0(0, 0);
	} else {
		fn_1_57C0(winner + 0xE, 0);
		fn_1_6FA8(1, 180.0f);
		fn_1_C04(winner + 0x46, 0x3C, 2);
		lbl_1_bss_0->player[winner].angle = 0.0f;
		Hu3DModelRotSet(lbl_1_bss_0->player[winner].modelId, 0.0f, lbl_1_bss_0->player[winner].angle, 0.0f);
	}
	Hu3DModelAttrReset(lbl_1_bss_0->models[winner + 0x2C], 0x40000002);
	Hu3DModelAttrSet(lbl_1_bss_0->models[winner + 0x2C], 0x40000001);
	Hu3DModelAttrReset(MDL(0x1E0), 0x40000002);
	Hu3DModelAttrReset(MDL(0x1E0), 1);
	Hu3DModelAttrReset(MDL(0x1E2), 0x40000002);
	Hu3DModelAttrReset(MDL(0x1E2), 1);
	Hu3DMotionTimeSet(MDL(0x1E2), Hu3DMotionMaxTimeGet(MDL(0x1E2)));
	fn_1_A30(5, 0x3C);
	p = lbl_1_bss_0->player;
	for (j = 0; j < 4; j++) {
		CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[3], 1);
		p++;
	}
	if (lbl_1_bss_0->unk10 & 8) {
		i = 4;
		while (i--) {
			p = &lbl_1_bss_0->player[i];
			fn_1_1778(p, 2);
			if (p->state != 2) {
				CharMotionShiftSet(p->charNo, p->motId[2], 0.0f, 15.0f, 0);
				p->state = 2;
			}
		}
	} else {
		fn_1_5EB0(&lbl_1_data_E0[winner + 0xE], 45.0f);
		while ((lbl_1_bss_0->unk10 & 0x200) == 0) {
			HuPrcVSleep();
		}
		p = &lbl_1_bss_0->player[winner];
		HuAudFXPlayPan(lbl_1_data_3FC[p->charNo][1], 0x40);
		fn_1_189C(0x3AA, 0x78);
		if (p->state != 1) {
			CharMotionShiftSet(p->charNo, p->motId[1], 0.0f, 15.0f, 0);
			p->state = 1;
		}
	}
	if (lbl_1_bss_0->unk10 & 8) {
		fn_1_7FC(fn_1_E87C);
		while ((lbl_1_bss_0->unkC & 0x400) == 0) {
			HuPrcVSleep();
		}
	} else {
		fn_1_7FC(fn_1_E908);
		fn_1_9F4(0xB4);
		p = &lbl_1_bss_0->player[winner];
		if (p->state != 0) {
			CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0);
			p->state = 0;
		}
		if (GwPlayerConf[winner].type != 0) {
			fn_1_8D9C(0x1000);
		}
		fn_1_7FC(fn_1_E9A0);
		while ((lbl_1_bss_0->unkC & 0x1000) == 0) {
			HuPrcVSleep();
		}
	}
	fn_1_64C(0x4000);
	HuPrcVSleep();
}

void fn_1_9BC8(void)
{
	s32 counts[19];
	s32 hexes[19];
	s32 emptyList[19];
	s32 changed[4];
	u8 player;
	s8 pl;
	s32 i;
	s32 j;
	s32 cnt PC_INIT(0);
	s32 nz;
	s32 cur;
	MGWARSHEX *hex;
	s16 model;
	s32 pad;
	s32 blink;
	s32 dir;
	s32 xs;
	s32 ys;
	s32 timer;
	s32 doneCnt;
	s32 changedCnt;
	s32 val;
	s32 t;
	s32 u;
	MGWARSPLAYER *p;
	OMOVLHIS *his;

	player = lbl_1_bss_0->unk8;
	pl = player;
	lbl_1_bss_0->unk352 = lbl_1_bss_0->models[pl + 0x34];
	Hu3DModelAttrReset(lbl_1_bss_0->unk352, 1);
	Hu3DModelAttrReset(MDL(0x248), 1);
	fn_1_AD8(lbl_1_bss_0->unk352, 0.0f, 240.0f);
	switch (pl) {
	case 0:
		fn_1_AD8(MDL(0x248), 0.0f, 60.0f);
		break;
	case 1:
		fn_1_AD8(MDL(0x248), 61.0f, 120.0f);
		break;
	case 2:
		fn_1_AD8(MDL(0x248), 121.0f, 180.0f);
		break;
	case 3:
		fn_1_AD8(MDL(0x248), 181.0f, 240.0f);
		break;
	}
	i = 0x13;
	while (i--) {
		lbl_1_data_2D8[i].owner = lbl_1_bss_4->owner[i];
		lbl_1_data_2D8[i].mark = lbl_1_data_2D8[i].owner;
	}
	fn_1_6FA8(1, 2.0f);
	fn_1_C04(0x44, 0x3C, 1);
	fn_1_7FC(fn_1_E9FC);
	while ((lbl_1_bss_0->unkC & 0x2000) == 0) {
		HuPrcVSleep();
	}
	fn_1_F860();
	if (lbl_1_bss_0->unk10 & 0x10) {
		i = 0x13;
		while (i--) {
			if (pl != lbl_1_data_2D8[i].owner) {
				lbl_1_data_2D8[i].reach = 1;
			}
		}
	} else {
		fn_1_37D4();
	}
	if ((lbl_1_bss_0->unk10 & 0x10) == 0) {
		cnt = 0;
		nz = 0;
		i = 0x13;
		while (i--) {
			if (lbl_1_data_2D8[i].reach != 0) {
				counts[cnt] = fn_1_3458(i, pl);
				hexes[cnt] = i;
				if (counts[cnt] == 0) {
					emptyList[nz] = i;
					nz++;
				}
				cnt++;
			}
		}
		if (cnt != nz) {
			i = nz;
			while (i--) {
				lbl_1_data_2D8[emptyList[i]].reach = 0;
			}
		}
		i = 0x13;
		while (i--) {
			lbl_1_data_2D8[i].mark = lbl_1_data_2D8[i].owner;
		}
	}
	i = 0x13;
	while (i--) {
		if (lbl_1_data_2D8[i].reach != 0) {
			Hu3DModelAttrReset(lbl_1_bss_0->models[i + 0x19], 1);
		}
	}
	if (GwPlayerConf[pl].type != 0) {
		if (lbl_1_bss_0->unk10 & 0x10) {
			cnt = 0;
			i = 0x13;
			while (i--) {
				if (lbl_1_data_2D8[i].reach != 0) {
					counts[cnt] = fn_1_3458(i, pl);
					hexes[cnt] = i;
					cnt++;
				}
			}
			i = 0x13;
			while (i--) {
				lbl_1_data_2D8[i].mark = lbl_1_data_2D8[i].owner;
			}
		}
		fn_1_164(counts, hexes, cnt);
		i = 0x13;
		while (i--) {
			lbl_1_data_2D8[i].mark = lbl_1_data_2D8[i].owner;
		}
		cur = hexes[cnt - 1];
		fn_1_4170(cur);
	} else {
		cur = 9;
		pad = GwPlayerConf[pl].padNo;
		hex = &lbl_1_data_2D8[9];
		model = MDL(0x248);
		blink = 0;
		do {
			do {
				HuPrcVSleep();
				dir = -1;
				if ((f32)HuPadStkX[pad] > 28.0f) {
					xs = 1;
				} else if ((f32)HuPadStkX[pad] < -28.0f) {
					xs = 2;
				} else {
					xs = 0;
				}
				if ((f32)HuPadStkY[pad] > 28.0f) {
					ys = 1;
				} else if ((f32)HuPadStkY[pad] < -28.0f) {
					ys = 2;
				} else {
					ys = 0;
				}
				switch ((xs << 4) | ys) {
				case 1:
					dir = 0;
					break;
				case 2:
					dir = 3;
					break;
				case 16:
					dir = 1;
					break;
				case 17:
					dir = 0;
					break;
				case 18:
					dir = 2;
					break;
				case 32:
					dir = 4;
					break;
				case 33:
					dir = 5;
					break;
				case 34:
					dir = 3;
					break;
				}
				if (dir >= 0 && hex->nb[dir] >= 0) {
					Hu3DModelAttrSet(model, 1);
					blink = 0;
					cur = hex->nb[dir];
					hex = &lbl_1_data_2D8[cur];
					fn_1_3AF0(cur);
				}
				if (blink == 3) {
					Hu3DModelAttrReset(model, 1);
					Hu3DModelPosSet(model, lbl_1_bss_0->unk3A4.x, 20.0f + lbl_1_bss_0->unk3A4.y, lbl_1_bss_0->unk3A4.z);
				} else {
					blink++;
				}
			} while ((HuPadBtnDown[pad] & 0x100) == 0);
		} while (hex->reach == 0);
		HuAudFXPlayPan(0x4BE, (s16)(64.0f + 24.0f * (lbl_1_data_1E8[cur].x / 320.0f)));
	}
	fn_1_568();
	i = 0x13;
	while (i--) {
		Hu3DModelAttrSet(lbl_1_bss_0->models[i + 0x19], 1);
	}
	fn_1_3458(cur, pl);
	i = 4;
	while (i--) {
		changed[i] = 0;
	}
	i = 0x13;
	while (i--) {
		if (lbl_1_data_2D8[i].mark != lbl_1_data_2D8[i].owner) {
			lbl_1_bss_0->hexState[i] = 0;
			if (lbl_1_data_2D8[i].owner >= 0) {
				changed[lbl_1_data_2D8[i].owner] = 1;
			}
		}
		lbl_1_bss_4->owner[i] = lbl_1_data_2D8[i].mark;
	}
	timer = 0x10E;
	lbl_1_bss_0->unk36C = Hu3DMotionTimeGet(MDL(0x1E4));
	fn_1_6318();
	while (1) {
		timer--;
		fn_1_4534(cur, lbl_1_bss_4->owner[cur]);
		if (lbl_1_bss_0->hexState[cur] == 4) {
			break;
		}
		lbl_1_bss_0->unk36C = (s16)(lbl_1_bss_0->unk36C + 1);
		lbl_1_bss_0->unk36C = lbl_1_bss_0->unk36C % 120;
		HuPrcVSleep();
	}
	lbl_1_data_2D8[cur].mark = lbl_1_data_2D8[cur].owner;
	do {
		timer--;
		changedCnt = 0;
		lbl_1_bss_0->unkA = 1;
		i = 0x13;
		while (i--) {
			if (lbl_1_data_2D8[i].mark != lbl_1_data_2D8[i].owner) {
				fn_1_4534(i, lbl_1_bss_4->owner[i]);
				lbl_1_bss_0->unkA = (s16)(lbl_1_bss_0->unkA + 1);
			}
			if (lbl_1_bss_0->hexState[i] == 4) {
				changedCnt++;
			}
		}
		lbl_1_bss_0->unk36C = (s16)(lbl_1_bss_0->unk36C + 1);
		lbl_1_bss_0->unk36C = lbl_1_bss_0->unk36C % 120;
		HuPrcVSleep();
	} while (changedCnt != 0x13);
	fn_1_189C(0x3AA, 0x78);
	i = 4;
	while (i--) {
		p = &lbl_1_bss_0->player[i];
		if (pl == i) {
			omVibrate(p->padNo, 0xC, 4, 2);
			fn_1_1778(p, 3);
			if (p->state != 1) {
				CharMotionShiftSet(p->charNo, p->motId[1], 0.0f, 15.0f, 0);
				p->state = 1;
			}
		} else if (changed[i] != 0) {
			omVibrate(p->padNo, 0xC, 4, 2);
			fn_1_1778(p, 4);
			if (p->state != 2) {
				CharMotionShiftSet(p->charNo, p->motId[2], 0.0f, 15.0f, 0);
				p->state = 2;
			}
		}
	}
	if (timer < 0x96) {
		timer = 0x96;
	}
	if (GwPlayerConf[pl].type != 0) {
		fn_1_8D9C(0x80000);
	}
	fn_1_7FC(fn_1_EA84);
	while ((lbl_1_bss_0->unkC & 0x80000) == 0) {
		timer--;
		HuPrcVSleep();
	}
	fn_1_568();
	while (timer > 0) {
		timer--;
		HuPrcVSleep();
	}
	i = 4;
	while (i--) {
		p = &lbl_1_bss_0->player[i];
		if (p->state != 0) {
			CharMotionShiftSet(p->charNo, p->motId[0], 0.0f, 15.0f, 0x40000001);
			p->state = 0;
		}
	}
	nz = 0;
	i = 0x13;
	while (i--) {
		if (lbl_1_bss_4->owner[i] == -1) {
			nz++;
		}
	}
	if (nz == 0) {
		his = omOvlHisGet(0);
		omOvlHisChg(0, his->ovl, 2, his->stat);
		lbl_1_bss_4->unk3 |= 4;
	}
	Hu3DModelAttrReset(lbl_1_bss_0->models[pl + 0x2C], 0x40000001);
	fn_1_F884();
}

void fn_1_B614(void)
{
	s8 sel;
	s32 i;
	s32 j;
	s32 end;
	s32 flag;
	s32 n;
	s16 model;
	f32 t;
	f32 c;
	MGWARSPLAYER *p;

	sel = lbl_1_bss_4->list[lbl_1_bss_4->unk2];
	if (lbl_1_bss_0->unk10 & 1) {
		fn_1_83C(0x42, 60.0f, 0);
		fn_1_57C0(0, 0x78);
	} else if (lbl_1_bss_0->unk10 & 8) {
		fn_1_83C(0x42, 10.0f, 0);
		fn_1_57C0(0, 0);
	} else {
		fn_1_57C0(0, 0x78);
		fn_1_83C(0x42, 80.0f, 1);
		fn_1_9F4(0x3C);
	}
	while ((lbl_1_bss_0->unk10 & 0x200) == 0) {
		HuPrcVSleep();
	}
	if (sel < lbl_1_bss_0->gameCnt / 2) {
		sel = (s8)(sel + lbl_1_bss_0->gameCnt);
	}
	if (sel < 10) {
		sel += (10 / lbl_1_bss_0->gameCnt) * lbl_1_bss_0->gameCnt;
	}
	fn_1_6FA8(2, 2.0f);
	fn_1_7FC(fn_1_E674);
	while ((lbl_1_bss_0->unkC & 0x80) == 0) {
		HuPrcVSleep();
	}
	fn_1_568();
	model = MDL(0x24A);
	Hu3DModelAttrReset(model, 1);
	Hu3DModelScaleSet(model, 1.0f, 0.0f, 1.0f);
	HuAudFXPlay(0x4B7);
	i = 0x14;
	while (i--) {
		t = (f32)i / 20.0f;
		c = cos((3.141592653589793 * (f64)(90.0f * t)) / 180.0);
		t = sin(3.141592653589793 * (f64)c * 0.5);
		t = sin(3.141592653589793 * (f64)t * 0.5);
		Hu3DModelScaleSet(model, 1.0f, t, 1.0f);
		HuPrcVSleep();
	}
	end = sel + lbl_1_bss_0->gameCnt;
	for (i = 0; i <= end; i++) {
		HuAudFXPlay(0x4B8);
		if (lbl_1_bss_0->unk124 >= 0) {
			Hu3DAnimKill(lbl_1_bss_0->unk124);
		}
		lbl_1_bss_0->unk124 = Hu3DAnimCreate(lbl_1_bss_0->unkB8[i % lbl_1_bss_0->gameCnt], MDL(0x24A), "mg_shot");
		fn_1_9F4(5);
	}
	HuAudFXPlay(0x4B9);
	fn_1_6130();
	fn_1_7FC(fn_1_E6FC);
	while ((lbl_1_bss_0->unkC & 0x100) == 0) {
		HuPrcVSleep();
	}
	flag = 0;
	for (i = 0; i < 0x13; i++) {
		if (lbl_1_bss_4->owner[i] >= 0) {
			flag = ((lbl_1_bss_4->unk0 + 1) % 10) == 0;
			break;
		}
	}
	if (flag != 0) {
		fn_1_110C();
		fn_1_721C();
		Hu3DModelAttrReset(MDL(0x250), 1);
		fn_1_AD8(MDL(0x1DE), 181.0f, 300.0f);
		fn_1_189C(0x3AA, 0x78);
		i = 4;
		while (i--) {
			p = &lbl_1_bss_0->player[i];
			omVibrate(p->padNo, 0xF, 4, 2);
			fn_1_1778(p, 6);
			if (p->state != 1) {
				CharMotionShiftSet(p->charNo, p->motId[1], 0.0f, 15.0f, 0);
				p->state = 1;
			}
		}
		fn_1_7138(0xB4);
		fn_1_7FC(fn_1_E5E8);
		while ((lbl_1_bss_0->unkC & 0x40) == 0) {
			HuPrcVSleep();
		}
		fn_1_64C(0x4000);
		HuPrcVSleep();
		lbl_1_bss_4->unk0 = 0;
		lbl_1_bss_4->unk3 |= 2;
	} else {
		lbl_1_bss_4->unk3 &= ~2;
	}
	if (lbl_1_bss_4->unk0 + 1 >= 10) {
		lbl_1_bss_4->unk0 = 0;
	}
	if (lbl_1_bss_0->games[lbl_1_bss_4->list[lbl_1_bss_4->unk2]].is3D != 0) {
		lbl_1_bss_4->unk3 |= 1;
		return;
	}
	lbl_1_bss_4->unk3 &= ~1;
}

void fn_1_C19C(void)
{
	OMOVLHIS *his;
	s32 unlock;

	fn_1_7FC(fn_1_E784);
	while ((lbl_1_bss_0->unkC & 0x200) == 0) {
		HuPrcVSleep();
	}
	if (lbl_1_bss_0->unk10 & 2) {
		his = omOvlHisGet(0);
		omOvlHisChg(0, his->ovl, 3, his->stat);
		return;
	}
	fn_1_568();
	Hu3DModelAttrReset(MDL(0x258), 0x40000002);
	Hu3DModelAttrReset(MDL(0x258), 1);
	fn_1_83C(0x43, 90.0f, 2);
	fn_1_9F4(0x3C);
	if (lbl_1_bss_0->unk378 >= 0) {
		HuAudSStreamFadeOut(lbl_1_bss_0->unk378, 0x3E8);
		lbl_1_bss_0->unk378 = -1;
	}
	fn_1_404(lbl_1_bss_0->games[lbl_1_bss_4->list[lbl_1_bss_4->unk2]].mgIdx);
	fn_1_A84(6, 0x1E);
	while (lbl_1_bss_0->unk370 == 0) {
		HuPrcVSleep();
	}
	GwSystem.mgNo = lbl_1_bss_0->games[lbl_1_bss_4->list[lbl_1_bss_4->unk2]].unlock - 0x1F5;
	unlock = lbl_1_bss_0->games[lbl_1_bss_4->list[lbl_1_bss_4->unk2]].unlock;
	GWMgUnlockSet(unlock);
	GwSystem.mgNo = unlock - 0x1F5;
	lbl_1_bss_4->unk2 = lbl_1_bss_4->unk2 + 1;
	lbl_1_bss_4->unk0 = (s16)(lbl_1_bss_4->unk0 + 1);
	if (lbl_1_bss_0->unk10 & 4) {
		GwSystem.mgInstF = 1;
	} else {
		GwSystem.mgInstF = 0;
	}
	omOvlCallEx(DLL_instdll, 1, 0, 0);
	while (1) {
		HuPrcVSleep();
	}
}

void fn_1_C620(void)
{
	s32 cnt[4];
	s32 i;
	s8 j;
	s32 k;
	s32 w;
	MGWARSPLAYER *p;
	OMOVLHIS *his;

	cnt[3] = 0;
	cnt[2] = 0;
	cnt[1] = 0;
	cnt[0] = 0;
	i = 0x13;
	while (i--) {
		if (lbl_1_bss_4->owner[i] >= 0) {
			cnt[lbl_1_bss_4->owner[i]]++;
		}
	}
	lbl_1_bss_0->unk19 = cnt[0];
	for (i = 1; i < 4; i++) {
		if ((s8)lbl_1_bss_0->unk19 < cnt[i]) {
			lbl_1_bss_0->unk19 = cnt[i];
		}
	}
	lbl_1_bss_0->unk18 = 0;
	for (i = 0; i < 4; i++) {
		if ((s8)lbl_1_bss_0->unk19 == cnt[i]) {
			lbl_1_bss_0->unk14[(s8)lbl_1_bss_0->unk18] = i;
			lbl_1_bss_0->unk18 = lbl_1_bss_0->unk18 + 1;
		}
	}
	for (j = lbl_1_bss_0->unk18; j < 4; j++) {
	}
	i = 4;
	while (i--) {
		lbl_1_bss_0->camTarget[i] = lbl_1_data_B0[i];
		lbl_1_bss_0->camPos[i] = lbl_1_data_398[i];
		lbl_1_bss_0->camTarget[i].y += lbl_1_data_3C8[lbl_1_bss_0->player[i].charNo] + (CharModelHeightGet(lbl_1_bss_0->player[i].charNo) - 40.0f);
		lbl_1_bss_0->camPos[i].y += lbl_1_data_3C8[lbl_1_bss_0->player[i].charNo] + (CharModelHeightGet(lbl_1_bss_0->player[i].charNo) - 40.0f);
	}
	fn_1_83C(0x42, 80.0f, 2);
	fn_1_57C0(0, 0x78);
	fn_1_6FA8(0, 2.0f);
	HuAudFXPlay(0x10);
	fn_1_7FC(fn_1_EB5C);
	while ((lbl_1_bss_0->unkC & 0x4000) == 0) {
		HuPrcVSleep();
	}
	while (lbl_1_bss_0->unk360 != -1) {
		HuPrcVSleep();
	}
	fn_1_7FC(fn_1_EBB8);
	while ((lbl_1_bss_0->unkC & 0x8000) == 0) {
		HuPrcVSleep();
	}
	HuAudSStreamPauseFadeOut(lbl_1_bss_0->unk378, 1, 0x64);
	lbl_1_bss_0->unk37C = HuAudFXPlay(0x1F);
	lbl_1_bss_0->unk380 = HuAudFXPlay(0x21);
	fn_1_568();
	fn_1_13FC();
	fn_1_E8C();
	Hu3DModelAttrReset(MDL(0x250), 1);
	fn_1_6F54();
	fn_1_4E94();
	Hu3DModelAttrSet(MDL(0x1D8), 1);
	fn_1_9F4(0x78);
	fn_1_7FC(fn_1_EC1C);
	while ((lbl_1_bss_0->unkC & 0x10000) == 0) {
		HuPrcVSleep();
	}
	fn_1_668(0x400);
	HuPrcVSleep();
	HuPrcVSleep();
	fn_1_AD8(MDL(0x1DE), 181.0f, 300.0f);
	HuAudFXPlay(0x20);
	HuAudFXStop(lbl_1_bss_0->unk37C);
	HuAudFXStop(lbl_1_bss_0->unk380);
	if ((s8)lbl_1_bss_0->unk18 != 1) {
		fn_1_C04(0x45, 20, 2);
		fn_1_50DC();
		lbl_1_bss_0->unk10 |= 0x2000;
		Hu3DModelAttrSet(MDL(0x1D8), 1);
		Hu3DModelAttrSet(MDL(0x1E0), 1);
		j = lbl_1_bss_0->unk18;
		while (j--) {
			switch ((s8)lbl_1_bss_0->unk14[j]) {
			case 0:
			case 2:
				HuAudFXPlayPan(0x4C3, 0x28);
				break;
			case 1:
			case 3:
				HuAudFXPlayPan(0x4C3, 0x58);
				break;
			}
			Hu3DModelAttrReset(lbl_1_bss_0->models[(s8)lbl_1_bss_0->unk14[j] + 0x30], 0x40000003);
			Hu3DModelAttrReset(lbl_1_bss_0->models[(s8)lbl_1_bss_0->unk14[j] + 0x30], 1);
			Hu3DMotionTimeSet(lbl_1_bss_0->models[(s8)lbl_1_bss_0->unk14[j] + 0x30], 0.0f);
		}
		fn_1_189C(0x39D, -1);
		Hu3DModelAttrSet(MDL(0x250), 1);
		fn_1_9F4(0x1E);
		fn_1_7FC(fn_1_ECA0);
		while ((lbl_1_bss_0->unkC & 0x20000) == 0) {
			HuPrcVSleep();
		}
		fn_1_568();
		j = lbl_1_bss_0->unk18;
		while (j--) {
			p = &lbl_1_bss_0->player[(s8)lbl_1_bss_0->unk14[j]];
			fn_1_1778(p, 5);
			if (p->state != 1) {
				CharMotionShiftSet(p->charNo, p->motId[1], 0.0f, 15.0f, 0);
				p->state = 1;
			}
		}
		fn_1_11AC();
	} else {
		w = lbl_1_bss_0->unk14[0];
		fn_1_12AC(w);
		Hu3DModelAttrSet(MDL(0x1D8), 1);
		Hu3DModelAttrSet(MDL(0x1E0), 1);
		Hu3DModelAttrSet(lbl_1_bss_0->models[(s8)w + 0x2C], 0x40000001);
		lbl_1_bss_0->unk10 |= 0x2000;
		switch ((s8)w) {
		case 0:
		case 2:
			HuAudFXPlayPan(0x4C3, 0x28);
			break;
		case 1:
		case 3:
			HuAudFXPlayPan(0x4C3, 0x58);
			break;
		}
		fn_1_189C(0x39D, -1);
		Hu3DModelAttrReset(lbl_1_bss_0->models[(s8)w + 0x30], 0x40000003);
		Hu3DModelAttrReset(lbl_1_bss_0->models[(s8)w + 0x30], 1);
		Hu3DMotionTimeSet(lbl_1_bss_0->models[(s8)w + 0x30], 0.0f);
		Hu3DModelAttrSet(MDL(0x250), 1);
		fn_1_9F4(0x1E);
		fn_1_7FC(fn_1_ECA0);
		while ((lbl_1_bss_0->unkC & 0x20000) == 0) {
			HuPrcVSleep();
		}
		fn_1_568();
		p = &lbl_1_bss_0->player[(s8)w];
		if (p->state != 1) {
			CharMotionShiftSet(p->charNo, p->motId[1], 0.0f, 15.0f, 0);
			p->state = 1;
		}
		HuAudFXPlayPan(lbl_1_data_3FC[p->charNo][5], 0x40);
		fn_1_11AC();
		fn_1_C04((s8)w + 0x46, 30, 1);
		switch ((s8)w) {
		case 0:
			fn_1_57C0(1, 0x3C);
			break;
		case 1:
			fn_1_57C0(2, 0x1E);
			break;
		case 2:
			fn_1_57C0(3, 0x69);
			break;
		case 3:
			fn_1_57C0(4, 0x3C);
			break;
		}
	}
	j = 4;
	while (j--) {
		p = &lbl_1_bss_0->player[(s8)lbl_1_bss_0->unk14[j]];
		if (p->state != 1) {
			CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[2], 1);
			if (p->state != 2) {
				CharMotionShiftSet(p->charNo, p->motId[2], 0.0f, 15.0f, 0);
				p->state = 2;
			}
		}
	}
	fn_1_668(0x100);
	HuPrcVSleep();
	fn_1_9F4(0x12C);
	fn_1_7FC(fn_1_EED8);
	while ((lbl_1_bss_0->unkC & 0x40000) == 0) {
		HuPrcVSleep();
	}
	his = omOvlHisGet(0);
	lbl_1_bss_0->unk388 = 0;
	if (lbl_1_bss_0->unk378 >= 0) {
		HuAudSStreamFadeOut(lbl_1_bss_0->unk378, 0x3E8);
		lbl_1_bss_0->unk378 = -1;
	}
	if (lbl_1_bss_0->unk10 & 0x20) {
		omOvlHisChg(0, his->ovl, 0, his->stat);
		fn_1_568();
		fn_1_9F4(0x1E);
		fn_1_A84(6, 0x3C);
		if ((s8)lbl_1_bss_0->unk18 != 1) {
			fn_1_668(0x800);
			HuPrcVSleep();
		}
		fn_1_195C();
		j = 4;
		while (j--) {
			p = &lbl_1_bss_0->player[(s8)lbl_1_bss_0->unk14[j]];
			CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[1], 0);
			CharMotionVoiceOnSet(p->charNo, (s16)lbl_1_data_9C[2], 0);
		}
		j = 4;
		while (j--) {
			Hu3DModelAttrSet(lbl_1_bss_0->models[j + 0x2C], 0x40000003);
			Hu3DMotionTimeSet(lbl_1_bss_0->models[j + 0x2C], 0.0f);
		}
		j = lbl_1_bss_0->unk18;
		while (j--) {
			Hu3DModelAttrSet(lbl_1_bss_0->models[(s8)lbl_1_bss_0->unk14[j] + 0x2C], 0x40000003);
			Hu3DModelAttrSet(lbl_1_bss_0->models[(s8)lbl_1_bss_0->unk14[j] + 0x30], 1);
			Hu3DMotionTimeSet(lbl_1_bss_0->models[(s8)lbl_1_bss_0->unk14[j] + 0x30], 0.0f);
		}
		Hu3DModelAttrSet(MDL(0x24E), 0x40000002);
		Hu3DModelAttrSet(MDL(0x24E), 1);
		fn_1_AD8(MDL(0x1DE), 0.0f, 180.0f);
		fn_1_668(0x20);
		Hu3DModelAttrReset(MDL(0x1D8), 1);
	} else {
		omOvlHisChg(0, his->ovl, 3, his->stat);
	}
	fn_1_568();
}
