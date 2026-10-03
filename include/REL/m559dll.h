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

// a 4-byte slot the code reaches through both the integer and the float view
typedef union M559Slot_s {
    s32 i;
    u32 u;
    f32 f;
} M559SLOT;

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
        s16 modelId;
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
    union {
        M559VEC pos;
        struct {
            f32 unk4;
            f32 unk8;
            f32 unkC;
        };
    };                // 0x04
    union {
        M559VEC start;
        struct {
            f32 unk10;
            f32 unk14;
            f32 unk18;
        };
    };                // 0x10
    union {
        M559SLOT field1C;
        M559SLOT unk1C;
    };               // 0x1C  // 0x1C
    union {
        M559SLOT field20;
        M559SLOT unk20;
    };               // 0x20  // 0x20
    union {
        M559SLOT field24;
        M559SLOT unk24;
    };               // 0x24  // 0x24
    union {
        M559SLOT field28;
        M559SLOT unk28;
    };               // 0x28  // 0x28
    union {
        M559SLOT field2C;
        M559SLOT unk2C;
    };               // 0x2C  // 0x2C
    union {
        M559SLOT field30;
        M559SLOT unk30;
    };               // 0x30  // 0x30
    union {
        M559SLOT field38;
        M559SLOT unk38;
    };               // 0x38  // 0x38
    union {
        M559SLOT field40;
        M559SLOT unk40;
    };               // 0x40  // 0x40
    union {
        M559SLOT field44;
        M559SLOT unk44;
    };               // 0x44  // 0x44
    union {
        M559SLOT field48;
        M559SLOT unk48;
    };               // 0x48  // 0x48
    union {
        M559SLOT field4C;
        M559SLOT unk4C;
    };               // 0x4C  // 0x4C
    union {
        M559SLOT field50;
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
    };               // 0x50  // 0x50
    union {
        M559SLOT field5C;
        M559SLOT unk5C;
    };               // 0x5C  // 0x5C
} M559HEAD; // size = 0x58

// per-part drawing callback stored in a part slot's 0x00 word
typedef void (*M559PARTCALLBACK)(HU3DMODEL *, void *, f32 (*)[4]);

// Hu3DData[].hookData of a model created by fn_1_7368 (size 0x6C)
typedef struct M559HookData_s {
    union {
        s32 state;       // 0x00
        M559PARTCALLBACK unk0;
    };                    // 0x00
    union {
        ANIMDATA *anim;  // 0x04
        ANIMDATA *unk4;
    };                    // 0x04
    union {
        M559VEC *field8;    // 0x08
        M559VEC *unk8;
    };                    // 0x08
    union {
        f32 *fieldC;        // 0x0C
        f32 *unkC;
    };                    // 0x0C
    union {
        M559ANIMPART *unk10;  // 0x10
        M559ANIMPART *partIdx;
    };                    // 0x10
    union {
        f32 *unk14;       // 0x14  GX_VA_POS array (0xC floats per part)
        f32 *partPos;
    };                    // 0x14
    union {
        u32 *unk18;       // 0x18  GX_VA_TEX0 array (8 u32 per part)
        u32 *partAttr;
    };                    // 0x18
    union {
        f32 *unk1C;       // 0x1C  drawn coordinate array (2 floats per entry)
        f32 *field1C;
    };                    // 0x1C
    union {
        M559CLR *unk20;   // 0x20  GX_VA_CLR0 array
        M559CLR *field20;
    };                    // 0x20
    union {
        void *unk24;      // 0x24  display list
        void *field24;
    };                    // 0x24
    union {
        f32 mtx28[4][3];  // 0x28  4x3 matrix (12 floats)
        struct {
            f32 field28;  // 0x28
            f32 field2C;  // 0x2C
            f32 field30;  // 0x30
            f32 field34;  // 0x34
            f32 field38;  // 0x38
            f32 field3C;  // 0x3C
            f32 field40;  // 0x40
            f32 field44;  // 0x44
            f32 field48;  // 0x48
            f32 field4C;  // 0x4C
            f32 field50;  // 0x50
            f32 field54;  // 0x54
        };
    };                    // 0x28
    union {
        s32 unk58;        // 0x58  display list size
        s32 field58;
    };                    // 0x58
    union {
        f32 unk5C;        // 0x5C
        f32 field5C;
    };                    // 0x5C
    union {
        f32 unk60;        // 0x60
        f32 field60;
    };                    // 0x60
    union {
        s16 arg64;        // 0x64  part count
        s16 unk64;
    };                    // 0x64
    union {
        s16 arg66;        // 0x66
        s16 unk66;
    };                    // 0x66
    union {
        s16 arg68;        // 0x68  drawn coordinate count
        s16 unk68;
    };                    // 0x68
    union {
        s8 flag6A;        // 0x6A
        s8 unk6A;
    };                    // 0x6A
    union {
        s8 flag6B;        // 0x6B
        s8 unk6B;
    };                    // 0x6B
} M559HOOKDATA; // size = 0x6C

// GX_VA_TEX0 record of M559HOOKDATA (8 floats, 0x20 bytes per part)
typedef struct M559TexAttr_s {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} M559TEXATTR; // size = 0x20

// node of the lbl_1_bss_BB8 list (size 0xC)
typedef struct M559ListNode_s {
    s16 id;           // 0x00
    s16 value;        // 0x02
    struct M559ListNode_s *prev; // 0x04
    struct M559ListNode_s *next; // 0x08
} M559LISTNODE; // size = 0xC

// 12 floats of per-vertex data
typedef struct M559Verts_s {
    f32 unk0;
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
} M559VERTS; // size = 0x30

void fn_1_694(OMOBJ *obj);
void fn_1_A5C(OMOBJ *obj);
void fn_1_F48(OMOBJ *obj);

#endif
