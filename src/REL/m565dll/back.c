#include "REL/m565dll.h"

static inline f32 M565WrapAngle(f32 a)
{
    if (a > 180.0f) {
        return a - 360.0f;
    }
    if (a <= -180.0f) {
        return 360.0f + a;
    }
    return a;
}

void fn_1_3690(M565Back *back, M565Sys *sys)
{
    HU3DMODELID mdl;

    memset(back, 0, sizeof(M565Back));
    back->sys = sys;

    mdl = Hu3DModelCreateData(0x780000);
    back->mdl0 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 1);
    back->mot0 = Hu3DJointMotionData(mdl, 0x780074);
    Hu3DMotionSet(mdl, back->mot0);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x780001);
    back->mdl1 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 1);
    back->mot1 = Hu3DJointMotionData(mdl, 0x780075);
    Hu3DMotionSet(mdl, back->mot1);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x780002);
    back->mdl2 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 1);
    back->mot2 = Hu3DJointMotionData(mdl, 0x780076);
    Hu3DMotionSet(mdl, back->mot2);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x78006D);
    back->mdl3 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 1);
    back->mot3 = Hu3DJointMotionData(mdl, 0x78007E);
    Hu3DMotionSet(mdl, back->mot3);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x78006E);
    back->mdl4 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrReset(mdl, 1);
    back->mot4 = Hu3DJointMotionData(mdl, 0x78007F);
    Hu3DMotionSet(mdl, back->mot4);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x78006F);
    back->mdl5 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrSet(mdl, 1);
    back->mot5 = Hu3DJointMotionData(mdl, 0x78007A);
    Hu3DMotionSet(mdl, back->mot5);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x780070);
    back->mdl6 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrSet(mdl, 1);
    back->mot6 = Hu3DJointMotionData(mdl, 0x78007B);
    Hu3DMotionSet(mdl, back->mot6);
    Hu3DModelAttrSet(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x780071);
    back->mdl7 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrSet(mdl, 1);
    back->mot7 = Hu3DJointMotionData(mdl, 0x78007C);
    Hu3DMotionShapeSet(mdl, back->mot7);
    Hu3DModelAttrReset(mdl, 0x40000001);

    mdl = Hu3DModelCreateData(0x780072);
    back->mdl8 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(mdl, 1);
    Hu3DModelLayerSet(mdl, 0);
    Hu3DModelAttrSet(mdl, 1);
}

void fn_1_3E54(M565Back *back)
{
    Hu3DModelKill(back->mdl8);
    Hu3DMotionKill(back->mot7);
    Hu3DModelKill(back->mdl7);
    Hu3DMotionKill(back->mot6);
    Hu3DModelKill(back->mdl6);
    Hu3DMotionKill(back->mot5);
    Hu3DModelKill(back->mdl5);
    Hu3DMotionKill(back->mot4);
    Hu3DModelKill(back->mdl4);
    Hu3DMotionKill(back->mot3);
    Hu3DModelKill(back->mdl3);
    Hu3DMotionKill(back->mot2);
    Hu3DModelKill(back->mdl2);
    Hu3DMotionKill(back->mot1);
    Hu3DModelKill(back->mdl1);
    Hu3DMotionKill(back->mot0);
    Hu3DModelKill(back->mdl0);
    memset(back, 0, sizeof(M565Back));
}

void fn_1_3F14(M565Back *back)
{
    HuVecF pos;
    f32 t;

    t = (f32)(back->time % 600) / 600.0f;
    pos.x = sin(3.141592653589793 * (360.0f * t) / 180.0);
    pos.y = 0.0f;
    pos.z = cos(3.141592653589793 * (360.0f * t) / 180.0);
    PSVECScale(&pos, &pos, 950.0f);
    Hu3DModelPosSet(back->mdl2, pos.x, -650.0f, pos.z);
    Hu3DModelRotSet(back->mdl2, 0.0f, M565WrapAngle(90.0f + 360.0f * t), 0.0f);
    Hu3DModelPosSet(back->mdl3, pos.x, -650.0f, pos.z);
    Hu3DModelRotSet(back->mdl3, 0.0f, M565WrapAngle(180.0f + 360.0f * t), 0.0f);
    t = (f32)((back->time + 300) % 600) / 600.0f;
    pos.x = sin(3.141592653589793 * (360.0f * t) / 180.0);
    pos.y = 0.0f;
    pos.z = cos(3.141592653589793 * (360.0f * t) / 180.0);
    PSVECScale(&pos, &pos, 950.0f);
    Hu3DModelPosSet(back->mdl4, pos.x, -650.0f, pos.z);
    Hu3DModelRotSet(back->mdl4, 0.0f, M565WrapAngle(90.0f + 360.0f * t), 0.0f);
    back->time = back->time + 1;
}
