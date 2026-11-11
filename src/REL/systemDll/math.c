#include "REL/systemDll/sysmd.h"
#include "game/hsfex.h"

#define MDMATH_OSC_0 0
#define MDMATH_OSC_90 1
#define MDMATH_OSC_90_REV 2
#define MDMATH_OSC_180 3
#define MDMATH_OSC_360 4
#define MDMATH_OSC_540 5
#define MDMATH_OSC_720 6
#define MDMATH_CURVE_SAMPLE_MAX 8
#define MDMATH_CURVE_STD_MAX 2

static float stdCurveSample[MDMATH_CURVE_STD_MAX][MDMATH_CURVE_SAMPLE_MAX] = {
    { 0.0f, 0.1f, 0.4f, 0.2f, 1.0f },
    { 0.0f, 0.5f, 1.0f },
};

static int stdCurveSampleNum[MDMATH_CURVE_STD_MAX];
static float stdCurveTime[MDMATH_CURVE_STD_MAX][MDMATH_CURVE_SAMPLE_MAX];
static float stdCurveSlope[MDMATH_CURVE_STD_MAX][MDMATH_CURVE_SAMPLE_MAX];

void MdMathMatrixRot(Mtx *matrix, HuVecF *rot)
{
    Mtx rotX, rotY, rotZ;
    MTXRotDeg(rotX, 'X', rot->x);
    MTXRotDeg(rotY, 'Y', rot->y);
    MTXRotDeg(rotZ, 'Z', rot->z);
    MTXConcat(rotZ, rotX, *matrix);
    MTXConcat(*matrix, rotY, *matrix);
}

void MdMathClamp(float *val, float min, float max)
{
    if(*val <= min) {
        *val = min;
    }
    if(*val >= max) {
        *val = max;
    }
}

void MdMathAngleClamp(float *angle)
{
    if(*angle > 180.0f) {
        *angle = -180.0f+(*angle-180.0f);
    } else if(*angle < -180.0f) {
        *angle = 180.0f+(*angle+180.0f);
    }
}

void MdMathOscillate(int type, float *in, float *out, float t)
{
    *in += 1/t;
    if(*in <= 0) {
        *in = 0;
    }
    if(*in >= 1) {
        *in = 1;
    }
    switch(type) {
        case MDMATH_OSC_0:
            *out = *in;
            break;
        
        case MDMATH_OSC_90:
            *out = HuSin((*in)*90);
            break;
        
        case MDMATH_OSC_90_REV:
            *out = 1-HuSin(90-((*in)*90));
            break;
        
        case MDMATH_OSC_180:
            *out = HuSin((*in)*180);
            break;
        
        case MDMATH_OSC_360:
            *out = HuSin((*in)*360);
            break;
        
        case MDMATH_OSC_540:
            *out = HuSin((*in)*540);
            break;
        
        case MDMATH_OSC_720:
            *out = HuSin((*in)*720);
            break;
        
    }
}

static float StdCurveGet(int type, float t);
static void CurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope);
static float CurveGet(int sampleNum, float t, float *sampleT, float *sample, float *sampleSlope);
static void StdCurveInit(int idx);

void MdMathStdCurveGet(int type, float *in, float *out, float t)
{
    *in += 1/t;
    if(*in <= 0) {
        *in = 0;
    }
    if(*in >= 1) {
        *in = 1;
    }
    *out = StdCurveGet(type, *in);
}

void MdMathLerp(float *out, float a, float b, float t)
{
    *out = a+(t*(b-a));
}

void MdMathLerpScale(HuVecF *out, float a, float b, float t)
{
    float result = a+(t*(b-a));
    out->x = result;
    out->y = result;
    out->z = result;
}

void MathLerpColor(u8 *out, float a, float b, float t)
{
    *out = a+(t*(b-a));
}

void MdMathLerpOfs(float *out, float a, float b, float t, float ofs)
{
    *out = a+((b-a)*(t+ofs));
}

void MdMathLerpScaleOfs(HuVecF *out, float a, float b, float t, float ofs)
{
    float result = a+((b-a)*(t+ofs));
    out->x = result;
    out->y = result;
    out->z = result;
}

void MdMathCurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope)
{
    int i;
    sampleT[0] = 0;
    (sampleT-1)[sampleNum] = 1;
    for(i=1; i<sampleNum-1; i++) {
        sampleT[i] = (1.0f/(sampleNum-1))*i;
    }
    
    CurveInit(sampleNum, sampleT, sample, sampleSlope);
}

void MdMathCurveGet(float *out, int sampleNum, float *sampleT, float *sample, float *sampleSlope, float t)
{
    *out = CurveGet(sampleNum, t, sampleT, sample, sampleSlope);
}

void MdMathModel3Dto2D(volatile s32 mdlNo, volatile s32 cameraNo, volatile HuVecF *dst)
{
    NEL_SYS *sys = &sysWork;

    Hu3D3Dto2D(sys->model->mdlPos[mdlNo], ((SYSCAMERA_WORK *)sys->camera)->camera[cameraNo].bit, (HuVecF *)dst);
}

void MdMathPoint3Dto2D(HuVecF *src, volatile s32 cameraNo, volatile HuVecF *dst)
{
    NEL_SYS *sys = &sysWork;

    Hu3D3Dto2D(src, ((SYSCAMERA_WORK *)sys->camera)->camera[cameraNo].bit, (HuVecF *)dst);
}

void MdMathStdCurveInit(void)
{
    int i;
    int j;
    for(i=0; i<MDMATH_CURVE_STD_MAX; i++) {
        for(j=0; j<MDMATH_CURVE_SAMPLE_MAX; j++) {
            if(1.0f == stdCurveSample[i][j]) {
                stdCurveSampleNum[i] = j+1;
                break;
            }
        }
    }
    for(i=0; i<MDMATH_CURVE_STD_MAX; i++) {
        stdCurveTime[i][0] = 0;
        stdCurveTime[i][stdCurveSampleNum[i]-1] = 1;
        for(j=1; j<stdCurveSampleNum[i]-1; j++) {
            stdCurveTime[i][j] = (1.0f/(stdCurveSampleNum[i]-1))*j;
        }
    }
    for(i=0; i<MDMATH_CURVE_STD_MAX; i++) {
        StdCurveInit(i);
    }
}

static void CurveInit(int sampleNum, float *sampleT, float *sample, float *sampleSlope)
{
    int i;
    float step;
    static float dt[MDMATH_CURVE_SAMPLE_MAX+1];
    static float dt2[MDMATH_CURVE_SAMPLE_MAX+1];
    static float slope[MDMATH_CURVE_SAMPLE_MAX+1];
    sampleNum--;
    for(i=0; i<sampleNum; i++) {
        dt[i] = sampleT[i+1]-sampleT[i];
        slope[i] = (sample[i+1]-sample[i])/dt[i];
    }
    slope[sampleNum] = slope[0];
    for(i=1; i<sampleNum; i++) {
        dt2[i] = 2*(sampleT[i+1]-sampleT[i-1]);
    }
    dt2[sampleNum] = 2*(dt[0]+dt[sampleNum-1]);
    for(i=1; i<=sampleNum; i++) {
        sampleSlope[i] = slope[i]-slope[i-1];
    }
    slope[1] = dt[0];
    slope[sampleNum-1] = dt[sampleNum-1];
    slope[sampleNum] = dt2[sampleNum];
    for(i=2; i<sampleNum-1; i++) {
        slope[i] = 0;
    }
    for(i=1; i<sampleNum; i++) {
        step = dt[i]/dt2[i];
        sampleSlope[i+1] = sampleSlope[i+1]-(step*sampleSlope[i]);
        dt2[i+1] = dt2[i+1]-(step*dt[i]);
        slope[i+1] = slope[i+1]-(step*slope[i]);
        
    }
    slope[0] = slope[sampleNum];
    sampleSlope[0] = sampleSlope[sampleNum];
    for(i=sampleNum-2; i>=0; i--) {
        step = dt[i]/dt2[i+1];
        sampleSlope[i] = sampleSlope[i]-(step*sampleSlope[i+1]);
        slope[i] = slope[i]-(step*slope[i+1]);
    }
    step = sampleSlope[0]/slope[0];
    sampleSlope[0] = step;
    sampleSlope[sampleNum] = step;
    for(i=1; i<sampleNum; i++) {
        sampleSlope[i] = (sampleSlope[i]-(step*slope[i]))/dt2[i];
    }
}

static float CurveGet(int sampleNum, float t, float *sampleT, float *sample, float *sampleSlope)
{
    float dt;
    float sampleDt;
    float len;
    
    int i;
    int num;
    int mid;
    
    sampleNum--;
    len = sampleT[sampleNum]-sampleT[0];
    while(t > sampleT[sampleNum]) {
        t -= len;
    }
    while(t < sampleT[0]) {
        t += len;
    }
    for(i=0, num=sampleNum; i<num;) {
        mid = (i+num)/2;
        if(sampleT[mid] < t) {
            i = mid+1;
        } else {
            num = mid;
        }
    }
    if(i > 0) {
        i--;
    }
    sampleDt = sampleT[i+1]-sampleT[i];
    dt = t-sampleT[i];
    return sample[i]+(dt*((dt*((3*sampleSlope[i])+((dt*(sampleSlope[i+1]-sampleSlope[i]))/sampleDt)))+(((sample[i+1]-sample[i])/sampleDt)-(sampleDt*((2*sampleSlope[i])+sampleSlope[i+1])))));
}

static void StdCurveInit(int idx)
{
    int i;
    float *point;
    int pointNum;
    float *time;
    float *samplePoint;
    float step;
    
    static float dt[MDMATH_CURVE_SAMPLE_MAX+1];
    static float dt2[MDMATH_CURVE_SAMPLE_MAX+1];
    static float slope[MDMATH_CURVE_SAMPLE_MAX+1];
    
    pointNum = stdCurveSampleNum[idx]-1;
    time = &stdCurveTime[idx][0];
    samplePoint = &stdCurveSample[idx][0];
    point = &stdCurveSlope[idx][0];
    for(i=0; i<pointNum; i++) {
        dt[i] = time[i+1]-time[i];
        slope[i] = (samplePoint[i+1]-samplePoint[i])/dt[i];
    }
    slope[pointNum] = slope[0];
    for(i=1; i<pointNum; i++) {
        dt2[i] = 2*(time[i+1]-time[i-1]);
    }
    dt2[pointNum] = 2*(dt[0]+dt[pointNum-1]);
    for(i=1; i<=pointNum; i++) {
        point[i] = slope[i]-slope[i-1];
    }
    slope[1] = dt[0];
    slope[pointNum-1] = dt[pointNum-1];
    slope[pointNum] = dt2[pointNum];
    for(i=2; i<pointNum-1; i++) {
        slope[i] = 0;
    }
    for(i=1; i<pointNum; i++) {
        step = dt[i]/dt2[i];
        point[i+1] = point[i+1]-(step*point[i]);
        dt2[i+1] = dt2[i+1]-(step*dt[i]);
        slope[i+1] = slope[i+1]-(step*slope[i]);
        
    }
    slope[0] = slope[pointNum];
    point[0] = point[pointNum];
    for(i=pointNum-2; i>=0; i--) {
        step = dt[i]/dt2[i+1];
        point[i] = point[i]-(step*point[i+1]);
        slope[i] = slope[i]-(step*slope[i+1]);
    }
    step = point[0]/slope[0];
    point[0] = step;
    point[pointNum] = step;
    for(i=1; i<pointNum; i++) {
        point[i] = (point[i]-(step*slope[i]))/dt2[i];
    }
}

static float StdCurveGet(int type, float t)
{
    float dt;
    float sampleDt;
    float len;
    
    float *sampleT;
    float *sampleSlope;
    float *sample;
    int i;
    int num;
    int mid;
    
    int sampleNum;
    sampleNum = stdCurveSampleNum[type]-1;
    sampleT = &stdCurveTime[type][0];
    sample = &stdCurveSample[type][0];
    sampleSlope = &stdCurveSlope[type][0];
    len = sampleT[sampleNum]-sampleT[0];
    while(t > sampleT[sampleNum]) {
        t -= len;
    }
    while(t < sampleT[0]) {
        t += len;
    }
    for(i=0, num=sampleNum; i<num;) {
        mid = (i+num)/2;
        if(sampleT[mid] < t) {
            i = mid+1;
        } else {
            num = mid;
        }
    }
    if(i > 0) {
        i--;
    }
    sampleDt = sampleT[i+1]-sampleT[i];
    dt = t-sampleT[i];
    return sample[i]+(dt*((dt*((3*sampleSlope[i])+((dt*(sampleSlope[i+1]-sampleSlope[i]))/sampleDt)))+(((sample[i+1]-sample[i])/sampleDt)-(sampleDt*((2*sampleSlope[i])+sampleSlope[i+1])))));
}

