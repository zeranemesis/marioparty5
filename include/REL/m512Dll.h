#ifndef _REL_M512DLL_H
#define _REL_M512DLL_H
#include "types.h"

// ring-list node: every object starts with one
struct DustNode {
    DustNode *next; // 0x00
    DustNode *prev; // 0x04
};

struct DustPuff {
    DustNode node;  // 0x00
    s16 modelId;    // 0x08
    s32 unkC;       // 0x0C
};


#endif
