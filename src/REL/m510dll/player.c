#include "REL/m510dll.h"

// lbl_1_data_110: 4 x Vec3 (12 floats) at .data:0x110
//   [0]={0,0,0} [1]={0,0,300} [2]={0,0,600} [3]={0,0,850}
HuVecF lbl_1_data_110[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 300.0f },
    { 0.0f, 0.0f, 600.0f },
    { 0.0f, 0.0f, 850.0f },
};

void fn_1_3A64(void *data, s16 playerNo, void *arg3)
{
    u8 *work = data;
    HuVecF *pos = &lbl_1_data_110[playerNo];
    u16 cameraBit;

    memset(work, 0, 0x880);
    *(void **)work = arg3;              //0x00
    *(s16 *)(work + 0x4) = playerNo;    //0x04

    fn_1_47C4(work + 0x118, work);      //pitch

    *(float *)(work + 0x128) = pos->x;  //0x128
    *(float *)(work + 0x12c) = pos->y;  //0x12c
    *(float *)(work + 0x130) = pos->z - 250.0f; //0x130

    fn_1_509C(work + 0x154, work);      //ball_path

    fn_1_3DC8(work + 0x8, work, playerNo); //playermodel

    fn_1_CE04(work + 0x850, 0, playerNo); //score

    *(float *)(work + 0xc0) = pos->z - 130.0f; //0xc0
    *(float *)(work + 0xc4) = pos->y;           //0xc4
    *(float *)(work + 0xc8) = 1300.0f;          //0xc8

    CharEffectLayerSet(4);

    *(s16 *)(work + 0x878) = Hu3DModelCreate(HuDataSelHeapReadNum(0x44 + 0xe, HU_MEMNUM_OVL, HEAP_MODEL)); //modelId
    Hu3DModelPosSet(*(s16 *)(work + 0x878), 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(*(s16 *)(work + 0x878), 0.0f, 0.0f, 0.0f);
    cameraBit = *(u16 *)((u8 *)arg3 + 0x2478);
    Hu3DModelCameraSet(*(s16 *)(work + 0x878), cameraBit);
    Hu3DModelLayerSet(*(s16 *)(work + 0x878), 4);
    Hu3DModelAttrSet(*(s16 *)(work + 0x878), 1);
    Hu3DModelScaleSet(*(s16 *)(work + 0x878), 8.0f, 8.0f, 8.0f);

    *(s16 *)(work + 0x87a) = Hu3DJointMotion(*(s16 *)(work + 0x878), HuDataSelHeapReadNum(0x44 + 0x1f, HU_MEMNUM_OVL, HEAP_MODEL)); //motionId
    Hu3DMotionSet(*(s16 *)(work + 0x878), *(s16 *)(work + 0x87a));
    Hu3DMotionSpeedSet(*(s16 *)(work + 0x878), 2.5f);
    Hu3DModelAttrReset(*(s16 *)(work + 0x878), 0x4001);
}

void fn_1_3CCC(void *data)
{
    u8 *work = data;

    Hu3DMotionKill(*(s16 *)(work + 0x87a));
    Hu3DModelKill(*(s16 *)(work + 0x878));
    fn_1_D1AC(work + 0x850);
    fn_1_4284(work + 0x8);
    fn_1_51B4(work + 0x154);
    fn_1_49A4(work + 0x118);
    memset(work, 0, 0x880);
}

void fn_1_3D34(void *data)
{
    u8 *work = data;
    float maxTime, time;

    fn_1_49F4(work + 0x118);
    fn_1_4328(work + 0x8);
    fn_1_D234(work + 0x850);
    fn_1_5230(work + 0x154);

    if (*(u32 *)(work + 0x87c) != 0) {
        maxTime = Hu3DMotionMaxTimeGet(*(s16 *)(work + 0x878));
        time = Hu3DMotionTimeGet(*(s16 *)(work + 0x878));
        if (time < maxTime || time == maxTime) {
            Hu3DModelAttrSet(*(s16 *)(work + 0x878), 1);
            *(u32 *)(work + 0x87c) = 0;
        }
    }
}