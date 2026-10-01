#ifndef _M504DLL_H
#define _M504DLL_H

#include "game/gamework.h"
#include "game/pad.h"
#include "game/object.h"
#include "game/hu3d.h"

#include "datanum/m504.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

int _prolog(void);
void _epilog(void);

#endif