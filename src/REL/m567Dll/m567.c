#include "game/object.h"

/*************************************************************
 * m567.c
 *
 * "567" overlay (MP5 board/mini-game runtime object).
 *
 * The original overlay shipped no symbol table for its internal
 * helpers, so most of them carry objdiff's placeholder names
 * (fn_1_XXXX) keyed to their .text offset.
 *
 * Only the module entry points and the smallest helpers are
 * reconstructed so far; the remaining fn_1_XXXX bodies are still
 * missing.
 *************************************************************/

/* lbl_1_bss_4 holds the OMOBJ registered by ObjectSetup (obj id 0x65).
 * OMOBJ.data (offset 0x5C) points at that object's private state block,
 * a 0xD8C-byte heap allocation made by HuMemDirectMallocNum. */
extern OMOBJ *lbl_1_bss_4;

/* The per-object state block. Its full layout is not recovered yet;
 * only its size and the address arithmetic performed on it are known. */
typedef struct M567Work_s M567Work;

/* Empty hook: called with the sub-object living at M567Work + 0xBC0. */
void fn_1_4164(void *sub);

void fn_1_371C(void);

void fn_1_3990(void);

void fn_1_3B74(void);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

/*************************************************************
 * module entry / teardown (MWCC crt stubs)
 *************************************************************/
int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/*************************************************************
 * fn_1_371C
 *
 * Runs the empty hook on the sub-object at M567Work + 0xBC0.
 * fn_1_1FC and fn_1_39F8 hand the same address to fn_1_3E94 /
 * fn_1_3EEC, so 0xBC0 is that sub-object's offset inside the work
 * block.
 *************************************************************/
void fn_1_371C(void) {
    u8 *work = (u8 *)lbl_1_bss_4->data;
    fn_1_4164(work + 0xbc0);
}

/*************************************************************
 * fn_1_3990 / fn_1_3B74 / fn_1_4164
 *
 * These three really are empty in the original object: each is a
 * single 0x4-byte `blr` with no frame, so there is no logic to
 * write. fn_1_4164 is reached from fn_1_371C and does nothing with
 * its argument; fn_1_3990 and fn_1_3B74 are never referenced at all
 * (not even from .data).
 *************************************************************/
void fn_1_3990(void) {
}

void fn_1_3B74(void) {
}

void fn_1_4164(void *sub) {
    (void)sub;
}