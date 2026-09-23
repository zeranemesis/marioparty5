#include "dolphin/types.h"

class CMT19937 {
public:
    CMT19937(int seed = 5489) { setSeed(seed); }
    void setSeed(int seed);
    u32 irandom();
    double frandom();
    double randomMantissa();

private:
    u32 m_state[624];
    s32 m_index;
};

class CRandom {
public:
    static void setSeed(int seed);
    static u32 irandom();
    static double randomMantissa();
    static float frandom(float min, float max);

    static CMT19937 randInstance;
};

CMT19937 CRandom::randInstance;

void CRandom::setSeed(int seed)
{
    randInstance.setSeed(seed);
}

u32 CRandom::irandom()
{
    return randInstance.irandom();
}

double CRandom::randomMantissa()
{
    return randInstance.randomMantissa();
}

float CRandom::frandom(float min, float max)
{
    return min + (max - min) * randInstance.frandom();
}
