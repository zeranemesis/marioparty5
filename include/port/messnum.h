#ifndef PORT_MESSNUM_H_
#define PORT_MESSNUM_H_

#include "dolphin/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Handle above 0x80000000 standing for a string the game built. */
u32 HuWinMesPtrToNum(const char *ptr);
/* The string a handle stands for, NULL if it is not one. */
char *HuWinMesNumToPtr(u32 messNum);

#ifdef __cplusplus
}
#endif

#endif
