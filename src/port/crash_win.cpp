// Last-chance crash report: on an unhandled exception, print the exception and a module+offset backtrace
// to stdout, so intermittent crashes (that never reproduce under a debugger) can be resolved with addr2line.py.
#ifdef _WIN32
#include <windows.h>
#include <cstdio>

static void printAddr(void* addr) {
    HMODULE mod = nullptr;
    char name[MAX_PATH] = "?";
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           (LPCSTR)addr, &mod)) {
        GetModuleFileNameA(mod, name, sizeof(name));
    }
    const char* base = strrchr(name, '\\');
    printf("CRASH:   %s+0x%llx\n", base ? base + 1 : name,
           (unsigned long long)((char*)addr - (char*)mod));
}

static LONG WINAPI crashFilter(EXCEPTION_POINTERS* ep) {
    printf("CRASH: exception 0x%08lx at\n", ep->ExceptionRecord->ExceptionCode);
    printAddr(ep->ExceptionRecord->ExceptionAddress);
    // walk the faulting thread's stack from the exception context
    CONTEXT ctx = *ep->ContextRecord;
    for (int i = 0; i < 24; i++) {
        DWORD64 imageBase = 0;
        PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(ctx.Rip, &imageBase, nullptr);
        if (!fn) {
            break;
        }
        void* handlerData = nullptr;
        DWORD64 frame = 0;
        RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, ctx.Rip, fn, &ctx, &handlerData, &frame, nullptr);
        if (!ctx.Rip) {
            break;
        }
        printAddr((void*)ctx.Rip);
    }
    fflush(stdout);
    return EXCEPTION_CONTINUE_SEARCH;
}

extern "C" void Mp5InstallCrashReport(void) { SetUnhandledExceptionFilter(crashFilter); }
#else
extern "C" void Mp5InstallCrashReport(void) {}
#endif
