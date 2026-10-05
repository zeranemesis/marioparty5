#include "game/mg/seqman.h"
#include "game/charman.h"

extern void (*_dtors[])(void);
extern void (*_ctors[])(void);
extern void ObjectSetup(void);

void fn_1_5F60(void *unk, s16 arg0)
{
    if (arg0 >= 0x3C) {
        MgSeqModeNext();
    }
}

void _epilog(void)
{
    void (**dtor)(void) = _dtors;
    goto check;
    
call:
    (*dtor)();
    dtor++;
    
check:
    if ((u32)*dtor != 0) {
        goto call;
    }
}

int _prolog(void)
{
    void (**ctor)(void) = _ctors;
    goto check;
    
call:
    (*ctor)();
    ctor++;
    
check:
    if ((u32)*ctor != 0) {
        goto call;
    }
    ObjectSetup();
    return 0;
}

void fn_1_B04(void *arg0)
{
    if (*(unsigned short *)((unsigned char *)arg0 + 0x10) == 0xC) {
        return;
    }
    
    CharMotionShiftSet(
        *(short *)((unsigned char *)arg0 + 0x2),
        *(short *)((unsigned char *)*(void **)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x8) + 0x48) + 0x18),
        0.0f, 1.0f, 0
    );
    *(unsigned short *)((unsigned char *)arg0 + 0x10) = 0xC;
}

// NONMATCHING: attr constant loaded as li (1 instr) vs lis+addi (2 instr), branch offset differs
// NONMATCHING: attr constant loaded as li (1 instr) vs lis+addi (2 instr), branch offset differs
void fn_1_A94(void *arg0)
{
    if (*(unsigned short *)((unsigned char *)arg0 + 0x10) == 0xB) {
        return;
    }
    
    CharMotionShiftSet(
        *(short *)((unsigned char *)arg0 + 0x2),
        *(short *)((unsigned char *)*(void **)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x8) + 0x48) + 0x16),
        0.0f, 1.0f, 0x4001
    );
    *(unsigned short *)((unsigned char *)arg0 + 0x10) = 0xB;
}

// NONMATCHING: float*mul scheduling differs (const load order, reg alloc)
void fn_1_A08(void *arg0)
{
    if (*(unsigned short *)((unsigned char *)arg0 + 0x10) == 0xA) {
        goto skip_motion;
    }
    
    CharMotionShiftSet(
        *(short *)((unsigned char *)arg0 + 0x2),
        *(short *)((unsigned char *)*(void **)((unsigned char *)*(void **)((unsigned char *)arg0 + 0x8) + 0x48) + 0x14),
        0.0f, 5.0f, 0
    );
    *(unsigned short *)((unsigned char *)arg0 + 0x10) = 0xA;
    
skip_motion:
    {
        float *fptr = *(float **)((unsigned char *)arg0 + 0x3C);
        float val = fptr[0x68/4];
        fptr[0x68/4] = (float)((double)val * 0.7);
    }
}