#include "types.h"

typedef struct MgActorUnit {
    s32 pad_00[0x0F];
    void *unk_3C;
} MgActorUnit;

typedef struct MgActorRoot {
    s32 pad_00[0x02];
    MgActorUnit *unk_08;
    MgActorUnit *unk_0C;
} MgActorRoot;

typedef struct MgEntry {
    s32 pad_00[0x03];
    s32 field_0C;
} MgEntry;

extern s32 fn_1_86E4(void);
extern s32 fn_1_91F8(void);
extern void fn_1_6770(void);
extern void fn_1_3CCC(void);
extern void MgActorKill(void *actor);
extern MgActorRoot lbl_1_bss_0;

s16 fn_1_5B78(void) {
    s16 result = 0;
    result = fn_1_86E4();
    fn_1_6770();
    return result;
}

s16 fn_1_6314(void) {
    s16 result = 0;
    result = fn_1_91F8();
    fn_1_6770();
    return result;
}

s16 fn_1_64AC(void) {
    s16 result = 1;
    fn_1_6770();
    return result;
}

s16 fn_1_64DC(void) {
    s16 result = 1;
    fn_1_3CCC();
    MgActorKill(lbl_1_bss_0.unk_08->unk_3C);
    MgActorKill(lbl_1_bss_0.unk_0C->unk_3C);
    return result;
}

MgEntry *fn_1_A69C(MgEntry *entry) {
    s32 field = entry->field_0C;
    return entry;
}