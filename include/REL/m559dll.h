#ifndef _M559DLL_H
#define _M559DLL_H

#include "dolphin.h"
#include "game/object.h"

// 3 contiguous floats. NOT HuVecF/Vec: HuVecF is 16-byte aligned and would
// shift every following offset of the M559 object layouts below.
typedef struct M559Vec_s {
    f32 x;
    f32 y;
    f32 z;
} M559VEC; // size = 0xC

// a vertex + texcoord pair as handed to GXSetArray (3 x vec4)
typedef struct M559Vec12_s {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} M559VEC12; // size = 0x20

// 4 bytes of colour
typedef struct M559Clr_s {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} M559CLR; // size = 0x4

// per-part record owned by an M559HOOKDATA (HuMemDirectMallocNum 0x40 * n)
typedef struct M559AnimPart_s {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
} M559ANIMPART; // size = 0x40

// object data of the fn_1_694 player hook objects (HuMemDirectMallocNum size 0x8C)
typedef struct M559Hook_s {
    union {
        s16 state;
        s16 unk0;
    }; // 0x00
    s16 pad2;         // 0x02
    union {
        M559VEC offset;
        struct {
            f32 unk4;
            f32 unk8;
            f32 unkC;
        };
    };                // 0x04
    s32 field10;      // 0x10
    union {
        s16 hookModel;
        s16 unk12;
    };                // 0x12
    union {
        s32 playerNo;
        s32 unk14;
    };                // 0x14
    s32 charNo;       // 0x18
    s32 padNo;        // 0x1C
    union {
        s32 mode;
        s32 unk20;
    };                // 0x20
    union {
        s32 timer;
        s32 unk24;
    };                // 0x24
    union {
        M559VEC pos;
        struct {
            f32 unk28;
            f32 unk2C;
            f32 unk30;
        };
    };                // 0x28
    M559VEC prevPos;  // 0x34
    union {
        M559VEC rot;
        struct {
            f32 unk40;
            f32 unk44;
            f32 unk48;
        };
    };                // 0x40
    union {
        s32 partCount;
        f32 unk4C;
    };                // 0x4C
    union {
        M559VEC offset50;
        struct {
            f32 unk50;
            f32 unk54;
            f32 unk58;
        };
    };                // 0x50
    union {
        f32 field5C;
        f32 unk5C;
    };                // 0x5C
    union {
        f32 stickX;
        f32 unk60;
    };                // 0x60
    union {
        f32 stickY;
        f32 unk64;
    };                // 0x64
    s16 pad66;        // 0x66
    union {
        s16 pad68;
        s16 unk68;
    };                // 0x68
    s8 pad6A;         // 0x6A
    s8 pad6B;         // 0x6B
    s32 dif;          // 0x6C
    s32 subMode;      // 0x70
    M559VEC target;   // 0x74
    s32 timer2;       // 0x80
    s32 timer3;       // 0x84
    s32 waitTime;     // 0x88
} M559HOOK; // size = 0x8C

// object data of the fn_1_3068 objects (size 0x2C)
typedef struct M559Nod_s {
    s16 state;        // 0x00
    s16 pad2;         // 0x02
    M559VEC pos;      // 0x04
    f32 field10;      // 0x10
    f32 rotY;         // 0x14
    f32 field18;      // 0x18
    s32 count;        // 0x1C
    s32 mode;         // 0x20
    s32 timer;        // 0x24
    s32 arg;          // 0x28
} M559NOD; // size = 0x2C

// object data of the fn_1_4364 / fn_1_4A9C objects (size 0x58)
typedef struct M559Head_s {
    s16 state;        // 0x00
    s16 modelId;      // 0x02
    M559VEC pos;      // 0x04
    f32 field10;      // 0x10
    s16 pad12;        // 0x12
    s16 pad14;        // 0x14
    s32 field20;      // 0x20
    s32 field28;      // 0x28
    s32 field30;      // 0x30
    union {
        s32 field38;
        s32 unk38;
    };                // 0x38
    union {
        f32 field40;
        f32 unk40;
    };                // 0x40
    union {
        s32 field44;
        s32 unk44;
    };                // 0x44
    union {
        s32 field48;
        s32 unk48;
    };                // 0x48
    union {
        s32 field4C;
        s32 unk4C;
    };                // 0x4C
    union {
        M559VEC field50;
        struct {
            f32 unk50;
            f32 unk54;
            f32 unk58;
        };
    };                // 0x50
    union {
        s32 field5C;
        s32 unk5C;
    };                // 0x5C
} M559HEAD; // size = 0x58

// Hu3DData[].hookData of a model created by fn_1_7368 (size 0x6C)
typedef struct M559HookData_s {
    s32 state;            // 0x00
    union {
        ANIMDATA *anim;
        ANIMDATA *unk4;
    };                    // 0x04
    s32 field8;           // 0x08
    union {
        f32 *fieldC;
        f32 *unkC;
    };                    // 0x0C
    M559ANIMPART *unk10;      // 0x10
    f32 *unk14;           // 0x14  GX_VA_POS array (0xC floats per part)
    u32 *unk18;           // 0x18  GX_VA_TEX0 array (8 u32 per part)
    void *unk1C;          // 0x1C  drawn coordinate array
    M559CLR *unk20;       // 0x20  GX_VA_CLR0 array
    void *unk24;          // 0x24  display list
    union {
        s32 arg64;
        s32 unk64;
    };                    // 0x64
    s16 arg66;            // 0x66
    s16 pad68;            // 0x68
    union {
        s8 flag6A;
        s8 unk6A;
    };                    // 0x6A
    union {
        s8 flag6B;
        s8 unk6B;
    };                    // 0x6B
} M559HOOKDATA; // size = 0x6C

// node of the lbl_1_bss_BB8 list (size 0xC)
typedef struct M559ListNode_s {
    union {
        s16 id;
        f32 unk0;
    }; // 0x00
    union {
        s16 value;
        s16 unk2;
    }; // 0x02
    union {
        struct M559ListNode_s *prev;
        f32 unk4;
    }; // 0x04
    union {
        struct M559ListNode_s *next;
        f32 unk8;
    }; // 0x08
} M559LISTNODE; // size = 0xC

void fn_1_694(OMOBJ *obj);
void fn_1_A5C(OMOBJ *obj);
void fn_1_F48(OMOBJ *obj);

#endif
