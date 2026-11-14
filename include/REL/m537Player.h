#ifndef _M537PLAYER_H
#define _M537PLAYER_H

#include "REL/m537Dll.h"
#include "game/flag.h"
#include <math.h>
#include <string.h>
#ifdef TARGET_PC
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Circular wrap range of an angle: [lo, hi) with period range, plus the current value
 * (fn_1_CD4C builds it from lo/hi/value, fn_1_BFE8 wraps a value into it). 0x10 bytes. */
typedef struct M537Wrap_s {
    f32 lo;     /* 0x00 */
    f32 hi;     /* 0x04 */
    f32 range;  /* 0x08 */
    f32 value;  /* 0x0C */
} M537_WRAP;

/* Ring of chunk pointers used by the deque (fn_1_C084 returns the slot of an index). */
typedef struct M537DequeMap_s {
    u32 cap;        /* 0x00 modulus of the ring */
    u32 count;      /* 0x04 chunks in use */
    u32 start;      /* 0x08 first used slot */
    u8 **data;      /* 0x0C slots, each pointing to one chunk of `chunk` bytes */
} M537_DEQUEMAP;

/* std::deque<bool> of the original, 0x1C bytes on GameCube. */
typedef struct M537Deque_s {
    u32 chunk;          /* 0x00 elements per chunk (0x200) */
    M537_DEQUEMAP map;  /* 0x04 */
    u32 offset;         /* 0x14 index of the first element inside the first chunk */
    u32 size;           /* 0x18 element count */
} M537_DEQUE;

struct M537Player;

/* One of the four players (0xC0 bytes on GameCube). Starts with its intrusive list node
 * (list head lbl_1_bss_A40). hook is called every frame by fn_1_A67C through __ptmf_scall. */
typedef struct M537Player {
    M537_NODE node;         /* 0x00 */
    s16 mdl;                /* 0x08 CModel */
    M537_PTMF hook;         /* 0x0C */
    s32 idx;                /* 0x18 player number */
    s32 charNo;             /* 0x1C */
    s32 pad;                /* 0x20 pad number, -1 for a CPU */
    s32 dif;                /* 0x24 CPU level */
    s32 aiBonus;            /* 0x28 */
    s32 vibCool;            /* 0x2C vibration cool down */
    s32 fallSec;            /* 0x30 seconds of fall time */
    f64 maxVal;             /* 0x38 */
    f64 midVal;             /* 0x40 */
    f64 minVal;             /* 0x48 */
    M537Dorrie *dorrie;     /* 0x50 */
    s32 timer;              /* 0x54 */
    u8 done;                /* 0x58 */
    s32 beatCnt;            /* 0x5C pressed frames inside the last FPS frames */
    M537_DEQUE beats;       /* 0x60 */
    Vec pos;                /* 0x7C */
    Vec vel;                /* 0x88 */
    f64 cur;                /* 0x98 */
    f64 smooth;             /* 0xA0 */
    f32 rotY;               /* 0xA8 */
    M537_VECS16 mots;       /* 0xAC std::vector<s16> of motion ids */
    s32 motion;             /* 0xB8 current motion index */
    f32 motTime;            /* 0xBC */
} M537Player;

#ifndef TARGET_PC
typedef char M537Player_size_check[(sizeof(M537Player) == 0xC0) ? 1 : -1];
#endif

extern M537_NODE lbl_1_bss_A40;
extern s32 lbl_1_bss_A48;

/* Other units of the module. */
M537Dorrie *fn_1_96CC(M537Stage *self, s32 index);
void fn_1_255C(s32 playerId);
void fn_1_6278(M537Dorrie *self);
void fn_1_672C(M537Dorrie *self);
MtxPtr fn_1_6558(M537Dorrie *self, s32 part, Mtx mtx);
void fn_1_66D0(M537Dorrie *self, f64 target, f64 duration);
s16 *fn_1_69E0(M537_VECS16 *vec, u32 index);
void fn_1_6D0C(M537_VECS16 *vec, u16 *pos, u32 count, s16 *value);
f32 fn_1_145A0(M537Water *self, Vec *pos);
void fn_1_14764(M537Water *self, Vec *pos, f32 amount);
M537Arm *fn_1_102F0(M537Center *self, s32 index);
void fn_1_10300(M537Center *self);
void fn_1_10BB0(M537Arm *self, Vec *target);

/* Part A (player.cpp). */
s16 fn_1_9930(Vec *pos);
void fn_1_99D0(void);
void fn_1_9ACC(void);
void fn_1_9B64(void);
void fn_1_9BFC(void);
void fn_1_9D74(void);
void fn_1_9EE4(void);
Vec *fn_1_9F88(Vec *out);
s32 fn_1_A0C4(void);
s32 fn_1_A0D4(void);
M537Player *fn_1_A164(M537Player *self, s32 idx, M537Dorrie *dorrie);
M537_DEQUE *fn_1_A3FC(M537_DEQUE *self, s16 freeIt);
M537_DEQUE *fn_1_A490(M537_DEQUE *self, s16 freeIt);
M537_NODE *fn_1_A51C(M537_NODE *self, s16 freeIt);
M537Player *fn_1_A574(M537Player *self, s16 freeIt);
void fn_1_A67C(M537Player *self);
void fn_1_A7E0(M537Player *self);
void fn_1_AB34(void);
void fn_1_AC88(void);
void fn_1_AE1C(M537Player *self);
void fn_1_AE98(M537Player *self);
void fn_1_AED4(M537Player *self);
void fn_1_B0F8(M537Player *self);
void fn_1_B2D8(M537Player *self);
void fn_1_B478(M537Player *self);
void fn_1_B4DC(M537Player *self);
void fn_1_B4E0(M537Player *self, u8 reset);
u8 fn_1_B66C(s32 pad);
u8 fn_1_B6AC(s32 n);
u8 fn_1_B71C(M537Player *self);
void fn_1_B80C(M537Player *self);
void fn_1_B8B8(M537Player *self, f64 amount);
u8 fn_1_B9D8(M537Player *self);
void fn_1_B9F0(M537Player *self);
Vec *fn_1_BA98(M537Player *self, Vec *out);
void fn_1_BB18(M537Player *self);
void fn_1_BC2C(M537Player *self, s32 dataNum);
void fn_1_BCA4(M537Player *self, s32 motion, u8 shift);
f32 fn_1_BDA4(M537Player *self);
void fn_1_BE10(M537Player *self, s32 kind);
void fn_1_BED0(M537Player *self, s32 seId);
f32 fn_1_BF8C(M537_WRAP *self, f32 delta, f64 scale);
f32 fn_1_BFE8(M537_WRAP *self, f32 value);

/* Part B (player_b.inc). */
u8 **fn_1_C084(M537_DEQUEMAP *map, u32 index);
void fn_1_C0B0(M537_NODE *list, void (*fn)(M537Player *));
void fn_1_C170(M537_DEQUE *self);
void fn_1_C420(M537_DEQUE *self);
void fn_1_C54C(M537_DEQUE *self, u8 *value);
M537_DEQUE *fn_1_C8A4(M537_DEQUE *self, M537_DEQUE *src);
void fn_1_CBCC(M537_DEQUE *self, void *alloc);
void fn_1_CC24(M537_VECS16 *self, u32 count);
M537_WRAP *fn_1_CD4C(M537_WRAP *self, f32 lo, f32 hi, f32 value);
f32 fn_1_CEB0(M537_WRAP *self, f32 target);
/* Part B, rest of the deque / cdeque template and the static init. */
void fn_1_C110(M537_NODE *first, M537_NODE *end, void (*fn)(M537Player *));
void *fn_1_C7C4(M537_DEQUEMAP *self, s16 freeIt);
void *fn_1_C838(M537_DEQUEMAP *self, s16 freeIt);
void fn_1_CEEC(M537_DEQUEMAP *self);
void fn_1_CF28(M537_DEQUEMAP *self, u8 *const *val);
void fn_1_DD3C(M537_DEQUEMAP *self, u8 *const *val);
f32 fn_1_E320(M537_WRAP *self, f32 from, f32 to);
u32 fn_1_FE0C(M537_DEQUEMAP *self);
void fn_1_FE6C(void);
void *fn_1_FEF4(void *self, s16 freeIt);
extern s32 lbl_1_bss_A28;
extern s32 lbl_1_bss_A2C;
extern s32 lbl_1_bss_A30;
extern u8 lbl_1_bss_A4C;

#ifdef __cplusplus
}
#endif

#endif
