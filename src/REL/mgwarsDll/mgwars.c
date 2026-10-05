#include "game/gamework.h"
#include "game/process.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

void fn_1_4BC4(void);
void fn_1_64C(u32 mask);
void fn_1_668(u32 mask);
u32 fn_1_684(u32 mask);
void fn_1_8C8(void);
void fn_1_F14(void);
void fn_1_FD0(void);
void fn_1_108C(void);
void fn_1_1148(void);

/* Minigame work pointer (lbl_1_bss_0) */
typedef struct MgWarsWork_s {
	u8 pad0[0x6];
	s16 unk6;
	u8 unk8;
	u8 pad9;
	s16 unkA;
	s32 unkC;
	s32 unk10;
	u8 pad14[0x4];
	u8 unk18;
	u8 unk19;
} MGWARSWORK;

/* Common/minigame-manager work pointer (lbl_1_bss_4) */
typedef struct MgWarsCom_s {
	u8 pad0[0x3];
	u8 unk3;
} MGWARSCOM;

MGWARSWORK *lbl_1_bss_0;
MGWARSCOM *lbl_1_bss_4;

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

void fn_1_4E94(void)
{
	lbl_1_bss_0->unk10 |= 0x400;
	HuPrcChildCreate(fn_1_4BC4, 0x64, 0x1000, 0, HuPrcCurrentGet());
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
	memcpy(dst, &lbl_1_bss_0->pad14[0], 4);
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

void fn_1_9F4(s32 frames)
{
	while (frames--) {
		HuPrcVSleep();
	}
}

void fn_1_F94(void)
{
	HuPrcChildCreate(fn_1_F14, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_1050(void)
{
	HuPrcChildCreate(fn_1_FD0, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_110C(void)
{
	HuPrcChildCreate(fn_1_108C, 0x64, 0x1000, 0, HuPrcCurrentGet());
}

void fn_1_11AC(void)
{
	HuPrcChildCreate(fn_1_1148, 0x64, 0x1000, 0, HuPrcCurrentGet());
}
