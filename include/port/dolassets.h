#ifndef PORT_DOLASSETS_H_
#define PORT_DOLASSETS_H_

/*
 * Binary data the matching build includes from files dtk extracts out of the
 * original main.dol and REL modules (fonts, environment maps, disc error
 * screens, the Nintendo logo). The repository cannot contain them, so the port
 * reads them from the player's own disc at startup.
 *
 * Each blob is described by its address and size in the original executable
 * (config/GP5E01_00/symbols.txt, "extract:" entries of config.yml).
 */

#ifdef __cplusplus
extern "C" {
#endif

enum DolIncludes {
    DOL_INCLUDE_ANK8X8_4B,
    DOL_INCLUDE_ASCII8X8_1BPP,

    DOL_INCLUDE_REFMAPDATA0,
    DOL_INCLUDE_REFMAPDATA1,
    DOL_INCLUDE_REFMAPDATA2,
    DOL_INCLUDE_REFMAPDATA3,
    DOL_INCLUDE_REFMAPDATA4,

    DOL_INCLUDE_TOONMAPDATA,
    DOL_INCLUDE_TOONMAPDATA2,

    DOL_INCLUDE_HILITEDATA,
    DOL_INCLUDE_HILITEDATA2,
    DOL_INCLUDE_HILITEDATA3,
    DOL_INCLUDE_HILITEDATA4,

    DOL_INCLUDE_COVEROPEN_EN,
    DOL_INCLUDE_FATALERROR_EN,
    DOL_INCLUDE_LOADING_EN,
    DOL_INCLUDE_NODISC_EN,
    DOL_INCLUDE_RETRYERROR_EN,
    DOL_INCLUDE_WRONGDISC_EN,

    DOL_INCLUDE_COUNT,
};

enum RelIncludes {
    REL_INCLUDE_BOOT_NINTENDODATA,
    REL_INCLUDE_E3BOOT_NINTENDODATA,

    REL_INCLUDE_COUNT,
};

/* Copies main.dol out of the disc image. Called once before the game starts. */
void InitializeDol(void);
/* Pointer to a copy of the blob, stable for the whole run; NULL if unavailable. */
void *GetDolIncludeData(int index);
/* Same for data stored in a REL module on the disc. */
void *GetRelIncludeData(int index);

#define ank8x8_4b ((u8 *)GetDolIncludeData(DOL_INCLUDE_ANK8X8_4B))
#define Ascii8x8_1bpp ((u8 *)GetDolIncludeData(DOL_INCLUDE_ASCII8X8_1BPP))

#define refMapData0 GetDolIncludeData(DOL_INCLUDE_REFMAPDATA0)
#define refMapData1 GetDolIncludeData(DOL_INCLUDE_REFMAPDATA1)
#define refMapData2 GetDolIncludeData(DOL_INCLUDE_REFMAPDATA2)
#define refMapData3 GetDolIncludeData(DOL_INCLUDE_REFMAPDATA3)
#define refMapData4 GetDolIncludeData(DOL_INCLUDE_REFMAPDATA4)

#define toonMapData GetDolIncludeData(DOL_INCLUDE_TOONMAPDATA)
#define toonMapData2 GetDolIncludeData(DOL_INCLUDE_TOONMAPDATA2)

#define hiliteData GetDolIncludeData(DOL_INCLUDE_HILITEDATA)
#define hiliteData2 GetDolIncludeData(DOL_INCLUDE_HILITEDATA2)
#define hiliteData3 GetDolIncludeData(DOL_INCLUDE_HILITEDATA3)
#define hiliteData4 GetDolIncludeData(DOL_INCLUDE_HILITEDATA4)

#define coveropen_en ((u8 *)GetDolIncludeData(DOL_INCLUDE_COVEROPEN_EN))
#define fatalerror_en ((u8 *)GetDolIncludeData(DOL_INCLUDE_FATALERROR_EN))
#define loading_en ((u8 *)GetDolIncludeData(DOL_INCLUDE_LOADING_EN))
#define nodisc_en ((u8 *)GetDolIncludeData(DOL_INCLUDE_NODISC_EN))
#define retryerror_en ((u8 *)GetDolIncludeData(DOL_INCLUDE_RETRYERROR_EN))
#define wrongdisc_en ((u8 *)GetDolIncludeData(DOL_INCLUDE_WRONGDISC_EN))

#ifdef __cplusplus
}
#endif

#endif
