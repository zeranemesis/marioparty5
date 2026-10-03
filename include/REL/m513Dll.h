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

/* MWCC pointer-to-member-function triple, as laid out by src/REL/ptmf.c. */
typedef struct M513PTMF_s {
    long this_delta;
    long v_offset;
    union {
        void *f_addr;
        long ve_offset;
    } f_data;
} M513_PTMF;

long __ptmf_test(const M513_PTMF *);
void __ptmf_scall(void *);

/* The stage singleton, and the process work list its nodes are spliced into. */
extern M513_OBJHEAD *lbl_1_bss_A1C;
extern M513_OBJHEAD lbl_1_bss_C;

/* Base and derived vtables of the stage object. */
extern void *lbl_1_data_218[12];
extern void *lbl_1_data_78C[5];

/* Assertion helper, singleton.h */
void fn_1_1D4(char *expr, char *file, int line);

/* Work list node: self-links on construction, splices in / unlinks out. */
void fn_1_1768(M513_OBJHEAD *);
void fn_1_1774(M513_OBJHEAD *, int);
void fn_1_1834(M513_OBJHEAD *, M513_OBJHEAD *);

/* HuMemDirectFree guarded against a null pointer. */
void fn_1_C8(void *);

/* Model load / kill for the s16 model ids kept in the stage and camera. */
void fn_1_3198(s16 *, u32);
void fn_1_35F8(s16 *, int);

#ifdef __cplusplus
}
#endif

#endif