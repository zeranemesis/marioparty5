#ifndef _M522DLL_H
#define _M522DLL_H

#include "types.h"

/* The mode controller of the main board, CGameMaster.  ObjectSetup() builds it
 * in a 0x368 byte allocation, so only the members this module touches need to
 * be spelled out. */
typedef struct M522Core_s {
    /* 0x000 */ void *m_vtable;
    /* 0x004 */ u8 m_body[0x334];
} M522_CORE;

typedef struct M522Queue_s {
    /* 0x000 */ s32 m_opaque[1];
} M522_QUEUE;

typedef struct M522HeapSel_s {
    /* 0x000 */ u8 m_opaque[0xC];
} M522_HEAPSEL;

typedef struct M522GameMaster_s {
    /* 0x000 */ s32 m_field00;
    /* 0x004 */ void *m_vtable;
    /* 0x008 */ s32 m_players[4];
    /* 0x018 */ s32 m_field18;
    /* 0x01C */ s32 m_field1C;
    /* 0x020 */ M522_CORE m_core;
    /* 0x358 */ M522_QUEUE m_queue;
    /* 0x35C */ M522_HEAPSEL m_heapSel;
} M522_GAMEMASTER;

typedef struct M522Msg_s {
    u8 m_opaque[0x1C];
} M522_MSG;

typedef struct M522Unk48_s {
    u8 m_opaque[0x48];
} M522_UNK48;

typedef struct M522UnkAC_s {
    u8 m_opaque[0xAC];
} M522_UNKAC;

/* The mode controller singleton, guarded by assert(ms_pointer != 0).  It is
 * handed to fn_1_F194 and fn_1_F1F8 by value, which is where the copy into a
 * second register comes from. */
extern void *lbl_1_bss_18;

/* Doubly linked list node used by every object of the module (fn_1_8004 and
 * friends).  The first word is the one the list walkers follow. */
typedef struct M522Node_s {
    /* 0x0 */ struct M522Node_s *prev;
    /* 0x4 */ struct M522Node_s *next;
} M522_NODE;

/* Virtual table slot, cast to the real signature at every call site. */
typedef void (*M522_VTFN)(void);

/* Base of the effect objects (fn_1_E44C): list node, hook model id, vtable. */
typedef struct M522Effect_s {
    /* 0x00 */ M522_NODE node;
    /* 0x08 */ s16 hook;
    /* 0x0C */ M522_VTFN *vtbl;
    /* 0x10 */ s32 handle;
} M522_EFFECT;

/* CodeWarrior pointer to member function: {this adjust, vtable offset (-1 for a
 * plain member), function}.  Only the plain form is used by the module. */
typedef struct M522Ptmf_s {
    s32 this_delta;
    s32 v_offset;
    union {
        void *f_addr;
        s32 ve_offset;
    } f_data;
} M522_PTMF;

/* Plain float triple (Vec without pulling dolphin/mtx.h into the unit). */
typedef struct M522Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M522_VEC3;

#endif
