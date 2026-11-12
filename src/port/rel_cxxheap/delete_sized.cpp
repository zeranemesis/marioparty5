/* Global operator new/delete of the C++ REL modules on the game heap (as their MWCC __nw__FUl/__dl__FPv).
 * One operator per file: the static library rel_cxxheap only supplies the ones a module does not define
 * itself (see CMakeLists.txt). Never part of dol. */
#ifndef TARGET_DOL
#include <stddef.h>
#include <new>
extern "C" {
#include "game/memory.h"
}
void operator delete(void *ptr, size_t) noexcept { ::operator delete(ptr); }
#endif
