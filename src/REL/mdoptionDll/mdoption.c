#include "game/object.h"
#include "game/process.h"
#include "game/data.h"
#include "game/wipe.h"

/*************************************************************
 * mdoption.c
 *
 * Record/option ("mdoption") overlay: module entry points
 * (_prolog/_epilog), the shared SYS work structure, the dead
 * loader thread and the overlay's startup/shutdown halves.
 *
 * Functions are declared in the order the original object emits
 * them, because MWCC inlines any callee whose body it has already
 * compiled. MdMain (0x84C) calls MdInit/MdClose, so it MUST be
 * inserted between _epilog and LoadProc below; putting it after
 * MdClose silently inlines MdClose into MdMain.
 *
 * The original overlay shipped no header, so the shared SYS
 * layout below was recovered from the sys-> field references in
 * this module's own object. Only the fields the recovered code
 * actually touches are named; the rest is explicit padding so
 * the offsets stay pinned to the original.
 *************************************************************/

/* Offsets recovered from the original object's sys-> accesses. */
typedef struct Sys_s {
    s32 pad00[10];             //0x00
    s32 flowMode;              //0x28 MdFlowTbl index
    s8 exitF;                  //0x2C
    s8 skipWipeF;              //0x2D
    s8 pad2E;                  //0x2E
    s8 cursorSelNoPrev;        //0x2F
    s8 pad30;                  //0x30
    s8 cursorSelNo;            //0x31 write cursor into cursorSel[]
    s8 cursorSelF[4];          //0x32
    s8 cursorSel[4];           //0x36
    s8 cursorSelPrev[4];       //0x3A
    s8 pad3E[0x4C - 0x3E];     //0x3E
    s16 loadState;             //0x4C
    s16 loadAsyncStat;         //0x4E
    s32 pad50;                 //0x50
    OMOBJMAN *objman;          //0x54
    HUPROCESS *mainProc;       //0x58
    void *unk5C[3];            //0x5C
    HUPROCESS *loadProc;       //0x68
    OMOBJ *obj[6];             //0x6C debugfont, pad, camera, light, shadow, mess
    void *debugFont;           //0x84
    void *padWork;             //0x88
    void *camera;              //0x8C
    void *light;               //0x90
    void *shadow;              //0x94
    void *mess;                //0x98
    void *sprite;              //0x9C
    void *model;               //0xA0
    u8 padA4[0x398 - 0xA4];    //0xA4
    s32 unk398;                //0x398
    s32 unk39C;                //0x39C
    u8 pad3A0[0x510 - 0x3A0];  //0x3A0
} SYS;

extern SYS *sys;
extern SYS *sysP;

/* sys->loadState states. The names are this decomp's; only the
 * ordering (0=none 1=read 2=done 3=idle) is fixed by the original's
 * switch dispatch. */
enum {
    MD_LOAD_STATE_NONE,      //0
    MD_LOAD_STATE_BOARD_LOAD,//1
    MD_LOAD_STATE_WAIT,      //2
    MD_LOAD_STATE_LOAD       //3
};

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);
extern void MdSpriteCreate(void *sprite);
extern void MdModelCreate(void *model, void *camera);
extern void MdMessKill(void *mess);
extern void fn_1_A324(void);
extern void fn_1_A930(s32 cmd);

/* 0x0 - module entry: run the static ctors, then bring the
 * overlay's objects up. Returns 0 to the loader. */
int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

/* 0x54 - module teardown: run the static dtors. */
void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* 0x900 - background loader thread. Nothing starts it in this
 * overlay any more (MdInit spawns fn_1_A324 instead), so it is
 * kept only because the original still ships the body. */
static void LoadProc(void)
{
    OSTick tick;

    while(1) {
        switch(sys->loadState) {
            case MD_LOAD_STATE_NONE:
                break;

            case MD_LOAD_STATE_BOARD_LOAD:
                tick = OSGetTick();
                sys->loadAsyncStat = HuDataDirReadAsync(DATA_rescard);
                while(!HuDataGetAsyncStat(sys->loadAsyncStat)) {
                    HuPrcVSleep();
                }
                OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
                OSReport("Finished!\n");
                sys->loadState = MD_LOAD_STATE_WAIT;
                break;

            case MD_LOAD_STATE_WAIT:
                HuPrcEnd();
                break;
        }
        HuPrcVSleep();
    }
}

/* 0x9F4 - overlay startup: build the sprite and model work areas,
 * release the overlay's own data directory and spawn the record
 * loader thread. */
void MdInit(void)
{
    MdSpriteCreate(sys->sprite);
    HuPrcVSleep();
    MdModelCreate(sys->model, sys->camera);
    HuPrcVSleep();
    HuDataDirClose(DATA_mdoption);
    HuPrcVSleep();
    sys->unk398 = 0;
    sys->unk39C = 0;
    sys->loadProc = HuPrcChildCreate(fn_1_A324, 0x6d, 0x1000, 0, sys->objman);
    HuPrcVSleep();
}

/* 0xAC0 - tear the option overlay down: wipe out unless the
 * caller already wiped, silence the audio hook, drop the message
 * windows and hand control back to the loader. */
void MdClose(void)
{
    if(!sys->skipWipeF) {
        WipeColorSet(255, 255, 255);
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        WipeWait();
    }
    fn_1_A930(0xb);
    MdMessKill(sys->mess);
    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

/* 0xA760 - latch the current per-entry cursor selections into
 * the display copy, blanking the entries past the last one. */
void fn_1_A760(void)
{
    void *sprite;
    int i;

    sprite = sys->sprite;
    for(i = 0; i < 4; i++) {
        sys->cursorSelPrev[i] = sys->cursorSel[i];
        if(i > sys->cursorSelNoPrev) {
            sys->cursorSelPrev[i] = -1;
        }
    }
}