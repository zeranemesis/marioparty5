#define _MATH_H

#include "game/sprite.h"

typedef struct M562SpriteParent_s {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} M562SpriteParent;

typedef struct M562Sprite_s {
    M562SpriteParent *parent;
    s32 show;
    s32 shown;
    HUSPRGRPID grpId;
    HUSPRID baseSprId;
    ANIMDATA *anim;
    HUSPRID sprId[10];
    ANIMDATA *wakuAnim;
    s16 count;
    s16 frame;
} M562Sprite;

static s32 lbl_1_data_BF0[16] = {
    0x960000,
    0x960001,
    0x960002,
    0x960003,
    0x960004,
    0x960006,
    0x960007,
    0x960008,
    0x960009,
    0x96000A,
    0x96000B,
    0x96000C,
    0x96000D,
    0x96000A,
    0x96000A,
    0x75006F
};

void fn_1_23ECC(M562Sprite *sprite, s32 animFile, s16 num, s32 right, s32 parent) {
    s32 i;
    HUSPRGRPID grpId;
    HUSPRID sprId;

    memset(sprite, 0, 0x30);
    sprite->parent = (M562SpriteParent *) parent;
    sprite->count = num;
    sprite->frame = sprite->count * 5;
    sprite->show = 0;
    sprite->shown = 0;
    sprite->anim = HuSprAnimDataRead(lbl_1_data_BF0[animFile]);
    sprite->wakuAnim = HuSprAnimDataRead(0x75006E);
    grpId = HuSprGrpCreate(0xB);
    sprite->grpId = grpId;
    if (right == 0) {
        HuSprGrpPosSet(grpId, 32.0f, 112.0f);
    } else {
        HuSprGrpPosSet(grpId, 544.0f, 112.0f);
    }
    sprId = HuSprCreate(sprite->anim, 0xA, 0);
    sprite->baseSprId = sprId;
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprPosSet(grpId, 0, 0.0f, 0.0f);
    HuSprScaleSet(grpId, 0, 0.0f, 0.0f);
    for (i = 0; i < 10; i++) {
        sprId = HuSprCreate(sprite->wakuAnim, 0xA, 0);
        sprite->sprId[i] = sprId;
        HuSprGrpMemberSet(grpId, i + 1, sprId);
        HuSprPosSet(grpId, i + 1, 0.0f, 32.0f + 24.0f * (f32) i);
        HuSprBankSet(grpId, i + 1, 0);
        HuSprScaleSet(grpId, i + 1, 0.0f, 0.0f);
        if (i >= num) {
            HuSprScaleSet(grpId, i + 1, 0.0f, 0.0f);
        }
    }
}

void fn_1_24178(M562Sprite *sprite) {
    s32 i;

    for (i = 0; i < 10; i++) {
        HuSprKill(sprite->sprId[i]);
    }
    HuSprKill(sprite->baseSprId);
    HuSprGrpKill(sprite->grpId);
    memset(sprite, 0, 0x30);
}

void fn_1_241EC(M562Sprite *sprite) {
    M562SpriteParent *parent;
    s32 num;
    s32 i;

    parent = sprite->parent;
    if (sprite->count > 10) {
        num = 10;
    } else {
        num = (sprite->count < 0) ? 0 : sprite->count;
    }
    sprite->count = num;
    if ((sprite->show != 0) && (sprite->shown == 0)) {
        HuSprScaleSet(sprite->grpId, 0, 1.0f, 1.0f);
        sprite->shown = 1;
    } else {
        if ((sprite->show == 0) && (sprite->shown != 0)) {
            for (i = 0; i < 11; i++) {
                HuSprScaleSet(sprite->grpId, i, 0.0f, 0.0f);
            }
            sprite->shown = 0;
        }
    }
    if (sprite->shown != 0) {
        for (i = 0; i < 10; i++) {
            if (i >= (sprite->frame + 4) / 5) {
                HuSprScaleSet(sprite->grpId, i + 1, 0.0f, 0.0f);
            } else {
                HuSprScaleSet(sprite->grpId, i + 1, 0.5f, 0.5f);
                if (i < sprite->frame / 5) {
                    HuSprBankSet(sprite->grpId, i + 1, 0);
                } else {
                    HuSprBankSet(sprite->grpId, i + 1, 4 - sprite->frame % 5);
                }
            }
        }
        if ((parent->unk8 % 2) == 0) {
            if (sprite->frame < sprite->count * 5) {
                sprite->frame += 1;
            } else {
                if (sprite->frame > sprite->count * 5) {
                    sprite->frame -= 1;
                }
            }
        }
    }
}
