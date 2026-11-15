#include "REL/sdroomdll.h"

/* part tables: name message, description message, model file, then cost (u8) + 3 flag bytes */
static SDMACHINE_PART PartTbl0[11] = {
    { DATANUM(DATA_donkeymdl0, 0x17), DATANUM(DATA_donkeymdl0, 0x46), DATANUM(DATA_sd00, 0x34), 0, 0, 0, 0 },
    { DATANUM(DATA_donkeymdl0, 0x18), DATANUM(DATA_donkeymdl0, 0x47), DATANUM(DATA_sd00, 0x35), 5, 2, -2, 0 },
    { DATANUM(DATA_donkeymdl0, 0x19), DATANUM(DATA_donkeymdl0, 0x48), DATANUM(DATA_sd00, 0x36), 5, 0, 4, 0 },
    { DATANUM(DATA_donkeymdl0, 0x1A), DATANUM(DATA_donkeymdl0, 0x49), DATANUM(DATA_sd00, 0x37), 5, 3, 1, 0 },
    { DATANUM(DATA_donkeymdl0, 0x1B), DATANUM(DATA_donkeymdl0, 0x4A), DATANUM(DATA_sd00, 0x38), 7, 0, 2, 1 },
    { DATANUM(DATA_donkeymdl0, 0x1C), DATANUM(DATA_donkeymdl0, 0x4B), DATANUM(DATA_sd00, 0x39), 7, -3, 1, 3 },
    { DATANUM(DATA_donkeymdl0, 0x1D), DATANUM(DATA_donkeymdl0, 0x4C), DATANUM(DATA_sd00, 0x3A), 10, 4, -4, 0 },
    { DATANUM(DATA_donkeymdl0, 0x1E), DATANUM(DATA_donkeymdl0, 0x4D), DATANUM(DATA_sd00, 0x3B), 10, 2, -1, 1 },
    { DATANUM(DATA_donkeymdl0, 0x1F), DATANUM(DATA_donkeymdl0, 0x4E), DATANUM(DATA_sd00, 0x3C), 15, -4, 3, 4 },
    { DATANUM(DATA_donkeymdl0, 0x20), DATANUM(DATA_donkeymdl0, 0x4F), DATANUM(DATA_sd00, 0x3D), 15, 2, 2, 2 },
    { DATANUM(DATA_donkeymdl0, 0x21), DATANUM(DATA_donkeymdl0, 0x50), DATANUM(DATA_sd00, 0x3E), 20, -1, 1, 2 },
};

static SDMACHINE_PART PartTbl1[11] = {
    { DATANUM(DATA_donkeymdl0, 0x01), DATANUM(DATA_donkeymdl0, 0x30), DATANUM(DATA_sd00, 0x1E), 0, 0, 0, 0 },
    { DATANUM(DATA_donkeymdl0, 0x02), DATANUM(DATA_donkeymdl0, 0x31), DATANUM(DATA_sd00, 0x20), 5, 3, 3, -1 },
    { DATANUM(DATA_donkeymdl0, 0x03), DATANUM(DATA_donkeymdl0, 0x32), DATANUM(DATA_sd00, 0x24), 5, -3, -3, 0 },
    { DATANUM(DATA_donkeymdl0, 0x04), DATANUM(DATA_donkeymdl0, 0x33), DATANUM(DATA_sd00, 0x26), 5, 0, 1, -4 },
    { DATANUM(DATA_donkeymdl0, 0x05), DATANUM(DATA_donkeymdl0, 0x34), DATANUM(DATA_sd00, 0x28), 7, 0, -1, -2 },
    { DATANUM(DATA_donkeymdl0, 0x06), DATANUM(DATA_donkeymdl0, 0x35), DATANUM(DATA_sd00, 0x2A), 7, 4, 4, -1 },
    { DATANUM(DATA_donkeymdl0, 0x07), DATANUM(DATA_donkeymdl0, 0x36), DATANUM(DATA_sd00, 0x2C), 10, -4, 4, -1 },
    { DATANUM(DATA_donkeymdl0, 0x08), DATANUM(DATA_donkeymdl0, 0x37), DATANUM(DATA_sd00, 0x2E), 10, -1, -2, 3 },
    { DATANUM(DATA_donkeymdl0, 0x09), DATANUM(DATA_donkeymdl0, 0x38), DATANUM(DATA_sd00, 0x2F), 15, -100, -100, -100 },
    { DATANUM(DATA_donkeymdl0, 0x0A), DATANUM(DATA_donkeymdl0, 0x39), DATANUM(DATA_sd00, 0x31), 15, -1, -1, 2 },
    { DATANUM(DATA_donkeymdl0, 0x0B), DATANUM(DATA_donkeymdl0, 0x3A), DATANUM(DATA_sd00, 0x32), 20, 0, 1, 1 },
};

static SDMACHINE_PART PartTbl2[11] = {
    { DATANUM(DATA_donkeymdl0, 0x0C), DATANUM(DATA_donkeymdl0, 0x3B), DATANUM(DATA_sdroom, 0x06), 0, 0, 0, 0 },
    { DATANUM(DATA_donkeymdl0, 0x0D), DATANUM(DATA_donkeymdl0, 0x3C), DATANUM(DATA_sdroom, 0x07), 5, 3, 3, -1 },
    { DATANUM(DATA_donkeymdl0, 0x0E), DATANUM(DATA_donkeymdl0, 0x3D), DATANUM(DATA_sdroom, 0x08), 5, -3, -3, 0 },
    { DATANUM(DATA_donkeymdl0, 0x0F), DATANUM(DATA_donkeymdl0, 0x3E), DATANUM(DATA_sdroom, 0x09), 5, 0, 1, -3 },
    { DATANUM(DATA_donkeymdl0, 0x10), DATANUM(DATA_donkeymdl0, 0x3F), DATANUM(DATA_sdroom, 0x0A), 7, -1, -2, -2 },
    { DATANUM(DATA_donkeymdl0, 0x11), DATANUM(DATA_donkeymdl0, 0x40), DATANUM(DATA_sdroom, 0x0B), 7, 4, 4, -1 },
    { DATANUM(DATA_donkeymdl0, 0x12), DATANUM(DATA_donkeymdl0, 0x41), DATANUM(DATA_sdroom, 0x0C), 10, -4, -4, -1 },
    { DATANUM(DATA_donkeymdl0, 0x13), DATANUM(DATA_donkeymdl0, 0x42), DATANUM(DATA_sdroom, 0x0D), 10, 1, 1, -4 },
    { DATANUM(DATA_donkeymdl0, 0x14), DATANUM(DATA_donkeymdl0, 0x43), DATANUM(DATA_sdroom, 0x0E), 15, -100, -100, -100 },
    { DATANUM(DATA_donkeymdl0, 0x15), DATANUM(DATA_donkeymdl0, 0x44), DATANUM(DATA_sdroom, 0x0F), 15, -2, 2, -1 },
    { DATANUM(DATA_donkeymdl0, 0x16), DATANUM(DATA_donkeymdl0, 0x45), DATANUM(DATA_sdroom, 0x10), 20, 1, 2, -3 },
};

static SDMACHINE_PART PartTbl3[13] = {
    { DATANUM(DATA_donkeymdl0, 0x22), DATANUM(DATA_donkeymdl0, 0x51), DATANUM(DATA_sd00, 0x3F), 0, -1, 1, 0 },
    { DATANUM(DATA_donkeymdl0, 0x23), DATANUM(DATA_donkeymdl0, 0x52), DATANUM(DATA_sd00, 0x40), 5, -1, 3, 0 },
    { DATANUM(DATA_donkeymdl0, 0x24), DATANUM(DATA_donkeymdl0, 0x53), DATANUM(DATA_sd00, 0x41), 5, 2, -2, 0 },
    { DATANUM(DATA_donkeymdl0, 0x25), DATANUM(DATA_donkeymdl0, 0x54), DATANUM(DATA_sd00, 0x42), 7, 0, -1, 0 },
    { DATANUM(DATA_donkeymdl0, 0x26), DATANUM(DATA_donkeymdl0, 0x55), DATANUM(DATA_sd00, 0x43), 7, 1, 0, 0 },
    { DATANUM(DATA_donkeymdl0, 0x27), DATANUM(DATA_donkeymdl0, 0x56), DATANUM(DATA_sd00, 0x44), 7, 2, 0, 0 },
    { DATANUM(DATA_donkeymdl0, 0x28), DATANUM(DATA_donkeymdl0, 0x57), DATANUM(DATA_sd00, 0x45), 10, 3, -1, 0 },
    { DATANUM(DATA_donkeymdl0, 0x29), DATANUM(DATA_donkeymdl0, 0x58), DATANUM(DATA_sd00, 0x46), 10, 2, 2, 0 },
    { DATANUM(DATA_donkeymdl0, 0x2A), DATANUM(DATA_donkeymdl0, 0x59), DATANUM(DATA_sd00, 0x47), 15, 4, -3, 0 },
    { DATANUM(DATA_donkeymdl0, 0x2B), DATANUM(DATA_donkeymdl0, 0x5A), DATANUM(DATA_sd00, 0x48), 15, 1, 0, 0 },
    { DATANUM(DATA_donkeymdl0, 0x2C), DATANUM(DATA_donkeymdl0, 0x5B), DATANUM(DATA_sd00, 0x49), 20, 3, -3, 0 },
    { DATANUM(DATA_donkeymdl0, 0x2D), DATANUM(DATA_donkeymdl0, 0x5C), DATANUM(DATA_sd00, 0x4A), 30, 4, -1, 0 },
    { DATANUM(DATA_donkeymdl0, 0x2E), DATANUM(DATA_donkeymdl0, 0x5D), DATANUM(DATA_sd00, 0x4B), 30, 2, 2, 0 },
};

static SDMACHINE_PART PartTbl4[11] = {
    { 0, 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x01), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x02), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x03), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x04), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x05), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x06), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x07), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x08), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_actman, 0x09), 0, 0, 0, 0, 0, 0 },
    { DATANUM(DATA_donkey, 0x60), 0, 0, 0, 0, 0, 0 },
};

SDMACHINE_PART *SdMachinePartTbl[5] = { PartTbl0, PartTbl1, PartTbl2, PartTbl3, PartTbl4 };

s16 SdMachinePartNum[6] = { 11, 11, 11, 13, 10, 0 };

static SDMACHINE_BODY BodyTbl[11] = {
    { { DATANUM(DATA_sd00, 0x1E), DATANUM(DATA_sd00, 0x1F), DATANUM(DATA_sd00, 0x1E), DATANUM(DATA_sd00, 0x1F) }, { 0, 0, 0 }, 50.0f, 50.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0 },
    { { DATANUM(DATA_sd00, 0x20), DATANUM(DATA_sd00, 0x21), DATANUM(DATA_sd00, 0x22), DATANUM(DATA_sd00, 0x23) }, { 3, 3, -1 }, 50.0f, 40.0f, 60.0f, 35.0f, 0.0f, 0.0f, 0 },
    { { DATANUM(DATA_sd00, 0x24), DATANUM(DATA_sd00, 0x25), DATANUM(DATA_sd00, 0x24), DATANUM(DATA_sd00, 0x25) }, { -3, -3, 0 }, 56.0f, 56.0f, 56.0f, 50.0f, 0.0f, 80.0f, 1 },
    { { DATANUM(DATA_sd00, 0x26), DATANUM(DATA_sd00, 0x27), DATANUM(DATA_sd00, 0x26), DATANUM(DATA_sd00, 0x27) }, { 0, 1, -4 }, 50.0f, 50.0f, 50.0f, 20.0f, 0.0f, 0.0f, 0 },
    { { DATANUM(DATA_sd00, 0x28), DATANUM(DATA_sd00, 0x29), DATANUM(DATA_sd00, 0x28), DATANUM(DATA_sd00, 0x29) }, { 0, -1, -2 }, 50.0f, 50.0f, 50.0f, 28.0f, 0.0f, 0.0f, 16 },
    { { DATANUM(DATA_sd00, 0x2A), DATANUM(DATA_sd00, 0x2B), DATANUM(DATA_sd00, 0x2A), DATANUM(DATA_sd00, 0x2B) }, { 4, 4, -1 }, 40.0f, 40.0f, 40.0f, 40.0f, 0.0f, 110.0f, 1 },
    { { DATANUM(DATA_sd00, 0x2C), DATANUM(DATA_sd00, 0x2D), DATANUM(DATA_sd00, 0x2C), DATANUM(DATA_sd00, 0x2D) }, { -4, -4, -1 }, 56.0f, 56.0f, 56.0f, 50.0f, 0.0f, 80.0f, 1 },
    { { DATANUM(DATA_sd00, 0x2E), DATANUM(DATA_sd00, 0x2E), DATANUM(DATA_sd00, 0x2E), DATANUM(DATA_sd00, 0x2E) }, { -1, -2, 3 }, 30.0f, 90.0f, 90.0f, 80.0f, 0.0f, 80.0f, 82 },
    { { DATANUM(DATA_sd00, 0x2F), DATANUM(DATA_sd00, 0x30), DATANUM(DATA_sd00, 0x2F), DATANUM(DATA_sd00, 0x30) }, { 0, 0, 0 }, 50.0f, 50.0f, 50.0f, 40.0f, 0.0f, 0.0f, 128 },
    { { DATANUM(DATA_sd00, 0x31), DATANUM(DATA_sd00, 0x31), DATANUM(DATA_sd00, 0x31), DATANUM(DATA_sd00, 0x31) }, { -1, -1, 2 }, 70.0f, 50.0f, 50.0f, 100.0f, 0.0f, 100.0f, 82 },
    { { DATANUM(DATA_sd00, 0x32), DATANUM(DATA_sd00, 0x33), DATANUM(DATA_sd00, 0x32), DATANUM(DATA_sd00, 0x33) }, { 0, 1, 1 }, 50.0f, 50.0f, 50.0f, 28.0f, 0.0f, 0.0f, 32 },
};

static SDMACHINE_HEAD HeadTbl[11] = {
    { DATANUM(DATA_sd00, 0x34), { 0, 0, 0 }, 120.0f, 180.0f, 2 },
    { DATANUM(DATA_sd00, 0x35), { 2, -2, 0 }, 130.0f, 220.0f, 4 },
    { DATANUM(DATA_sd00, 0x36), { 0, 4, 0 }, 125.0f, 180.0f, 1 },
    { DATANUM(DATA_sd00, 0x37), { 3, 1, 0 }, 90.0f, 240.0f, 2 },
    { DATANUM(DATA_sd00, 0x38), { 0, 2, 1 }, 90.0f, 180.0f, 1 },
    { DATANUM(DATA_sd00, 0x39), { -3, 1, 3 }, 120.0f, 170.0f, 2 },
    { DATANUM(DATA_sd00, 0x3A), { 4, -4, 0 }, 90.0f, 160.0f, 2 },
    { DATANUM(DATA_sd00, 0x3B), { 2, -1, 1 }, 90.0f, 200.0f, 2 },
    { DATANUM(DATA_sd00, 0x3C), { -4, 3, 4 }, 80.0f, 180.0f, 2 },
    { DATANUM(DATA_sd00, 0x3D), { 2, 2, 2 }, 90.0f, 185.0f, 4 },
    { DATANUM(DATA_sd00, 0x3E), { -1, 1, 2 }, 80.0f, 190.0f, 2 },
};

static SDMACHINE_TAIL TailTbl[13] = {
    { DATANUM(DATA_sd00, 0x3F), DATANUM(DATA_sd00, 0x4C), { 0, -1, 1, 5, 24 } },
    { DATANUM(DATA_sd00, 0x40), DATANUM(DATA_sd00, 0x4D), { 3, -1, 1, 9, 6 } },
    { DATANUM(DATA_sd00, 0x41), DATANUM(DATA_sd00, 0x4E), { -2, 2, 1, 3, 1 } },
    { DATANUM(DATA_sd00, 0x42), DATANUM(DATA_sd00, 0x4F), { -1, 0, 1, 4, 30 } },
    { DATANUM(DATA_sd00, 0x43), DATANUM(DATA_sd00, 0x50), { 0, 1, 2, 4, 18 } },
    { DATANUM(DATA_sd00, 0x44), -1, { 0, 2, 1, 2, 1 } },
    { DATANUM(DATA_sd00, 0x45), -1, { -1, 3, 1, 1, 1 } },
    { DATANUM(DATA_sd00, 0x46), DATANUM(DATA_sd00, 0x51), { 2, 2, 1, 3, 40 } },
    { DATANUM(DATA_sd00, 0x47), DATANUM(DATA_sd00, 0x52), { -3, 4, 1, 2, 60 } },
    { DATANUM(DATA_sd00, 0x48), DATANUM(DATA_sd00, 0x53), { 0, 1, 1, 4, 18 } },
    { DATANUM(DATA_sd00, 0x49), DATANUM(DATA_sd00, 0x54), { -3, 4, 1, 1, 60 } },
    { DATANUM(DATA_sd00, 0x4A), -1, { -3, 4, 1, 1, 1 } },
    { DATANUM(DATA_sd00, 0x4B), DATANUM(DATA_sd00, 0x55), { -2, 2, 1, 4, 30 } },
};

/* remainder of the original .data blob, not referenced by this unit */
static u32 TailTblRest[328] = {
    0x00000001, 0x00000005, 0x00000005, 0x45160000, 0x00000000, 0x41F00000, 0x00000000, 0x00000000,
    0x42C80000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000000A, 0x00000008,
    0x44E10000, 0x00000000, 0x41F00000, 0x00000000, 0x00000000, 0x42C80000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000008, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000002, 0x00000003, 0x00000003, 0x44D20000, 0x42700000, 0x42200000,
    0x43C80000, 0x00000000, 0x42C80000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000019,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000004, 0x00000005, 0x43340000, 0x00000000, 0x41F00000, 0x43480000, 0x42B40000, 0x42C80000,
    0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000019, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000004, 0x45160000,
    0x00000000, 0x41F00000, 0x00000000, 0x00000000, 0x44160000, 0x00000000, 0x00000000, 0x00000000,
    0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000002, 0x00000006, 0x00000006, 0x45610000, 0x00000000, 0x428C0000, 0x00000000,
    0x00000000, 0x42C80000, 0x00000000, 0x00000018, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006,
    0x00000006, 0x00000000, 0x00000000, 0x41F00000, 0x00000000, 0x00000000, 0x42C80000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000003, 0x0000000F, 0x45610000, 0x00000000,
    0x41F00000, 0x00000000, 0x00000000, 0x42C80000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000002, 0x00000004, 0x45340000, 0x00000000, 0x41F00000, 0x43C80000, 0x00000000,
    0x42C80000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000019, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000007, 0x00000004,
    0x43960000, 0x00000000, 0x41F00000, 0x43960000, 0x42700000, 0x42C80000, 0x00000000, 0x00000000,
    0x00000000, 0x00000040, 0x00000019, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000014, 0x00000000, 0x00000000, 0x00000002, 0x00000003, 0x45340000, 0x00000000, 0x41F00000,
    0x44160000, 0x00000000, 0x42C80000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000019,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000002, 0x00000004, 0x45340000, 0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42C80000,
    0x00000000, 0x00000000, 0x00000000, 0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000005, 0x00000005, 0x42DC0000,
    0x42700000, 0x42200000, 0x43C80000, 0x00000000, 0x42C80000, 0x00000000, 0x00000000, 0x00000000,
    0x00000040, 0x00000019, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000003,
    0x00000003, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFD, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00000001, 0xFFFFFFFD, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE,
    0xFFFFFFFE, 0x00000000, 0xFFFFFFFF, 0x00000004, 0x00000004, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000001, 0x00000001, 0xFFFFFFFC,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFE,
    0x00000002, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000001, 0x00000002, 0xFFFFFFFD, 0x00000001,
};

static unsigned int ItemFileTbl[12] = {
    DATANUM(DATA_sd00, 0x12), DATANUM(DATA_sd00, 0x13), DATANUM(DATA_sd00, 0x14), DATANUM(DATA_sd00, 0x15), DATANUM(DATA_sd00, 0x16), DATANUM(DATA_sd00, 0x17), DATANUM(DATA_sd00, 0x18), DATANUM(DATA_sd00, 0x19), DATANUM(DATA_sd00, 0x1A), DATANUM(DATA_sd00, 0x1B), DATANUM(DATA_sd00, 0x1C), DATANUM(DATA_sd00, 0x1D)
};

static char *HookNameTbl[17] = {
    "itemhook_a", "itemhook_b", "itemhook_e", "itemhook_f",
    "itemhook_m", "itemhook_n", "itemhook_gf", "itemhook_v",
    "itemhook_w", "itemhook_x", "itemhook_w2", "itemhook_x2",
    "itemhook_z", "itemhook_t1", "itemhook_t2", "itemhook_t3",
    "itemhook_t4"
};

void fn_1_3E8C(s16 charNo, s16 partNo)
{
    GwSdCommon.partFlag[charNo] |= 1 << partNo;
}

BOOL fn_1_3EC0(s16 charNo, s16 partNo)
{
    if(charNo == 4) {
        return TRUE;
    }
    return GwSdCommon.partFlag[charNo] & (1 << partNo);
}

void fn_1_3F00(void)
{
    GWSDCOMMON *sdCommon = &GwSdCommon;
    SdMachineMask[0] = -1;
    SdMachineMask[1] = -1;
    SdMachineMask[2] = -1;
    SdMachineMask[3] = -1;
    if(!sdCommon->extCharSelF) {
        SdMachineMask[0] &= ~0x100;
        SdMachineMask[1] &= ~0x100;
        SdMachineMask[2] &= ~0x100;
        SdMachineMask[3] &= ~0x100;
    }
    if(!sdCommon->battleEasyWinF || !sdCommon->battleNormalWinF || !sdCommon->battleHardWinF
        || !sdCommon->flagEasyWinF || !sdCommon->flagNormalWinF || !sdCommon->flagHardWinF
        || !sdCommon->rabbitEasyWinF || !sdCommon->rabbitNormalWinF || !sdCommon->rabbitHardWinF) {
        SdMachineMask[0] &= ~0x400;
        SdMachineMask[1] &= ~0x400;
        SdMachineMask[2] &= ~0x400;
        SdMachineMask[3] &= ~0x400;
    }
    if((sdCommon->partFlag[0] & 0x7FF) != 0x7FF || (sdCommon->partFlag[1] & 0x7FF) != 0x7FF
        || (sdCommon->partFlag[2] & 0x7FF) != 0x7FF || (sdCommon->partFlag[3] & 0x7FF) != 0x7FF) {
        SdMachineMask[3] &= ~0x1800;
    }
}

int fn_1_414C(s16 charNo, s16 partNo)
{
    int owned;
    if(!(SdMachineMask[charNo] & (1 << partNo))) {
        return 1;
    }
    if(charNo == 4) {
        owned = TRUE;
    } else {
        owned = GwSdCommon.partFlag[charNo] & (1 << partNo);
    }
    if(owned) {
        return 2;
    }
    if(SdMachinePartTbl[charNo][partNo].cost > GwCommon.sdPoint[charNo]) {
        return 3;
    }
    return 0;
}

void fn_1_4238(s16 stat)
{
    OMOVLHIS *his;
    if(!WipeCheck()) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        HuAudFadeOut(1000);
    }
    while(WipeCheckEnd()) {
        HuPrcVSleep();
    }
    HuDataDirClose(DATA_sdd1);
    HuDataDirClose(DATA_sdroom);
    HuDataDirClose(DATA_sd00);
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, stat, his->stat);
    omOvlCallEx(DLL_sd00dll, 1, 0, 0);
    while(1) {
        HuPrcVSleep();
    }
}

// NONMATCHING: register allocation of the part loop
void fn_1_42E0(SDMACHINE_DISP *disp, s16 headNo, s16 bodyNo, s16 tailNo, s16 itemNo)
{
    s16 i;
    s16 itemNoSet;
    disp->partNo[0] = headNo;
    disp->partNo[1] = bodyNo;
    disp->partNo[2] = tailNo;
    if(itemNo == -1) {
        itemNoSet = 64;
    } else {
        itemNoSet = itemNo;
    }
    disp->partNo[3] = itemNoSet;
    HuDataDirClose(DATA_sdroom);
    HuDataDirClose(DATA_win);
    if(headNo != -1) {
        fn_1_51C4(headNo, &disp->headMdl);
        Hu3DModelAttrSet(disp->headMdl, 0x40000082);
    }
    if(bodyNo != -1) {
        fn_1_521C(bodyNo, disp->bodyMdl);
        for(i=0; i<4; i++) {
            if(disp->bodyMdl[i] != -1) {
                Hu3DModelAttrSet(disp->bodyMdl[i], 0x40000082);
            }
        }
    }
    if(tailNo != -1) {
        fn_1_52E0(tailNo, &disp->tailMdl);
        Hu3DModelAttrSet(disp->tailMdl, 0x40000082);
    } else {
        disp->tailMdl = -1;
    }
    if(itemNo != -1 && itemNo != 64) {
        fn_1_5338(itemNo, &disp->itemMdl);
        Hu3DModelAttrSet(disp->itemMdl, 0x40000082);
    } else {
        disp->itemMdl = -1;
    }
    if(bodyNo != -1) {
        disp->yOfs = fn_1_5390(bodyNo, disp->headMdl, disp->bodyMdl, disp->tailMdl, disp->itemMdl);
        if(bodyNo == 5) {
            disp->yOfs += 5.0f;
        }
        if(headNo == 3) {
            disp->yOfs += 5.0f;
        }
    }
    Hu3DModelAttrReset(disp->headMdl, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(disp->headMdl, 0x4000);
}

void fn_1_44E4(SDMACHINE_DISP *disp, s16 headNo, s16 bodyNo, s16 tailNo, s16 itemNo)
{
    fn_1_42E0(disp, headNo, bodyNo, tailNo, itemNo);
    HuDataDirClose(DATA_sd00);
}

void fn_1_4534(SDMACHINE_DISP *disp, float x, float y, float z)
{
    if(disp->partNo[0] != -1) {
        Hu3DModelPosSet(disp->headMdl, x, y+disp->yOfs, z);
    }
}

void fn_1_4590(SDMACHINE_DISP *disp, float x, float y, float z)
{
    if(disp->partNo[0] != -1) {
        Hu3DModelRotSet(disp->headMdl, x, y, z);
    }
}

void fn_1_45E4(SDMACHINE_DISP *disp, float x, float y, float z)
{
    if(disp->partNo[0] != -1) {
        Hu3DModelScaleSet(disp->headMdl, x, y, z);
    }
}

void fn_1_4638(SDMACHINE_DISP *disp, s16 cameraBit)
{
    if(disp->partNo[0] != -1) {
        Hu3DModelCameraSet(disp->headMdl, cameraBit);
    }
}

void fn_1_4680(SDMACHINE_DISP *disp, s16 layerNo)
{
    if(disp->partNo[0] != -1) {
        Hu3DModelLayerSet(disp->headMdl, layerNo);
    }
}

void fn_1_46C4(SDMACHINE_DISP *disp, BOOL dispF)
{
    if(disp->partNo[0] != -1) {
        if(dispF) {
            Hu3DModelAttrReset(disp->headMdl, HU3D_ATTR_DISPOFF);
            return;
        }
        Hu3DModelAttrSet(disp->headMdl, HU3D_ATTR_DISPOFF);
    }
}

void fn_1_4724(SDMACHINE_DISP *disp)
{
    s16 i;
    if(disp->partNo[0] != -1) {
        Hu3DModelKill(disp->headMdl);
    }
    if(disp->partNo[1] != -1) {
        for(i=0; i<4; i++) {
            if(disp->bodyMdl[i] != -1) {
                Hu3DModelKill(disp->bodyMdl[i]);
            }
        }
    }
    if(disp->partNo[2] != -1) {
        Hu3DModelKill(disp->tailMdl);
    }
    if(disp->partNo[3] != -1) {
        Hu3DModelKill(disp->itemMdl);
    }
    for(i=0; i<4; i++) {
        disp->partNo[i] = -1;
    }
}

void fn_1_480C(SDMACHINE_DISP *disp)
{
    s16 i;
    for(i=0; i<4; i++) {
        disp->partNo[i] = -1;
    }
}

// NONMATCHING: stack slot / constant ordering
void fn_1_484C(s16 layerNo)
{
    HU3DCAMERA *cameraP = &Hu3DCamera[Hu3DCameraNo];
    Mtx44 proj;
    Mtx modelview;
    Vec pos;
    Vec target;
    Vec up;
    GXColor color = { 0, 0, 0, 0 };
    float z;
    if(Hu3DCameraNo == 2) {
        MTXPerspective(proj, cameraP->fov, cameraP->aspect, cameraP->near, cameraP->far);
        GXSetProjection(proj, GX_PERSPECTIVE);
        GXSetViewport(cameraP->viewportX, cameraP->viewportY, cameraP->viewportW, cameraP->viewportH, cameraP->viewportNear, cameraP->viewportFar);
        GXSetScissor(cameraP->scissorX, cameraP->scissorY, cameraP->scissorW, cameraP->scissorH);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        color.a = 255.0f*SdMachineFadeAlpha;
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, color);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetCullMode(GX_CULL_NONE);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
        GXSetColorUpdate(GX_TRUE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_SPEC);
        pos.x = pos.y = pos.z = 0;
        target.x = target.y = 0;
        target.z = -100;
        up.x = up.z = 0;
        up.y = 1;
        MTXLookAt(modelview, &pos, &up, &target);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        z = cameraP->far-1;
        pos.x = 1.2f*(z*-sin(cameraP->fov*0.5f));
        pos.y = (1/1.2f)*(z*-sin(cameraP->fov*0.5f));
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-pos.x, -pos.y, -z);
        GXPosition3f32(pos.x, -pos.y, -z);
        GXPosition3f32(pos.x, pos.y, -z);
        GXPosition3f32(-pos.x, pos.y, -z);
    }
}

void fn_1_4D0C(s16 layerNo)
{
    s16 i;
    SdMachineFadeCount++;
    if(SdMachineFadeCount == 1) {
        Hu3DLayerHookSet(layerNo, fn_1_484C);
        SdMachineFadeLayer = layerNo;
        SdMachineFadeAlpha = 0;
        for(i=1; i<=10; i++) {
            SdMachineFadeAlpha = 0.5*(i/10.0);
            HuPrcVSleep();
        }
    }
}

void fn_1_4E14(void)
{
    s16 i;
    SdMachineFadeCount--;
    if(SdMachineFadeCount < 0) {
        SdMachineFadeCount = 0;
        return;
    }
    if(SdMachineFadeCount == 0) {
        for(i=1; i<=10; i++) {
            SdMachineFadeAlpha = 0.5*(1.0-(i/10.0));
            HuPrcVSleep();
        }
        Hu3DLayerHookReset(SdMachineFadeLayer);
    }
}

u16 fn_1_4F24(u16 padMask)
{
    s16 ret;
    s16 i;
    i = ret = 0;
    for(; i<4; i++) {
        if((1 << i) & padMask) {
            ret |= HuPadBtn[i];
        }
        SdMachinePadBtn[i] = HuPadBtn[i];
    }
    return ret;
}

u16 fn_1_4FD0(u16 padMask)
{
    s16 ret;
    s16 i;
    i = ret = 0;
    for(; i<4; i++) {
        if((1 << i) & padMask) {
            ret |= HuPadBtnDown[i];
        }
        SdMachinePadBtn[i] = HuPadBtnDown[i];
    }
    return ret;
}

// NONMATCHING: register allocation of the accumulator
u16 fn_1_507C(u16 padMask)
{
    u16 ret;
    s16 i;
    i = ret = 0;
    for(; i<4; i++) {
        if((1 << i) & padMask) {
            ret |= HuPadDStkRep[i];
        }
        SdMachinePadBtn[i] = HuPadDStkRep[i];
    }
    return ret;
}

u16 fn_1_5118(u16 padMask)
{
    s16 ret;
    s16 i;
    i = ret = 0;
    for(; i<4; i++) {
        if((1 << i) & padMask) {
            ret |= HuPadBtnRep[i];
        }
        SdMachinePadBtn[i] = HuPadBtnRep[i];
    }
    return ret;
}

void fn_1_51C4(int headNo, s16 *mdlId)
{
    *mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(HeadTbl[headNo].file, HU_MEMNUM_OVL, HEAP_MODEL));
}

void fn_1_521C(int bodyNo, s16 *mdlId)
{
    int flags = BodyTbl[bodyNo].flags;
    int type = flags & 0xF;
    int i;
    for(i=0; i<4; i++) {
        mdlId[i] = -1;
        if(type == 1) {
            if(i >= 2) {
                continue;
            }
        } else if(type == 2 && i >= 1) {
            continue;
        }
        mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(BodyTbl[bodyNo].file[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
}

void fn_1_52E0(int tailNo, s16 *mdlId)
{
    *mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(TailTbl[tailNo].file, HU_MEMNUM_OVL, HEAP_MODEL));
}

void fn_1_5338(int itemNo, s16 *mdlId)
{
    *mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(ItemFileTbl[itemNo], HU_MEMNUM_OVL, HEAP_MODEL));
}

float fn_1_5390(int bodyNo, s16 headMdl, s16 *bodyMdl, s16 tailMdl, s16 itemMdl)
{
    float yOfs = 0;
    int flags;
    int type;
    int i;
    if(headMdl == -1) {
        return yOfs;
    }
    flags = BodyTbl[bodyNo].flags;
    type = flags & 0xF;
    for(i=0; i<4; i++) {
        if(bodyMdl[i] != -1) {
            if(type == 0) {
                Hu3DModelHookSet(headMdl, HookNameTbl[i], bodyMdl[i]);
                yOfs = BodyTbl[bodyNo].yOfs;
            } else if(type == 1) {
                if(i < 2) {
                    Hu3DModelHookSet(headMdl, HookNameTbl[i+4], bodyMdl[i]);
                    yOfs = BodyTbl[bodyNo].yOfs;
                }
            } else if(i < 1) {
                Hu3DModelHookSet(headMdl, HookNameTbl[7], bodyMdl[i]);
                yOfs = BodyTbl[bodyNo].yOfs+BodyTbl[bodyNo].yOfs2;
            }
        }
    }
    if(tailMdl != -1) {
        Hu3DModelHookSet(headMdl, HookNameTbl[6], tailMdl);
    }
    if(itemMdl != -1) {
        Hu3DModelHookSet(headMdl, HookNameTbl[12], itemMdl);
    }
    return yOfs;
}
