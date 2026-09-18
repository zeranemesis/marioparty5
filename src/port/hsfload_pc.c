/*
 * HSF model loading on a 64-bit host.
 *
 * src/game/hsfload.c reads a model straight into the game's structures: on
 * GameCube a pointer is four bytes, the file's values are already big-endian,
 * and the loader only turns offsets and indices into pointers, in place.
 *
 * Here every record is converted instead: read through include/port/hsffile.h,
 * which describes the file as it is written, and written into a freshly
 * allocated structure of the game's own type. The order of the steps and the
 * meaning of every field follow src/game/hsfload.c exactly — read the two side
 * by side.
 *
 * What is NOT converted, deliberately:
 *   - texture and palette data stay in GameCube byte order, which is what
 *     Aurora's texture decoder expects;
 *   - vertex, normal, texture-coordinate and colour arrays ARE converted, and
 *     the draw code tells Aurora so (GXSETARRAY's `le` argument).
 *
 * Allocations are tagged with the model's own address, which is the number
 * Hu3DModelKill frees them by, exactly like the rest of a model's memory.
 */

#include "game/hsfload.h"

#include <string.h>
#include <ctype.h>

#include "dolphin.h"
#include "game/EnvelopeExec.h"
#include "game/memory.h"
#include "port/endian.h"
#include "port/hsffile.h"

static GXColor rgba[100];
static char *StringTable;

static const u8 *sFile;          /* the model file, untouched */
static HSFFILEHEADER sFileHead;  /* its header, byte-swapped */
static HSFDATA *sModel;          /* what the loader is building */
static u32 sMallocNo;            /* memory number every allocation carries */
static const u32 *sSymbols;      /* symbol table: indices, big-endian */
static HSFOBJECT *sObjects;      /* converted object array */
static HSFBUFFER *sVertex;       /* converted buffer tables, needed by index */
static HSFBUFFER *sNormal;
static HSFBUFFER *sSt;
static HSFBUFFER *sColor;
static HSFBUFFER *sFace;
static HSFCENV *sCenv;
static HSFPART *sPart;
static HSFPALETTE *sPalette;
static HSFBITMAP *sBitmap;
static HSFCLUSTER *sCluster;
static const u8 *sVertexRegion;  /* file addresses of the raw data regions */
static const u8 *sNormalRegion;
static HuVecF *sVertexData;      /* and their converted copies */
static HuVecF *sNormalData;
static u32 sNormalElemSize;

/* ---------------------------------------------------------------- helpers */

static void *Alloc(u32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_MODEL, (s32)size, sMallocNo);
    if (ptr == NULL) {
        OSReport("hsfload> out of model memory for %u bytes\n", size);
    } else {
        memset(ptr, 0, size);
    }
    return ptr;
}

static const void *FilePtr(u32 offset)
{
    return sFile + offset;
}

static float ReadFloat(u32 raw)
{
    return PORT_BEF32(raw);
}

static void ReadVec(HuVecF *dst, const u32 *src)
{
    dst->x = ReadFloat(src[0]);
    dst->y = ReadFloat(src[1]);
    dst->z = ReadFloat(src[2]);
}

static void ReadVec2(HuVec2f *dst, const u32 *src)
{
    dst->x = ReadFloat(src[0]);
    dst->y = ReadFloat(src[1]);
}

static void ReadTransform(HSFTRANSFORM *dst, const HSFFILETRANSFORM *src)
{
    ReadVec(&dst->pos, src->pos);
    ReadVec(&dst->rot, src->rot);
    ReadVec(&dst->scale, src->scale);
}

/* A name is stored as an offset into the string table. */
static char *NameAt(u32 rawOffset)
{
    u32 offset = PORT_BE32(rawOffset);
    if (offset == 0xFFFFFFFFu) {
        return NULL;
    }
    return StringTable + offset;
}

char *SetName(u32 *strOfs)
{
    return NameAt(*strOfs);
}

static u32 SymbolAt(u32 index)
{
    return PORT_BE32(sSymbols[index]);
}

/* ------------------------------------------------------------- the header */

static void FileLoad(const void *data)
{
    u32 i;
    const u32 *src;
    u32 *dst;

    sFile = data;
    memcpy(&sFileHead, data, sizeof(HSFFILEHEADER));
    /* every field of the header but the magic is a 32-bit count or offset */
    src = (const u32 *)((const u8 *)data + 8);
    dst = (u32 *)((u8 *)&sFileHead + 8);
    for (i = 0; i < (sizeof(HSFFILEHEADER) - 8) / 4; i++) {
        dst[i] = PORT_BE32(src[i]);
    }

    StringTable = (char *)FilePtr(sFileHead.string.ofs);
    sSymbols = FilePtr(sFileHead.symbol.ofs);

    sModel = HuMemDirectMallocNum(HEAP_MODEL, sizeof(HSFDATA), HuMemNumGet((void *)data));
    memset(sModel, 0, sizeof(HSFDATA));
    memcpy(sModel->magic, data, sizeof(sModel->magic));
    sMallocNo = (u32)(uintptr_t)sModel;

    sObjects = NULL;
    sVertex = sNormal = sSt = sColor = sFace = NULL;
    sCenv = NULL;
    sPart = NULL;
    sPalette = NULL;
    sBitmap = NULL;
    sCluster = NULL;
    sVertexData = sNormalData = NULL;
    sVertexRegion = sNormalRegion = NULL;
}

/* --------------------------------------------------------------- sections */

static void SceneLoad(void)
{
    const HSFFILESCENE *file;

    if (sFileHead.scene.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.scene.ofs);
    sModel->scene = Alloc(sizeof(HSFSCENE) * sFileHead.scene.num);
    sModel->sceneNum = (s16)sFileHead.scene.num;
    /* the game only keeps the fog range of the first scene */
    sModel->scene->fogType = (GXFogType)PORT_BE32(file->fogType);
    sModel->scene->fogStart = ReadFloat(file->fogStart);
    sModel->scene->fogEnd = ReadFloat(file->fogEnd);
    sModel->scene->fogColor.r = file->fogColor[0];
    sModel->scene->fogColor.g = file->fogColor[1];
    sModel->scene->fogColor.b = file->fogColor[2];
    sModel->scene->fogColor.a = file->fogColor[3];
}

static void PaletteLoad(void)
{
    const HSFFILEPALETTE *file;
    const u8 *dataBase;
    u32 i;

    if (sFileHead.palette.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.palette.ofs);
    dataBase = (const u8 *)&file[sFileHead.palette.num];
    sPalette = Alloc(sizeof(HSFPALETTE) * sFileHead.palette.num);
    sModel->palette = sPalette;
    sModel->paletteNum = (s16)sFileHead.palette.num;
    for (i = 0; i < sFileHead.palette.num; i++) {
        sPalette[i].name = NameAt(file[i].name);
        sPalette[i].unk = (s32)PORT_BE32(file[i].unk);
        sPalette[i].palSize = PORT_BE32(file[i].palSize);
        /* palette entries stay in GameCube order for the texture decoder */
        sPalette[i].data = (u16 *)(dataBase + PORT_BE32(file[i].data));
    }
}

static HSFPALETTE *SearchPalettePtr(s32 id)
{
    if (id == -1 || sPalette == NULL) {
        return NULL;
    }
    return &sPalette[id];
}

static HSFBITMAP *SearchBitmapPtr(s32 id)
{
    if (id == -1 || sBitmap == NULL) {
        return NULL;
    }
    return &sBitmap[id];
}

static void BitmapLoad(void)
{
    const HSFFILEBITMAP *file;
    const u8 *dataBase;
    u32 i;

    if (sFileHead.bitmap.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.bitmap.ofs);
    dataBase = (const u8 *)&file[sFileHead.bitmap.num];
    sBitmap = Alloc(sizeof(HSFBITMAP) * sFileHead.bitmap.num);
    sModel->bitmap = sBitmap;
    sModel->bitmapNum = (s16)sFileHead.bitmap.num;
    for (i = 0; i < sFileHead.bitmap.num; i++) {
        HSFPALETTE *palette;

        sBitmap[i].name = NameAt(file[i].name);
        sBitmap[i].maxLod = PORT_BE32(file[i].maxLod);
        sBitmap[i].dataFmt = file[i].dataFmt;
        sBitmap[i].pixSize = file[i].pixSize;
        sBitmap[i].sizeX = (s16)PORT_BE16(file[i].sizeX);
        sBitmap[i].sizeY = (s16)PORT_BE16(file[i].sizeY);
        sBitmap[i].palSize = (s16)PORT_BE16(file[i].palSize);
        sBitmap[i].tint.r = file[i].tint[0];
        sBitmap[i].tint.g = file[i].tint[1];
        sBitmap[i].tint.b = file[i].tint[2];
        sBitmap[i].tint.a = file[i].tint[3];
        palette = SearchPalettePtr((s32)PORT_BE32(file[i].palData));
        sBitmap[i].palData = palette ? palette->data : NULL;
        sBitmap[i].unk = PORT_BE32(file[i].unk);
        /* texel data stays in GameCube order too */
        sBitmap[i].data = (void *)(dataBase + PORT_BE32(file[i].data));
    }
}

static void MaterialLoad(void)
{
    const HSFFILEMATERIAL *file;
    HSFMATERIAL *material;
    u32 i;
    u32 j;

    if (sFileHead.material.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.material.ofs);
    material = Alloc(sizeof(HSFMATERIAL) * sFileHead.material.num);
    sModel->material = material;
    sModel->materialNum = (s16)sFileHead.material.num;
    for (i = 0; i < sFileHead.material.num; i++) {
        u32 attrSymbol = PORT_BE32(file[i].attr);

        material[i].name = NameAt(file[i].name);
        material[i].pass = PORT_BE16(file[i].pass);
        material[i].vtxMode = file[i].vtxMode;
        material[i].litColor[0] = file[i].litColor[0];
        material[i].litColor[1] = file[i].litColor[1];
        material[i].litColor[2] = file[i].litColor[2];
        material[i].color[0] = file[i].color[0];
        material[i].color[1] = file[i].color[1];
        material[i].color[2] = file[i].color[2];
        material[i].shadowColor[0] = file[i].shadowColor[0];
        material[i].shadowColor[1] = file[i].shadowColor[1];
        material[i].shadowColor[2] = file[i].shadowColor[2];
        material[i].hiliteScale = ReadFloat(file[i].hiliteScale);
        material[i].unk18 = ReadFloat(file[i].unk18);
        material[i].invAlpha = ReadFloat(file[i].invAlpha);
        material[i].unk20[0] = ReadFloat(file[i].unk20[0]);
        material[i].unk20[1] = ReadFloat(file[i].unk20[1]);
        material[i].refAlpha = ReadFloat(file[i].refAlpha);
        material[i].unk2C = ReadFloat(file[i].unk2C);
        material[i].flags = PORT_BE32(file[i].flags);
        material[i].attrNum = PORT_BE32(file[i].attrNum);
        /* the attribute list is a run of the symbol table */
        material[i].attr = Alloc(sizeof(s32) * (material[i].attrNum ? material[i].attrNum : 1));
        for (j = 0; j < material[i].attrNum; j++) {
            material[i].attr[j] = (s32)SymbolAt(attrSymbol + j);
        }
        if (i < 100) {
            rgba[i].r = material[i].litColor[0];
            rgba[i].g = material[i].litColor[1];
            rgba[i].b = material[i].litColor[2];
            rgba[i].a = 255;
        }
    }
}

static void AttributeLoad(void)
{
    const HSFFILEATTRIBUTE *file;
    HSFATTRIBUTE *attribute;
    u32 i;

    if (sFileHead.attribute.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.attribute.ofs);
    attribute = Alloc(sizeof(HSFATTRIBUTE) * sFileHead.attribute.num);
    sModel->attribute = attribute;
    sModel->attributeNum = (s16)sFileHead.attribute.num;
    for (i = 0; i < sFileHead.attribute.num; i++) {
        attribute[i].name = NameAt(file[i].name);
        attribute[i].animWorkP = NULL;
        attribute[i].kColor = ReadFloat(file[i].kColor);
        attribute[i].nbtTpLvl = ReadFloat(file[i].nbtTpLvl);
        attribute[i].unk20 = ReadFloat(file[i].unk20);
        ReadVec2(&attribute[i].scale, file[i].scale);
        ReadVec2(&attribute[i].trans, file[i].trans);
        attribute[i].wrapS = PORT_BE32(file[i].wrapS);
        attribute[i].wrapT = PORT_BE32(file[i].wrapT);
        attribute[i].maxLod = PORT_BE32(file[i].maxLod);
        attribute[i].flag = PORT_BE32(file[i].flag);
        attribute[i].bitmap = SearchBitmapPtr((s32)PORT_BE32(file[i].bitmap));
    }
}

/*
 * A buffer table is followed by the data its entries point into. The size of
 * that region is not written anywhere, so it is taken from the entry that
 * reaches furthest.
 */
static u32 BufferRegionSize(const HSFFILEBUFFER *file, u32 num, u32 elemSize)
{
    u32 size = 0;
    u32 i;

    for (i = 0; i < num; i++) {
        u32 end = PORT_BE32(file[i].data) + PORT_BE32(file[i].count) * elemSize;
        if (end > size) {
            size = end;
        }
    }
    return size;
}

static void ConvertFloats(void *dst, const void *src, u32 count)
{
    const u32 *in = src;
    u32 *out = dst;
    u32 i;

    for (i = 0; i < count; i++) {
        out[i] = PORT_BE32(in[i]);
    }
}

static void VertexLoad(void)
{
    const HSFFILEBUFFER *file;
    u32 regionSize;
    u32 i;

    if (sFileHead.vertex.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.vertex.ofs);
    sVertexRegion = (const u8 *)&file[sFileHead.vertex.num];
    regionSize = BufferRegionSize(file, sFileHead.vertex.num, sizeof(HuVecF));
    sVertexData = Alloc(regionSize);
    ConvertFloats(sVertexData, sVertexRegion, regionSize / 4);

    sVertex = Alloc(sizeof(HSFBUFFER) * sFileHead.vertex.num);
    sModel->vertex = sVertex;
    sModel->vertexNum = (s16)sFileHead.vertex.num;
    for (i = 0; i < sFileHead.vertex.num; i++) {
        sVertex[i].name = NameAt(file[i].name);
        sVertex[i].count = (s32)PORT_BE32(file[i].count);
        sVertex[i].data = (u8 *)sVertexData + PORT_BE32(file[i].data);
    }
}

static void NormalLoad(void)
{
    const HSFFILEBUFFER *file;
    u32 regionSize;
    u32 i;

    if (sFileHead.normal.num == 0) {
        return;
    }
    /*
     * Normals are three floats per vertex on a model with envelopes, and three
     * signed bytes otherwise — the same test the draw code makes.
     */
    sNormalElemSize = (sFileHead.cenv.num == 0) ? 3 : sizeof(HuVecF);
    file = FilePtr(sFileHead.normal.ofs);
    sNormalRegion = (const u8 *)&file[sFileHead.normal.num];
    regionSize = BufferRegionSize(file, sFileHead.normal.num, sNormalElemSize);
    sNormalData = Alloc(regionSize);
    if (sNormalElemSize == 3) {
        memcpy(sNormalData, sNormalRegion, regionSize);
    } else {
        ConvertFloats(sNormalData, sNormalRegion, regionSize / 4);
    }

    sNormal = Alloc(sizeof(HSFBUFFER) * sFileHead.normal.num);
    sModel->normal = sNormal;
    sModel->normalNum = (s16)sFileHead.normal.num;
    for (i = 0; i < sFileHead.normal.num; i++) {
        sNormal[i].name = NameAt(file[i].name);
        sNormal[i].count = (s32)PORT_BE32(file[i].count);
        sNormal[i].data = (u8 *)sNormalData + PORT_BE32(file[i].data);
    }
}

static void STLoad(void)
{
    const HSFFILEBUFFER *file;
    const u8 *region;
    void *data;
    u32 regionSize;
    u32 i;

    if (sFileHead.st.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.st.ofs);
    region = (const u8 *)&file[sFileHead.st.num];
    regionSize = BufferRegionSize(file, sFileHead.st.num, sizeof(HuVec2f));
    data = Alloc(regionSize);
    ConvertFloats(data, region, regionSize / 4);

    sSt = Alloc(sizeof(HSFBUFFER) * sFileHead.st.num);
    sModel->st = sSt;
    sModel->stNum = (s16)sFileHead.st.num;
    for (i = 0; i < sFileHead.st.num; i++) {
        sSt[i].name = NameAt(file[i].name);
        sSt[i].count = (s32)PORT_BE32(file[i].count);
        sSt[i].data = (u8 *)data + PORT_BE32(file[i].data);
    }
}

static void ColorLoad(void)
{
    const HSFFILEBUFFER *file;
    const u8 *region;
    void *data;
    u32 regionSize;
    u32 i;

    if (sFileHead.color.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.color.ofs);
    region = (const u8 *)&file[sFileHead.color.num];
    regionSize = BufferRegionSize(file, sFileHead.color.num, sizeof(GXColor));
    data = Alloc(regionSize);
    memcpy(data, region, regionSize); /* four bytes per colour: nothing to swap */

    sColor = Alloc(sizeof(HSFBUFFER) * sFileHead.color.num);
    sModel->color = sColor;
    sModel->colorNum = (s16)sFileHead.color.num;
    for (i = 0; i < sFileHead.color.num; i++) {
        sColor[i].name = NameAt(file[i].name);
        sColor[i].count = (s32)PORT_BE32(file[i].count);
        sColor[i].data = (u8 *)data + PORT_BE32(file[i].data);
    }
}

static void ReadFaceIndex(HSFFACEINDEX *dst, const HSFFILEFACEINDEX *src)
{
    dst->vertex = (s16)PORT_BE16(src->vertex);
    dst->normal = (s16)PORT_BE16(src->normal);
    dst->color = (s16)PORT_BE16(src->color);
    dst->st = (s16)PORT_BE16(src->st);
}

static void FaceLoad(void)
{
    const HSFFILEBUFFER *file;
    const u8 *region;
    const HSFFILEFACE *fileFaces;
    HSFFACE *faces;
    HSFFACEINDEX *stripData;
    const HSFFILEFACEINDEX *fileStrip;
    u32 faceCount;
    u32 stripCount;
    u32 regionSize;
    u32 i;
    u32 j;
    u32 k;

    if (sFileHead.face.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.face.ofs);
    region = (const u8 *)&file[sFileHead.face.num];
    regionSize = BufferRegionSize(file, sFileHead.face.num, sizeof(HSFFILEFACE));
    faceCount = regionSize / sizeof(HSFFILEFACE);
    fileFaces = (const HSFFILEFACE *)region;
    faces = Alloc(sizeof(HSFFACE) * faceCount);

    /* the strips' indices follow the last face record */
    fileStrip = (const HSFFILEFACEINDEX *)(region + regionSize);
    stripCount = 0;
    for (i = 0; i < faceCount; i++) {
        if (PORT_BE16(fileFaces[i].type) == HSF_FACE_TRISTRIP) {
            u32 end = PORT_BE32(fileFaces[i].strip.data) + PORT_BE32(fileFaces[i].strip.count);
            if (end > stripCount) {
                stripCount = end;
            }
        }
    }
    stripData = stripCount ? Alloc(sizeof(HSFFACEINDEX) * stripCount) : NULL;
    for (i = 0; i < stripCount; i++) {
        ReadFaceIndex(&stripData[i], &fileStrip[i]);
    }

    for (i = 0; i < faceCount; i++) {
        faces[i].type = (s16)PORT_BE16(fileFaces[i].type);
        faces[i].mat = (s16)PORT_BE16(fileFaces[i].mat);
        if (faces[i].typeSrc == HSF_FACE_TRISTRIP) {
            for (j = 0; j < 3; j++) {
                ReadFaceIndex(&faces[i].strip.index[j], &fileFaces[i].strip.index[j]);
            }
            faces[i].strip.count = PORT_BE32(fileFaces[i].strip.count);
            faces[i].strip.data = stripData + PORT_BE32(fileFaces[i].strip.data);
        } else {
            for (j = 0; j < 4; j++) {
                ReadFaceIndex(&faces[i].index[j], &fileFaces[i].index[j]);
            }
        }
        for (k = 0; k < 3; k++) {
            faces[i].nbt[k] = ReadFloat(fileFaces[i].nbt[k]);
        }
    }

    sFace = Alloc(sizeof(HSFBUFFER) * sFileHead.face.num);
    sModel->face = sFace;
    sModel->faceNum = (s16)sFileHead.face.num;
    for (i = 0; i < sFileHead.face.num; i++) {
        sFace[i].name = NameAt(file[i].name);
        sFace[i].count = (s32)PORT_BE32(file[i].count);
        sFace[i].data = faces + PORT_BE32(file[i].data) / sizeof(HSFFILEFACE);
    }
}

static HSFBUFFER *BufferAt(HSFBUFFER *table, s32 id)
{
    if (id == -1 || table == NULL) {
        return NULL;
    }
    return &table[id];
}

static HSFCENV *SearchCenvPtr(s32 id)
{
    if (id == -1 || sCenv == NULL) {
        return NULL;
    }
    return &sCenv[id];
}

static HSFPART *SearchPartPtr(s32 id)
{
    if (id == -1 || sPart == NULL) {
        return NULL;
    }
    return &sPart[id];
}

/* --------------------------------------------------------------- objects */

static void ObjectMeshLoad(HSFOBJECT *object, const HSFFILEOBJECT *file)
{
    const HSFFILEMESH *mesh = &file->mesh;
    u32 childNum = PORT_BE32(mesh->childNum);
    u32 i;

    object->mesh.childNum = childNum;
    if (childNum != 0) {
        u32 childSymbol = PORT_BE32(mesh->child);
        object->mesh.child = Alloc(sizeof(HSFOBJECT *) * childNum);
        for (i = 0; i < childNum; i++) {
            object->mesh.child[i] = &sObjects[SymbolAt(childSymbol + i)];
        }
    }
    ReadTransform(&object->mesh.base, &mesh->base);
    ReadTransform(&object->mesh.curr, &mesh->curr);

    switch (object->type) {
        case HSF_OBJ_REPLICA:
            object->mesh.replica = &sObjects[PORT_BE32(mesh->replica)];
            break;

        case HSF_OBJ_MESH:
        case HSF_OBJ_MAP: {
            u32 shapeNum = PORT_BE32(mesh->shapeNum);
            u32 clusterNum = PORT_BE32(mesh->clusterNum);

            ReadVec(&object->mesh.mesh.min, mesh->mesh.min);
            ReadVec(&object->mesh.mesh.max, mesh->mesh.max);
            object->mesh.mesh.baseMorph = ReadFloat(mesh->mesh.baseMorph);
            for (i = 0; i < 33; i++) {
                object->mesh.mesh.morphWeight[i] = ReadFloat(mesh->mesh.morphWeight[i]);
            }
            object->mesh.face = BufferAt(sFace, (s32)PORT_BE32(mesh->face));
            object->mesh.vertex = BufferAt(sVertex, (s32)PORT_BE32(mesh->vertex));
            object->mesh.normal = BufferAt(sNormal, (s32)PORT_BE32(mesh->normal));
            object->mesh.color = BufferAt(sColor, (s32)PORT_BE32(mesh->color));
            object->mesh.st = BufferAt(sSt, (s32)PORT_BE32(mesh->st));
            object->mesh.material = sModel->material;
            object->mesh.attribute = ((s32)PORT_BE32(mesh->attribute) >= 0) ? sModel->attribute : NULL;
            object->mesh.writeNum = mesh->writeNum;
            object->mesh.unk121 = mesh->unk121;
            object->mesh.shapeType = mesh->shapeType;
            object->mesh.matPass = mesh->matPass;
            object->mesh.shapeNum = shapeNum;
            if (shapeNum != 0) {
                u32 shapeSymbol = PORT_BE32(mesh->shape);
                object->mesh.shape = Alloc(sizeof(HSFBUFFER *) * shapeNum);
                for (i = 0; i < shapeNum; i++) {
                    object->mesh.shape[i] = &sVertex[SymbolAt(shapeSymbol + i)];
                }
            }
            object->mesh.clusterNum = clusterNum;
            if (clusterNum != 0) {
                u32 clusterSymbol = PORT_BE32(mesh->cluster);
                object->mesh.cluster = Alloc(sizeof(HSFCLUSTER *) * clusterNum);
                for (i = 0; i < clusterNum; i++) {
                    object->mesh.cluster[i] = &sCluster[SymbolAt(clusterSymbol + i)];
                }
            }
            object->mesh.cenvNum = PORT_BE32(mesh->cenvNum);
            object->mesh.cenv = SearchCenvPtr((s32)PORT_BE32(mesh->cenv));
            /*
             * vtxtop and normtop are the untransformed vertices and normals an
             * envelope or a cluster reads: they point into the converted
             * copies, at the same place they pointed into the file.
             */
            if (sVertexData != NULL && mesh->vtxtop != 0) {
                u32 offset = PORT_BE32(mesh->vtxtop);
                object->mesh.vtxtop = (HuVecF *)((u8 *)sVertexData + (offset - (u32)(sVertexRegion - sFile)));
            }
            if (sNormalData != NULL && mesh->normtop != 0) {
                u32 offset = PORT_BE32(mesh->normtop);
                object->mesh.normtop = (HuVecF *)((u8 *)sNormalData + (offset - (u32)(sNormalRegion - sFile)));
            }
            break;
        }

        default:
            break;
    }
}

static void ObjectLoad(void)
{
    const HSFFILEOBJECT *file;
    u32 i;

    if (sFileHead.object.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.object.ofs);
    sObjects = Alloc(sizeof(HSFOBJECT) * sFileHead.object.num);
    sModel->object = sObjects;
    sModel->objectNum = (s16)sFileHead.object.num;

    for (i = 0; i < sFileHead.object.num; i++) {
        sObjects[i].name = NameAt(file[i].name);
        sObjects[i].type = PORT_BE32(file[i].type);
        sObjects[i].constData = NULL;
        sObjects[i].flags = PORT_BE32(file[i].flags);
    }
    for (i = 0; i < sFileHead.object.num; i++) {
        switch (sObjects[i].type) {
            case HSF_OBJ_CAMERA: {
                const HSFFILECAMERA *camera = &file[i].camera;
                ReadVec(&sObjects[i].camera.pos, camera->pos);
                ReadVec(&sObjects[i].camera.target, camera->target);
                sObjects[i].camera.upRot = ReadFloat(camera->upRot);
                sObjects[i].camera.fov = ReadFloat(camera->fov);
                sObjects[i].camera.near = ReadFloat(camera->nearPlane);
                sObjects[i].camera.far = ReadFloat(camera->farPlane);
                break;
            }

            case HSF_OBJ_LIGHT: {
                const HSFFILELIGHT *light = &file[i].light;
                ReadVec(&sObjects[i].light.pos, light->pos);
                ReadVec(&sObjects[i].light.target, light->target);
                sObjects[i].light.type = light->type;
                sObjects[i].light.r = light->r;
                sObjects[i].light.g = light->g;
                sObjects[i].light.b = light->b;
                sObjects[i].light.unk2C = ReadFloat(light->unk2C);
                sObjects[i].light.refDistance = ReadFloat(light->refDistance);
                sObjects[i].light.refBrightness = ReadFloat(light->refBrightness);
                sObjects[i].light.cutoff = ReadFloat(light->cutoff);
                break;
            }

            default:
                ObjectMeshLoad(&sObjects[i], &file[i]);
                break;
        }
    }
    /* the root is the object without a parent, and every child knows it now */
    for (i = 0; i < sFileHead.object.num; i++) {
        if ((s32)PORT_BE32(file[i].mesh.parent) == -1) {
            sModel->root = &sObjects[i];
            break;
        }
    }
    for (i = 0; i < sFileHead.object.num; i++) {
        u32 j;
        for (j = 0; j < sObjects[i].mesh.childNum; j++) {
            if (sObjects[i].mesh.child != NULL && sObjects[i].mesh.child[j] != NULL) {
                sObjects[i].mesh.child[j]->mesh.parent = &sObjects[i];
            }
        }
    }
}

static void CenvLoad(void)
{
    const HSFFILECENV *file;
    const u8 *dataBase;
    const u8 *weightBase;
    u32 i;
    u32 j;
    u32 recordsSize = 0;

    if (sFileHead.cenv.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.cenv.ofs);
    dataBase = (const u8 *)&file[sFileHead.cenv.num];
    sCenv = Alloc(sizeof(HSFCENV) * sFileHead.cenv.num);
    sModel->cenv = sCenv;
    sModel->cenvNum = (s16)sFileHead.cenv.num;

    /* the weights follow every single/dual/multi record of every envelope */
    for (i = 0; i < sFileHead.cenv.num; i++) {
        recordsSize += PORT_BE32(file[i].singleCount) * sizeof(HSFFILECENVSINGLE);
        recordsSize += PORT_BE32(file[i].dualCount) * sizeof(HSFFILECENVDUAL);
        recordsSize += PORT_BE32(file[i].multiCount) * sizeof(HSFFILECENVMULTI);
    }
    weightBase = dataBase + recordsSize;

    for (i = 0; i < sFileHead.cenv.num; i++) {
        u32 singleCount = PORT_BE32(file[i].singleCount);
        u32 dualCount = PORT_BE32(file[i].dualCount);
        u32 multiCount = PORT_BE32(file[i].multiCount);
        const HSFFILECENVSINGLE *fileSingle = (const HSFFILECENVSINGLE *)(dataBase + PORT_BE32(file[i].singleData));
        const HSFFILECENVDUAL *fileDual = (const HSFFILECENVDUAL *)(dataBase + PORT_BE32(file[i].dualData));
        const HSFFILECENVMULTI *fileMulti = (const HSFFILECENVMULTI *)(dataBase + PORT_BE32(file[i].multiData));

        sCenv[i].name = NameAt(file[i].name);
        sCenv[i].singleCount = singleCount;
        sCenv[i].dualCount = dualCount;
        sCenv[i].multiCount = multiCount;
        sCenv[i].vtxCount = PORT_BE32(file[i].vtxCount);
        sCenv[i].copyCount = PORT_BE32(file[i].copyCount);

        if (singleCount != 0) {
            sCenv[i].singleData = Alloc(sizeof(HSFCENVSINGLE) * singleCount);
            for (j = 0; j < singleCount; j++) {
                sCenv[i].singleData[j].target = PORT_BE32(fileSingle[j].target);
                sCenv[i].singleData[j].pos = PORT_BE16(fileSingle[j].pos);
                sCenv[i].singleData[j].posNum = PORT_BE16(fileSingle[j].posNum);
                sCenv[i].singleData[j].normal = PORT_BE16(fileSingle[j].normal);
                sCenv[i].singleData[j].normalNum = PORT_BE16(fileSingle[j].normalNum);
            }
        }
        if (dualCount != 0) {
            sCenv[i].dualData = Alloc(sizeof(HSFCENVDUAL) * dualCount);
            for (j = 0; j < dualCount; j++) {
                u32 weightNum = PORT_BE32(fileDual[j].weightNum);
                const HSFFILECENVDUALWEIGHT *fileWeight =
                    (const HSFFILECENVDUALWEIGHT *)(weightBase + PORT_BE32(fileDual[j].weight));
                u32 k;

                sCenv[i].dualData[j].target1 = PORT_BE32(fileDual[j].target1);
                sCenv[i].dualData[j].target2 = PORT_BE32(fileDual[j].target2);
                sCenv[i].dualData[j].weightNum = weightNum;
                sCenv[i].dualData[j].weight = Alloc(sizeof(HSFCENVDUALWEIGHT) * (weightNum ? weightNum : 1));
                for (k = 0; k < weightNum; k++) {
                    sCenv[i].dualData[j].weight[k].weight = ReadFloat(fileWeight[k].weight);
                    sCenv[i].dualData[j].weight[k].pos = PORT_BE16(fileWeight[k].pos);
                    sCenv[i].dualData[j].weight[k].posNum = PORT_BE16(fileWeight[k].posNum);
                    sCenv[i].dualData[j].weight[k].normal = PORT_BE16(fileWeight[k].normal);
                    sCenv[i].dualData[j].weight[k].normalNum = PORT_BE16(fileWeight[k].normalNum);
                }
            }
        }
        if (multiCount != 0) {
            sCenv[i].multiData = Alloc(sizeof(HSFCENVMULTI) * multiCount);
            for (j = 0; j < multiCount; j++) {
                u32 weightNum = PORT_BE32(fileMulti[j].weightNum);
                const HSFFILECENVMULTIWEIGHT *fileWeight =
                    (const HSFFILECENVMULTIWEIGHT *)(weightBase + PORT_BE32(fileMulti[j].weight));
                u32 k;

                sCenv[i].multiData[j].weightNum = weightNum;
                sCenv[i].multiData[j].pos = PORT_BE16(fileMulti[j].pos);
                sCenv[i].multiData[j].posNum = PORT_BE16(fileMulti[j].posNum);
                sCenv[i].multiData[j].normal = PORT_BE16(fileMulti[j].normal);
                sCenv[i].multiData[j].normalNum = PORT_BE16(fileMulti[j].normalNum);
                sCenv[i].multiData[j].weight = Alloc(sizeof(HSFCENVMULTIWEIGHT) * (weightNum ? weightNum : 1));
                for (k = 0; k < weightNum; k++) {
                    sCenv[i].multiData[j].weight[k].target = PORT_BE32(fileWeight[k].target);
                    sCenv[i].multiData[j].weight[k].value = ReadFloat(fileWeight[k].value);
                }
            }
        }
    }
}

static void SkeletonLoad(void)
{
    const HSFFILESKELETON *file;
    HSFSKELETON *skeleton;
    u32 i;

    if (sFileHead.skeleton.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.skeleton.ofs);
    skeleton = Alloc(sizeof(HSFSKELETON) * sFileHead.skeleton.num);
    sModel->skeleton = skeleton;
    sModel->skeletonNum = (s16)sFileHead.skeleton.num;
    for (i = 0; i < sFileHead.skeleton.num; i++) {
        skeleton[i].name = NameAt(file[i].name);
        ReadTransform(&skeleton[i].transform, &file[i].transform);
    }
}

static void PartLoad(void)
{
    const HSFFILEPART *file;
    const u16 *data;
    u32 i;
    u32 j;

    if (sFileHead.part.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.part.ofs);
    data = (const u16 *)&file[sFileHead.part.num];
    sPart = Alloc(sizeof(HSFPART) * sFileHead.part.num);
    sModel->part = sPart;
    sModel->partNum = (s16)sFileHead.part.num;
    for (i = 0; i < sFileHead.part.num; i++) {
        u32 num = PORT_BE32(file[i].num);
        const u16 *src = data + PORT_BE32(file[i].vertex);

        sPart[i].name = NameAt(file[i].name);
        sPart[i].num = num;
        sPart[i].vertex = Alloc(sizeof(u16) * (num ? num : 1));
        for (j = 0; j < num; j++) {
            sPart[i].vertex[j] = PORT_BE16(src[j]);
        }
    }
}

static void ClusterLoad(void)
{
    const HSFFILECLUSTER *file;
    u32 i;
    u32 j;

    if (sFileHead.cluster.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.cluster.ofs);
    sCluster = Alloc(sizeof(HSFCLUSTER) * sFileHead.cluster.num);
    sModel->cluster = sCluster;
    sModel->clusterNum = (s16)sFileHead.cluster.num;
    for (i = 0; i < sFileHead.cluster.num; i++) {
        u32 vertexNum = PORT_BE32(file[i].vertexNum);
        u32 vertexSymbol = PORT_BE32(file[i].vertex);

        sCluster[i].name[0] = NameAt(file[i].name[0]);
        sCluster[i].name[1] = NameAt(file[i].name[1]);
        sCluster[i].targetName = NameAt(file[i].targetName);
        sCluster[i].part = SearchPartPtr((s32)PORT_BE32(file[i].part));
        sCluster[i].index = ReadFloat(file[i].index);
        for (j = 0; j < 32; j++) {
            sCluster[i].weight[j] = ReadFloat(file[i].weight[j]);
        }
        sCluster[i].adjusted = file[i].adjusted;
        sCluster[i].unk95 = file[i].unk95;
        sCluster[i].type = PORT_BE16(file[i].type);
        sCluster[i].vertexNum = vertexNum;
        if (vertexNum != 0) {
            sCluster[i].vertex = Alloc(sizeof(HSFBUFFER *) * vertexNum);
            for (j = 0; j < vertexNum; j++) {
                sCluster[i].vertex[j] = BufferAt(sVertex, (s32)SymbolAt(vertexSymbol + j));
            }
        }
    }
}

static void ShapeLoad(void)
{
    const HSFFILESHAPE *file;
    HSFSHAPE *shape;
    u32 i;
    u32 j;

    if (sFileHead.shape.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.shape.ofs);
    shape = Alloc(sizeof(HSFSHAPE) * sFileHead.shape.num);
    sModel->shape = shape;
    sModel->shapeNum = (s16)sFileHead.shape.num;
    for (i = 0; i < sFileHead.shape.num; i++) {
        u32 vertexSymbol = PORT_BE32(file[i].vertex);

        shape[i].name = NameAt(file[i].name);
        shape[i].num16[0] = PORT_BE16(file[i].num16[0]);
        shape[i].num16[1] = PORT_BE16(file[i].num16[1]);
        if (shape[i].num16[1] != 0) {
            shape[i].vertex = Alloc(sizeof(HSFBUFFER *) * shape[i].num16[1]);
            for (j = 0; j < shape[i].num16[1]; j++) {
                shape[i].vertex[j] = &sVertex[SymbolAt(vertexSymbol + j)];
            }
        }
    }
}

static void MapAttrLoad(void)
{
    const HSFFILEMAPATTR *file;
    const u16 *data;
    HSFMAPATTR *mapAttr;
    u32 i;
    u32 j;

    if (sFileHead.mapAttr.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.mapAttr.ofs);
    data = (const u16 *)&file[sFileHead.mapAttr.num];
    mapAttr = Alloc(sizeof(HSFMAPATTR) * sFileHead.mapAttr.num);
    sModel->mapAttr = mapAttr;
    sModel->mapAttrNum = (s16)sFileHead.mapAttr.num;
    for (i = 0; i < sFileHead.mapAttr.num; i++) {
        u32 dataLen = PORT_BE32(file[i].dataLen);
        const u16 *src = data + PORT_BE32(file[i].data);

        mapAttr[i].minX = ReadFloat(file[i].minX);
        mapAttr[i].minZ = ReadFloat(file[i].minZ);
        mapAttr[i].maxX = ReadFloat(file[i].maxX);
        mapAttr[i].maxZ = ReadFloat(file[i].maxZ);
        mapAttr[i].dataLen = dataLen;
        mapAttr[i].data = Alloc(sizeof(u16) * (dataLen ? dataLen : 1));
        for (j = 0; j < dataLen; j++) {
            mapAttr[i].data[j] = PORT_BE16(src[j]);
        }
    }
}

/* --------------------------------------------------------------- motions */

static s32 FindObjectName(const char *name)
{
    s32 i;

    for (i = 0; i < sModel->objectNum; i++) {
        if (CmpObjectName(sObjects[i].name, (char *)name) == 0) {
            return i;
        }
    }
    return -1;
}

static s32 FindClusterName(const char *name)
{
    s32 i;

    for (i = 0; i < sModel->clusterNum; i++) {
        if (strcmp(sCluster[i].name[0], name) == 0) {
            return i;
        }
    }
    return -1;
}

static s32 FindAttributeName(const char *name)
{
    s32 i;

    for (i = 0; i < sModel->attributeNum; i++) {
        if (sModel->attribute[i].name == NULL) {
            continue;
        }
        if (strcmp(sModel->attribute[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* How many floats a curve holds per keyframe. */
static u32 CurveStride(u16 curveType)
{
    switch (curveType) {
        case HSF_CURVE_STEP:
        case HSF_CURVE_LINEAR:
            return 2;
        case HSF_CURVE_BEZIER:
            return 4;
        default:
            return 0;
    }
}

static void MotionLoad(void)
{
    const HSFFILEMOTION *file;
    const HSFFILETRACK *fileTracks;
    const u8 *curveData;
    HSFMOTION *motion;
    HSFTRACK *tracks;
    u32 trackNum;
    u32 i;

    if (sFileHead.motion.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.motion.ofs);
    trackNum = PORT_BE32(file->numTracks);
    fileTracks = (const HSFFILETRACK *)&file[sFileHead.motion.num];
    curveData = (const u8 *)&fileTracks[trackNum];

    motion = Alloc(sizeof(HSFMOTION) * sFileHead.motion.num);
    tracks = Alloc(sizeof(HSFTRACK) * (trackNum ? trackNum : 1));
    sModel->motion = motion;
    /* the game keeps the track count here, as hsfmotion.c reads it */
    sModel->motionNum = (s16)trackNum;
    motion->name = NameAt(file->name);
    motion->numTracks = (s32)trackNum;
    motion->maxTime = ReadFloat(file->maxTime);
    motion->track = tracks;

    for (i = 0; i < trackNum; i++) {
        const HSFFILETRACK *src = &fileTracks[i];
        HSFTRACK *dst = &tracks[i];
        u16 curveType = PORT_BE16(src->curveType);
        u16 keyframes = PORT_BE16(src->numKeyframes);
        u32 dataOffset = PORT_BE32(src->data);
        const char *name;

        dst->type = src->type;
        dst->start = src->start;
        dst->target = PORT_BE16(src->target);
        dst->index = PORT_BE16(src->index);
        dst->channel = PORT_BE16(src->channel);
        dst->curveType = curveType;
        dst->numKeyframes = keyframes;

        switch (dst->type) {
            case HSF_TRACK_TRANSFORM:
            case HSF_TRACK_MORPH:
                name = StringTable + dst->target;
                if (sObjects != NULL) {
                    dst->target = (u16)FindObjectName(name);
                }
                break;

            case HSF_TRACK_CLUSTER:
            case HSF_TRACK_CLUSTER_WEIGHT:
                name = StringTable + dst->target;
                dst->cluster = (s16)FindClusterName(name);
                break;

            case HSF_TRACK_ATTRIBUTE:
                if (dst->cluster != -1) {
                    name = StringTable + dst->target;
                    dst->attrIdx = (s16)FindAttributeName(name);
                }
                break;

            default:
                break;
        }

        if (curveType == HSF_CURVE_CONST) {
            dst->value = ReadFloat(src->data);
        } else if (curveType == HSF_CURVE_BITMAP) {
            const HSFFILEBITMAPKEY *fileKeys = (const HSFFILEBITMAPKEY *)(curveData + dataOffset);
            HSFBITMAPKEY *keys = Alloc(sizeof(HSFBITMAPKEY) * (keyframes ? keyframes : 1));
            u32 j;

            for (j = 0; j < keyframes; j++) {
                keys[j].time = ReadFloat(fileKeys[j].time);
                keys[j].data = SearchBitmapPtr((s32)PORT_BE32(fileKeys[j].data));
            }
            dst->data = keys;
        } else {
            u32 count = CurveStride(curveType) * keyframes;
            float *values = Alloc(sizeof(float) * (count ? count : 1));

            ConvertFloats(values, curveData + dataOffset, count);
            dst->data = values;
        }
    }
}

static void MatrixLoad(void)
{
    const HSFFILEMATRIX *file;
    HSFMATRIX *matrix;
    u32 count;

    if (sFileHead.matrix.num == 0) {
        return;
    }
    file = FilePtr(sFileHead.matrix.ofs);
    count = PORT_BE32(file->count);
    matrix = Alloc(sizeof(HSFMATRIX));
    matrix->base_idx = PORT_BE32(file->base_idx);
    matrix->count = count;
    matrix->data = Alloc(sizeof(Mtx) * (count ? count : 1));
    /* the matrices follow the single record they belong to */
    ConvertFloats(matrix->data, (const u8 *)file + sizeof(HSFFILEMATRIX), count * 12);
    sModel->matrix = matrix;
    sModel->matrixNum = (s16)sFileHead.matrix.num;
}

/* ------------------------------------------------------------------ entry */

HSFDATA *LoadHSF(void *data)
{
    HSFDATA *model;

    FileLoad(data);
    SceneLoad();
    ColorLoad();
    PaletteLoad();
    BitmapLoad();
    MaterialLoad();
    AttributeLoad();
    VertexLoad();
    NormalLoad();
    STLoad();
    FaceLoad();
    ClusterLoad();   /* objects point at clusters, so they come first here */
    CenvLoad();
    ObjectLoad();
    SkeletonLoad();
    PartLoad();
    ShapeLoad();
    MapAttrLoad();
    MotionLoad();
    MatrixLoad();

    model = sModel;
    InitEnvelope(model);
    sModel = NULL;
    sObjects = NULL;
    return model;
}

void ClusterAdjustObject(HSFDATA *model, HSFDATA *srcModel)
{
    HSFCLUSTER *cluster;
    s32 i;

    if (srcModel == NULL || srcModel->clusterNum == 0) {
        return;
    }
    cluster = srcModel->cluster;
    if (cluster->adjusted) {
        return;
    }
    cluster->adjusted = 1;
    for (i = 0; i < srcModel->clusterNum; i++, cluster++) {
        HSFOBJECT *object = model->object;
        s32 j;

        cluster->target = -1;
        for (j = 0; j < model->objectNum; j++, object++) {
            if (CmpObjectName(object->name, cluster->targetName) == 0) {
                cluster->target = j;
                break;
            }
        }
        if (cluster->target == -1) {
            OSReport("Search Object Error %s\n", cluster->targetName);
        }
    }
}

char *MakeObjectName(char *name)
{
    static char buf[768];
    s32 index;
    char *nameP;

    index = 0;
    nameP = name;
    while (*nameP) {
        if (*nameP == '-') {
            name = nameP + 1;
            break;
        }
        nameP++;
    }
    while (*name) {
        if (*name == '_' && !isalpha((unsigned char)name[1])) {
            break;
        }
        buf[index] = *name;
        name++;
        index++;
    }
    buf[index] = '\0';
    return buf;
}

s32 CmpObjectName(char *name1, char *name2)
{
    if (name1 == NULL || name2 == NULL) {
        return name1 == name2 ? 0 : 1;
    }
    return strcmp(name1, name2);
}
