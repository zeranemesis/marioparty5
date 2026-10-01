#include "dolphin/os.h"

#include "dolphin/os/OSPriv.h"

/* Time base frequency, and the software millisecond accumulator. */
#define OS_TIME_FREQ (*(volatile u32*)0x800000F8)
#define OS_TIME_BASE (OS_TIME_FREQ >> 2)

/* Running base for the software clock, updated by the timer interrupt. It is a
   64-bit value: high word at 0x800030DC, low word at 0x800030D8. */
#define __OSSystemTime (*(volatile OSTime*)0x800030D8)

extern s32 __mod2i(s32 n_hi, s32 n_lo, s32 d_hi, s32 d_lo);
extern s32 __div2i(s32 n_hi, s32 n_lo, s32 d_hi, s32 d_lo);

asm OSTime OSGetTime(void)
{
    nofralloc
    mftb r3, 269
    mftb r4, 268
    mftb r5, 269
    cmpw r3, r5
    bne OSGetTime
    blr
}

asm OSTick OSGetTick(void)
{
    nofralloc
    mftb r3, 268
    blr
}

OSTime __OSGetSystemTime(OSContext* interruptContext)
{
    BOOL flag;
    OSTime t;

    flag = OSDisableInterrupts();

    t = __OSSystemTime + OSGetTime();

    OSRestoreInterrupts(flag);

    return t;
}

/* Days elapsed before the start of each month, for common and leap years. */
static const s32 YearDays[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334,
};

static const s32 LeapYearDays[] = {
    0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335,
};

/* Leap days accumulated before the start of a year, by the Gregorian rule. */
static s32 LeapDays(s32 year)
{
    if (year < 1) {
        return 0;
    }
    return (year + 3) / 4 - (year - 1) / 100 + (year - 1) / 400;
}

static void GetDates(s32 days, OSCalendarTime* td)
{
    s32 year;
    s32 base;
    s32 ystart;
    s32 i;
    BOOL leap;
    const s32* table;

    /* 2000-01-01, the origin of the tick counter, was a Saturday. */
    td->wday = (days + 6) % 7;

    /* The year is close enough to days/365 for the walk below to be short. */
    year = days / 365;
    base = year * 365;

    year--;
    for (;;) {
        ystart = base - LeapDays(year);
        if (days >= ystart) {
            break;
        }
        base -= 365;
        year--;
    }

    td->year = year;
    td->yday = days - ystart;

    leap = (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0);
    table = leap ? LeapYearDays : YearDays;

    i = 12;
    while (td->yday < table[--i]) {
    }

    td->mon = i;
    td->mday = td->yday - table[i] + 1;
}

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td)
{
    OSTime rest;
    OSTime secs;
    s32 days;
    s32 sod;

    /* Split the tick count into whole seconds and the remainder. */
    rest = ticks % OS_TIME_BASE;
    if (rest < 0) {
        rest += OS_TIME_BASE;
    }

    td->usec = OSTicksToMicroseconds(rest) % 1000;
    td->msec = OSTicksToMilliseconds(rest) % 1000;

    secs = (ticks - rest) / OS_TIME_BASE;

    /* days counts from year 0, the calendar starts at 2000-01-01. */
    days = secs / 86400 + 730485;
    sod = secs % 86400;
    if (sod < 0) {
        sod += 86400;
        days--;
    }

    GetDates(days, td);

    td->hour = sod / 60 / 60;
    td->min = (sod / 60) % 60;
    td->sec = sod % 60;
}
