#ifndef _M532DLL_SHARED_B_H
#define _M532DLL_SHARED_B_H

#include "dolphin/types.h"

// Types of units owned by the other agent (m532.cpp, unk.cpp), declared minimally.
// Each section is guarded so several units may declare the same type.

// ---- ball.cpp: model wrapper and object base (m532.cpp / unk.cpp)
#ifndef M532_BALL_SHARED
#define M532_BALL_SHARED
// One definition lives in obj.h (owned by m532.cpp / unk.cpp).
#include "REL/m532Dll/obj.h"
typedef M532Node M532BallNode;
typedef M532UnkObjVtbl M532ObjVtbl;
typedef M532UnkObj M532Obj;
#endif

#endif
