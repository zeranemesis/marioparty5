#include "REL/sd00dll.h"

/* Declared locally: sd00dll.h covers neither the rabbit state nor
   Hu3DModelPosSetV.  game/hu3d.h is deliberately not included - it pulls
   MSL math.h, whose `extern inline sqrtf` emits 16 bytes of rodata this
   unit does not have. */
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} RabbitVecF;

void Hu3DModelPosSetV(s16 modelId, RabbitVecF *pos);

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} RabbitVec;

/* Flat s32 rabbit state table.  Word 4 is the shared reset flag, word
   5 + 4 * player is that player's display timer, and word 1 is the
   fixed point position vector handed to Hu3DModelPosSetV. */
static s32 rabbitBaseModel[10];

/* The per-player timer lives at a byte offset of 4 * player past the table
   base, so it has to be reached through a byte pointer: an ordinary
   `rabbitBaseModel[player * 4 + 5]` re-scales the index by the element
   size and lands one `addi` away from the original. */
#define RABBIT_DISP_TIMER(player) (*((s32 *)((u8 *)rabbitBaseModel + (player) * 4) + 5))

static s32 rabbitMode;
static RabbitVec rabbitZAxis;

static s32 rabbitOverridePos;
static s16 rabbitEffectMdlId;

static void RabbitEffectPosSet(void)
{
    rabbitOverridePos = 1;
    Hu3DModelPosSetV(rabbitEffectMdlId, (RabbitVecF *)&rabbitBaseModel[1]);
}

s32 SdRabbitCheck(void)
{
    s32 ret;

    if (rabbitMode == 1) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void SdRabbitPlayerCheck(s32 player)
{
    if (rabbitMode == 1) {
        rabbitMode++;
        RABBIT_DISP_TIMER(player)++;
        rabbitBaseModel[4] = 1;
    }
}

s32 SdRabbitDispTimerGet(s32 player)
{
    return RABBIT_DISP_TIMER(player);
}

void SdRabbitZAxisGet(RabbitVec *zAxis)
{
    *zAxis = rabbitZAxis;
}