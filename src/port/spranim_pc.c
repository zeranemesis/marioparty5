/*
 * Sprite animations (ANM) on a 64-bit host.
 *
 * HuSprAnimRead turns the offsets an ANM file holds into pointers, in place,
 * and hands the file buffer itself back as the animation. That works on
 * GameCube, where a pointer is four bytes and the file is already big-endian.
 *
 * Here the file is converted into one freshly allocated block laid out like
 * the file was — header, banks, frames, patterns, layers, bitmaps, then the
 * texture and palette bytes — so that HuSprAnimKill still frees an animation
 * with a single HuMemDirectFree, exactly as it does on the console.
 *
 * Texture and palette bytes are copied without being swapped: that is the
 * order Aurora's texture decoder expects.
 */

#include "port/spranim.h"

#include <string.h>

#include "dolphin.h"
#include "game/animdata.h"
#include "game/memory.h"
#include "port/endian.h"

/* the file's own layout: every pointer is a 32-bit offset from its start */
typedef struct AnimFileData_s {
    u16 bankNum;
    u16 patNum;
    u16 bmpNum;
    u16 useNum;
    u32 bank;
    u32 pat;
    u32 bmp;
} ANIMFILEDATA;

typedef struct AnimFileBank_s {
    u16 timeNum;
    u16 unk;
    u32 frame;
} ANIMFILEBANK;

typedef struct AnimFileFrame_s {
    u16 pat;
    u16 time;
    u16 shiftX;
    u16 shiftY;
    u16 flip;
    u16 pad;
} ANIMFILEFRAME;

typedef struct AnimFilePat_s {
    u16 layerNum;
    u16 centerX;
    u16 centerY;
    u16 sizeX;
    u16 sizeY;
    u32 layer;
} ANIMFILEPAT;

typedef struct AnimFileLayer_s {
    u8 alpha;
    u8 flip;
    u16 bmpNo;
    u16 startX;
    u16 startY;
    u16 sizeX;
    u16 sizeY;
    u16 shiftX;
    u16 shiftY;
    u16 vtx[8];
} ANIMFILELAYER;

typedef struct AnimFileBmp_s {
    u8 pixSize;
    u8 dataFmt;
    u16 palNum;
    u16 sizeX;
    u16 sizeY;
    u32 dataSize;
    u32 palData;
    u32 data;
} ANIMFILEBMP;

/*
 * Reading the same file twice must give the same animation back, as it did
 * when the animation was the file: hsfman.c, for one, compares what it got
 * against the data it passed in.
 */
#define SPRANIM_CACHE_MAX 256

static struct {
    const void *file;
    ANIMDATA *anim;
} sCache[SPRANIM_CACHE_MAX];

static u32 Align32(u32 value)
{
    return (value + 31) & ~31u;
}

ANIMDATA *PortSprAnimFind(const void *file)
{
    u32 i;

    for (i = 0; i < SPRANIM_CACHE_MAX; i++) {
        if (sCache[i].file == file) {
            return sCache[i].anim;
        }
    }
    return NULL;
}

static void CacheAdd(const void *file, ANIMDATA *anim)
{
    u32 i;

    for (i = 0; i < SPRANIM_CACHE_MAX; i++) {
        if (sCache[i].file == NULL) {
            sCache[i].file = file;
            sCache[i].anim = anim;
            return;
        }
    }
    OSReport("spranim> no room left to remember a converted animation\n");
}

void PortSprAnimForget(const ANIMDATA *anim)
{
    u32 i;

    for (i = 0; i < SPRANIM_CACHE_MAX; i++) {
        if (sCache[i].anim == anim) {
            sCache[i].file = NULL;
            sCache[i].anim = NULL;
        }
    }
}

ANIMDATA *PortSprAnimRead(void *file)
{
    const u8 *base = file;
    const ANIMFILEDATA *fileAnim = file;
    const ANIMFILEBANK *fileBanks;
    const ANIMFILEPAT *filePats;
    const ANIMFILEBMP *fileBmps;
    ANIMDATA *anim;
    ANIMBANK *banks;
    ANIMFRAME *frames;
    ANIMPAT *pats;
    ANIMLAYER *layers;
    ANIMBMP *bmps;
    u8 *blob;
    u32 bankNum;
    u32 patNum;
    u32 bmpNum;
    u32 frameNum = 0;
    u32 layerNum = 0;
    u32 blobSize = 0;
    u32 size;
    u32 i;
    u32 j;

    anim = PortSprAnimFind(file);
    if (anim != NULL) {
        anim->useNum++;
        return anim;
    }

    bankNum = PORT_BE16(fileAnim->bankNum);
    patNum = PORT_BE16(fileAnim->patNum);
    bmpNum = PORT_BE16(fileAnim->bmpNum) & ANIM_BMP_NUM_MASK;
    fileBanks = (const ANIMFILEBANK *)(base + PORT_BE32(fileAnim->bank));
    filePats = (const ANIMFILEPAT *)(base + PORT_BE32(fileAnim->pat));
    fileBmps = (const ANIMFILEBMP *)(base + PORT_BE32(fileAnim->bmp));

    for (i = 0; i < bankNum; i++) {
        frameNum += PORT_BE16(fileBanks[i].timeNum);
    }
    for (i = 0; i < patNum; i++) {
        layerNum += PORT_BE16(filePats[i].layerNum);
    }
    for (i = 0; i < bmpNum; i++) {
        blobSize += Align32(PORT_BE32(fileBmps[i].dataSize));
        blobSize += Align32(PORT_BE16(fileBmps[i].palNum) * sizeof(u16));
    }

    size = sizeof(ANIMDATA) + bankNum * sizeof(ANIMBANK) + frameNum * sizeof(ANIMFRAME)
         + patNum * sizeof(ANIMPAT) + layerNum * sizeof(ANIMLAYER) + bmpNum * sizeof(ANIMBMP);
    size = Align32(size) + blobSize;

    anim = HuMemDirectMallocNum(HEAP_MODEL, (s32)size, HuMemNumGet(file));
    if (anim == NULL) {
        OSReport("spranim> no memory to convert an animation of %u bytes\n", size);
        return NULL;
    }
    memset(anim, 0, size);

    banks = (ANIMBANK *)(anim + 1);
    frames = (ANIMFRAME *)(banks + bankNum);
    pats = (ANIMPAT *)(frames + frameNum);
    layers = (ANIMLAYER *)(pats + patNum);
    bmps = (ANIMBMP *)(layers + layerNum);
    blob = (u8 *)anim + Align32((u32)((u8 *)(bmps + bmpNum) - (u8 *)anim));

    anim->bankNum = (s16)bankNum;
    anim->patNum = (s16)patNum;
    anim->bmpNum = (s16)PORT_BE16(fileAnim->bmpNum);
    anim->useNum = 0;
    anim->bank = banks;
    anim->pat = pats;
    anim->bmp = bmps;

    for (i = 0; i < bankNum; i++) {
        const ANIMFILEFRAME *fileFrames = (const ANIMFILEFRAME *)(base + PORT_BE32(fileBanks[i].frame));
        u32 timeNum = PORT_BE16(fileBanks[i].timeNum);

        banks[i].timeNum = (s16)timeNum;
        banks[i].unk = (s16)PORT_BE16(fileBanks[i].unk);
        banks[i].frame = frames;
        for (j = 0; j < timeNum; j++, frames++) {
            frames->pat = (s16)PORT_BE16(fileFrames[j].pat);
            frames->time = (s16)PORT_BE16(fileFrames[j].time);
            frames->shiftX = (s16)PORT_BE16(fileFrames[j].shiftX);
            frames->shiftY = (s16)PORT_BE16(fileFrames[j].shiftY);
            frames->flip = (s16)PORT_BE16(fileFrames[j].flip);
            frames->pad = (s16)PORT_BE16(fileFrames[j].pad);
        }
    }

    for (i = 0; i < patNum; i++) {
        const ANIMFILELAYER *fileLayers = (const ANIMFILELAYER *)(base + PORT_BE32(filePats[i].layer));
        u32 count = PORT_BE16(filePats[i].layerNum);

        pats[i].layerNum = (s16)count;
        pats[i].centerX = (s16)PORT_BE16(filePats[i].centerX);
        pats[i].centerY = (s16)PORT_BE16(filePats[i].centerY);
        pats[i].sizeX = (s16)PORT_BE16(filePats[i].sizeX);
        pats[i].sizeY = (s16)PORT_BE16(filePats[i].sizeY);
        pats[i].layer = layers;
        for (j = 0; j < count; j++, layers++) {
            u32 k;

            layers->alpha = fileLayers[j].alpha;
            layers->flip = fileLayers[j].flip;
            layers->bmpNo = (s16)PORT_BE16(fileLayers[j].bmpNo);
            layers->startX = (s16)PORT_BE16(fileLayers[j].startX);
            layers->startY = (s16)PORT_BE16(fileLayers[j].startY);
            layers->sizeX = (s16)PORT_BE16(fileLayers[j].sizeX);
            layers->sizeY = (s16)PORT_BE16(fileLayers[j].sizeY);
            layers->shiftX = (s16)PORT_BE16(fileLayers[j].shiftX);
            layers->shiftY = (s16)PORT_BE16(fileLayers[j].shiftY);
            for (k = 0; k < 8; k++) {
                layers->vtx[k] = (s16)PORT_BE16(fileLayers[j].vtx[k]);
            }
        }
    }

    for (i = 0; i < bmpNum; i++) {
        u32 dataSize = PORT_BE32(fileBmps[i].dataSize);
        u32 palSize = PORT_BE16(fileBmps[i].palNum) * sizeof(u16);

        bmps[i].pixSize = fileBmps[i].pixSize;
        bmps[i].dataFmt = fileBmps[i].dataFmt;
        bmps[i].palNum = (s16)PORT_BE16(fileBmps[i].palNum);
        bmps[i].sizeX = (s16)PORT_BE16(fileBmps[i].sizeX);
        bmps[i].sizeY = (s16)PORT_BE16(fileBmps[i].sizeY);
        bmps[i].dataSize = dataSize;
        if (dataSize != 0) {
            memcpy(blob, base + PORT_BE32(fileBmps[i].data), dataSize);
            bmps[i].data = blob;
            blob += Align32(dataSize);
        }
        if (palSize != 0) {
            memcpy(blob, base + PORT_BE32(fileBmps[i].palData), palSize);
            bmps[i].palData = blob;
            blob += Align32(palSize);
        }
    }

    CacheAdd(file, anim);
    return anim;
}
