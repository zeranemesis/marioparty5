#include "dolphin/os.h"

/* A stopwatch measures a duration with the timebase counter. The struct keeps the
   running time in 'total' and, since one stopwatch can be started and stopped
   repeatedly, the shortest and longest measured durations, together with the
   number of times the stopwatch has been used. 'last' is the timestamp taken by
   OSStartStopwatch, i.e. the beginning of the run in progress. */
void OSInitStopwatch(OSStopwatch* sw, char* name)
{
    sw->name = name;
    sw->total = 0;
    sw->hits = 0;
    sw->min = 0xFFFFFFFF;
    sw->max = 0;
}

void OSStartStopwatch(OSStopwatch* sw)
{
    sw->running = TRUE;
    sw->last = OSGetTime();
}

void OSStopStopwatch(OSStopwatch* sw)
{
    OSTime diff;

    if (sw->running == FALSE) {
        return;
    }

    diff = OSGetTime() - sw->last;

    sw->total += diff;
    sw->running = FALSE;
    sw->hits++;

    if (sw->max < diff) {
        sw->max = diff;
    }
    if (diff < sw->min) {
        sw->min = diff;
    }
}

OSTime OSCheckStopwatch(OSStopwatch* sw)
{
    OSTime total = sw->total;

    if (sw->running) {
        total += OSGetTime() - sw->last;
    }

    return total;
}

void OSResetStopwatch(OSStopwatch* sw)
{
    sw->total = 0;
    sw->hits = 0;
    sw->min = 0xFFFFFFFF;
    sw->max = 0;
}

void OSDumpStopwatch(OSStopwatch* sw)
{
    OSReport("Stopwatch [%s]:\n", sw->name);
    OSReport("\tTotal= %lld us\n", OSTicksToMicroseconds(sw->total));
    OSReport("\tHits = %d \n", sw->hits);
    OSReport("\tMin  = %lld us\n", OSTicksToMicroseconds(sw->min));
    OSReport("\tMax  = %lld us\n", OSTicksToMicroseconds(sw->max));
    OSReport("\tMean = %lld us\n", OSTicksToMicroseconds(sw->total / sw->hits));
}
