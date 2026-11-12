#ifdef __MWERKS__
#define _MATH_H
#endif

#include "game/gamework.h"
#include "game/process.h"
#include "game/window.h"
#include "game/audio.h"
#include "game/object.h"

#ifdef __MWERKS__
extern int sprintf();
#else
#include <stdio.h>
#endif

extern s16 fn_1_50C(void);
extern void fn_1_520(void);
extern s8 fn_1_5C4(void);
extern s16 fn_1_5D8(void);
extern s8 fn_1_5EC(void *dst);
extern s8 fn_1_638(void);
extern void fn_1_64C(u32 mask);
extern void fn_1_668(u32 mask);
extern u32 fn_1_684(u32 mask);
extern void fn_1_6F4(u32 mask);
extern void fn_1_728(u32 mask);

s8 lbl_1_data_658[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9 };

void fn_1_E2D0(void)
{
	s16 win;
	s32 choice;

	win = fn_1_50C();
	fn_1_728(1);
	fn_1_520();
	HuAudFXPlayPan(0x2F5, 0x40);
	HuWinMesSet(win, 0x110000);
	HuWinMesWait(win);
	choice = HuWinChoiceGet(win, -1);
	if (choice == 0) {
		HuWinMesSet(win, 0x11001B);
		HuWinMesWait(win);
	}
	fn_1_6F4(1);
	HuPrcEnd();
}

void fn_1_E36C(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(2);
	fn_1_520();
	HuWinMesSet(win, 0x11000A);
	HuWinMesWait(win);
	fn_1_6F4(2);
	HuPrcEnd();
}

void fn_1_E3C8(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(4);
	fn_1_520();
	HuWinMesSet(win, 0x11000B);
	HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[0].charNo], 0);
	HuWinMesWait(win);
	fn_1_6F4(4);
	HuPrcEnd();
}

void fn_1_E450(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(8);
	fn_1_520();
	HuWinMesSet(win, 0x11000C);
	HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[1].charNo], 1);
	HuWinMesWait(win);
	fn_1_6F4(8);
	HuPrcEnd();
}

void fn_1_E4D8(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x10);
	fn_1_520();
	HuWinMesSet(win, 0x11000D);
	HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[2].charNo], 2);
	HuWinMesWait(win);
	fn_1_6F4(0x10);
	HuPrcEnd();
}

void fn_1_E560(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x20);
	fn_1_520();
	HuWinMesSet(win, 0x11000E);
	HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[3].charNo], 3);
	HuWinMesWait(win);
	fn_1_6F4(0x20);
	HuPrcEnd();
}

void fn_1_E5E8(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x40);
	fn_1_520();
	HuWinMesSet(win, 0x110012);
	HuWinMesWait(win);
	HuWinMesSet(win, 0x110013);
	HuWinMesWait(win);
	HuWinMesSet(win, 0x110014);
	HuWinMesWait(win);
	fn_1_6F4(0x40);
	HuPrcEnd();
}

void fn_1_E674(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x80);
	fn_1_520();
	if (fn_1_684(1) != 0) {
		HuWinMesSet(win, 0x110001);
		HuWinMesWait(win);
	} else {
		HuWinMesSet(win, 0x110008);
		HuWinMesWait(win);
	}
	fn_1_6F4(0x80);
	HuPrcEnd();
}

void fn_1_E6FC(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x100);
	fn_1_520();
	if (fn_1_684(1) != 0) {
		HuWinMesSet(win, 0x110002);
		HuWinMesWait(win);
	} else {
		HuWinMesSet(win, 0x110009);
		HuWinMesWait(win);
	}
	fn_1_6F4(0x100);
	HuPrcEnd();
}

void fn_1_E784(void)
{
	s16 win;
	s32 choice;
	s32 retry;

	win = fn_1_50C();
	fn_1_728(0x200);
	fn_1_668(6);
	fn_1_520();
	do {
		retry = 0;
		HuWinMesSet(win, 0x110003);
		HuWinMesWait(win);
		choice = HuWinChoiceGet(win, -1);
		if (choice < 0) {
			choice = 1;
		}
		if (choice == 1) {
			HuWinMesSet(win, 0x110004);
			HuWinMesWait(win);
			if (HuWinChoiceGet(win, -1) == 0) {
				fn_1_64C(2);
			} else {
				retry = 1;
			}
		} else if (choice == 2) {
			fn_1_64C(4);
		}
	} while (retry != 0);
	fn_1_6F4(0x200);
	HuPrcEnd();
}

void fn_1_E87C(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x400);
	fn_1_520();
	HuAudFXPlayPan(0x2F6, 0x40);
	if (fn_1_684(0x10) != 0) {
		HuWinMesSet(win, 0x11001E);
	} else {
		HuWinMesSet(win, 0x110005);
	}
	HuWinMesWait(win);
	fn_1_6F4(0x400);
	HuPrcEnd();
}

void fn_1_E908(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x800);
	fn_1_520();
	HuWinMesSet(win, 0x110006);
	HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[fn_1_5C4()].charNo], 0);
	HuWinMesWait(win);
	fn_1_6F4(0x800);
	HuPrcEnd();
}

void fn_1_E9A0(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x1000);
	fn_1_520();
	HuWinMesSet(win, 0x110007);
	HuWinMesWait(win);
	fn_1_6F4(0x1000);
	HuPrcEnd();
}

void fn_1_E9FC(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x2000);
	fn_1_520();
	if (fn_1_684(0x10) != 0) {
		HuWinMesSet(win, 0x110010);
		HuWinMesWait(win);
	} else {
		HuWinMesSet(win, 0x11000F);
		HuWinMesWait(win);
	}
	fn_1_6F4(0x2000);
	HuPrcEnd();
}

void fn_1_EA84(void)
{
	char buf[24];
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x80000);
	fn_1_520();
	sprintf(buf, "%d", fn_1_5D8());
	switch (fn_1_5C4()) {
	case 0:
	case 1:
	case 2:
	case 3:
		break;
	}
	HuWinMesSet(win, 0x110011);
	HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[fn_1_5C4()].charNo], 0);
	HuWinInsertMesSet(win, MESSNUM_PTR(buf), 1);
	HuWinMesWait(win);
	fn_1_6F4(0x80000);
	HuPrcEnd();
}

void fn_1_EB5C(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x4000);
	fn_1_520();
	HuWinMesSet(win, 0x110015);
	HuWinMesWait(win);
	fn_1_6F4(0x4000);
	HuPrcEnd();
}

void fn_1_EBB8(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x8000);
	fn_1_520();
	HuWinMesSet(win, 0x110016);
	HuWinMesWait(win);
	fn_1_6F4(0x8000);
	HuPrcEnd();
}

void fn_1_EC1C(void)
{
	char buf[24];
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x10000);
	fn_1_520();
	sprintf(buf, "%2d", fn_1_638());
	HuWinMesSet(win, 0x110017);
	HuWinInsertMesSet(win, MESSNUM_PTR(buf), 0);
	HuWinMesWait(win);
	fn_1_6F4(0x10000);
	HuPrcEnd();
}

void fn_1_ECA0(void)
{
	u8 order[4];
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x20000);
	fn_1_520();
	HuAudFXPlayPan(0x2F4, 0x40);
	switch (fn_1_5EC(order)) {
	case 1:
		HuWinMesSet(win, 0x110018);
		HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[(s8)order[0]].charNo], 0);
		HuWinMesWait(win);
		break;
	case 2:
		HuWinMesSet(win, 0x11001C);
		HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[(s8)order[0]].charNo], 0);
		HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[(s8)order[1]].charNo], 1);
		HuWinMesWait(win);
		break;
	case 3:
		HuWinMesSet(win, 0x11001D);
		HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[(s8)order[0]].charNo], 0);
		HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[(s8)order[1]].charNo], 1);
		HuWinInsertMesSet(win, lbl_1_data_658[GwPlayerConf[(s8)order[2]].charNo], 2);
		HuWinMesWait(win);
		break;
	}
	fn_1_6F4(0x20000);
	HuPrcEnd();
}

void fn_1_EED8(void)
{
	s16 win;

	win = fn_1_50C();
	fn_1_728(0x40000);
	fn_1_520();
	HuWinMesSet(win, 0x110019);
	HuWinMesWait(win);
	if (HuWinChoiceGet(win, -1) == 0) {
		fn_1_64C(0x20);
	} else {
		HuWinMesSet(win, 0x11001A);
		HuWinMesWait(win);
	}
	fn_1_6F4(0x40000);
	HuPrcEnd();
}
