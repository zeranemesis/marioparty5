#ifndef _M513DLL_H
#define _M513DLL_H

#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "datanum/m513.h"

#ifdef __cplusplus
extern "C" {
#endif

/* m513Dll - "Taimatsu" (the doll minigame).
 *
 * Every process work structure in this module starts with this header: the
 * object links itself into the module wide work list at construction time
 * (fn_1_1768 makes the two list pointers self referential, fn_1_1834 splices
 * the node in) and unlinks at destruction time (fn_1_1774).
 */
typedef struct M513ObjHead_s {
    /* 0x00 */ struct M513ObjHead_s *next;
    /* 0x04 */ struct M513ObjHead_s *prev;
} M513_OBJHEAD;

typedef void (*M513_VOIDFN)(void);

/* The stage singleton, and the process work list its nodes are spliced into. */
extern M513_OBJHEAD *lbl_1_bss_A1C;
extern M513_OBJHEAD lbl_1_bss_C;

/* Base and derived vtables of the stage object. */
extern void *lbl_1_data_218[12];
extern void *lbl_1_data_78C[5];

/* Assertion helper, singleton.h */
void fn_1_1D4(char *expr, char *file, int line);

#ifdef __cplusplus
}
#endif

#endif