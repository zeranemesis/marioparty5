#include "dolphin/os.h"

#include "dolphin/os/OSPriv.h"

/* Time base frequency, and the software millisecond accumulator. */
#define OS_TIME_FREQ (*(volatile u32*)0x800000F8)
#define OS_TIME_BASE (OS_TIME_FREQ >> 2)

/* Running base for the software clock, updated by the timer interrupt. It is a
   64-bit value: high word at 0x800030DC, low word at 0x800030D8. */
#define __OSSystemTime (*(volatile OSTime*)0x800030D8)

extern s32 __mod2i(s32 a, s32 b, s32 c);
extern s32 __div2i(s32 a, s32 b, s32 c);

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
    OSTime t;
    BOOL flag;

    flag = OSDisableInterrupts();

    t = OSGetTime();
    t += __OSSystemTime;

    OSRestoreInterrupts(flag);

    return t;
}

/* Days elapsed before the start of each month, for common and leap years. */
static const u16 YearDays[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334,
};

static const u16 LeapYearDays[] = {
    0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335,
};

/* Number of days in the years [0, y), i.e. 365 per year plus one extra day for
   every leap year. The expression is the one the compiler turns into the
   multiply/shift sequence seen in the original: the /32 and /128 terms cancel
   the leap days of the Gregorian century rules. */
static s32 DaysBeforeYear(s32 y)
{
    return y * 365 + ((y + 3) / 4) - (y / 128) - ((y + 128) / 128);
}

static BOOL IsLeapYear(s32 y)
{
    if (y % 4 != 0) {
        return FALSE;
    }
    if (y % 100 != 0) {
        return TRUE;
    }
    return y % 400 == 0;
}

static void GetDates(s32 days, OSCalendarTime* td)
{
    const u16* table;
    s32 year;
    s32 dayOfYear;
    s32 base;
    s32 i;

    /* days counts from 2000-01-01, which the loop below treats as day 0 of a
       year whose DaysBeforeYear is adjusted so the arithmetic stays positive. */
    base = (days / 7) * 7 - days;
    td->wday = base;

    year = (days + 6) / 7;
    dayOfYear = year * 365;

    /* Step forward one year at a time until the target day is inside the year. */
    for (i = year; days < dayOfYear + DaysBeforeYear(i - year); i++) {
        dayOfYear -= 365;
        year--;
    }

    td->year = year;
    td->yday = dayOfYear - days;

    if (IsLeapYear(year)) {
        table = LeapYearDays;
    } else {
        table = YearDays;
    }

    for (i = 11; ; i--) {
        if (td->yday < table[i]) {
            break;
        }
    }
    td->mon = i;
    td->mday = table[i] - td->yday;
}

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td)
{
    OSTime seconds;
    OSTime days;
    OSTime rest;
    s32 freq;
    s32 weekday;

    freq = OS_TIME_BASE;

    /* ticks counts from 2000-01-01 00:00:00. */
    rest = ticks % freq;
    if (rest < 0) {
        rest += freq;
    }
    days = (ticks - rest) / freq;

    seconds = rest / 1000;
    td->msec = rest - seconds * 1000;

    seconds = seconds / 1000;
    td->usec = rest - (rest / 1000) * 1000;

    seconds = seconds / 60;
    td->min = seconds - (seconds / 60) * 60;

    seconds = seconds / 60;
    td->hour = seconds / 24;
    td->sec = seconds - (td->hour * 60) * 24;

    /* 2000-01-01 was a Saturday, hence the +6 before the modulo. */
    weekday = (s32)(days + 6) / 7;
    td->wday = weekday * 7 - (s32)(days + 6);

    GetDates((s32)days, td);
}
