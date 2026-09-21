/*
 * Streamed audio (msmStream*), until src/msm/msmstream.c is decompiled.
 *
 * msmSys already calls these; they answer "nothing playing, nothing to do"
 * so the rest of the sound system keeps working without background music
 * played from a stream (looping ambience, some song tracks). Sequenced music
 * (msmMus) and one-shot effects (msmSe) are unaffected: those units match.
 */

#include "msm.h"

#include "dolphin/types.h"

int msmStreamPlay(int streamId, MSM_STREAMPARAM *streamParam)
{
    (void)streamId;
    (void)streamParam;
    return -1;
}

s32 msmStreamStop(int streamNo, s32 speed)
{
    (void)streamNo;
    (void)speed;
    return 0;
}

void msmStreamPauseAll(BOOL pause, s32 speed)
{
    (void)pause;
    (void)speed;
}

s32 msmStreamPause(int streamNo, BOOL pause, s32 speed)
{
    (void)streamNo;
    (void)pause;
    (void)speed;
    return 0;
}

void msmStreamStopAll(s32 speed)
{
    (void)speed;
}

s32 msmStreamGetStatus(int streamNo)
{
    (void)streamNo;
    return 0;
}

s32 msmStreamSetParam(int streamNo, MSM_STREAMPARAM *param)
{
    (void)streamNo;
    (void)param;
    return 0;
}

void msmStreamSetMasterVolume(s32 vol)
{
    (void)vol;
}

void msmStreamPeriodicProc(void)
{
}

void msmStreamSetOutputMode(s32 outputMode)
{
    (void)outputMode;
}

void msmStreamAmemFree(void)
{
}

s32 msmStreamAmemAlloc(void)
{
    return 0;
}

s32 msmStreamInit(char *pdtPath)
{
    (void)pdtPath;
    return 0;
}
