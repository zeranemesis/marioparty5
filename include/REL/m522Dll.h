#ifndef _M522DLL_H
#define _M522DLL_H

#include "game/audio.h"
#include "game/wipe.h"

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

/* The mode controller singleton, guarded by assert(ms_pointer != 0). */
extern s32 lbl_1_bss_18;

#endif