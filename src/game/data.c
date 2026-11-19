#include "version.h"
#ifdef TARGET_PC
#include "port/dvd_timing.h"
#endif
#include "game/data.h"
#include "game/dvd.h"

#include "game/armem.h"
#include "game/process.h"
#include "dolphin/dvd.h"
#ifdef TARGET_PC
#include "port/endian.h"
#include "port/port_version.h"
#include "game/gamework.h"
#include <string.h>
#endif

#define STAT_ID_ARAM 0x10000

#define PTR_OFFSET(ptr, offset) (void *)(((u8 *)(ptr)+(u32)(offset)))
#define DATA_EFF_SIZE(size) (((size)+1) & ~0x1)

static void **HuDataReadMultiSub(int *dataNum, BOOL use_num, s32 num);

#define DATA_MAX_READSTAT 128


typedef struct DataDirStat_s {
    char *name;
    s32 entryNum;
} DATADIRSTAT;

#define DATADIR(name) { "data/" #name ".bin", -1 },

static DATADIRSTAT DataDirStat[] = {
    #include "datadir_table.h"
    { NULL, -1 },
#ifdef TARGET_PC
    /* room for the PAL-only and per-language archives HuDataPalInit appends after the US ones */
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
    { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 }, { NULL, -1 },
#endif
};

#undef DATADIR

#ifdef TARGET_PC
/* GP5P01: the port runs the US code, whose data numbers index the US archive layouts. Several archives were
 * reordered on the PAL disc; HuDataPalRemap turns a US data number into the PAL entry holding the same file. */
typedef struct PalDataEntry_s {
    s16 archive; /* PalArchives index, -1: keep the US entry */
    s16 entry;
} PalDataEntry;
typedef struct PalDataMap_s {
    const char *name;
    const PalDataEntry *map;
    s32 count;
} PalDataMap;
#include "../port/pal_datamap.inc"

#define PAL_DIR_MAX ((s32)(sizeof(DataDirStat) / sizeof(DataDirStat[0])))
#define PAL_ARCHIVE_MAX ((s32)(sizeof(PalArchives) / sizeof(PalArchives[0])))
static BOOL PalActive;
static const PalDataMap *PalDirMap[PAL_DIR_MAX];
static s16 PalArchiveDir[PAL_ARCHIVE_MAX];
/* per dir: its copy for HUWIN_LANG_GERMANY..SPAIN (PalLangArchives), -1 when the archive is not localized */
static s16 PalLangDir[PAL_DIR_MAX][4];

static s32 HuDataPalFindDir(const char *name, s32 dirMax)
{
    s32 i;
    for(i=0; i<dirMax; i++) {
        if(strcmp(DataDirStat[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* the dir of an archive, appended to DataDirStat when the US table does not have it */
static s32 HuDataPalAddDir(const char *name, s32 *dirMax)
{
    s32 dir = HuDataPalFindDir(name, *dirMax);
    if(dir >= 0) {
        return dir;
    }
    if(*dirMax >= PAL_DIR_MAX - 1) {
        OSPanic("data.c", __LINE__, "too many PAL archives\n");
    }
    dir = (*dirMax)++;
    DataDirStat[dir].name = (char *)name;
    if((DataDirStat[dir].entryNum = DVDConvertPathToEntrynum(DataDirStat[dir].name)) == -1) {
        OSReport("data.c: Data File Error(%s)\n", DataDirStat[dir].name);
        OSPanic("data.c", __LINE__, "\n");
    }
    return dir;
}

/* dirMax: US archives in DataDirStat; returns the new count with the PAL-only archives appended */
static s32 HuDataPalInit(s32 dirMax)
{
    s32 i, dir;
    PalActive = mp5native_version_is_pal();
    if(!PalActive) {
        return dirMax;
    }
    for(i=0; i<PAL_ARCHIVE_MAX; i++) {
        PalArchiveDir[i] = (s16)HuDataPalAddDir(PalArchives[i], &dirMax);
    }
    for(i=0; i<PAL_DIR_MAX; i++) {
        PalLangDir[i][0] = PalLangDir[i][1] = PalLangDir[i][2] = PalLangDir[i][3] = -1;
    }
    for(i=0; i<(s32)(sizeof(PalLangArchives) / sizeof(PalLangArchives[0])); i++) {
        s32 lang;
        dir = HuDataPalAddDir(PalLangArchives[i][0], &dirMax);
        for(lang=0; lang<4; lang++) {
            if(PalLangArchives[i][lang+1]) {
                PalLangDir[dir][lang] = (s16)HuDataPalAddDir(PalLangArchives[i][lang+1], &dirMax);
            }
        }
    }
    for(i=0; i<(s32)(sizeof(PalDataMaps) / sizeof(PalDataMaps[0])); i++) {
        dir = HuDataPalFindDir(PalDataMaps[i].name, dirMax);
        if(dir >= 0) {
            PalDirMap[dir] = &PalDataMaps[i];
        }
    }
    return dirMax;
}

static int HuDataPalRemap(int dataNum)
{
    s32 dir = (dataNum >> 16) & 0xFFFF;
    s32 entry = dataNum & 0xFFFF;
    const PalDataMap *map;
    if(dataNum != HU_DATANUM_NONE && (entry & DATA_PAL_NATIVE)) {
        return dataNum & ~DATA_PAL_NATIVE;
    }
    if(!PalActive || dataNum == HU_DATANUM_NONE || dir >= PAL_DIR_MAX || !(map = PalDirMap[dir])) {
        return dataNum;
    }
    if(entry >= map->count || map->map[entry].archive < 0) {
        return dataNum;
    }
    return (PalArchiveDir[map->map[entry].archive] << 16) | map->map[entry].entry;
}
#define PAL_REMAP(dataNum) ((dataNum) = HuDataPalRemap(dataNum))

/* the archive a data number reads from: GP5P01 swaps a localized archive for its copy in the game's language,
 * for every access to it (read, lookup, close), so the English and localized copies are never both loaded */
static s32 HuDataPalDirId(int dataNum)
{
    s32 dir = (dataNum >> 16) & 0xFFFF;
    s32 lang = GWLanguageGet() - HUWIN_LANG_GERMANY;
    if(PalActive && dir < PAL_DIR_MAX && lang >= 0 && lang < 4 && PalLangDir[dir][lang] >= 0) {
        return PalLangDir[dir][lang];
    }
    return dir;
}
#define DATA_DIRID(dataNum) HuDataPalDirId(dataNum)
#else
#define PAL_REMAP(dataNum) ((void)0)
#define DATA_DIRID(dataNum) ((dataNum) >> 16)
#endif

SHARED_SYM u32 DirDataSize;
static u32 DataDirMax;
static s32 shortAccessSleep;
static HUDATASTAT ATTRIBUTE_ALIGN(32) ReadDataStat[DATA_MAX_READSTAT];

void HuDataInit(void)
{
    s32 i = 0;
    DATADIRSTAT *dirStat = DataDirStat;
    HUDATASTAT *readStat;
    while(dirStat->name) {
        if((dirStat->entryNum = DVDConvertPathToEntrynum(dirStat->name)) == -1) {
            OSReport("data.c: Data File Error(%s)\n", dirStat->name);
            OSPanic("data.c", 66, "\n");
        }
        i++;
        dirStat++;
    }
#ifdef TARGET_PC
    i = HuDataPalInit(i);
#endif
    DataDirMax = i;
    for(i=0, readStat = ReadDataStat; i<DATA_MAX_READSTAT; i++, readStat++) {
        readStat->dirId = HU_DATANUM_NONE;
        readStat->used = FALSE;
        readStat->status = 0;
    }
}

static s32 HuDataReadStatusGet(void)
{
    s32 i;
    for(i=0; i<DATA_MAX_READSTAT; i++) {
        if(ReadDataStat[i].dirId == HU_DATANUM_NONE) {
            break;
        }
    }
    if(i >= DATA_MAX_READSTAT) {
        i = HU_DATA_STAT_NONE;
    }
    return i;
}

s32 HuDataReadChk(int dirNum)
{
    s32 i;
#ifdef TARGET_PC
    dirNum = DATA_DIRID(dirNum);
#else
    dirNum >>= 16;
#endif
    for(i=0; i<DATA_MAX_READSTAT; i++) {
        if(ReadDataStat[i].dirId == dirNum && ReadDataStat[i].status != 1) {
            break;
        }
    }
    if(i >= DATA_MAX_READSTAT) {
        i = HU_DATA_STAT_NONE;
    }
    return i;
}

HUDATASTAT *HuDataGetStatus(void *dirP)
{
    s32 i;
    for(i=0; i<DATA_MAX_READSTAT; i++) {
        if(ReadDataStat[i].dirP == dirP) {
            break;
        }
    }
    if(i > DATA_MAX_READSTAT) {
        return NULL;
    }
    return &ReadDataStat[i];
}

void *HuDataGetDirPtr(int dirNum)
{
    s32 statId = HuDataReadChk(dirNum);
    if(statId < 0) {
        return NULL;
    }
    return ReadDataStat[statId].dirP;
}

HUDATASTAT *HuDataDirRead(int dataNum)
{
    HUDATASTAT *readStat;
    s32 statId;
    s32 dirId;
    dirId  = DATA_DIRID(dataNum);
    if(DataDirMax <= dirId) {
        OSReport("data.c: Data Number Error(%d)\n", dataNum);
        return NULL;
    }
    
    if((statId = HuDataReadChk(dataNum)) < 0) {
        AMEM_PTR dirAMemP;
        if(dirAMemP = HuARDirCheck(dataNum)) {
            HuAR_ARAMtoMRAM(dirAMemP);
            while(HuARDMACheck());
            statId = HuDataReadChk(dataNum);
            readStat = &ReadDataStat[statId];
        } else {
            if((statId = HuDataReadStatusGet()) == HU_DATA_STAT_NONE) {
                OSReport("data.c: Data Work Max Error\n");
                return NULL;
            }
            readStat = &ReadDataStat[statId];
            readStat->dirP = HuDvdDataFastRead(DataDirStat[dirId].entryNum);
            if(readStat->dirP) {
                readStat->dirId = dirId;
            }
        }
    } else {
        readStat = &ReadDataStat[statId];
        DirDataSize = readStat->dvdFile.length;
    }
    return readStat;
}

static HUDATASTAT *HuDataDirReadNum(int dataNum, s32 num)
{
    HUDATASTAT *readStat;
    s32 statId;
    s32 dirId;
    dirId  = DATA_DIRID(dataNum);
    if(DataDirMax <= dirId) {
        OSReport("data.c: Data Number Error(%d)\n", dataNum);
        return NULL;
    }
    
    if((statId = HuDataReadChk(dataNum)) < 0) {
        AMEM_PTR dirAMemP;
        if((dirAMemP = HuARDirCheck(dataNum))) {
            OSReport("ARAM data num %x\n", dataNum);
            HuAR_ARAMtoMRAMNum(dirAMemP, num);
            while(HuARDMACheck());
            statId = HuDataReadChk(dataNum);
            readStat = &ReadDataStat[statId];
            readStat->used = TRUE;
            readStat->num = num;
        } else {
            OSReport("data num %x\n", dataNum);
            if((statId = HuDataReadStatusGet()) == HU_DATA_STAT_NONE) {
                OSReport("data.c: Data Work Max Error\n");
                return NULL;
            }
            readStat = &ReadDataStat[statId];
            readStat->dirP = HuDvdDataFastReadNum(DataDirStat[dirId].entryNum, num);
            if(readStat->dirP) {
                readStat->dirId = dirId;
                readStat->used = TRUE;
                readStat->num = num;
            }
        }
    } else {
        readStat = &ReadDataStat[statId];
    }
    return readStat;
}

HUDATASTAT *HuDataDirSet(void *dirP, int dataNum)
{
    HUDATASTAT *readStat = HuDataGetStatus(dirP);
    s32 statId;
    if((statId = HuDataReadChk(readStat->dirId << 16)) >= 0) {
        HuDataDirClose(dataNum);
    }
    if((statId = HuDataReadStatusGet()) == HU_DATA_STAT_NONE) {
        OSReport("data.c: Data Work Max Error\n");
        return NULL;
    } else {
        readStat = &ReadDataStat[statId];
        readStat->dirP = dirP;
        readStat->dirId = dataNum >>16;
        return readStat;
    }
}

#ifdef TARGET_PC
static void HuDataDirReadAsyncDone(void *arg)
{
    ((HUDATASTAT *)arg)->status = 0;
}
#endif

void HuDataDirReadAsyncCallBack(s32 result, DVDFileInfo* fileInfo)
{
    HUDATASTAT *readStat;
    s32 i;
    for(i=0; i<DATA_MAX_READSTAT; i++) {
        if(ReadDataStat[i].status == 1 && ReadDataStat[i].dvdFile.startAddr == fileInfo->startAddr) {
            break;
        }
    }
    if(i >= DATA_MAX_READSTAT) {
        OSPanic("data.c", 361, "dvd.c AsyncCallBack Error");
    }
    readStat = &ReadDataStat[i];
    if(result == DVD_RESULT_CANCELED) {
        DVDClose(&readStat->dvdFile);
        readStat->status = 0;
        return;
    }
#ifdef TARGET_PC
    /* the block just read keeps the drive busy: the background read ends when the drive would have */
    if(Mp5Native_DvdTimingOn()) {
        u32 blockSize = readStat->readLen-readStat->readOfs;
        if(blockSize > HU_DVD_BLOCKSIZE) {
            blockSize = HU_DVD_BLOCKSIZE;
        }
        readStat->doneAt = Mp5Native_DvdTimingRead(fileInfo->startAddr+readStat->readOfs, OSRoundUp32B(blockSize));
    }
#endif
    readStat->readOfs += HU_DVD_BLOCKSIZE;
    if(readStat->readLen > readStat->readOfs) {
        u32 readSize = readStat->readLen-readStat->readOfs;
        if(readSize > HU_DVD_BLOCKSIZE) {
            readSize = HU_DVD_BLOCKSIZE;
        }
        DVDReadAsyncPrio(fileInfo, ((u8 *)readStat->dirP)+readStat->readOfs, OSRoundUp32B(readSize), readStat->readOfs, HuDataDirReadAsyncCallBack, 3);
    } else {
#ifdef TARGET_PC
        if(Mp5Native_DvdTimingOn()) {
            Mp5Native_DvdTimingDefer(readStat->doneAt, HuDataDirReadAsyncDone, readStat);
        } else {
            readStat->status = 0;
        }
#else
        readStat->status = 0;
#endif
        DVDClose(&readStat->dvdFile);
    }
}

void HuDataDirCancel(s16 statId)
{
    s32 ret = DVDCancel(&ReadDataStat[statId].dvdFile.cb);
#ifdef TARGET_PC
    while(ReadDataStat[statId].status) {
        Mp5Native_DvdTimingPump(); /* completes a background read held for the drive's time */
    }
#else
    while(ReadDataStat[statId].status);
#endif
    HuDataDirClose(ReadDataStat[statId].dirId << 16);
}

void HuDataDirCancelAsync(s16 statId)
{
    DVDCancelAsync(&ReadDataStat[statId].dvdFile.cb, NULL);
}

BOOL HuDataDirCloseAsync(s16 statId)
{
    if(ReadDataStat[statId].status) {
        return FALSE;
    }
    HuDataDirClose(ReadDataStat[statId].dirId << 16);
    return TRUE;
}


s32 HuDataDirReadAsync(int dataNum)
{
    HUDATASTAT *readStat;
    s32 statId;
    s32 dirId;
    dirId  = DATA_DIRID(dataNum);
    if(DataDirMax <= dirId) {
        OSReport("data.c: Data Number Error(%d)\n", dataNum);
        return HU_DATA_STAT_NONE;
    }
    if((statId = HuDataReadChk(dataNum)) < 0) {
        AMEM_PTR dirAMemP;
        if(dirAMemP = HuARDirCheck(dataNum)) {
            OSReport("ARAM data num %x\n", dataNum);
            HuAR_ARAMtoMRAM(dirAMemP);
            statId = STAT_ID_ARAM;
        } else {
            statId = HuDataReadStatusGet();
            if(statId == HU_DATA_STAT_NONE) {
                OSReport("data.c: Data Work Max Error\n");
                return HU_DATA_STAT_NONE;
            }
            readStat = &ReadDataStat[statId];
            readStat->status = 1;
            readStat->dirId = dirId;
            readStat->dirP = HuDvdDataFastReadAsync(DataDirStat[dirId].entryNum, readStat);
        }
    } else {
        statId = HU_DATA_STAT_NONE;
    }
    return statId;
}

s32 HuDataDirReadNumAsync(int dataNum, s32 num)
{
    HUDATASTAT *readStat;
    s32 statId;
    s32 dirId;
    dirId  = DATA_DIRID(dataNum);
    if(DataDirMax <= dirId) {
        OSReport("data.c: Data Number Error(%d)\n", dataNum);
        return HU_DATA_STAT_NONE;
    }
    if((statId = HuDataReadChk(dataNum)) < 0) {
        if((statId = HuDataReadStatusGet()) == HU_DATA_STAT_NONE) {
            OSReport("data.c: Data Work Max Error\n");
            return HU_DATA_STAT_NONE;
        }
        ReadDataStat[statId].status = TRUE;
        ReadDataStat[statId].dirId = dirId;
        readStat = &ReadDataStat[statId];
        readStat->used = TRUE;
        readStat->num = num;
        readStat->dirP = HuDvdDataFastReadAsync(DataDirStat[dirId].entryNum, readStat);
    } else {
        statId = HU_DATA_STAT_NONE;
    }
    return statId;
}

BOOL HuDataGetAsyncStat(s32 statId)
{
#ifdef TARGET_PC
    Mp5Native_DvdTimingPump();
#endif
    if(statId == STAT_ID_ARAM) {
        return HuARDMACheck() == 0;
    } else {
        return ReadDataStat[statId].status == 0;
    }
}

static void GetFileInfo(HUDATASTAT *readStat, s32 fileNum)
{
    u32 *ptr;
    ptr = (u32 *)PTR_OFFSET(readStat->dirP, (fileNum * 4))+1;
#ifdef TARGET_PC
    // the archive header is big-endian on the disc
    readStat->fileDataP = PTR_OFFSET(readStat->dirP, PORT_BE32(*ptr));
    ptr = readStat->fileDataP;
    readStat->rawLen = PORT_BE32(*ptr); ptr++;
    readStat->decodeType = PORT_BE32(*ptr); ptr++;
#else
    readStat->fileDataP = PTR_OFFSET(readStat->dirP, *ptr);
    ptr = readStat->fileDataP;
    readStat->rawLen = *ptr++;
    readStat->decodeType = *ptr++;
#endif
    readStat->fileDataP = ptr;
}

void *HuDataRead(int dataNum)
{
    HUDATASTAT *readStat;
    s32 statId;
    void *buf;
    PAL_REMAP(dataNum);
    if(!HuDataDirRead(dataNum)) {
        return NULL;
    }
    if((statId = HuDataReadChk(dataNum)) == HU_DATA_STAT_NONE) {
        return NULL;
    }
    readStat = &ReadDataStat[statId];
    GetFileInfo(readStat, dataNum & 0xFFFF);
    buf = HuMemDirectMalloc(0, DATA_EFF_SIZE(readStat->rawLen));
    if(buf) {
        HuDecodeData(readStat->fileDataP, buf, readStat->rawLen, readStat->decodeType);
    }
    return buf;
}

void *HuDataReadNum(int dataNum, s32 num)
{
    HUDATASTAT *readStat;
    s32 statId;
    void *buf;
    PAL_REMAP(dataNum);
    if(!HuDataDirReadNum(dataNum, num)) {
        return NULL;
    }
    if((statId = HuDataReadChk(dataNum)) == HU_DATA_STAT_NONE) {
        return NULL;
    }
    readStat = &ReadDataStat[statId];
    GetFileInfo(readStat, dataNum & 0xFFFF);
    buf = HuMemDirectMallocNum(0, DATA_EFF_SIZE(readStat->rawLen), num);
    if(buf) {
        HuDecodeData(readStat->fileDataP, buf, readStat->rawLen, readStat->decodeType);
    }
    return buf;
}

void *HuDataSelHeapRead(int dataNum, HEAPID heap)
{
    HUDATASTAT *readStat;
    s32 statId;
    void *buf;
    PAL_REMAP(dataNum);
    if(!HuDataDirRead(dataNum)) {
        return NULL;
    }
    if((statId = HuDataReadChk(dataNum)) == HU_DATA_STAT_NONE) {
        return NULL;
    }
    readStat = &ReadDataStat[statId];
    GetFileInfo(readStat, dataNum & 0xFFFF);
    switch(heap) {
        case HEAP_SOUND:
            buf = HuMemDirectMalloc(HEAP_SOUND, DATA_EFF_SIZE(readStat->rawLen));
            break;
            
        case HEAP_MODEL:
            buf = HuMemDirectMalloc(HEAP_MODEL, DATA_EFF_SIZE(readStat->rawLen));
            break;
            
        case HEAP_DVD:
            buf = HuMemDirectMalloc(HEAP_DVD, DATA_EFF_SIZE(readStat->rawLen));
            break;
            
        default:
            buf = HuMemDirectMalloc(HEAP_HEAP, DATA_EFF_SIZE(readStat->rawLen));
            break;
    }
    if(buf) {
        HuDecodeData(readStat->fileDataP, buf, readStat->rawLen, readStat->decodeType);
    }
    return buf;
}

void *HuDataSelHeapReadNum(int dataNum, s32 num, HEAPID heap)
{
    HUDATASTAT *readStat;
    s32 statId;
    void *buf;
    PAL_REMAP(dataNum);
    if(!HuDataDirReadNum(dataNum, num)) {
        return NULL;
    }
    if((statId = HuDataReadChk(dataNum)) == HU_DATA_STAT_NONE) {
        return NULL;
    }
    readStat = &ReadDataStat[statId];
    GetFileInfo(readStat, dataNum & 0xFFFF);
    switch(heap) {
        case HEAP_SOUND:
            buf = HuMemDirectMalloc(HEAP_SOUND, DATA_EFF_SIZE(readStat->rawLen));
            break;
            
        case HEAP_MODEL:
            buf = HuMemDirectMallocNum(HEAP_MODEL, DATA_EFF_SIZE(readStat->rawLen), num);
            break;
            
        case HEAP_DVD:
            buf = HuMemDirectMallocNum(HEAP_DVD, DATA_EFF_SIZE(readStat->rawLen), num);
            break;
            
        default:
            buf = HuMemDirectMallocNum(HEAP_HEAP, DATA_EFF_SIZE(readStat->rawLen), num);
            break;
    }
    if(buf) {
        HuDecodeData(readStat->fileDataP, buf, readStat->rawLen, readStat->decodeType);
    }
    return buf;
}

void **HuDataReadMulti(int *dataNum)
{
    return HuDataReadMultiSub(dataNum, FALSE, 0);
}

static void **HuDataReadMultiSub(int *dataNum, BOOL use_num, s32 num)
{
    s32 *dirIds;
    char **pathTbl;
    void **dirP;
    void **outList;
    s32 i, count, numFiles;
    u32 dirId;
    for(i=0, count=0; dataNum[i] != HU_DATANUM_NONE; i++) {
        dirId = DATA_DIRID(dataNum[i]);
        if(DataDirMax <= dirId) {
            OSReport("data.c: Data Number Error(%d)\n", dataNum[i]);
            return NULL;
        }
        if(HuDataReadChk(dataNum[i]) < 0) {
            count++;
        }
    }
    numFiles = i;
    dirIds = HuMemDirectMalloc(HEAP_HEAP, (count+1)*sizeof(s32));
    for(i=0; i<count+1; i++) {
        dirIds[i] = HU_DATANUM_NONE;
    }
    pathTbl = HuMemDirectMalloc(HEAP_HEAP, (count+1)*sizeof(char *));
    for(i=0, count=0; dataNum[i] != HU_DATANUM_NONE; i++) {
        dirId = DATA_DIRID(dataNum[i]);
        if(HuDataReadChk(dataNum[i]) < 0) {
            s32 j;
            for(j=0; dirIds[j] != HU_DATANUM_NONE; j++) {
                if(dirIds[j] == dirId){
                    break;
                }
            }
            if(dirIds[j] == HU_DATANUM_NONE) {
                dirIds[j] = dirId;
                pathTbl[count++] = DataDirStat[dirId].name;
            }
        }
    }
    dirP = HuDvdDataReadMulti(pathTbl);
    for(i=0; dirIds[i] != HU_DATANUM_NONE; i++) {
        s32 statId;
        if((statId = HuDataReadStatusGet()) == HU_DATA_STAT_NONE) {
            OSReport("data.c: Data Work Max Error\n");
            (void)count; //HACK to match HuDataReadMultiSub
            HuMemDirectFree(dirIds);
            HuMemDirectFree(pathTbl);
            return NULL;
        } else {
            ReadDataStat[statId].dirP = dirP[i];
            ReadDataStat[statId].dirId = dirIds[i];
        }
    }
    HuMemDirectFree(dirIds);
    HuMemDirectFree(pathTbl);
    HuMemDirectFree(dirP);
    if(use_num) {
        outList = HuMemDirectMallocNum(HEAP_HEAP, (numFiles+1)*sizeof(void *), num);
    } else {
        outList = HuMemDirectMalloc(HEAP_HEAP, (numFiles+1)*sizeof(void *));
    }
    for(i=0; dataNum[i] != HU_DATANUM_NONE; i++) {
        if(use_num) {
            outList[i] = HuDataReadNum(dataNum[i], num);
        } else {
            outList[i] = HuDataRead(dataNum[i]);
        }
    }
    outList[i] = NULL;
    return outList;
}

s32 HuDataGetSize(int dataNum)
{
    HUDATASTAT *readStat;
    s32 statId;
    PAL_REMAP(dataNum);
    if((statId = HuDataReadChk(dataNum)) == HU_DATA_STAT_NONE) {
        return -1;
    }
    readStat = &ReadDataStat[statId];
    GetFileInfo(readStat, dataNum & 0xFFFF);
    return DATA_EFF_SIZE(readStat->rawLen);
}

void HuDataClose(void *ptr)
{
    if(ptr) {
        HuMemDirectFree(ptr);
    }
}

void HuDataCloseMulti(void **ptrs)
{
    s32 i;
    for(i=0; ptrs[i]; i++) {
        void *ptr = ptrs[i];
        if(ptr) {
            HuMemDirectFree(ptr);
        }
    }
    if(ptrs) {
        HuMemDirectFree(ptrs);
    }
}

void HuDataDirClose(int dataNum)
{
    HUDATASTAT *readStat;
    s32 i;
    s32 dirId = DATA_DIRID(dataNum);
    for(i=0; i<DATA_MAX_READSTAT; i++) {
        if(ReadDataStat[i].dirId == dirId) {
            break;
        }
    }
    if(i >= DATA_MAX_READSTAT) {
        return;
    }
    readStat = &ReadDataStat[i];
    if(readStat->status == 1) {
        DVDCancel(&ReadDataStat[i].dvdFile.cb);
#ifdef TARGET_PC
        while(ReadDataStat[i].status) {
            Mp5Native_DvdTimingPump(); /* completes a background read held for the drive's time */
        }
#else
        while(ReadDataStat[i].status);
#endif
    }
    readStat->dirId = HU_DATANUM_NONE;
    HuDvdDataClose(readStat->dirP);
    readStat->dirP = NULL;
    readStat->used = FALSE;
    readStat->status = 0;
}

void HuDataDirCloseNum(s32 num)
{
    s32 i;
    for(i=0; i<DATA_MAX_READSTAT; i++) {
        if(ReadDataStat[i].used == TRUE && ReadDataStat[i].num == num) {
            HuDataDirClose(ReadDataStat[i].dirId << 16);
        }
    }
}

static s32 HuDataDVDdirDirectOpen(int dataNum, DVDFileInfo *fileInfo)
{
	s32 dir = DATA_DIRID(dataNum);
	if(dir >= (s32)DataDirMax) {
		OSReport("data.c: Data Number Error(0x%08x)\n", dataNum);
		return 0;
	}
	if(!DVDFastOpen(DataDirStat[dir].entryNum, fileInfo)) {
		char panic_str[48];
		sprintf(panic_str, "HuDataDVDdirDirectOpen: File Open Error(%08x)", dataNum);
		OSPanic("data.c", 958, panic_str);
	}
	return 1;
}

static s32 HuDataDVDdirDirectRead(DVDFileInfo *fileInfo, void *dest, s32 len, s32 offset)
{
	s32 result = DVDReadAsync(fileInfo, dest, len, offset, NULL);
	if(result != 1) {
		OSPanic("data.c", 967, "HuDataDVDdirDirectRead: File Read Error");
	}
#ifdef TARGET_PC
	/* the loop below waits for the drive: frame by frame with shortAccessSleep, else the whole game */
	if(Mp5Native_DvdTimingOn()) {
		double doneAt = Mp5Native_DvdTimingRead(fileInfo->startAddr+offset, len);
		while(Mp5Native_DvdTimingNow() < doneAt) {
			if(shortAccessSleep) {
				HuPrcVSleep();
			} else {
				Mp5Native_DvdTimingSleepUntil(doneAt);
			}
		}
	}
#endif
	while(DVDGetCommandBlockStatus(&fileInfo->cb)) {
		if(shortAccessSleep) {
			HuPrcVSleep();
		}
	}
	return result;
}

static void *HuDataDecodeIt(void *bufP, s32 bufOfs, s32 num, HEAPID heap)
{
	void *dataStart;
	s32 *buf;
	s32 rawLen, decodeType;
	
	void *dest;
	buf =  (s32 *)((u8 *)bufP+bufOfs);
	if((uintptr_t)buf & 0x3) {
		u8 *data = (u8 *)buf;
		rawLen = *data++ << 24;
		rawLen += *data++ << 16;
		rawLen += *data++ << 8;
		rawLen += *data++;
		decodeType = *data++ << 24;
		decodeType += *data++ << 16;
		decodeType += *data++ << 8;
		decodeType += *data++;
		dataStart = data;
	} else {
		s32 *data = buf;
#ifdef TARGET_PC
		// the archive header is big-endian on the disc
		rawLen = PORT_BE32(*data); data++;
		decodeType = PORT_BE32(*data); data++;
#else
		rawLen = *data++;
		decodeType = *data++;
#endif
		dataStart = data;
	}
	switch(heap) {
        case HEAP_SOUND:
            dest = HuMemDirectMalloc(HEAP_SOUND, DATA_EFF_SIZE(rawLen));
            break;
            
        case HEAP_MODEL:
            dest = HuMemDirectMallocNum(HEAP_MODEL, DATA_EFF_SIZE(rawLen), num);
            break;
            
        case HEAP_DVD:
            dest = HuMemDirectMallocNum(HEAP_DVD, DATA_EFF_SIZE(rawLen), num);
            break;
            
        default:
            dest = HuMemDirectMallocNum(HEAP_HEAP, DATA_EFF_SIZE(rawLen), num);
            break;
    }
    if(dest) {
        HuDecodeData(dataStart, dest, rawLen, decodeType);
    }
    return dest;
}

void *HuDataReadNumHeapShortForce(int dataNum, s32 num, HEAPID heap)
{
	DVDFileInfo fileInfo;
	s32 *dataHdr;
	s32 *fileData;
	void *fileBuf;
	s32 readLen;
	s32 fileNum;
	s32 fileOfs;
	s32 readOfs;
	s32 dataOfs;
	void *ret;
	s32 dir;
	s32 dvdLen;
	s32 fileNumMax;

    PAL_REMAP(dataNum);
	if(!HuDataDVDdirDirectOpen(dataNum, &fileInfo)) {
		return NULL;
	}
	dir = (dataNum >> 16) & 0xFFFF0000;
	fileNum = dataNum & 0xFFFF;
	fileOfs = (fileNum*4)+4;
	dvdLen = OSRoundUp32B(fileOfs+8);
	fileData = HuMemDirectMalloc(HEAP_HEAP, dvdLen);
	if(!HuDataDVDdirDirectRead(&fileInfo, fileData, dvdLen, 0)) {
		HuMemDirectFree(fileData);
		DVDClose(&fileInfo);
		return NULL;
	}
#ifdef TARGET_PC
	fileNumMax = PORT_BE32(*fileData);
#else
	fileNumMax = *fileData;
#endif
	if(fileNumMax <= fileNum) {
		HuMemDirectFree(fileData);
		OSReport("data.c%d: Data Number Error(0x%08x)\n", 1068, dataNum);
		DVDClose(&fileInfo);
		return NULL;
	}
	dataHdr = fileData;
	dataHdr += fileNum+1;
#ifdef TARGET_PC
	fileOfs = PORT_BE32(*dataHdr);
#else
	fileOfs = *dataHdr;
#endif
	readOfs = OSRoundDown32B(fileOfs);
	if(fileNumMax <= fileNum+1) {
		readLen = fileInfo.length;
		dataOfs = readLen-readOfs;
	} else {
		dataHdr++;
#ifdef TARGET_PC
		dataOfs = PORT_BE32(*dataHdr)-readOfs;
#else
		dataOfs = (*dataHdr)-readOfs;
#endif
		readLen = fileInfo.length;
	}
	readLen = OSRoundUp32B(dataOfs);
	HuMemDirectFree(fileData);
	fileBuf = HuMemDirectMalloc(HEAP_HEAP, (readLen+4) & ~0x3);
	if(fileBuf == NULL) {
		OSReport("data.c: couldn't allocate read buffer(0x%08x)\n", dataNum);
		DVDClose(&fileInfo);
		return NULL;
	}
	if(!HuDataDVDdirDirectRead(&fileInfo, fileBuf, readLen, readOfs)) {
		HuMemDirectFree(fileBuf);
		DVDClose(&fileInfo);
		return NULL;
	}
	DVDClose(&fileInfo);
	dataOfs = fileOfs-readOfs;
	ret = HuDataDecodeIt(fileBuf, dataOfs, num, heap);
	HuMemDirectFree(fileBuf);
    return ret;
}

char lbl_8011FDA6[] = "** dcnt %d tmp %08x sp1 %08x\n";
char lbl_8011FDC4[] = "** dcnt %d lastNum %08x\n";