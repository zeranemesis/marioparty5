#include <dolphin/hw_regs.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>

static u16 FontEncode = 0xFFFF;

u16 OSGetFontEncode(void)
{
    if (FontEncode <= 1) {
        return FontEncode;
    }
    switch (*(int *)OSPhysicalToCached(0xCC)) {
        case VI_NTSC:
            FontEncode = (__VIRegs[VI_DTV_STAT] & 2) ? OS_FONT_ENCODE_SJIS : OS_FONT_ENCODE_ANSI;
            break;

        case VI_PAL:
        case VI_MPAL:
        case VI_DEBUG:
        case VI_DEBUG_PAL:
        case VI_EURGB60:
        default:
            FontEncode = OS_FONT_ENCODE_ANSI;
    }

    return FontEncode;
}
