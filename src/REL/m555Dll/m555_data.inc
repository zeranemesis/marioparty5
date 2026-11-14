#include "game/mg/seqman.h"

MGSEQ_PARAM lbl_1_data_0 = {
    0,
    0,
    (MGSEQ_FUNC)fn_1_308,
    (MGSEQ_FUNC)fn_1_33C,
    (MGSEQ_FUNC)fn_1_368,
    (MGSEQ_FUNC)fn_1_394,
    (MGSEQ_FUNC)fn_1_3C0,
    (MGSEQ_FUNC)fn_1_3EC,
    (MGSEQ_FUNC)fn_1_418,
    (MGSEQ_FUNC)fn_1_444,
    (MGSEQ_FUNC)fn_1_464,
};

s32 lbl_1_data_28[13][2] = {
    { 0x008C001C, 0x008700BE },
    { 0x0033001C, 0x002E00BE },
    { 0x00B7001C, 0x00B200BE },
    { 0x00ED001C, 0x00E800BE },
    { 0x00E6001C, 0x00E100BE },
    { 0x0017001C, 0x001200BE },
    { 0x00E0001C, 0x00DB00BE },
    { 0x002D001C, 0x002800BE },
    { 0x00C9001C, 0x00C400BE },
    { 0x00B0001C, 0x009C00BE },
    { 0x00B0001C, 0x009C00BE },
    { 0x00B0001C, 0x009C00BE },
    { 0x00B0001C, 0x009C00BE },
};

s32 lbl_1_data_90[48] = {
    0x00710000, 0x00710001, 0x00710002, 0x00710003,
    0x00710004, 0x00710005, 0x00710006, 0x00710007,
    0x00710008, 0x00710009, 0x0071000A, 0x0071000B,
    0x0071000C, 0x0071000D, 0x0071000E, 0x0071000F,
    0x00710010, 0x00710011, 0x00710012, 0x00710013,
    0x00710014, 0x00710015, 0x00710016, 0x00710017,
    0x00710018, 0x00710019, 0x0071001A, 0x0071001B,
    0x0071001C, 0x0071001D, 0x0071001E, 0x0071001F,
    0x00710020, 0x00710021, 0x00710022, 0x00710023,
    0x00710024, 0x00710025, 0x00710026, 0x00710027,
    0x00710028, 0x00710029, 0x0071002A, 0x0071002B,
    0x0071002C, 0x0071002D, 0x0071002E, 0x0071002F,
};

f32 lbl_1_data_150[4][13] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1250.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1250.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { 0.0f, 208.0f, 20.0f, -35.0f, 0.0f, 0.0f, 1415.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};

s16 lbl_1_data_220[16] = {
    0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000,
};

char lbl_1_data_240[] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
char lbl_1_data_26E[] = "OBJW %d";
char lbl_1_data_276[] = "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f";
char lbl_1_data_29B[] = "CAMROT X:%.1f Y:%.1f Z:%.1f";

s16 lbl_1_data_2B8[12] = {
    0x0893, 0x0894, 0x0895, 0x0896, 0x0897, 0x0898, 0x0899, 0x089A, 0x089B, 0x089C, 0x089D, 0x089E,
};

/* .rodata of m555.s (literal pool objects) */
const f64 pad_03_00000000_rodata[2] = { 0.5, 3.0 };
const f32 lbl_1_rodata_10 = 0.0f;
const f32 lbl_1_rodata_14 = 1.0f;
const f32 lbl_1_rodata_18 = 20.0f;
const f32 lbl_1_rodata_1C = 40000.0f;
const f64 lbl_1_rodata_20 = 3.141592653589793;
const f64 lbl_1_rodata_28 = 180.0;
const f32 lbl_1_rodata_30 = -23.0f;
const f32 lbl_1_rodata_34 = 1000.0f;
const f32 lbl_1_rodata_38 = 30.0f;
const f32 lbl_1_rodata_3C = -90.0f;
const f32 lbl_1_rodata_40 = 0.5f;
const f32 lbl_1_rodata_44 = 3500.0f;
const f32 lbl_1_rodata_48 = -1.0f;
const f32 lbl_1_rodata_4C = 1.5f;
const f64 lbl_1_rodata_50 = 4503601774854144.0;
const f64 lbl_1_rodata_58 = 4503599627370496.0;
const f32 lbl_1_rodata_60 = 10.0f;
const f32 lbl_1_rodata_64 = 2.0f;
