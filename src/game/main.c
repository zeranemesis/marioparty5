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

#include "port/imgui.h"
#include "port/main.h"
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
        if(!aurora_begin_frame()) {
            continue;
        }
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
        pfClsScr();
        HuPrcCall(1);
        GameMesExec();
        HuPerfBegin(1);
        Hu3DExec();
        HuDvdErrorWatch();
        WipeExecAlways();
        HuPerfEnd(0);
        pfDrawFonts();
        HuPerfEnd(1);
        msmMusFdoutEnd();
        HuSysDoneRender(retrace);
        frand();
        rand8();
        HuPerfEnd(2);
        GlobalCounter++;
#ifdef TARGET_PC
        ui_update();
        aurora_end_frame();
        frame_limiter();
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