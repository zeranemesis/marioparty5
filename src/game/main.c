#include "version.h"
#include "game/main.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/charman.h"
#include "game/process.h"
#include "game/printfunc.h"
#include "game/perf.h"
#include "game/sprite.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/sreset.h"
#include "game/saveload.h"
#include "game/frand.h"

#ifdef TARGET_PC
#include <aurora/aurora.h>
#include <aurora/event.h>

#include "port/frame_interp.h"
#include "port/imgui.h"
#include "port/main.h"
#include "port/rollback.h"
#include "port/retroachievements.h"
#include "port/cheats.h"
#include "port/dvd_timing.h"
void Mp5Native_ThpLogicalTick(void);
#include "port/ui.h"

bool Mp5Native_IsRunning = true;
bool Mp5Native_IsShuttingDown = false;
bool Mp5Native_IsGameLaunched = false;
bool Mp5Native_RestartRequested = false;

void Mp5Native_RequestRestart(void)
{
    Mp5Native_RestartRequested = SUPPORTS_PROCESS_RESTART;
    Mp5Native_IsRunning = false;
}
#endif

extern OVLTBL _ovltbl[];

SHARED_SYM u32 GlobalCounter;
SHARED_SYM BOOL HuDvdErrWait;
SHARED_SYM BOOL HuSRDisableF;
SHARED_SYM BOOL NintendoDispF;

#ifdef TARGET_PC
int game_main(void)
#else
void main(void)
#endif
{
    s16 i;
    s32 retrace;
    s16 temp = 0;
    
    HuSRDisableF = FALSE;
    HuDvdErrWait = 0;
    NintendoDispF = 0;
    HuSysInit(&GXNtsc480IntDf);
    HuPrcInit();
    HuPadInit();
    GWInit();
    pfInit();
    GlobalCounter = 0;
    HuDataInit();
    HuSprInit();
    Hu3DInit();
    SLSaveFlagSet(FALSE);
    HuPerfInit();
    HuPerfCreate("USR0", 0xFF, 0xFF, 0xFF, 0xFF);
    HuPerfCreate("USR1", 0, 0xFF, 0xFF, 0xFF);
    WipeInit(RenderMode);
    
    for(i=0; i<GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].charNo = CHARNO_NONE;
    }
    
    omMasterInit(0, _ovltbl, DLL_MAX, DLL_bootdll);
    VIWaitForRetrace();
    
    if(VIGetNextField() == 0) {
        OSReport("VI_FIELD_BELOW\n");
        VIWaitForRetrace();
    }
    OSReport("%s USA Mode\n", "Sep 25 2003");
    SLSaveFlagSet(FALSE);
#ifdef TARGET_PC
    while(Mp5Native_IsRunning) {
        s32 ticks;
        /* the window's events decide whether the game keeps running */
        const AuroraEvent *event = aurora_update();
        while(event != NULL && event->type != AURORA_NONE) {
            if(event->type == AURORA_EXIT) {
                Mp5Native_IsRunning = false;
                break;
            }
            if(event->type == AURORA_SDL_EVENT) {
                ui_handle_sdl_event(&event->sdl);
            }
            event++;
        }
        if(!Mp5Native_IsRunning) {
            break;
        }
        /* logic ticks due in this frame: 1 at the original 60 fps; 0 or more at a high frame rate */
        ticks = HuInterpFrameBegin();
        if(ticks < 0) {
            continue;
        }
        if(!aurora_begin_frame()) {
            continue;
        }
        ticks += Mp5Rb_FrameBegin(); /* netplay rollback: an earlier tick restored, the ticks since run again */
        for(; ticks > 0; ticks--) {
            ticks += Mp5Rb_TickBegin(); /* netplay rollback: snapshots, and the ticks to run again after a misprediction */
#else
    while(1) {
#endif
        retrace = VIGetRetraceCount();
        if (HuSoftResetButtonCheck() || HuDvdErrWait) {
            continue;
        }
        HuPerfZero();
        HuPerfBegin(2);
        HuSysBeforeRender();
        HuPerfBegin(0);
        Hu3DPreProc();
        HuPadRead();
#ifdef TARGET_PC
        Mp5Rb_Probe(1);
#endif
        pfClsScr();
#ifdef TARGET_PC
        Mp5Native_DvdTimingPump(); /* background disc reads that end this tick (GameCube loading speed) */
#endif
        HuPrcCall(1);
#ifdef TARGET_PC
        Mp5Rb_Probe(2);
#endif
        GameMesExec();
#ifdef TARGET_PC
        Mp5Rb_Probe(3);
#endif
        HuPerfBegin(1);
        Hu3DExec();
#ifdef TARGET_PC
        Mp5Rb_Probe(4);
#endif
        HuDvdErrorWatch();
        WipeExecAlways();
        HuPerfEnd(0);
        pfDrawFonts();
        HuPerfEnd(1);
        msmMusFdoutEnd();
#ifdef TARGET_PC
        Mp5Rb_Probe(5);
#endif
        HuSysDoneRender(retrace);
#ifdef TARGET_PC
        Mp5Rb_Probe(6);
#endif
        frand();
        rand8();
        HuPerfEnd(2);
        GlobalCounter++;
#ifdef TARGET_PC
        HuInterpTickEnd();
        /* the console evaluated achievements once per logic tick, whatever the display rate */
        Mp5Native_RAGameTick();
        Mp5Native_CheatsTick();
        Mp5Native_ThpLogicalTick();
        }
        Mp5Native_RAFramePump();
        HuInterpDraw();
        ui_update();
        aurora_end_frame();
        HuInterpFrameEnd();
#endif
    }
#ifdef TARGET_PC
    return 0;
#endif
}

void HuSysVWaitSet(s16 vWait)
{
    minimumVcount = vWait;
    minimumVcountf = vWait;
}

s16 HuSysVWaitGet(s16 prev)
{
    return minimumVcount;
}

s32 rnd_seed = 0x0000D9ED;

int rand8(void)
{
    rnd_seed = (rnd_seed * 0x41C64E6D) + 0x3039;
    return (u8)(((rnd_seed + 1) >> 16) & 0xFF);
}