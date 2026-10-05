// m512Dll/REL/m512Dll/m512

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

struct Vec {
    f32 x, y, z;
};

struct Mtx {
    f32 m[3][4];
};

// HU3D - matches game/hu3d.h: sizeof(HU3DMODEL) is 0x134, hookData at 0x120
struct HU3DMODEL {
    u8 unk000[0x120];
    void *hookData;
    u8 unk124[0x134 - 0x124];
};

extern HU3DMODEL Hu3DData[];

// OSThread
typedef s32 OSPriority;
struct OSThread;
OSThread *OSGetCurrentThread(void);
OSPriority OSGetThreadPriority(OSThread *thread);

// game/mg/flow
void *HuMemDirectFree(void *ptr);

// extern variables
extern s16 omSysExitReq;

// forward declarations for vtable
extern "C" void fn_1_4874(void *self, void *arg);
extern "C" void fn_1_4934(void *self, s16 flag);
extern "C" void fn_1_48C0(void);

// vtable for fn_1_4854
void *lbl_1_data_1D4[4] = {
    0, 0,
    (void *)fn_1_4934,
    (void *)fn_1_48C0
};

// -------------------------------------------------------------------- functions

extern "C" void fn_1_2C44(void)
{
    omSysExitReq = 1;
}

extern "C" void fn_1_30BC(s16 *obj, void *flow)
{
    ((HU3DMODEL *)((u8 *)Hu3DData + *obj * sizeof(HU3DMODEL)))->hookData = flow;
}

extern "C" f32 (*fn_1_30D8(s16 *obj))[4]
{
    return (f32 (*)[4])((u8 *)Hu3DData + *obj * sizeof(HU3DMODEL) + 0xF0);
}

extern "C" void fn_1_4854(void *self, void *arg)
{
    *(void **)self = lbl_1_data_1D4;
    ((void **)arg)[0x5C / 4] = self;
    ((void **)arg)[0x14 / 4] = (void *)fn_1_4874;
}

extern "C" void fn_1_12DC(void *self)
{
    ((void **)((void **)self)[0])[1] = ((void **)self)[1];
    ((void **)((void **)self)[1])[0] = ((void **)self)[0];
    ((void **)self)[0] = self;
    ((void **)self)[1] = self;
}

extern "C" OSPriority fn_1_5244(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}