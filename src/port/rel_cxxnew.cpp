/*
 * MWCC's mangled operator new/delete (__nw__FUl, __dl__FPv, __nwa__FUl, __dla__FPv).
 * The decompiled C++ REL modules call them by those names with C linkage, as the
 * GameCube objects do. Every REL module gets this file (see CMakeLists.txt): the
 * defaults below forward to the module's own operator new/delete, which allocate on
 * the game heaps. /alternatename makes them defaults only: a module that defines
 * the MWCC names itself keeps its own.
 */
#ifndef TARGET_DOL
#include <stddef.h>

extern "C" void *PortRelNew(size_t size) { return ::operator new(size); }
extern "C" void *PortRelNewArray(size_t size) { return ::operator new[](size); }
extern "C" void PortRelDelete(void *ptr) { ::operator delete(ptr); }
extern "C" void PortRelDeleteArray(void *ptr) { ::operator delete[](ptr); }

#pragma comment(linker, "/alternatename:__nw__FUl=PortRelNew")
#pragma comment(linker, "/alternatename:__nwa__FUl=PortRelNewArray")
#pragma comment(linker, "/alternatename:__dl__FPv=PortRelDelete")
#pragma comment(linker, "/alternatename:__dla__FPv=PortRelDeleteArray")
#endif
