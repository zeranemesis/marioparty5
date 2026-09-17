/*
 * THP movies are not played yet.
 *
 * The GameCube decodes them with the DSP and the AI audio DMA, neither of
 * which Aurora provides, so the port replaces game/thpmain.c, THPSimple.c and
 * THPDraw.c with this: every movie reports that it started and ended at once,
 * which makes the game skip it and carry on (the title screen still appears,
 * without its background movie).
 */

#include "game/thpmain.h"

#include "dolphin/os.h"

HUPROCESS *THPProc;

static void report_once(void)
{
    static BOOL reported = FALSE;
    if (!reported) {
        reported = TRUE;
        OSReport("thp> movies are skipped: the port has no THP player yet\n");
    }
}

HUSPRID HuTHPSprCreateVol(char *path, s16 loop, s16 prio, float volume)
{
    (void)path;
    (void)loop;
    (void)prio;
    (void)volume;
    report_once();
    return -1;
}

HUSPRID HuTHPSprCreate(char *path, s16 loop, s16 prio)
{
    return HuTHPSprCreateVol(path, loop, prio, 1.0f);
}

HU3DMODELID HuTHP3DCreateVol(char *path, s16 loop, float volume)
{
    (void)path;
    (void)loop;
    (void)volume;
    report_once();
    return -1;
}

HU3DMODELID HuTHP3DCreate(char *path, s16 loop)
{
    return HuTHP3DCreateVol(path, loop, 1.0f);
}

void HuTHPStop(void) {}

void HuTHPClose(void) {}

void HuTHPRestart(void) {}

BOOL HuTHPEndCheck(void)
{
    return TRUE;
}

s32 HuTHPFrameGet(void)
{
    return 0;
}

s32 HuTHPTotalFrameGet(void)
{
    return 0;
}

void HuTHPSetVolume(s32 left, s32 right)
{
    (void)left;
    (void)right;
}

BOOL HuTHPProcCheck(void)
{
    return FALSE;
}

BOOL HuTHPStartCheck(void)
{
    return TRUE;
}

void HuTHPMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    (void)drawObj;
    (void)material;
}
