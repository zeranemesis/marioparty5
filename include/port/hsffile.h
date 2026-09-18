#ifndef PORT_HSFFILE_H_
#define PORT_HSFFILE_H_

/*
 * The HSF model format, as it is written on the disc.
 *
 * The GameCube reads these records straight into the game's own structures:
 * a pointer is four bytes there, the values are big-endian, and the loader
 * only turns the offsets it finds into pointers, in place.
 *
 * On a 64-bit little-endian host neither holds, so the file is described here
 * with its own structures — every pointer replaced by the 32-bit offset or
 * index it really is — and src/port/hsfload_pc.c converts each record into the
 * game's structure.
 *
 * The sizes asserted below are those of the GameCube structures in
 * include/game/hsfformat.h: they are the layout of the file and must not move.
 */

#include "dolphin/types.h"

#define HSF_FILE_ASSERT_SIZE(type, size) \
    typedef char hsffile_##type##_size_check[(sizeof(type) == (size)) ? 1 : -1]

typedef struct HsfFileSection_s {
    u32 ofs;
    u32 num;
} HSFFILESECTION;
HSF_FILE_ASSERT_SIZE(HSFFILESECTION, 8);

typedef struct HsfFileHeader_s {
    char magic[8];
    HSFFILESECTION scene;
    HSFFILESECTION color;
    HSFFILESECTION material;
    HSFFILESECTION attribute;
    HSFFILESECTION vertex;
    HSFFILESECTION normal;
    HSFFILESECTION st;
    HSFFILESECTION face;
    HSFFILESECTION object;
    HSFFILESECTION bitmap;
    HSFFILESECTION palette;
    HSFFILESECTION motion;
    HSFFILESECTION cenv;
    HSFFILESECTION skeleton;
    HSFFILESECTION part;
    HSFFILESECTION cluster;
    HSFFILESECTION shape;
    HSFFILESECTION mapAttr;
    HSFFILESECTION matrix;
    HSFFILESECTION symbol;
    HSFFILESECTION string;
} HSFFILEHEADER;
HSF_FILE_ASSERT_SIZE(HSFFILEHEADER, 176);

typedef struct HsfFileScene_s {
    u32 fogType;
    u32 fogStart;
    u32 fogEnd;
    u8 fogColor[4];
} HSFFILESCENE;
HSF_FILE_ASSERT_SIZE(HSFFILESCENE, 16);

typedef struct HsfFileBitmap_s {
    u32 name;
    u32 maxLod;
    u8 dataFmt;
    u8 pixSize;
    u16 sizeX;
    u16 sizeY;
    u16 palSize;
    u8 tint[4];
    u32 palData; /* palette index */
    u32 unk;
    u32 data; /* offset from the end of the bitmap table */
} HSFFILEBITMAP;
HSF_FILE_ASSERT_SIZE(HSFFILEBITMAP, 32);

typedef struct HsfFilePalette_s {
    u32 name;
    u32 unk;
    u32 palSize;
    u32 data; /* offset from the end of the palette table */
} HSFFILEPALETTE;
HSF_FILE_ASSERT_SIZE(HSFFILEPALETTE, 16);

typedef struct HsfFileAttribute_s {
    u32 name;
    u32 animWorkP;
    u8 unk8[4];
    u32 kColor;
    u8 unk10[4];
    u32 nbtTpLvl;
    u8 unk18[8];
    u32 unk20;
    u8 unk24[4];
    u32 scale[2];
    u32 trans[2];
    u8 unk38[44];
    u32 wrapS;
    u32 wrapT;
    u8 unk6C[12];
    u32 maxLod;
    u32 flag;
    u32 bitmap; /* bitmap index */
} HSFFILEATTRIBUTE;
HSF_FILE_ASSERT_SIZE(HSFFILEATTRIBUTE, 132);

typedef struct HsfFileMaterial_s {
    u32 name;
    u8 unk4[4];
    u16 pass;
    u8 vtxMode;
    u8 litColor[3];
    u8 color[3];
    u8 shadowColor[3];
    u32 hiliteScale;
    u32 unk18;
    u32 invAlpha;
    u32 unk20[2];
    u32 refAlpha;
    u32 unk2C;
    u32 flags;
    u32 attrNum;
    u32 attr; /* index into the symbol table */
} HSFFILEMATERIAL;
HSF_FILE_ASSERT_SIZE(HSFFILEMATERIAL, 60);

typedef struct HsfFileBuffer_s {
    u32 name;
    u32 count;
    u32 data; /* offset from the end of the table this buffer belongs to */
} HSFFILEBUFFER;
HSF_FILE_ASSERT_SIZE(HSFFILEBUFFER, 12);

typedef struct HsfFileFaceIndex_s {
    u16 vertex;
    u16 normal;
    u16 color;
    u16 st;
} HSFFILEFACEINDEX;
HSF_FILE_ASSERT_SIZE(HSFFILEFACEINDEX, 8);

typedef struct HsfFileFace_s {
    u16 type;
    u16 mat;
    union {
        struct {
            HSFFILEFACEINDEX index[3];
            u32 count;
            u32 data; /* index into the strip index array */
        } strip;
        HSFFILEFACEINDEX index[4];
    };
    u32 nbt[3];
} HSFFILEFACE;
HSF_FILE_ASSERT_SIZE(HSFFILEFACE, 48);

typedef struct HsfFileTransform_s {
    u32 pos[3];
    u32 rot[3];
    u32 scale[3];
} HSFFILETRANSFORM;
HSF_FILE_ASSERT_SIZE(HSFFILETRANSFORM, 36);

typedef struct HsfFileCenvSingle_s {
    u32 target;
    u16 pos;
    u16 posNum;
    u16 normal;
    u16 normalNum;
} HSFFILECENVSINGLE;
HSF_FILE_ASSERT_SIZE(HSFFILECENVSINGLE, 12);

typedef struct HsfFileCenvDualWeight_s {
    u32 weight;
    u16 pos;
    u16 posNum;
    u16 normal;
    u16 normalNum;
} HSFFILECENVDUALWEIGHT;
HSF_FILE_ASSERT_SIZE(HSFFILECENVDUALWEIGHT, 12);

typedef struct HsfFileCenvDual_s {
    u32 target1;
    u32 target2;
    u32 weightNum;
    u32 weight; /* offset from the end of the cenv records */
} HSFFILECENVDUAL;
HSF_FILE_ASSERT_SIZE(HSFFILECENVDUAL, 16);

typedef struct HsfFileCenvMultiWeight_s {
    u32 target;
    u32 value;
} HSFFILECENVMULTIWEIGHT;
HSF_FILE_ASSERT_SIZE(HSFFILECENVMULTIWEIGHT, 8);

typedef struct HsfFileCenvMulti_s {
    u32 weightNum;
    u16 pos;
    u16 posNum;
    u16 normal;
    u16 normalNum;
    u32 weight; /* offset from the end of the cenv records */
} HSFFILECENVMULTI;
HSF_FILE_ASSERT_SIZE(HSFFILECENVMULTI, 16);

typedef struct HsfFileCenv_s {
    u32 name;
    u32 singleData; /* offsets from the end of the cenv table */
    u32 dualData;
    u32 multiData;
    u32 singleCount;
    u32 dualCount;
    u32 multiCount;
    u32 vtxCount;
    u32 copyCount;
} HSFFILECENV;
HSF_FILE_ASSERT_SIZE(HSFFILECENV, 36);

typedef struct HsfFilePart_s {
    u32 name;
    u32 num;
    u32 vertex; /* index into the u16 array after the part table */
} HSFFILEPART;
HSF_FILE_ASSERT_SIZE(HSFFILEPART, 12);

typedef struct HsfFileCluster_s {
    u32 name[2];
    u32 targetName;
    u32 part; /* part index */
    u32 index;
    u32 weight[32];
    u8 adjusted;
    u8 unk95;
    u16 type;
    u32 vertexNum;
    u32 vertex; /* index into the symbol table */
} HSFFILECLUSTER;
HSF_FILE_ASSERT_SIZE(HSFFILECLUSTER, 160);

typedef struct HsfFileShape_s {
    u32 name;
    u16 num16[2];
    u32 vertex; /* index into the symbol table */
} HSFFILESHAPE;
HSF_FILE_ASSERT_SIZE(HSFFILESHAPE, 12);

typedef struct HsfFileMesh_s {
    u32 parent;
    u32 childNum;
    u32 child; /* index into the symbol table */
    HSFFILETRANSFORM base;
    HSFFILETRANSFORM curr;
    union {
        struct {
            u32 min[3];
            u32 max[3];
            u32 baseMorph;
            u32 morphWeight[33];
        } mesh;
        u32 replica; /* object index */
    };
    u32 face;      /* buffer indices */
    u32 vertex;
    u32 normal;
    u32 color;
    u32 st;
    u32 material;
    u32 attribute;
    u8 writeNum;
    u8 unk121;
    u8 shapeType;
    u8 matPass;
    u32 shapeNum;
    u32 shape; /* index into the symbol table */
    u32 clusterNum;
    u32 cluster; /* index into the symbol table */
    u32 cenvNum;
    u32 cenv; /* cenv index */
    u32 vtxtop; /* offsets from the start of the file */
    u32 normtop;
} HSFFILEMESH;
HSF_FILE_ASSERT_SIZE(HSFFILEMESH, 308);

typedef struct HsfFileCamera_s {
    u32 pos[3];
    u32 target[3];
    u32 upRot;
    u32 fov;
    u32 nearPlane;
    u32 farPlane;
} HSFFILECAMERA;
HSF_FILE_ASSERT_SIZE(HSFFILECAMERA, 40);

typedef struct HsfFileLight_s {
    u32 pos[3];
    u32 target[3];
    u8 type;
    u8 r;
    u8 g;
    u8 b;
    u32 unk2C;
    u32 refDistance;
    u32 refBrightness;
    u32 cutoff;
} HSFFILELIGHT;
HSF_FILE_ASSERT_SIZE(HSFFILELIGHT, 44);

typedef struct HsfFileObject_s {
    u32 name;
    u32 type;
    u32 constData;
    u32 flags;
    union {
        HSFFILEMESH mesh;
        HSFFILECAMERA camera;
        HSFFILELIGHT light;
    };
} HSFFILEOBJECT;
HSF_FILE_ASSERT_SIZE(HSFFILEOBJECT, 324);

typedef struct HsfFileSkeleton_s {
    u32 name;
    HSFFILETRANSFORM transform;
} HSFFILESKELETON;
HSF_FILE_ASSERT_SIZE(HSFFILESKELETON, 40);

typedef struct HsfFileBitmapKey_s {
    u32 time;
    u32 data; /* bitmap index */
} HSFFILEBITMAPKEY;
HSF_FILE_ASSERT_SIZE(HSFFILEBITMAPKEY, 8);

typedef struct HsfFileTrack_s {
    u8 type;
    u8 start;
    u16 target;
    u16 index;   /* attribute index, or the low half of clusterWeight */
    u16 channel;
    u16 curveType;
    u16 numKeyframes;
    u32 data; /* offset from the end of the track array, or a float value */
} HSFFILETRACK;
HSF_FILE_ASSERT_SIZE(HSFFILETRACK, 16);

typedef struct HsfFileMotion_s {
    u32 name;
    u32 numTracks;
    u32 track;
    u32 maxTime;
} HSFFILEMOTION;
HSF_FILE_ASSERT_SIZE(HSFFILEMOTION, 16);

typedef struct HsfFileMapAttr_s {
    u32 minX;
    u32 minZ;
    u32 maxX;
    u32 maxZ;
    u32 data; /* index into the u16 array after the table */
    u32 dataLen;
} HSFFILEMAPATTR;
HSF_FILE_ASSERT_SIZE(HSFFILEMAPATTR, 24);

typedef struct HsfFileMatrix_s {
    u32 base_idx;
    u32 count;
    u32 data;
} HSFFILEMATRIX;
HSF_FILE_ASSERT_SIZE(HSFFILEMATRIX, 12);

#endif
