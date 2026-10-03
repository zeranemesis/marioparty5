#include "REL/m513Dll.h"

typedef struct M513Camera_s {
    /* 0x00 */ M513_OBJHEAD head;
    /* 0x08 */ void **vtable;
    /* 0x0C */ u32 flags;
    /* 0x10 */ HuVecF pos;
    /* 0x1C */ HuVecF up;
    /* 0x28 */ HuVecF target;
    /* 0x34 */ M513_PTMF pmf;
    /* 0x40 */ s16 motionId;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ f32 angle;
    /* 0x48 */ HuVecF destPos;
    /* 0x54 */ HuVecF destTarget;
} M513Camera;

extern "C" {
void fn_1_5C0(HuVecF *);
void fn_1_9A0(HuVecF *, int);
void fn_1_9E0(HuVecF *, int);
void fn_1_FF9C(M513Camera *);
void fn_1_FFCC(M513Camera *);
void fn_1_1008C(M513Camera *);

M513Camera *fn_1_103E4(M513Camera *, int);
void fn_1_101E0(M513Camera *, int);
void fn_1_10358(M513Camera *);

static char lbl_1_data_6F8[] = "ms_pointer == 0";
static char lbl_1_data_708[] = "singleton.h";
static char lbl_1_data_714[] = "ms_pointer != 0";

M513_PTMF lbl_1_data_6EC = { 0, -1, { (void *) fn_1_FF9C } };
M513_PTMF lbl_1_data_724 = { 0, -1, { (void *) fn_1_FF9C } };
M513_PTMF lbl_1_data_730 = { 0, -1, { (void *) fn_1_FFCC } };
M513_PTMF lbl_1_data_73C = { 0, -1, { (void *) fn_1_1008C } };

void *lbl_1_data_748[6] = {
    NULL, NULL, (void *) fn_1_103E4, (void *) fn_1_101E0, (void *) fn_1_10358, NULL
};

M513_OBJHEAD *lbl_1_bss_A18;
}

// NONMATCHING: singleton pointer cast
extern "C" M513Camera *fn_1_FD60(M513Camera *arg0)
{
    M513_OBJHEAD *ms;

    fn_1_1768(&arg0->head);
    arg0->vtable = lbl_1_data_218;
    arg0->flags = 0;
    fn_1_1834(&arg0->head, &lbl_1_bss_C);
    if (lbl_1_bss_A18 != NULL) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M513_OBJHEAD *) ((u8 *) arg0 + 0x10);
    if (ms != NULL) {
        ms = (M513_OBJHEAD *) ((u8 *) ms - 0x10);
    }
    lbl_1_bss_A18 = ms;
    arg0->vtable = lbl_1_data_748;
    arg0->pmf = lbl_1_data_6EC;
    arg0->angle = 0.0f;

    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 35.0f, 100.0f, 6000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x280, 0x1E0);
    fn_1_5C0(&arg0->pos);
    return arg0;
}

// NONMATCHING: singleton pointer cast
extern "C" M513Camera *fn_1_FF10(M513Camera *arg0, int arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A18 == NULL) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3d);
        }
        lbl_1_bss_A18 = (M513_OBJHEAD *) NULL;
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}

extern "C" void fn_1_FF9C(M513Camera *arg0)
{
    fn_1_9A0(&arg0->pos, 1);
}

// NONMATCHING: pmf copy goes through a temporary
extern "C" void fn_1_FFCC(M513Camera *arg0)
{
    M513_PTMF pmf;

    fn_1_9E0(&arg0->pos, 1);
    if (Hu3DMotionEndCheck(arg0->motionId) != 0) {
        arg0->destPos = arg0->pos;
        arg0->destTarget = arg0->target;
        Hu3DModelKill(arg0->motionId);
        arg0->flags &= 0xfffffffe;
        pmf = lbl_1_data_724;
        arg0->pmf = pmf;
    }
}

// NONMATCHING: frame size
extern "C" void fn_1_1008C(M513Camera *arg0)
{
    HuVecF destPos;
    HuVecF destTarget;

    destPos = arg0->destPos;
    destTarget = arg0->destTarget;

    arg0->pos.x += (destPos.x - arg0->pos.x) / 24.0f;
    arg0->pos.y += (destPos.y - arg0->pos.y) / 24.0f;
    arg0->pos.z += (destPos.z - arg0->pos.z) / 24.0f;
    arg0->target.x += (destTarget.x - arg0->target.x) / 24.0f;
    arg0->target.y += (destTarget.y - arg0->target.y) / 24.0f;
    arg0->target.z += (destTarget.z - arg0->target.z) / 24.0f;
    Hu3DCameraPosSetV(1, &arg0->pos, &arg0->up, &arg0->target);
}

// NONMATCHING: switch dispatch order
extern "C" void fn_1_101E0(M513Camera *arg0, int arg1)
{
    M513_PTMF pmf;

    switch (arg1) {
    case 0:
        arg0->motionId = Hu3DModelCameraCreate(Hu3DMotionCreateData(M513_ANM_camera), 1);
        Hu3DCameraMotionStart(arg0->motionId, 1);
        pmf = lbl_1_data_730;
        arg0->pmf = pmf;
        break;
    case 1:
        Hu3DModelKill(arg0->motionId);
        arg0->flags &= ~(1 << arg1);
        break;
    case 2:
        arg0->flags &= ~(1 << arg1);
        break;
    case 3:
        pmf = lbl_1_data_73C;
        arg0->pmf = pmf;
        arg0->flags &= ~(1 << arg1);
        break;
    case 4:
        arg0->flags &= ~(1 << arg1);
        break;
    }
}

// NONMATCHING: ptmf dispatch
extern "C" void fn_1_10358(M513Camera *arg0)
{
    HuVecF diff;

    if (__ptmf_test(&arg0->pmf) != 0) {
        __ptmf_scall(arg0);
    }
    PSVECSubtract(&arg0->pos, &arg0->target, &diff);
    arg0->angle = (f32) (180.0 * (atan2(diff.x, diff.z) / 3.141592653589793));
}

// NONMATCHING: singleton pointer cast
extern "C" M513Camera *fn_1_103E4(M513Camera *arg0, int arg1)
{
    if (arg0 != NULL) {
        arg0->vtable = lbl_1_data_748;
        if ((void *) ((u8 *) arg0 + 0x10) != NULL) {
            if (lbl_1_bss_A18 == NULL) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3d);
            }
            lbl_1_bss_A18 = (M513_OBJHEAD *) NULL;
        }
        if (arg0 != NULL) {
            arg0->vtable = lbl_1_data_218;
            if (arg0 != NULL) {
                fn_1_1774(&arg0->head, 0);
            }
        }
        if (arg1 > 0) {
            fn_1_C8(arg0);
        }
    }
    return arg0;
}