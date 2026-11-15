#ifndef _SDROOMDLL_SDROOM_H
#define _SDROOMDLL_SDROOM_H

#include "REL/sdroomdll.h"

/* sdroom.c bss (module bss 0x000..0x568) */
extern s16 lbl_1_bss_0;
extern s32 lbl_1_bss_4; /* free camera debug flag */
extern s32 lbl_1_bss_8; /* garage intro sprite hold flag */
extern GWCOMMON lbl_1_bss_10; /* scratch copy of a saved GwCommon */

/* bss defined in garage.c (0x568..0x750) */
extern s16 lbl_1_bss_568[2]; /* light ids */
extern s16 lbl_1_bss_56C; /* selected menu entry */
extern s16 lbl_1_bss_56E; /* hint window */
extern s16 lbl_1_bss_570; /* main message window */
extern s16 lbl_1_bss_584[3]; /* mascot motions */
extern s16 lbl_1_bss_58A; /* mascot model */
extern s16 lbl_1_bss_58C[16]; /* room models */
extern OMOBJMAN *lbl_1_bss_5AC;

void ObjectSetup(void);

#endif
