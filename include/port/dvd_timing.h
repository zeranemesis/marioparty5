#ifndef MP5_NATIVE_DVD_TIMING_H
#define MP5_NATIVE_DVD_TIMING_H

/* Settings > Loading Speed = GameCube: disc reads take the console drive's time (src/port/dvd_timing.cpp). */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int Mp5Native_DvdTimingOn(void);
double Mp5Native_DvdTimingNow(void);
/* the read of [offset, offset + length) on the disc was issued now: returns when the drive is done */
double Mp5Native_DvdTimingRead(uint32_t offset, uint32_t length);
/* a blocking read: the caller waits, as the console's CPU did */
void Mp5Native_DvdTimingSleepUntil(double deadline);
/* the end of a background read: fn(arg) runs on the main thread once the deadline has passed */
void Mp5Native_DvdTimingDefer(double deadline, void (*fn)(void *), void *arg);
/* once per game tick, and inside the game's wait loops */
void Mp5Native_DvdTimingPump(void);

#ifdef __cplusplus
}
#endif

#endif
