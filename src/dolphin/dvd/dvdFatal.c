#include <dolphin/dvd.h>

static void (*FatalFunc)(void) = NULL;

void __DVDPrintFatalMessage(void)
{
    if (FatalFunc) {
        FatalFunc();
    }
}
