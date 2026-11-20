// Linked into dol and every REL module: tells rollback.cpp where this module keeps the game's variables (the sections
// include/port/gamestate.h puts them in), for as long as the module is loaded.
#include <stddef.h>
#include <string.h>

extern "C" void Mp5Rb_Sections(const void* key, void* d0, void* d1, void* b0, void* b1);
extern "C" void Mp5Rb_Forget(const void* key);

#ifdef _WIN32
#include <windows.h>
extern "C" IMAGE_DOS_HEADER __ImageBase;

static void range(const char* name, void** lo, void** hi) {
    auto* base = (unsigned char*)&__ImageBase;
    auto* nt = (IMAGE_NT_HEADERS*)(base + __ImageBase.e_lfanew);
    IMAGE_SECTION_HEADER* s = IMAGE_FIRST_SECTION(nt);
    *lo = *hi = nullptr;
    for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; i++, s++) {
        if (strncmp((const char*)s->Name, name, IMAGE_SIZEOF_SHORT_NAME) == 0) {
            *lo = base + s->VirtualAddress;
            *hi = base + s->VirtualAddress + s->Misc.VirtualSize;
        }
    }
}
#else
// lld defines __start_/__stop_ for sections named like C identifiers; weak: a module may have none
extern "C" char __start_mp5gd[] __attribute__((weak, visibility("hidden")));
extern "C" char __stop_mp5gd[] __attribute__((weak, visibility("hidden")));
extern "C" char __start_mp5gb[] __attribute__((weak, visibility("hidden")));
extern "C" char __stop_mp5gb[] __attribute__((weak, visibility("hidden")));

static void range(const char* name, void** lo, void** hi) {
    const bool data = strcmp(name, "mp5gd") == 0;
    *lo = data ? __start_mp5gd : __start_mp5gb;
    *hi = data ? __stop_mp5gd : __stop_mp5gb;
}
#endif

namespace {
struct Register {
    Register() {
        void *d0, *d1, *b0, *b1;
        range("mp5gd", &d0, &d1);
        range("mp5gb", &b0, &b1);
        Mp5Rb_Sections(this, d0, d1, b0, b1);
    }
    ~Register() { Mp5Rb_Forget(this); }
} sRegister;
} // namespace
