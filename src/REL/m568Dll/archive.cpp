#include "REL/m568Dll.h"
#include "datadir_enum.h"
#include "game/memory.h"
#include <string.h>
#ifdef TARGET_PC
#include "port/endian.h"
#endif

/*
 * archive.cpp: a small in-memory archive loaded from a data file.
 *
 * File layout: a 0x20-byte header, then (each only if its count is nonzero)
 * a data block, a relocation table (offsets of words inside the data block
 * that hold block-relative offsets), a public table of {data offset, name
 * offset} pairs, a second table of pairs, and a name string pool.
 *
 * The file comes from the big-endian disc. GC relocates the data block in
 * place (offset words become pointers). The PC build cannot widen those 4-byte
 * words and must byte-swap what it reads, so it leaves the file buffer
 * untouched, reads every header/table word through AR32, and resolves each
 * relocation word into a real pointer in a side table (relocPtrs[i] is the
 * target of the word listed in relocs[i]).
 */

#ifdef TARGET_PC
#define AR32(x) ((u32)PORT_BE32(x))
#else
#define AR32(x) (x)
#endif

struct ArchiveHeader {
    u32 size;        // 0x00 total size, bounds the name pool
    u32 dataSize;    // 0x04
    u32 relocNum;    // 0x08
    u32 publicNum;   // 0x0C
    u32 tableNum;    // 0x10
    u8 flags[4];     // 0x14
    s32 w18;         // 0x18
    s32 w1C;         // 0x1C
};

struct ArchiveEntry {
    u32 dataOff;  // 0x00
    u32 nameOff;  // 0x04
};

struct Archive {
    void **vptr;              // 0x00
    u32 size;                 // 0x04
    u32 dataSize;             // 0x08
    u32 relocNum;             // 0x0C
    u32 publicNum;            // 0x10
    u32 tableNum;             // 0x14
    u8 flags[4];              // 0x18
    s32 w1C;                  // 0x1C
    s32 w20;                  // 0x20
    u8 *data;                 // 0x24
    s32 *relocs;              // 0x28
    ArchiveEntry *publics;    // 0x2C
    ArchiveEntry *table;      // 0x30
    char *names;              // 0x34
    u8 pad38[8];              // 0x38
    s32 loaded;               // 0x40
    ArchiveHeader *file;      // 0x44
    ArchiveHeader *buf;       // 0x48
    u8 relocated;             // 0x4C
#ifdef TARGET_PC
    void **relocPtrs;         // resolved relocation targets, one per reloc word
#endif
};

extern "C" {

void *HuDataSelHeapReadNum(int dataNum, s32 num, HEAPID heap);
s32 fn_1_24C(Archive *self, ArchiveHeader *file, s32 arg);
void *fn_1_534(Archive *self, char *name, s32 arg);

s32 lbl_1_data_0 = 0;
char lbl_1_data_4[] = "idx >=0 && idx <= GetPublicNum()";
char lbl_1_data_25[15] = "archive.cpp";
s32 lbl_1_data_34 = 0;
void *lbl_1_data_38[3] = { 0, 0, (void *)fn_1_534 };
void *lbl_1_data_44[3] = { 0, 0, 0 };

Archive *fn_1_0(Archive *self, s32 dataNum)
{
    s8 *p;

    self->vptr = lbl_1_data_44;
    self->vptr = lbl_1_data_38;
    for (p = (s8 *)&self->size; p != (s8 *)&self->buf; p++) {
        *p = (s8)lbl_1_data_0;
    }
    self->buf = (ArchiveHeader *)HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL);
    self->relocated = 0;
#ifdef TARGET_PC
    self->relocPtrs = 0;
#endif
    fn_1_24C(self, self->buf, 0);
    return self;
}

Archive *fn_1_A8(Archive *self, s16 flags)
{
    if (self != 0) {
        self->vptr = lbl_1_data_38;
#ifdef TARGET_PC
        if (self->relocPtrs != 0) {
            HuMemDirectFree(self->relocPtrs);
            self->relocPtrs = 0;
        }
#endif
        if (flags > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Fixes up the offset words inside the data block. GC rewrites them in place;
// PC resolves them into relocPtrs (see top).
void fn_1_100(Archive *self)
{
#ifdef TARGET_PC
    if (self->data != 0 && self->relocs != 0 && self->relocated == 0) {
        u32 i;

        self->relocPtrs = (void **)HuMemDirectMallocNum(HEAP_MODEL, self->relocNum * sizeof(void *), 0x10000000);
        for (i = 0; i < self->relocNum; i++) {
            u32 *word = (u32 *)(self->data + AR32((u32)self->relocs[i]));

            self->relocPtrs[i] = self->data + AR32(*word);
        }
        self->relocated = 1;
    }
#else
    if (self->data != 0 && self->relocs != 0 && self->relocated == 0
        && *(u32 *)(self->data + self->relocs[0]) < (u32)self->data) {
        u32 i;
        for (i = 0; i < self->relocNum; i++) {
            u32 *word = (u32 *)(self->data + self->relocs[i]);
            *word += (u32)self->data;
        }
        self->relocated = 1;
    }
#endif
}

s32 fn_1_1A8(Archive *self)
{
    return self->publicNum;
}

void *fn_1_1B0(Archive *self, s32 idx)
{
    u8 ok = 0;

#ifdef TARGET_PC
    if (idx >= 0 && idx < (s32)self->publicNum) {
#else
    if (idx >= 0 && idx <= (s32)self->publicNum) {
#endif
        ok = 1;
    }
    if (ok != 0) {
    } else {
        fn_1_7EC(lbl_1_data_4, lbl_1_data_25, 0x5C);
#ifdef TARGET_PC
        return 0;
#endif
    }
    return self->data + AR32(self->publics[idx].dataOff);
}

s32 fn_1_24C(Archive *self, ArchiveHeader *file, s32 arg)
{
    s8 *p;
    u32 off;

    for (p = (s8 *)&self->size; p != (s8 *)&self->buf; p++) {
        *p = (s8)lbl_1_data_34;
    }
    self->loaded = 1;
    self->size = AR32(file->size);
    self->dataSize = AR32(file->dataSize);
    self->relocNum = AR32(file->relocNum);
    self->publicNum = AR32(file->publicNum);
    self->tableNum = AR32(file->tableNum);
    self->flags[0] = file->flags[0];
    self->flags[1] = file->flags[1];
    self->flags[2] = file->flags[2];
    self->flags[3] = file->flags[3];
    self->w1C = AR32(file->w18);
    self->w20 = AR32(file->w1C);
    off = sizeof(ArchiveHeader);
    if (self->dataSize != 0) {
        self->data = (u8 *)(file + 1);
        off = self->dataSize + sizeof(ArchiveHeader);
    }
    if (self->relocNum != 0) {
        self->relocs = (s32 *)((u8 *)file + off);
        off += self->relocNum * 4;
    }
    if (self->publicNum != 0) {
        self->publics = (ArchiveEntry *)((u8 *)file + off);
        off += self->publicNum * 8;
    }
    if (self->tableNum != 0) {
        self->table = (ArchiveEntry *)((u8 *)file + off);
        off += self->tableNum * 8;
    }
    if (off < self->size) {
        self->names = (char *)file + off;
    }
    self->file = file;
    fn_1_100(self);
    return 0;
}

char *fn_1_428(Archive *self, s32 idx)
{
    if (idx < 0 || self->publicNum <= (u32)idx) {
        return 0;
    }
    return self->names + AR32(self->publics[idx].nameOff);
}

void *fn_1_460(Archive *self, char *name, s32 arg)
{
    ArchiveEntry *publics = self->publics;
    char *names = self->names;
    s32 num = self->publicNum;
    s32 i;

    for (i = 0; i < num; i++) {
        if (strcmp(names + AR32(publics[i].nameOff), name) == 0) {
            u8 ok = 0;
            if (i >= 0 && i <= (s32)self->publicNum) {
                ok = 1;
            }
            if (ok != 0) {
            } else {
                fn_1_7EC(lbl_1_data_4, lbl_1_data_25, 0x5C);
            }
            return self->data + AR32(self->publics[i].dataOff);
        }
    }
    return 0;
}

void *fn_1_534(Archive *self, char *name, s32 arg)
{
    ArchiveEntry *publics = self->publics;
    char *names = self->names;
    s32 num = self->publicNum;
    void *result;
    s32 i = 0;

    for (;;) {
        if (i >= num) {
            result = 0;
            break;
        }
        if (strcmp(names + AR32(publics[i].nameOff), name) == 0) {
            u8 ok = 0;
            if (i >= 0 && i <= (s32)self->publicNum) {
                ok = 1;
            }
            if (ok != 0) {
            } else {
                fn_1_7EC(lbl_1_data_4, lbl_1_data_25, 0x5C);
            }
            result = self->data + AR32(self->publics[i].dataOff);
            break;
        }
        i++;
    }
    return result;
}

}
