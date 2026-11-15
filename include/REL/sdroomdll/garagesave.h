#ifndef _SDROOMDLL_GARAGESAVE_H
#define _SDROOMDLL_GARAGESAVE_H

#include "REL/sdroomdll.h"

/* owned by garagesave.c (.bss 0x750..0x9A0) */
extern s32 lbl_1_bss_750; /* card access enabled flag */
extern s16 lbl_1_bss_754[5]; /* window ids of the backup browser */
extern s16 lbl_1_bss_75E; /* message window id */
extern SDMACHINE_DISP lbl_1_bss_770; /* machine shown on the backup stand */

void fn_1_13B1C(void);
void fn_1_13D08(void);
void fn_1_16024(void);
void fn_1_16064(void);
s16 fn_1_160A4(s16 errNo);
s32 fn_1_16AB0(s32 length, void *addr);
s32 fn_1_16B20(void);

#endif
