#include "game/object.h"
#include "game/charman.h"
#include "game/gamework.h"

void fn_1_3440(OMOBJMAN *objMan);
void fn_1_34F4(void);
void *fn_1_53B8(void *arg0);
void fn_1_3518(OMOBJ *obj);

OMOBJ *lbl_1_bss_30[4];

void fn_1_3440(OMOBJMAN *objMan)
{
    s32 i;
    OMOBJ *obj;

    omMakeGroupEx(objMan, 0, 4);
    i = 0;
    goto loop_cond;
loop_body:
    obj = omAddObjEx(objMan, 0x1E, 1, 8, 0, fn_1_3518);
    lbl_1_bss_30[i] = obj;
    obj->work[0] = i;
    obj->work[1] = GwPlayerConf[i].grp;
    obj->work[2] = i;
    i++;
loop_cond:
    if (i < 4) {
        goto loop_body;
    }
}

void fn_1_34F4(void)
{
    CharModelKill(CHARNO_NONE);
}

void *fn_1_53B8(void *arg0)
{
    void *temp_r31;

    temp_r31 = *(void **)((u8 *)arg0 + 0x5C);
    return *(void **)temp_r31;
}