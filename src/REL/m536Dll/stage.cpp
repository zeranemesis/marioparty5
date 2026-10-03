#include "dolphin/types.h"
#include "datadir_enum.h"
#include "dol2asm.h"

// Declared here rather than through game/hu3d.h and dolphin/gx.h: in C++ those pull in
// MSL math.h, whose inline std::sqrtf emits constants into .rodata.
extern "C" {
typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

struct GXColor {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct M536Base {
    M536Base *m_next;
    M536Base *m_prev;
    void **m_vptr;
    s32 m_flag;
};

struct M536Cloud {
    M536Cloud *m_next;
    M536Cloud *m_prev;
    HU3DMODELID m_mdl;
    s32 m_index;
};

struct M536Stage {
    M536Base *m_next;
    M536Base *m_prev;
    void **m_vptr;
    s32 m_flag;
    HU3DMODELID m_mdl[5];
    s32 m_timer[4];
};

struct M536Sky {
    M536Base *m_next;
    M536Base *m_prev;
    void **m_vptr;
    s32 m_flag;
    HU3DMODELID m_mdl[5];
    Vec3 m_pos[4];
};

void Hu3DCameraPerspectiveGet(int cameraBit, f32 *fov, f32 *nearZ, f32 *farZ);
void GXSetFog(s32 fogType, f32 nearZ, f32 farZ, f32 nearDist, f32 farDist, GXColor color);
void Hu3DLayerHookSet(s16 layerNo, void (*hookFunc)(s16 layerNo));
void Hu3DZClearLayerSet(s16 layerNo);
HU3DMODELID Hu3DModelLink(HU3DMODELID linkMdlId);
void Hu3DModelLayerSet(HU3DMODELID modelId, s16 layerNo);
void Hu3DModelCameraSet(HU3DMODELID modelId, u16 cameraBit);
void Hu3DModelPosSet(HU3DMODELID modelId, f32 posX, f32 posY, f32 posZ);
void Hu3DModelRotSet(HU3DMODELID modelId, f32 rotX, f32 rotY, f32 rotZ);
void Hu3DModelScaleSet(HU3DMODELID modelId, f32 scaleX, f32 scaleY, f32 scaleZ);
void Hu3DModelAttrSet(HU3DMODELID modelId, u32 attr);
void Hu3DModelAttrReset(HU3DMODELID modelId, u32 attr);
s32 Hu3DModelLightInfoSet(HU3DMODELID modelId, s16 staticF);
HU3DMOTID Hu3DMotionIDGet(HU3DMODELID modelId);
void Hu3DMotionSet(HU3DMODELID modelId, HU3DMOTID motId);
void Hu3DMotionSpeedSet(HU3DMODELID modelId, f32 speed);
void Hu3DMotionTimeSet(HU3DMODELID modelId, f32 time);
s32 Hu3DMotionEndCheck(HU3DMODELID modelId);
void HuMemDirectFree(void *ptr);

void *fn_1_0(u32 size);
void fn_1_C8(void *ptr);
void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 flag);
void fn_1_1330(void *self, void *head);
void fn_1_1D4(char *msg, char *file, s32 line);
void fn_1_2C94(HU3DMODELID *mdl, s32 dataNum);
void fn_1_2DB4(HU3DMODELID *mdl, HU3DMODELID *src);
void fn_1_30F4(HU3DMODELID *mdl, s16 flag);
s32 fn_1_4AE0(void);
void *__register_global_object(void *object, void *destructor, void *regmem);

void fn_1_8FA4(M536Stage *self);
void fn_1_9044(M536Stage *self, s32 bit);
void fn_1_9354(M536Sky *self);
void fn_1_944C(M536Sky *self, s32 bit);
void *fn_1_9800(M536Sky *self, s16 flag);
void *fn_1_98D8(M536Stage *self, s16 flag);
void fn_1_9744(s32 index);
void fn_1_96E0(M536Cloud *self);
void fn_1_99E0(M536Cloud *head, void (*fn)(M536Cloud *));
void fn_1_9A40(M536Cloud *node, M536Cloud *head, void (*fn)(M536Cloud *));

extern void **lbl_1_data_1FC;
extern void *lbl_1_bss_C;
extern void *lbl_1_bss_A0C;
extern void *lbl_1_bss_A10;
}

// ---------------------------------------------------------------- data

GXColor lbl_1_data_338 = { 0x00, 0x00, 0x00, 0x00 };
GXColor lbl_1_data_33C = { 0xFF, 0xFF, 0xFF, 0x00 };
char lbl_1_data_340[] = "ms_pointer == 0";
char lbl_1_data_350[] = "singleton.h";
char lbl_1_data_35C[] = "ms_pointer != 0";
Vec3 lbl_1_data_36C[4] = {
    { -1200.0f, -200.0f, -400.0f },
    { 0.0f, -200.0f, -1000.0f },
    { 500.0f, -100.0f, -1200.0f },
    { -700.0f, -300.0f, -1400.0f },
};
void *lbl_1_data_39C[5] = {
    NULL, NULL, (void *)fn_1_9800, (void *)fn_1_944C, (void *)fn_1_9354,
};
void *lbl_1_data_3B0[6] = {
    NULL, NULL, (void *)fn_1_98D8, (void *)fn_1_9044, (void *)fn_1_8FA4, NULL,
};

// ---------------------------------------------------------------- types

extern "C" {

M536Cloud lbl_1_bss_A74;
void *lbl_1_bss_A68[3];

// ---------------------------------------------------------------- stage

void fn_1_8A74(s16 layerNo)
{
    f32 fov;
    f32 nearZ;
    f32 farZ;

    Hu3DCameraPerspectiveGet(1, &fov, &nearZ, &farZ);
    GXSetFog(4, 2200.0f, 15000.0f, nearZ, farZ, lbl_1_data_338);
}

void fn_1_8B04(s16 layerNo)
{
    f32 fov;
    f32 nearZ;
    f32 farZ;

    Hu3DCameraPerspectiveGet(1, &fov, &nearZ, &farZ);
    GXSetFog(0, 2200.0f, 15000.0f, nearZ, farZ, lbl_1_data_33C);
}

M536Cloud *fn_1_9474(M536Cloud *self, HU3DMODELID *mdl, s32 index, f32 angle);

void *fn_1_8B94(M536Stage *self)
{
    s32 i;
    f32 angle;
    M536Cloud *cloud;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, lbl_1_bss_C);

    if (lbl_1_bss_A10) {
        fn_1_1D4(lbl_1_data_340, lbl_1_data_350, 0x35);
    }

    lbl_1_bss_A10 = self;
    self->m_vptr = lbl_1_data_3B0;

    fn_1_2C94(&self->m_mdl[0], DATANUM(DATA_m536, 0));
    fn_1_2C94(&self->m_mdl[1], DATANUM(DATA_m536, 1));
    fn_1_2C94(&self->m_mdl[2], DATANUM(DATA_m536, 2));
    fn_1_2C94(&self->m_mdl[3], DATANUM(DATA_m536, 4));
    fn_1_2C94(&self->m_mdl[4], DATANUM(DATA_m536, 5));

    Hu3DLayerHookSet(0, fn_1_8B04);
    Hu3DLayerHookSet(8, fn_1_8A74);
    Hu3DZClearLayerSet(1);

    Hu3DModelLayerSet(self->m_mdl[2], 0);
    Hu3DModelCameraSet(self->m_mdl[2], 1);
    Hu3DModelPosSet(self->m_mdl[2], 0.0f, 0.0f, 0.0f);

    Hu3DModelLayerSet(self->m_mdl[1], 1);
    Hu3DModelCameraSet(self->m_mdl[1], 1);
    Hu3DModelPosSet(self->m_mdl[1], 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(self->m_mdl[1], 0x40000002);
    Hu3DModelAttrSet(self->m_mdl[1], 0x40000001);
    Hu3DMotionSet(self->m_mdl[1], Hu3DMotionIDGet(self->m_mdl[1]));

    Hu3DModelLayerSet(self->m_mdl[0], 1);
    Hu3DModelCameraSet(self->m_mdl[0], 1);
    Hu3DModelLightInfoSet(self->m_mdl[0], 1);
    Hu3DModelPosSet(self->m_mdl[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrReset(self->m_mdl[0], 0x40000002);
    Hu3DModelAttrSet(self->m_mdl[0], 0x40000001);
    Hu3DMotionSet(self->m_mdl[0], Hu3DMotionIDGet(self->m_mdl[0]));

    Hu3DModelAttrSet(self->m_mdl[3], 1);
    Hu3DModelAttrSet(self->m_mdl[4], 1);

    i = 0;
    angle = -180.0f;

    for (i = 0; i < 4; i++) {
        cloud = (M536Cloud *)fn_1_0(0x10);
        if (cloud) {
            cloud = fn_1_9474(cloud, &self->m_mdl[3], i * 2, angle);
        }

        cloud = (M536Cloud *)fn_1_0(0x10);
        if (cloud) {
            cloud = fn_1_9474(cloud, &self->m_mdl[4], (i * 2) + 1, angle);
        }

        self->m_timer[i] = 0;
        angle += 90.0f;
    }

    return self;
}

void *fn_1_8F18(M536Stage *self, s16 flag)
{
    if (self) {
        if (!lbl_1_bss_A10) {
            fn_1_1D4(lbl_1_data_35C, lbl_1_data_350, 0x3D);
        }
        lbl_1_bss_A10 = NULL;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_8FA4(M536Stage *self)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if ((self->m_timer[i] -= 1) < 0) {
            self->m_timer[i] = (fn_1_4AE0() & 0x7F) + 0xF0;
            if (fn_1_4AE0() & 1) {
                fn_1_9744(i * 2);
            } else {
                fn_1_9744((i * 2) + 1);
            }
}
    }
}

void fn_1_9044(M536Stage *self, s32 bit)
{
    self->m_flag &= ~((u32)1 << bit);
    if (bit < 5) {
        if (bit < 0) {
            return;
        } else {
            return;
        }
    }
}

void *fn_1_906C(M536Sky *self)
{
    s32 i;
    HU3DMODELID mdl;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, lbl_1_bss_C);

    if (lbl_1_bss_A0C) {
        fn_1_1D4(lbl_1_data_340, lbl_1_data_350, 0x35);
    }

    lbl_1_bss_A0C = self;
    self->m_vptr = lbl_1_data_39C;

    fn_1_2C94(&self->m_mdl[0], DATANUM(DATA_m536, 3));
    self->m_mdl[1] = self->m_mdl[0];
    self->m_mdl[2] = Hu3DModelLink(self->m_mdl[0]);
    self->m_mdl[3] = Hu3DModelLink(self->m_mdl[0]);
    self->m_mdl[4] = Hu3DModelLink(self->m_mdl[0]);

    for (i = 0; i < 4; i++) {
        mdl = self->m_mdl[i + 1];
        self->m_pos[i].x = lbl_1_data_36C[i].x;
        self->m_pos[i].y = lbl_1_data_36C[i].y;
        self->m_pos[i].z = lbl_1_data_36C[i].z;
        Hu3DModelLayerSet(mdl, 0);
        Hu3DModelCameraSet(mdl, 1);
        Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(mdl, 0.2f, 0.2f, 0.2f);
        Hu3DModelPosSet(mdl, self->m_pos[i].x, self->m_pos[i].y, self->m_pos[i].z);
        Hu3DModelAttrReset(mdl, 0x40000002);
        Hu3DModelAttrSet(mdl, 0x40000001);
    }

    return self;
}

void *fn_1_92C8(M536Sky *self, s16 flag)
{
    if (self) {
        if (!lbl_1_bss_A0C) {
            fn_1_1D4(lbl_1_data_35C, lbl_1_data_350, 0x3D);
        }
        lbl_1_bss_A0C = NULL;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_9354(M536Sky *self)
{
    s32 i;
    f32 limit;
    HU3DMODELID mdl;

    for (i = 0; i < 4; i++) {
        mdl = self->m_mdl[i + 1];
        limit = -(f32)(i * 200 + 1500);
        self->m_pos[i].x += 1.0f;
        if (self->m_pos[i].x > limit) {
            self->m_pos[i].x = -limit;
        }
Hu3DModelPosSet(mdl, self->m_pos[i].x, self->m_pos[i].y, self->m_pos[i].z);
    }
}

void fn_1_944C(M536Sky *self, s32 bit)
{
    self->m_flag &= ~((u32)1 << bit);
    if (bit < 5) {
        if (bit < 0) {
            return;
        } else {
            return;
        }
    }
}

M536Cloud *fn_1_9474(M536Cloud *self, HU3DMODELID *mdl, s32 index, f32 angle)
{
    fn_1_1264(self);
    fn_1_2DB4(&self->m_mdl, mdl);
    fn_1_1330(self, &lbl_1_bss_A74);
    self->m_index = index;

    Hu3DModelLayerSet(self->m_mdl, 4);
    Hu3DModelCameraSet(self->m_mdl, 1);
    Hu3DModelAttrReset(self->m_mdl, 1);
    Hu3DMotionSpeedSet(self->m_mdl, 1.0f);
    Hu3DMotionSet(self->m_mdl, Hu3DMotionIDGet(self->m_mdl));
    Hu3DModelAttrSet(self->m_mdl, 0x40000002);
    Hu3DModelAttrReset(self->m_mdl, 0x40000001);
    Hu3DModelRotSet(self->m_mdl, 0.0f, angle, 0.0f);

    return self;
}

void *fn_1_9584(M536Cloud *self, s16 flag)
{
    if (self) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_95DC(void)
{
    M536Cloud *node;
    s32 empty;

    for (; (u8)empty == 0;
         empty = (lbl_1_bss_A74.m_next == &lbl_1_bss_A74 && lbl_1_bss_A74.m_next == lbl_1_bss_A74.m_prev)) {
        node = lbl_1_bss_A74.m_prev;
        if (node) {
            fn_1_30F4(&node->m_mdl, -1);
            if (node) {
                fn_1_1270(node, 0);
            }
            fn_1_C8(node);
        }
    }
}

void fn_1_96E0(M536Cloud *self)
{
    HU3DMODELID mdl;
    HU3DMODELID mdl2;

    mdl = self->m_mdl;
    if (Hu3DMotionEndCheck(mdl) != 0) {
        mdl2 = self->m_mdl;
        Hu3DModelAttrSet(mdl2, 1);
    }
}

void fn_1_9744(s32 index)
{
    M536Cloud *node;

    node = lbl_1_bss_A74.m_next;
    while (node != &lbl_1_bss_A74) {
        if (node->m_index == index) {
            Hu3DModelAttrReset(node->m_mdl, 0x40000002);
            Hu3DModelAttrReset(node->m_mdl, 0x40000001);
            Hu3DModelAttrReset(node->m_mdl, 1);
            Hu3DMotionTimeSet(node->m_mdl, 0.0f);
        }
        node = node->m_next;
    }
}

void fn_1_96B0(void)
{
    fn_1_99E0(&lbl_1_bss_A74, fn_1_96E0);
}

void *fn_1_9800(M536Sky *self, s16 flag)
{
    if (self) {
        self->m_vptr = lbl_1_data_39C;
        fn_1_30F4(&self->m_mdl[0], -1);

        if (&self->m_mdl[0]) {
            if (!lbl_1_bss_A0C) {
                fn_1_1D4(lbl_1_data_35C, lbl_1_data_350, 0x3D);
            }
            lbl_1_bss_A0C = NULL;
        }

        if (self) {
            self->m_vptr = lbl_1_data_1FC;
            if (self) {
                fn_1_1270(self, 0);
            }
        }

        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_98D8(M536Stage *self, s16 flag)
{
    if (self) {
        self->m_vptr = lbl_1_data_3B0;
        fn_1_30F4(&self->m_mdl[4], -1);
        fn_1_30F4(&self->m_mdl[3], -1);
        fn_1_30F4(&self->m_mdl[2], -1);
        fn_1_30F4(&self->m_mdl[1], -1);
        fn_1_30F4(&self->m_mdl[0], -1);

        if (&self->m_mdl[0]) {
            if (!lbl_1_bss_A10) {
                fn_1_1D4(lbl_1_data_35C, lbl_1_data_350, 0x3D);
            }
            lbl_1_bss_A10 = NULL;
        }

        if (self) {
            self->m_vptr = lbl_1_data_1FC;
            if (self) {
                fn_1_1270(self, 0);
            }
        }

        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_99E0(M536Cloud *head, void (*fn)(M536Cloud *))
{
    M536Cloud *list;

    list = head;
    fn_1_9A40(head->m_next, list, fn);
}

void fn_1_9A40(M536Cloud *node, M536Cloud *head, void (*fn)(M536Cloud *))
{
    while (node != head) {
        fn(node);
        node = node->m_next;
    }
}

void fn_1_9AF8(M536Cloud *self, s16 flag)
{
    if (self) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
}

void fn_1_9AA0(void)
{
    fn_1_1264(&lbl_1_bss_A74);
    __register_global_object(&lbl_1_bss_A74, (void *)fn_1_9AF8, lbl_1_bss_A68);
}

}

SECTION_CTORS void *const _ctors_stage = (void *)fn_1_9AA0;


