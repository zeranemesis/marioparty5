/*
 * REL modules as host shared libraries.
 *
 * On GameCube, omDLLLink reads dll/<name>.rel from the disc and relocates it
 * with OSLink. On PC every decompiled module is built as its own shared
 * library (<name>.dll, <name>.so or <name>.dylib, lower-case, next to the
 * executable), which keeps the modules' identical symbol names apart exactly
 * like separate RELs, and gives them fresh data each time they are loaded.
 */

#include "port/rel_loader.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "dolphin/os.h"

#if defined(_WIN32)
static const char kSuffix[] = ".dll";
#elif defined(__APPLE__)
static const char kSuffix[] = ".dylib";
#else
static const char kSuffix[] = ".so";
#endif

/* "dll/bootdll.rel" -> "bootdll" */
static void module_base_name(const char *discPath, char *out, size_t outSize)
{
    const char *start = strrchr(discPath, '/');
    const char *end;
    size_t len;
    size_t i;

    start = start ? start + 1 : discPath;
    end = strrchr(start, '.');
    len = end ? (size_t)(end - start) : strlen(start);
    if (len >= outSize) {
        len = outSize - 1;
    }
    for (i = 0; i < len; i++) {
        out[i] = (char)tolower((unsigned char)start[i]);
    }
    out[len] = '\0';
}

void *PortRelOpen(const char *discPath)
{
    char base[64];
    char fileName[96];
    void *handle;

    module_base_name(discPath, base, sizeof(base));
    snprintf(fileName, sizeof(fileName), "%s%s", base, kSuffix);
#ifdef _WIN32
    handle = LoadLibraryA(fileName);
    if (handle == NULL) {
        OSReport("rel_loader> %s is not available (error %lu): module not decompiled yet?\n", fileName,
            (unsigned long)GetLastError());
    }
#else
    handle = dlopen(fileName, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        /* desktop builds keep modules beside the executable */
        char path[1024];
        Dl_info info;
        if (dladdr((void *)&PortRelOpen, &info) && info.dli_fname) {
            const char *slash = strrchr(info.dli_fname, '/');
            if (slash) {
                snprintf(path, sizeof(path), "%.*s/%s", (int)(slash - info.dli_fname), info.dli_fname, fileName);
                handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
            }
        }
    }
    if (handle == NULL) {
        OSReport("rel_loader> %s is not available (%s): module not decompiled yet?\n", fileName, dlerror());
    }
#endif
    return handle;
}

PortRelFunc PortRelSymbol(void *handle, const char *name)
{
    if (handle == NULL) {
        return NULL;
    }
#ifdef _WIN32
    return (PortRelFunc)GetProcAddress((HMODULE)handle, name);
#else
    return (PortRelFunc)dlsym(handle, name);
#endif
}

void PortRelClose(void *handle)
{
    if (handle == NULL) {
        return;
    }
#ifdef _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}
