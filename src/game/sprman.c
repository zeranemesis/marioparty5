#include "version.h"
#include "game/sprite.h"
#include "game/memory.h"
#include "game/init.h"

#include "dolphin/mtx.h"
#ifdef TARGET_PC
#include "port/spranim.h"
#include "port/widescreen.h"
#include "game/disp.h"
#include <math.h>
#endif

#ifdef TARGET_PC
/* no prototype in the original: an implicit declaration passes the floats as doubles */
void mtxTransCat(Mtx mtx, float x, float y, float z);
#endif

#define HUSPR_ORDER_MAX 768

#define SPRITE_DIRTY_ATTR 0x1
#define SPRITE_DIRTY_XFORM 0x2
#define SPRITE_DIRTY_COLOR 0x4

typedef struct HuSprOrder_s {
    u16 grpId;
    u16 sprId;
    u16 prio;
    u16 next;
} HUSPRORDER;

SHARED_SYM HUSPRITE HuSprData[HUSPR_MAX];
SHARED_SYM HUSPRGRP HuSprGrpData[HUSPR_GRP_MAX];
static HUSPRORDER HuSprOrder[HUSPR_ORDER_MAX];

#ifdef TARGET_PC
/* GC writes through a HUSPR_NONE / out of range id land in harmless memory before HuSprData; on PC they hit HuSprGrpData */
static HUSPRITE HuSprScratch;
static HUSPRGRP HuSprGrpScratch;
static HUSPRITE *SprPtr(s32 id)
{
    return (id >= 0 && id < HUSPR_MAX) ? &HuSprData[id] : (HuSprScratch.data = NULL, HuSprScratch.bg = NULL, &HuSprScratch);
}
static HUSPRGRP *GrpPtr(s32 id)
{
    return (id >= 0 && id < HUSPR_GRP_MAX) ? &HuSprGrpData[id] : (HuSprGrpScratch.sprNum = 0, &HuSprGrpScratch);
}
static HUSPRITE *SprMember(s32 grpId, s32 memberNo)
{
    HUSPRGRP *gp = GrpPtr(grpId);
    if(gp->sprNum <= 0 || memberNo < 0 || memberNo >= gp->sprNum) {
        return &HuSprScratch;
    }
    return SprPtr(gp->sprId[memberNo]);
}
#define SPRP(id) SprPtr(id)
#define GRPP(id) GrpPtr(id)
#define SPRMEM(g, m) SprMember(g, m)
#else
#define SPRP(id) (&HuSprData[id])
#define GRPP(id) (&HuSprGrpData[id])
#define SPRMEM(g, m) (&HuSprData[HuSprGrpData[g].sprId[m]])
#endif

static s16 HuSprOrderNum;
static s16 HuSprOrderNo;
static BOOL HuSprPauseF;

static void HuSprOrderEntry(HUSPRGRPID grpId, HUSPRID sprId);

#ifdef TARGET_PC
/* Adaptive widescreen HUD layout, adapted from the Mario Party 4 port "Party Board"
 * (github.com/zeranemesis/Marioparty4, src/game/sprman.c HuSprWidescreenLayout). */
float HuSprWideShift[HUSPR_MAX];
float HuSprWideStretch[HUSPR_MAX][3];
float HuSprDispWideShift;
BOOL HuSprWideClip[HUSPR_MAX];
/* moved out to the window edge but covered there in 4:3 (a player panel under a message): not drawn */
BOOL HuSprWideHidden[HUSPR_MAX];
/* sprites set over a 3D object or drawn as a shape: the layout leaves them where the
 * 4:3 code put them and as they were (never stretched as a backdrop) */
BOOL HuSprWideFixed[HUSPR_MAX];

void HuSprWideFixSet(int sprId)
{
    if(sprId > 0 && sprId < HUSPR_MAX) {
        HuSprWideFixed[sprId] = TRUE;
    }
}

BOOL HuSprWidescreenBackdrop(ANIMPAT *pat, float scaleX, float scaleY);

typedef struct {
    float x0, x1, y0, y1;
    float x;
    s16 sprite;
    s16 group;
    s16 prio;
    s16 root;
    BOOL frame;
    BOOL visible;
    BOOL window; /* its group holds a window frame */
} WideBox;

static s16 WideRoot(WideBox *boxes, s16 i)
{
    while(boxes[i].root != i) {
        i = boxes[i].root = boxes[boxes[i].root].root;
    }
    return i;
}

/* Sprites of a group that touch belong together; across groups only when one
 * lies on the other, drawn over it (a label on its bar, a counter in its frame). A
 * window or what belongs to it never joins what lies under it (the speaker's portrait
 * over a player panel). */
static BOOL WideJoined(WideBox *a, WideBox *b)
{
    float w = (a->x1 < b->x1 ? a->x1 : b->x1) - (a->x0 > b->x0 ? a->x0 : b->x0);
    float h = (a->y1 < b->y1 ? a->y1 : b->y1) - (a->y0 > b->y0 ? a->y0 : b->y0);
    float areaA = (a->x1 - a->x0)*(a->y1 - a->y0);
    float areaB = (b->x1 - b->x0)*(b->y1 - b->y0);
    if(w < 0.0f || h < 0.0f) {
        return FALSE;
    }
    if(a->group == b->group) {
        return TRUE;
    }
    if(a->window && b->window) {
        return FALSE;
    }
    if(a->window || b->window) {
        /* only the other piece inside the window and drawn over it */
        if(a->window) {
            WideBox *swap = a;
            a = b;
            b = swap;
            areaA = areaB;
        }
        return areaA > 0.0f && a->prio <= b->prio && w*h >= 0.75f*areaA;
    }
    if(areaA > areaB) {
        WideBox *swap = a;
        a = b;
        b = swap;
        areaA = areaB;
    }
    /* an empty box (a text line not drawn yet) covers nothing: it must not weld a panel to a window */
    return areaA > 0.0f && a->prio <= b->prio && w*h >= 0.75f*areaA;
}

/* Pieces across the middle stay; one to a side follows its outer edge, so a corner
 * panel reaches the corner; a lone sprite follows its position (which Hu3D3Dto2D may
 * have set over a 3D point). Hidden sprites count: an invisible window frame ties its
 * text to the icons in it. */
static WideBox boxes[HUSPR_MAX];
static s16 boxNum;

static void HuSprWidescreenLayout(void)
{
    static float lo[HUSPR_MAX], hi[HUSPR_MAX];
    static s16 count[HUSPR_MAX];
    static BOOL hidden[HUSPR_MAX];
    float left, right;
    HUSPRGRP *gp;
    HUSPRITE *sp;
    ANIMPAT *pat;
    ANIMBMP *bmp;
    float x, y, w, h;
    s16 i, j, n = 0;
    for(i=0; i<HUSPR_MAX; i++) {
        HuSprWideShift[i] = 0.0f;
        HuSprWideStretch[i][0] = 1.0f;
        HuSprWideClip[i] = FALSE;
        HuSprWideHidden[i] = FALSE;
    }
    boxNum = 0;
    if(HuWideHudShift(0.0f) == 0.0f) {
        return;
    }
    for(i=0, gp=HuSprGrpData; i<HUSPR_GRP_MAX; i++, gp++) {
        for(j=0; j<gp->sprNum && n<HUSPR_MAX; j++) {
            if(gp->sprId[j] == -1) {
                continue;
            }
            if(HuSprWideFixed[gp->sprId[j]]) {
                continue;
            }
            sp = &HuSprData[gp->sprId[j]];
            x = gp->mtx[0][0]*sp->pos.x + gp->mtx[0][1]*sp->pos.y + gp->mtx[0][3];
            y = gp->mtx[1][0]*sp->pos.x + gp->mtx[1][1]*sp->pos.y + gp->mtx[1][3];
            boxes[n].x = boxes[n].x0 = boxes[n].x1 = x;
            boxes[n].y0 = boxes[n].y1 = y;
            if((!(sp->attr & HUSPR_ATTR_FUNC) && sp->data) || sp->bg) {
                pat = sp->data && !(sp->attr & HUSPR_ATTR_FUNC) ? &sp->data->pat[sp->data->bank[sp->bank].frame[sp->animNo].pat] : NULL;
                /* a backdrop is stretched in place (HuSprDisp), never moved */
                if(pat && HuSprWidescreenBackdrop(pat, gp->mtx[0][0]*sp->scale.x, gp->mtx[1][1]*sp->scale.y)) {
                    continue;
                }
                w = fabsf(gp->mtx[0][0]*sp->scale.x);
                h = fabsf(gp->mtx[1][1]*sp->scale.y);
                if(sp->bg && (sp->attr & HUSPR_ATTR_FUNC)) {
                    /* the mini-game score window (gamemes.c DrawScoreWin): half its background, centred */
                    w *= sp->bg->bmp->sizeX*16*0.5f;
                    h *= sp->bg->bmp->sizeY*16*0.5f;
                    boxes[n].x0 = x - w*0.5f;
                    boxes[n].y0 = y - h*0.5f;
                } else if(sp->bg) {
                    /* a window frame: drawn the size of its background, centred */
                    bmp = &sp->bg->bmp[sp->bg->pat[sp->bg->bank[sp->bgBank].frame->pat].layer->bmpNo];
                    w *= bmp->sizeX*16;
                    h *= bmp->sizeY*16;
                    boxes[n].x0 = x - w*0.5f;
                    boxes[n].y0 = y - h*0.5f;
                } else {
                    boxes[n].x0 = x - w*pat->centerX;
                    boxes[n].y0 = y - h*pat->centerY;
                    w *= pat->sizeX;
                    h *= pat->sizeY;
                }
                boxes[n].x1 = boxes[n].x0 + w;
                boxes[n].y1 = boxes[n].y0 + h;
            }
            boxes[n].sprite = gp->sprId[j];
            boxes[n].group = i;
            boxes[n].prio = sp->prio;
            boxes[n].frame = sp->bg != NULL;
            boxes[n].visible = !(sp->attr & HUSPR_ATTR_DISPOFF) && sp->a > 0.0f;
            boxes[n].root = n;
            n++;
        }
    }
    for(i=0; i<n; i++) {
        boxes[i].window = FALSE;
        for(j=0; j<n; j++) {
            if(boxes[j].frame && boxes[j].group == boxes[i].group) {
                boxes[i].window = TRUE;
                break;
            }
        }
    }
    for(i=0; i<n; i++) {
        for(j=i+1; j<n; j++) {
            if(WideJoined(&boxes[i], &boxes[j])) {
                boxes[WideRoot(boxes, i)].root = WideRoot(boxes, j);
            }
        }
    }
    for(i=0; i<n; i++) {
        lo[i] = 1e9f;
        hi[i] = -1e9f;
        count[i] = 0;
    }
    for(i=0; i<n; i++) {
        j = WideRoot(boxes, i);
        count[j]++;
        if(boxes[i].x0 < lo[j]) {
            lo[j] = boxes[i].x0;
        }
        if(boxes[i].x1 > hi[j]) {
            hi[j] = boxes[i].x1;
        }
    }
    for(i=0; i<n; i++) {
        if(boxes[i].root != i) {
            continue;
        }
        if(lo[i] < HU_DISP_CENTERX && hi[i] > HU_DISP_CENTERX) {
            x = HU_DISP_CENTERX;
            count[i] = -1;
        } else if(lo[i] < 0.0f) {
            x = lo[i]; /* runs off the left of 4:3: keep its place against the edge */
        } else if(hi[i] > HU_DISP_WIDTH) {
            x = hi[i];
        } else if(count[i] == 1) {
            x = boxes[i].x;
        } else if(hi[i] < HU_DISP_CENTERX) {
            x = lo[i];
        } else if(lo[i] > HU_DISP_CENTERX) {
            x = hi[i];
        } else {
            x = HU_DISP_CENTERX;
        }
        lo[i] = HuWideHudShift(x);
    }
    /* The 4:3 frame is the authored composition: a piece moving out that lies under
     * a piece staying in the middle and drawn over it (a message window over a player
     * panel) stays where it was, so it stays hidden as in 4:3; so does one set beside a
     * window staying in the middle (the speaker next to a message) */
    for(i=0; i<n; i++) {
        hi[i] = lo[i];
        hidden[i] = FALSE;
    }
    /* pass 0: a piece drawn over by one staying in the middle (a message window over a player
     * panel) keeps its corner but is not drawn while covered, rather than jumping back to the
     * middle; pass 1: one beside a window staying in the middle stays with it */
    for(j=0; j<2; j++) {
        for(i=0; i<n; i++) {
            s16 r = WideRoot(boxes, i);
            s16 k;
            if(hi[r] == 0.0f || lo[r] == 0.0f || hidden[r]) {
                continue;
            }
            /* a sprite not drawn (an unused digit of a counter) is covered by nothing */
            if(!boxes[i].visible || boxes[i].x1 <= boxes[i].x0) {
                continue;
            }
            for(k=0; k<n; k++) {
                s16 rk = WideRoot(boxes, k);
                float margin;
                if(rk == r || hi[rk] != 0.0f || !boxes[k].visible || boxes[k].x1 <= boxes[k].x0) {
                    continue;
                }
                if(j == 0) {
                    /* HuSprOrderEntry: lower prio is drawn later, and so is a later entry of the same prio */
                    if(boxes[k].prio > boxes[i].prio || (boxes[k].prio == boxes[i].prio && k < i)) {
                        continue;
                    }
                    margin = 0.0f;
                } else if(boxes[k].frame) {
                    margin = 32.0f;
                } else {
                    continue;
                }
                if(boxes[i].x0 < boxes[k].x1+margin && boxes[k].x0-margin < boxes[i].x1
                    && boxes[i].y0 < boxes[k].y1 && boxes[k].y0 < boxes[i].y1) {
                    if(j == 0) {
                        /* hidden only when mostly under it (a player panel under a message window): a piece the
                         * window only clips at its edge (the instruction screen's PRACTICE/START bubbles) stays */
                        float ow = (boxes[i].x1 < boxes[k].x1 ? boxes[i].x1 : boxes[k].x1) - (boxes[i].x0 > boxes[k].x0 ? boxes[i].x0 : boxes[k].x0);
                        float oh = (boxes[i].y1 < boxes[k].y1 ? boxes[i].y1 : boxes[k].y1) - (boxes[i].y0 > boxes[k].y0 ? boxes[i].y0 : boxes[k].y0);
                        if(ow*oh < 0.5f*(boxes[i].x1-boxes[i].x0)*(boxes[i].y1-boxes[i].y0)) {
                            continue;
                        }
                        hidden[r] = TRUE;
                    } else {
                        lo[r] = 0.0f;
                    }
                    break;
                }
            }
        }
    }
    /* a piece across the middle stays, but what of it ran off a side of the
     * 4:3 picture (a title bar) is lengthened to run off the new side */
    left = HuWideHudShift(0.0f);
    right = HuWideHudShift(HU_DISP_WIDTH);
    for(i=0; i<n; i++) {
        j = WideRoot(boxes, i);
        HuSprWideShift[boxes[i].sprite] = lo[j];
        HuSprWideHidden[boxes[i].sprite] = hidden[j];
        if(count[j] == -1 && boxes[i].x1-boxes[i].x0 >= HU_DISP_CENTERX*0.5f
            && boxes[i].x1 > 0.0f && boxes[i].x0 < HU_DISP_WIDTH
            && (boxes[i].x0 <= 0.5f || boxes[i].x1 >= HU_DISP_WIDTH - 0.5f)) {
            x = boxes[i].x0 + (boxes[i].x0 <= 0.5f ? left : 0.0f);
            HuSprWideStretch[boxes[i].sprite][0] = (boxes[i].x1 + (boxes[i].x1 >= HU_DISP_WIDTH - 0.5f ? right : 0.0f) - x)
                / (boxes[i].x1 - boxes[i].x0);
            HuSprWideStretch[boxes[i].sprite][1] = boxes[i].x0;
            HuSprWideStretch[boxes[i].sprite][2] = x;
        } else if(lo[j] == 0.0f && (boxes[i].x0 < -0.5f || boxes[i].x1 > HU_DISP_WIDTH + 0.5f)) {
            /* a piece left in place that 4:3 cuts at its edge (sliding in, parked):
             * cut it at the same place */
            HuSprWideClip[boxes[i].sprite] = TRUE;
        }
    }
    boxNum = n;
}

/* How far this frame's layout moved HUD point (x, y): a 3D model placed at a HUD
 * point (Hu3D2Dto3D) goes with the sprites it sits in, stays beside a message
 * window left in the middle (the board's host), and otherwise follows HuWideHudShift */
float HuSprWidePointShift(float x, float y)
{
    s16 i;
    for(i=0; i<boxNum; i++) {
        if(boxes[i].visible && boxes[i].x1 > boxes[i].x0 && boxes[i].sprite >= 0
            && x >= boxes[i].x0 && x <= boxes[i].x1 && y >= boxes[i].y0 && y <= boxes[i].y1) {
            /* a model in a hidden panel stays under the window, as in 4:3 */
            return HuSprWideHidden[boxes[i].sprite] ? 0.0f : HuSprWideShift[boxes[i].sprite];
        }
    }
    for(i=0; i<boxNum; i++) {
        if(boxes[i].visible && boxes[i].frame && HuSprWideShift[boxes[i].sprite] == 0.0f
            && x >= boxes[i].x0-96.0f && x <= boxes[i].x1+96.0f
            && y >= boxes[i].y0-32.0f && y <= boxes[i].y1+32.0f) {
            return 0.0f;
        }
    }
    return HuWideHudShift(x);
}
#endif


void HuSprInit(void)
{
    s16 i;
    HUSPRITE *sp;
    HUSPRGRP *gp;
    for(sp = &HuSprData[1], i=1; i<HUSPR_MAX; i++, sp++) {
        sp->data = NULL;
    }
    for(gp = HuSprGrpData, i=0; i<HUSPR_GRP_MAX; i++, gp++) {
        gp->sprNum = 0;
    }
    HuSprExecLayerInit();
    sp = &HuSprData[0];
    sp->prio = 0;
    sp->data = (void *)1;
    HuSprPauseF = FALSE;
}

void HuSprClose(void)
{
    s16 i;
    HUSPRGRP *gp;
    HUSPRITE *sp;
    
    for(gp = HuSprGrpData, i=0; i<HUSPR_GRP_MAX; i++, gp++) {
        if(gp->sprNum != 0) {
            HuSprGrpKill(i);
        }
    }
    for(sp = &HuSprData[1], i=1; i<HUSPR_MAX; i++, sp++) {
        if(sp->data) {
            HuSprKill(i);
        }
    }
    HuSprExecLayerInit();
    HuSprPauseF = FALSE;
}

void HuSprExec(s16 drawNo)
{
    HUSPRITE *sp;
    while(sp = HuSprCall()) {
        if(!(sp->attr & HUSPR_ATTR_DISPOFF) && sp->drawNo == drawNo) {
            HuSprDisp(sp);
        }
    }
}

void HuSprBegin(void)
{
    Mtx temp, rot;
    s16 i, j;
    Vec axis = {0, 0, 1};
    HUSPRGRP *gp;
    gp = HuSprGrpData;
    HuSprOrderNum = 1;
    HuSprOrder[0].next = 0;
    HuSprOrder[0].prio = -1;
    for(i=0; i<HUSPR_GRP_MAX; i++, gp++) {
        if(gp->sprNum != 0) {
            MTXTrans(temp, gp->center.x*gp->scale.x, gp->center.y*gp->scale.y, 0.0f);
            MTXRotAxisDeg(rot, &axis, gp->zRot);
            MTXConcat(rot, temp, gp->mtx);
            MTXScale(temp, gp->scale.x, gp->scale.y, 1.0f);
            MTXConcat(gp->mtx, temp, gp->mtx);
            mtxTransCat(gp->mtx, gp->pos.x, gp->pos.y, 0);
            for(j=0; j<gp->sprNum; j++) {
                if(gp->sprId[j] != -1) {
                    HuSprOrderEntry(i, gp->sprId[j]);
                }
            }
        }
    }
#ifdef TARGET_PC
    HuSprWidescreenLayout();
#endif
    HuSprOrderNo = 0;
}

static void HuSprOrderEntry(HUSPRGRPID grpId, HUSPRID sprId)
{
    HUSPRORDER *order = &HuSprOrder[HuSprOrderNum];
    s16 prio = HuSprData[sprId].prio;
    s16 prev, next;
    if(HuSprOrderNum >= HUSPR_ORDER_MAX) {
        OSReport("Order Max Over!\n");
        return;
    }
    next = HuSprOrder[0].next;
    for(prev = 0; next != 0; prev = next, next = HuSprOrder[next].next) {
        if(HuSprOrder[next].prio < prio) {
            break;
        }
    }
    order->next = HuSprOrder[prev].next;
    HuSprOrder[prev].next = HuSprOrderNum;
    order->prio = prio;
    order->grpId = grpId;
    order->sprId = sprId;
    HuSprOrderNum++;
}

HUSPRITE *HuSprCall(void)
{
    HuSprOrderNo = HuSprOrder[HuSprOrderNo].next;
    if(HuSprOrderNo != 0) {
        HUSPRORDER *order = &HuSprOrder[HuSprOrderNo];
        HUSPRITE *sp = &HuSprData[order->sprId];
        sp->groupMtx = &HuSprGrpData[order->grpId].mtx;
        if(sp->attr & HUSPR_ATTR_FUNC) {
            return sp;
        }
        sp->frameP = &sp->data->bank[sp->bank].frame[sp->animNo];
        sp->patP = &sp->data->pat[sp->frameP->pat];
        return sp;
    } else {
        return NULL;
    }
}

static inline void SpriteCalcFrame(HUSPRITE *sp, ANIMBANK *bank, ANIMFRAME **frame, s16 loopF)
{
    if(sp->time >= (*frame)->time) {
        sp->animNo++;
        sp->time -= (*frame)->time;
        if(sp->animNo >= bank->timeNum || (*frame)[1].time == -1) {
            if(loopF) {
                sp->animNo = 0;
            } else {
                sp->animNo = bank->timeNum-1;
            }
        }
        *frame = &bank->frame[sp->animNo];
    } else if(sp->time < 0) {
        sp->animNo--;
        if(sp->animNo < 0) {
            if(loopF) {
                sp->animNo = bank->timeNum-1;
            } else {
                sp->animNo = 0;
            }
        }
        *frame = &bank->frame[sp->animNo];
        sp->time += (*frame)->time;
    }
}

void HuSprFinish(void)
{
    ANIMDATA *anim;
    ANIMBANK *bank;
    ANIMFRAME *frame;
    HUSPRITE *sp;
    s16 i;
    s16 j;
    s16 loopF;
    s16 timeInc;
    
    for(sp = &HuSprData[1], i=1; i<HUSPR_MAX; i++, sp++) {
        if(sp->data && !(sp->attr & HUSPR_ATTR_FUNC)) {
            if(!HuSprPauseF || (sp->attr & HUSPR_ATTR_NOPAUSE)) {
                anim = sp->data;
                bank = &anim->bank[sp->bank];
                frame = &bank->frame[sp->animNo];
                loopF = (sp->attr & HUSPR_ATTR_LOOP) ? 0 : 1;
                if(!(sp->attr & HUSPR_ATTR_NOANIM)) {
                    timeInc = (sp->attr & HUSPR_ATTR_REVERSE) ? -1 : 1;
                    for(j=0; j<(s32)sp->speed*minimumVcount; j++) {
                        sp->time += timeInc;
                        SpriteCalcFrame(sp, bank, &frame, loopF);
                    }
                    sp->time += (sp->speed*(float)minimumVcount)-j;
                    SpriteCalcFrame(sp, bank, &frame, loopF);
                }
                sp->dirty = 0;
            }
        }
    }
}

void HuSprPauseSet(BOOL value)
{
    HuSprPauseF = value;
}

ANIMDATA *HuSprAnimRead(void *data)
{
#ifdef TARGET_PC
    /* an ANM file cannot hold host pointers: src/port/spranim_pc.c converts it */
    return PortSprAnimRead(data);
#else
    s16 i;
    ANIMBMP *bmp;
    ANIMBANK *bank;
    ANIMPAT *pat;
    
    ANIMDATA *anim = (ANIMDATA *)data;
    if((u32)anim->bank & 0xFFFF0000) {
        anim->useNum++;
        return anim;
    }
    bank = (ANIMBANK *)((u32)anim->bank+(u32)data);
    anim->bank = bank;
    pat = (ANIMPAT *)((u32)anim->pat+(u32)data);
    anim->pat = pat;
    bmp = (ANIMBMP *)((u32)anim->bmp+(u32)data);
    anim->bmp = bmp;
    for(i=0; i<anim->bankNum; i++, bank++) {
        bank->frame = (ANIMFRAME *)((u32)bank->frame+(u32)data);
    }
    for(i=0; i<anim->patNum; i++, pat++) {
        pat->layer = (ANIMLAYER *)((u32)pat->layer+(u32)data);
    }
    for(i=0; i<anim->bmpNum; i++, bmp++) {
        bmp->palData = (void *)((u32)bmp->palData+(u32)data);
        bmp->data = (void *)((u32)bmp->data+(u32)data);
    }
    anim->useNum = 0;
    return anim;
#endif
}

void HuSprAnimLock(ANIMDATA *anim)
{
    anim->useNum++;
}

HUSPRID HuSprCreate(ANIMDATA *anim, s16 prio, s16 bank)
{
    HUSPRITE *sp;
    s16 i;
    for(sp = &HuSprData[1], i=1; i<HUSPR_MAX; i++, sp++) {
        if(!sp->data) {
            break;
        }
    }
    if(i == HUSPR_MAX) {
        OSReport("Error: Sprite Max Over!\n");
        return HUSPR_NONE;
    }
    sp->data = anim;
    sp->speed = 1.0f;
    sp->animNo = 0;
    sp->bank = bank;
    sp->time = 0.0f;
    sp->attr = 0;
    sp->drawNo = 0;
    sp->r = sp->g = sp->b = sp->a = 255;
    sp->pos.x = sp->pos.y = sp->zRot = 0.0f;
    sp->prio = prio;
    sp->scale.x = sp->scale.y = 1.0f;
    sp->wrapS = sp->wrapT = GX_CLAMP;
    sp->uvScaleX = sp->uvScaleY = 1;
    sp->bg = NULL;
    sp->scissorX = sp->scissorY = 0;
    sp->scissorW = 640;
    sp->scissorH = 480;
#ifdef TARGET_PC
    HuSprWideFixed[i] = FALSE;
#endif
    if(anim) {
        HuSprAnimLock(anim);
    }
    return i;
}

HUSPRID HuSprFuncCreate(HUSPRFUNC func, s16 prio)
{
    HUSPRITE *sp;
    HUSPRID sprId = HuSprCreate(NULL, prio, 0);
    if(sprId == HUSPR_NONE) {
        return HUSPR_NONE;
    }
    sp = &HuSprData[sprId];
    sp->func = func;
    sp->attr |= HUSPR_ATTR_FUNC;
    return sprId;
}

HUSPRGRPID HuSprGrpCreate(s16 sprNum)
{
    HUSPRGRP *gp;
    s16 i, j;
    for(gp = HuSprGrpData, i=0; i<HUSPR_GRP_MAX; i++, gp++) {
        if(gp->sprNum == 0) {
            break;
        }
    }
    if(i == HUSPR_GRP_MAX) {
        return HUSPR_GRP_NONE;
    }
    gp->sprId = HuMemDirectMalloc(HEAP_HEAP, sizeof(HUSPRID)*sprNum);
    for(j=0; j<sprNum; j++) {
        gp->sprId[j] = HUSPR_NONE;
    }
    gp->sprNum = sprNum;
    gp->pos.x = gp->pos.y = gp->zRot = gp->center.x = gp->center.y = 0.0f;
    gp->scale.x = gp->scale.y = 1.0f;
    return i;
}

HUSPRGRPID HuSprGrpCopy(HUSPRGRPID grpId)
{
    HUSPRGRP *newGp;
    HUSPRGRP *gp = GRPP(grpId);
    HUSPRGRPID newGrpId = HuSprGrpCreate(gp->sprNum);
    s16 i;
    if(newGrpId == HUSPR_GRP_NONE) {
        return HUSPR_GRP_NONE;
    }
    newGp = &HuSprGrpData[newGrpId];
    newGp->pos.x = gp->pos.x;
    newGp->pos.y = gp->pos.y;
    newGp->zRot = gp->zRot;
    newGp->scale.x = gp->scale.x;
    newGp->scale.y = gp->scale.y;
    newGp->center.x = gp->center.x;
    newGp->center.y = gp->center.y;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HUSPRITE *sp = &HuSprData[gp->sprId[i]];
            s16 sprId = HuSprCreate(sp->data, sp->prio, sp->bank);
            HuSprData[sprId] = *sp;
            HuSprGrpMemberSet(newGrpId, i, sprId);
        }
    }
    return newGrpId;
}

void HuSprGrpMemberSet(HUSPRGRPID grpId, s16 memberNo, HUSPRID sprId)
{
    HUSPRGRP *gp = GRPP(grpId);
    HUSPRITE *sp = SPRP(sprId);
    if(gp->sprNum == 0 || gp->sprNum <= memberNo || gp->sprId[memberNo] != HUSPR_NONE) {
        return;
    }
    gp->sprId[memberNo] = sprId;
}

void HuSprGrpMemberKill(HUSPRGRPID grpId, s16 memberNo)
{
    HUSPRGRP *gp = GRPP(grpId);
    if(gp->sprNum == 0 || gp->sprNum <= memberNo || gp->sprId[memberNo] == HUSPR_NONE) {
        return;
    }
    HuSprKill(gp->sprId[memberNo]);
    gp->sprId[memberNo] = HUSPR_NONE;
}

void HuSprGrpKill(HUSPRGRPID grpId)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprKill(gp->sprId[i]);
        }
    }
    gp->sprNum = 0;
    HuMemDirectFree(gp->sprId);
}

void HuSprKill(HUSPRID sprId)
{
    HUSPRITE *sp = SPRP(sprId);
    if(!sp->data) {
        return;
    }
    if(!(sp->attr & HUSPR_ATTR_FUNC)) {
        HuSprAnimKill(sp->data);
    }
    if(sp->bg) {
        HuSprAnimKill(sp->bg);
        sp->bg = NULL;
    }
    sp->data = NULL;
}

void HuSprAnimKill(ANIMDATA *anim)
{
#ifdef TARGET_PC
    /* a stale animation (the board's capsule glow effect, killed after its memory was freed and reused):
     * the console reads RAM there and HuMemDirectFree reports the bad block; here it may not be readable */
    if(!HuMemBlockValid(anim)) {
        OSReport("HuMem>memory free error. %p( call 00000000)\n", (void *)anim);
        return;
    }
#endif
    if(--anim->useNum <= 0) {
#ifdef TARGET_PC
        /* its file may be read again after this */
        PortSprAnimForget(anim);
#endif
        if(anim->bmpNum & ANIM_BMP_ALLOC) {
            if(anim->bmp->data) {
                HuMemDirectFree(anim->bmp->data);
            }
            if(anim->bmp->palData) {
                HuMemDirectFree(anim->bmp->palData);
            }
        }
        HuMemDirectFree(anim);
    }
}

void HuSprAttrSet(HUSPRGRPID grpId, s16 memberNo, s32 attr)
{
    HUSPRGRP *gp = GRPP(grpId);
    HUSPRITE *sp;
    if(gp->sprNum == 0 || gp->sprNum <= memberNo || gp->sprId[memberNo] == HUSPR_NONE) {
        return;
    }
    sp = &HuSprData[gp->sprId[memberNo]];
    sp->attr |= attr;
    sp->dirty |= SPRITE_DIRTY_ATTR;
}

void HuSprAttrReset(HUSPRGRPID grpId, s16 memberNo, s32 attr)
{
    HUSPRGRP *gp = GRPP(grpId);
    HUSPRITE *sp;
    if(gp->sprNum == 0 || gp->sprNum <= memberNo || gp->sprId[memberNo] == HUSPR_NONE) {
        return;
    }
    sp = &HuSprData[gp->sprId[memberNo]];
    sp->attr &= ~attr;
    sp->dirty |= SPRITE_DIRTY_ATTR;
}

u16 HuSprAttrGet(HUSPRGRPID grpId, s16 memberNo)
{
    HUSPRGRP *gp = GRPP(grpId);
    HUSPRITE *sp;
    if(gp->sprNum == 0 || gp->sprNum <= memberNo || gp->sprId[memberNo] == HUSPR_NONE) {
#ifdef TARGET_PC
        return 0; // GC r3 = grpId (untouched arg); no callers
#else
        return;
#endif
    }
    sp = &HuSprData[gp->sprId[memberNo]];
    return sp->attr;
}

void HuSprPosSet(HUSPRGRPID grpId, s16 memberNo, float posX, float posY)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->pos.x = posX;
    sp->pos.y = posY;
    sp->dirty |= SPRITE_DIRTY_XFORM;
}

void HuSprZRotSet(HUSPRGRPID grpId, s16 memberNo, float zRot)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->zRot = zRot;
    sp->dirty |= SPRITE_DIRTY_XFORM;
}

void HuSprScaleSet(HUSPRGRPID grpId, s16 memberNo, float scaleX, float scaleY)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->scale.x = scaleX;
    sp->scale.y = scaleY;
    sp->dirty |= SPRITE_DIRTY_XFORM;
}

void HuSprTPLvlSet(HUSPRGRPID grpId, s16 memberNo, float tpLvl)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->a = tpLvl*255;
    sp->dirty |= SPRITE_DIRTY_COLOR;
}

void HuSprColorSet(HUSPRGRPID grpId, s16 memberNo, u8 r, u8 g, u8 b)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->r = r;
    sp->g = g;
    sp->b = b;
    sp->dirty |= SPRITE_DIRTY_COLOR;
}

void HuSprSpeedSet(HUSPRGRPID grpId, s16 memberNo, float speed)
{
    HUSPRGRP *gp = GRPP(grpId);
#ifdef TARGET_PC
    SPRMEM(grpId, memberNo)->speed = speed;
#else
    HuSprData[gp->sprId[memberNo]].speed = speed;
#endif
}

void HuSprBankSet(HUSPRGRPID grpId, s16 memberNo, s16 bank)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    ANIMDATA *anim = sp->data;
    ANIMBANK *bank_ptr = &anim->bank[sp->bank];
    ANIMFRAME *frame_ptr = &bank_ptr->frame[sp->animNo];
    sp->bank = bank;
    if(sp->attr & HUSPR_ATTR_REVERSE) {
        sp->animNo = bank_ptr->timeNum-1;
        frame_ptr = &bank_ptr->frame[sp->animNo];
        sp->time = frame_ptr->time;
    } else {
        sp->time = 0;
        sp->animNo = 0;
    }
}

void HuSprAnimNoSet(HUSPRGRPID grpId, s16 memberNo, s16 animNo)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    ANIMDATA *anim = sp->data;
    ANIMBANK *bank_ptr = &anim->bank[sp->bank];
    if(bank_ptr->timeNum <= animNo) {
        OSReport("Error: AnimNoSet Over %d\n", animNo);
        animNo = 0;
    }
    sp->animNo = animNo;
    sp->time = 0;
}

void HuSprAnimNoSetPause(HUSPRGRPID grpId, s16 memberNo, s16 animNo)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    HuSprAnimNoSet(grpId, memberNo, animNo);
    sp->attr |= HUSPR_ATTR_NOANIM;
}

void HuSprGrpPosSet(HUSPRGRPID grpId, float posX, float posY)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    gp->pos.x = posX;
    gp->pos.y = posY;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != -1) {
            HuSprData[gp->sprId[i]].dirty |= SPRITE_DIRTY_XFORM;
        }
    }
}

void HuSprGrpCenterSet(HUSPRGRPID grpId, float centerX, float centerY)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    gp->center.x = centerX;
    gp->center.y = centerY;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprData[gp->sprId[i]].dirty |= SPRITE_DIRTY_XFORM;
        }
    }
}

void HuSprGrpZRotSet(HUSPRGRPID grpId, float zRot)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    gp->zRot = zRot;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprData[gp->sprId[i]].dirty |= SPRITE_DIRTY_XFORM;
        }
    }
}

void HuSprGrpScaleSet(HUSPRGRPID grpId, float scaleX, float scaleY)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    gp->scale.x = scaleX;
    gp->scale.y = scaleY;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprData[gp->sprId[i]].dirty |= SPRITE_DIRTY_XFORM;
        }
    }
}

void HuSprGrpTPLvlSet(HUSPRGRPID grpId, float tpLvl)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprData[gp->sprId[i]].a = tpLvl*255;
            HuSprData[gp->sprId[i]].dirty |= SPRITE_DIRTY_COLOR;
        }
    }
}

void HuSprGrpDrawNoSet(HUSPRGRPID grpId, s32 drawNo)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprData[gp->sprId[i]].drawNo = drawNo;
        }
    }
}

void HuSprDrawNoSet(HUSPRGRPID grpId, s16 memberNo, s32 drawNo)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->drawNo = drawNo;
}

void HuSprPriSet(HUSPRGRPID grpId, s16 memberNo, s16 prio)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->prio = prio;
}

void HuSprGrpScissorSet(HUSPRGRPID grpId, s16 x, s16 y, s16 w, s16 h)
{
    HUSPRGRP *gp = GRPP(grpId);
    s16 i;
    for(i=0; i<gp->sprNum; i++) {
        if(gp->sprId[i] != HUSPR_NONE) {
            HuSprScissorSet(grpId, i, x, y, w, h);
        }
    }
}

void HuSprScissorSet(HUSPRGRPID grpId, s16 memberNo, s16 x, s16 y, s16 w, s16 h)
{
    HUSPRITE *sp = SPRMEM(grpId, memberNo);
    sp->scissorX = x;
    sp->scissorY = y;
    sp->scissorW = w;
    sp->scissorH = h;
}

static s16 bitSizeTbl[11] = { 32, 24, 16, 8, 4, 16, 8, 8, 4, 8, 4 };

ANIMDATA *HuSprAnimMake(s16 sizeX, s16 sizeY, s16 dataFmt)
{
    ANIMLAYER *layer;
    ANIMBMP *bmp;
    ANIMDATA *anim;
    ANIMPAT *pat;
    ANIMFRAME *frame;
    void *temp;
    ANIMBANK *bank;
    ANIMDATA *newAnim;

    anim = newAnim = HuMemDirectMalloc(HEAP_MODEL, sizeof(ANIMDATA)+sizeof(ANIMBANK)+sizeof(ANIMFRAME)
                                            +sizeof(ANIMPAT)+sizeof(ANIMLAYER)+sizeof(ANIMBMP));

    bank = temp = &newAnim[1];
    anim->bank = bank;
    frame = temp = ((char *)temp+sizeof(ANIMBANK));
    bank->frame = frame;
    pat = temp = ((char *)temp+sizeof(ANIMFRAME));
    anim->pat = pat;
    layer = temp = ((char *)temp+sizeof(ANIMPAT));
    pat->layer = layer;
    bmp = temp = ((char *)temp+sizeof(ANIMLAYER));
    anim->bmp = bmp;
    anim->useNum = 0;
    anim->bankNum = 1;
    anim->patNum = 1;
    anim->bmpNum = (1|ANIM_BMP_ALLOC);
    bank->timeNum = 1;
    bank->unk = 10;
    frame->pat = 0;
    frame->time = 10;
    frame->shiftX = frame->shiftY = frame->flip = 0;
    pat->layerNum = 1;
    pat->centerX = sizeX/2;
    pat->centerY = sizeY/2;
    pat->sizeX = sizeX;
    pat->sizeY = sizeY;
    layer->alpha = 255;
    layer->flip = 0;
    layer->bmpNo = 0;
    layer->startX = layer->startY = 0;
    layer->sizeX = sizeX;
    layer->sizeY = sizeY;
    layer->shiftX = layer->shiftY = 0;
    layer->vtx[0] = layer->vtx[1] = 0;
    layer->vtx[2] = sizeX;
    layer->vtx[3] = 0;
    layer->vtx[4] = sizeX;
    layer->vtx[5] = sizeY;
    layer->vtx[6] = 0;
    layer->vtx[7] = sizeY;
    bmp->pixSize = bitSizeTbl[dataFmt];
    bmp->dataFmt = dataFmt;
    bmp->palNum = 0;
    bmp->sizeX = sizeX;
    bmp->sizeY = sizeY;
    bmp->dataSize = sizeX*sizeY*bitSizeTbl[dataFmt]/8;
    bmp->palData = NULL;
    bmp->data = NULL;
    return anim;
}

void HuSprBGSet(HUSPRGRPID grpId, s16 memberNo, ANIMDATA *bg, s16 bgBank)
{
#ifdef TARGET_PC
    HUSPRGRP *gp = GRPP(grpId);
    HUSPRID sprite = (gp->sprNum > 0 && memberNo >= 0 && memberNo < gp->sprNum) ? gp->sprId[memberNo] : HUSPR_NONE;
    HuSprSprBGSet(sprite, bg, bgBank);
#else
    HUSPRID sprite = HuSprGrpData[grpId].sprId[memberNo];
    HuSprSprBGSet(sprite, bg, bgBank);
#endif
}

void HuSprSprBGSet(HUSPRID sprite, ANIMDATA *bg, s16 bgBank)
{
    HUSPRITE *sp = SPRP(sprite);
    sp->bg = bg;
    sp->bgBank = bgBank;
    sp->wrapT = sp->wrapS = GX_REPEAT;
    sp->attr &= ~HUSPR_ATTR_LINEAR;
    HuSprAnimLock(bg);
}

void AnimDebug(ANIMDATA *anim)
{
    ANIMPAT *pat;
    ANIMLAYER *layer;
    s16 i;
    s16 j;
    ANIMFRAME *frame;
    ANIMBANK *bank;
    ANIMBMP *bmp;
    
    OSReport("patNum %d,bankNum %d,bmpNum %d\n", anim->patNum, anim->bankNum, anim->bmpNum & ANIM_BMP_NUM_MASK);
    pat = anim->pat;
    for(i=0; i<anim->patNum; i++) {
        OSReport("PATTERN%d:\n", i);
        OSReport("\tlayerNum %d,center (%d,%d),size (%d,%d)\n", pat->layerNum, pat->centerX, pat->centerX, pat->sizeX, pat->sizeY);
        layer = pat->layer;
        for(j=0; j<pat->layerNum; j++) {
            OSReport("\t\tfileNo %d,flip %x\n", layer->bmpNo, layer->flip);
            OSReport("\t\tstart (%d,%d),size (%d,%d),shift (%d,%d)\n", layer->startX, layer->startY, layer->sizeX, layer->sizeY, layer->shiftX, layer->shiftY);
            if(j != pat->layerNum-1) {
                OSReport("\n");
            }
            layer++;
        }
        pat++;
    }
    bank = anim->bank;
    for(i=0; i<anim->bankNum; i++) {
        OSReport("BANK%d:\n", i);
        OSReport("\ttimeNum %d\n", bank->timeNum);
        frame = bank->frame;
        for(j=0; j<bank->timeNum; j++) {
            OSReport("\t\tpat %d,time %d,shift(%d,%d),flip %x\n", frame->pat, frame->time, frame->shiftX, frame->shiftY, frame->flip);
            frame++;
        }
        bank++;
    }
    bmp = anim->bmp;
    for(i=0; i<anim->bmpNum & ANIM_BMP_NUM_MASK; i++) {
        OSReport("BMP%d:\n", i);
        OSReport("\tpixSize %d,palNum %d,size (%d,%d)\n", bmp->pixSize, bmp->palNum, bmp->sizeX, bmp->sizeY);
        bmp++;
    }
}