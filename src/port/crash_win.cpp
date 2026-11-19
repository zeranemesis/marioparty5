// Last-chance crash report: on an unhandled exception, print the exception and a module+offset backtrace
// to stdout, so intermittent crashes (that never reproduce under a debugger) can be resolved with addr2line.py.
#ifdef _WIN32
#include <csignal>
#include <cstdlib>
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

// Some deaths never reach the unhandled-exception filter: a stack overflow on a libco coroutine (no stack left for
// the filter), heap corruption, abort() and the CRT's invalid-parameter handler. Report those too.
static LONG WINAPI fatalVectored(EXCEPTION_POINTERS* ep) {
    const DWORD code = ep->ExceptionRecord->ExceptionCode;
    if (code == EXCEPTION_STACK_OVERFLOW || code == 0xC0000374 /* heap corruption */) {
        crashFilter(ep);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static void reportHere(const char* what) {
    printf("CRASH: %s\n", what);
    CONTEXT ctx;
    RtlCaptureContext(&ctx);
    EXCEPTION_RECORD rec{};
    rec.ExceptionAddress = (void*)ctx.Rip;
    EXCEPTION_POINTERS ep{&rec, &ctx};
    crashFilter(&ep);
}

extern "C" void Mp5InstallCrashReport(void) {
    SetUnhandledExceptionFilter(crashFilter);
    AddVectoredExceptionHandler(1, fatalVectored);
    signal(SIGABRT, [](int) { reportHere("abort()"); });
    _set_invalid_parameter_handler([](const wchar_t*, const wchar_t*, const wchar_t*, unsigned, uintptr_t) {
        reportHere("CRT invalid parameter");
    });
}
#else
extern "C" void Mp5InstallCrashReport(void) {}
#endif
