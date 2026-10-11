// m502dll/ending ' the epilogue/ending sequence: pad rumble cues and the
// crowd/applause gate that plays them.
#include "types.h"

void omVibrate(s16 padNo, s16 playerNo, s16 len, s16 amp);

void fn_1_A914(void *arg0);
void fn_1_A914(void *arg0)
{
}

// Maps a crowd-intensity tier onto a pad rumble: 0 = the light "cheer" buzz,
// 1 = a shorter pulse, 2 = a long low one. Tier 3 or anything else is silent.
void fn_1_C3C4(s32 padNo, s32 tier, s32 playerNo)
{
    switch (tier) {
    case 0:
        omVibrate((s16)padNo, (s16)playerNo, 6, 6);
        break;
    case 1:
        omVibrate((s16)padNo, (s16)playerNo, 4, 2);
        break;
    case 2:
        omVibrate((s16)padNo, (s16)playerNo, 0xC, 0);
        break;
    }
}
