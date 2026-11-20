// Netplay rollback: which pages of the game's memory changed, and what they held before (see rollback_pages.hpp).
//
// The tracked memory is write-protected. The first write to a page in an interval faults; the handler copies the page
// as it was into the interval's undo log, makes it writable, and the write goes on. Marking a new interval protects
// again the pages written in the last one. Restoring the state at the start of an interval copies the logs back,
// newest first. A tick costs one fault and one page copy per page it writes (a few hundred), instead of a copy of the
// whole state (hundreds of MB).
#include "rollback_pages.hpp"

#include <algorithm>
#include <cstring>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif
#endif

namespace mp5rb {
namespace {

struct Range {
    u8* base;    // page aligned
    size_t size; // whole pages
    std::vector<u8> writable; // per page: written (and unprotected) in the current interval
};

struct Entry {
    u8* page;
    u32 slot; // in sPool
};

struct Interval {
    u32 label;
    size_t first; // index of its first entry
};

constexpr size_t kMaxPages = 32768; // undo log capacity: 128 MB of 4 KB pages (a board tick can write 500+ pages)

size_t sPage;
std::vector<Range> sRanges;
std::vector<u8> sPool;          // kMaxPages pages
std::vector<u32> sFreeSlots;    // reserved to kMaxPages: never grows in the handler
std::vector<Entry> sEntries;    // reserved to kMaxPages
std::vector<u8*> sDirty;        // pages writable now (reserved)
std::vector<Interval> sIntervals;
bool sActive, sRestoring;
u32 sOverflowAt = 0xFFFFFFFF; // label of the interval whose log is incomplete: it and earlier ones cannot be restored
u64 sFaults;

void protect(u8* p, size_t n, bool writable) {
#ifdef _WIN32
    DWORD old;
    VirtualProtect(p, n, writable ? PAGE_READWRITE : PAGE_READONLY, &old);
#else
    mprotect(p, n, writable ? PROT_READ | PROT_WRITE : PROT_READ);
#endif
}

// an address without its top byte: Android tags heap pointers there (0xb4...), and a fault reports the address
// untagged (on x86-64 user addresses have it clear anyway)
uintptr_t untag(const void* p) { return (uintptr_t)p & 0x00FFFFFFFFFFFFFFull; }

// the page holding addr, if it is tracked
bool locate(const void* addr, Range** range, size_t* index) {
    const uintptr_t a = untag(addr);
    for (Range& r : sRanges) {
        const uintptr_t base = untag(r.base);
        if (a >= base && a < base + r.size) {
            *range = &r;
            *index = (size_t)(a - base) / sPage;
            return true;
        }
    }
    return false;
}

// a write to a protected page: log it as it was and let the write through; false if the fault is not ours
bool onWrite(const void* addr) {
    Range* r;
    size_t i;
    if (!sActive || !locate(addr, &r, &i) || r->writable[i]) {
        return false;
    }
    u8* page = r->base + i * sPage;
    if (!sRestoring) {
        if (sFreeSlots.empty() || sEntries.size() == sEntries.capacity()) {
            sOverflowAt = sIntervals.empty() ? 0 : sIntervals.back().label; // no restore can reach back past this point
        } else {
            const u32 slot = sFreeSlots.back();
            sFreeSlots.pop_back();
            memcpy(sPool.data() + (size_t)slot * sPage, page, sPage);
            sEntries.push_back({page, slot});
        }
    }
    r->writable[i] = 1;
    sDirty.push_back(page);
    protect(page, sPage, true);
    sFaults++;
    return true;
}

#ifdef _WIN32
PVOID sHandler;
LONG CALLBACK vectored(EXCEPTION_POINTERS* e) {
    const EXCEPTION_RECORD* x = e->ExceptionRecord;
    if (x->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && x->NumberParameters >= 2 && x->ExceptionInformation[0] == 1 &&
        onWrite((const void*)x->ExceptionInformation[1])) {
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
void install() {
    if (sHandler == nullptr) {
        sHandler = AddVectoredExceptionHandler(1, vectored); // before the crash reporter's
    }
}
#else
struct sigaction sPrev;
bool sInstalled;
void onSegv(int sig, siginfo_t* info, void* ctx) {
    if (onWrite(info->si_addr)) {
        return;
    }
#ifdef __ANDROID__
    { // a fault that is not a first write to a tracked page: say what the tracker knew, for the crash report
        Range* r = nullptr;
        size_t i = 0;
        const bool tracked = locate(info->si_addr, &r, &i);
        __android_log_print(ANDROID_LOG_ERROR, "MP5", "rollback pages: fault at %p, active %d, tracked %d, page writable %d, %zu ranges",
                            info->si_addr, (int)sActive, (int)tracked, tracked ? (int)r->writable[i] : -1, sRanges.size());
    }
#endif
    if (sPrev.sa_flags & SA_SIGINFO) {
        if (sPrev.sa_sigaction != nullptr) {
            sPrev.sa_sigaction(sig, info, ctx);
            return;
        }
    } else if (sPrev.sa_handler != SIG_DFL && sPrev.sa_handler != SIG_IGN) {
        sPrev.sa_handler(sig);
        return;
    }
    signal(SIGSEGV, SIG_DFL); // nothing else wants it: crash as usual
}
void install() {
    if (!sInstalled) {
        struct sigaction sa {};
        sa.sa_sigaction = onSegv;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGSEGV, &sa, &sPrev);
        sInstalled = true;
    }
}
#endif

void reprotectDirty() {
    for (u8* page : sDirty) {
        Range* r;
        size_t i;
        if (locate(page, &r, &i)) {
            r->writable[i] = 0;
            protect(page, sPage, false);
        }
    }
    sDirty.clear();
}

void releaseEntries(size_t from, size_t to) {
    for (size_t k = from; k < to; k++) {
        sFreeSlots.push_back(sEntries[k].slot);
    }
}

} // namespace

size_t pageSize() {
    if (sPage == 0) {
#ifdef _WIN32
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        sPage = si.dwPageSize;
#else
        sPage = (size_t)sysconf(_SC_PAGESIZE);
#endif
    }
    return sPage;
}

void track(const std::vector<std::pair<u8*, size_t>>& ranges) {
    stop();
    pageSize();
    install();
    if (sPool.empty()) {
        sPool.resize(kMaxPages * sPage);
        sFreeSlots.reserve(kMaxPages);
        sEntries.reserve(kMaxPages);
        sDirty.reserve(kMaxPages * 4);
    }
    sFreeSlots.clear();
    for (u32 s = 0; s < kMaxPages; s++) {
        sFreeSlots.push_back(kMaxPages - 1 - s);
    }
    for (const auto& [p, n] : ranges) {
        u8* lo = (u8*)(((uintptr_t)p + sPage - 1) & ~(uintptr_t)(sPage - 1));
        u8* hi = (u8*)(((uintptr_t)p + n) & ~(uintptr_t)(sPage - 1));
        if (hi > lo) {
            sRanges.push_back({lo, (size_t)(hi - lo), std::vector<u8>((hi - lo) / sPage, 0)});
        }
    }
    sOverflowAt = 0xFFFFFFFF;
    sActive = true;
    for (Range& r : sRanges) {
        protect(r.base, r.size, false);
    }
}

void stop() {
    sActive = false;
    for (Range& r : sRanges) {
        protect(r.base, r.size, true);
    }
    sRanges.clear();
    sEntries.clear();
    sIntervals.clear();
    sDirty.clear();
}

bool active() { return sActive; }

void mark(u32 label) {
    if (!sActive) {
        return;
    }
    reprotectDirty();
    if (!sIntervals.empty() && sIntervals.back().label == label && sIntervals.back().first == sEntries.size()) {
        return; // just restored to this point: the interval is already open
    }
    sIntervals.push_back({label, sEntries.size()});
}

bool restore(u32 label) {
    auto it = std::find_if(sIntervals.begin(), sIntervals.end(), [&](const Interval& v) { return v.label == label; });
    if (!sActive || it == sIntervals.end() || (sOverflowAt != 0xFFFFFFFF && label <= sOverflowAt)) {
        return false;
    }
    const size_t first = it->first;
    sRestoring = true;
    for (size_t k = sEntries.size(); k-- > first;) {
        const Entry& e = sEntries[k];
        u8* page = e.page;
        *(volatile u8*)page = *(volatile u8*)page; // a protected page becomes writable through onWrite (no log while restoring)
        memcpy(page, sPool.data() + (size_t)e.slot * sPage, sPage);
    }
    sRestoring = false;
    releaseEntries(first, sEntries.size());
    sEntries.resize(first);
    sIntervals.erase(it + 1, sIntervals.end());
    reprotectDirty(); // the interval starts again from here
    return true;
}

void forget(u32 label) {
    auto it = std::find_if(sIntervals.begin(), sIntervals.end(), [&](const Interval& v) { return v.label >= label; });
    if (it == sIntervals.begin()) {
        return;
    }
    const size_t keep = it == sIntervals.end() ? sEntries.size() : it->first;
    releaseEntries(0, keep);
    sEntries.erase(sEntries.begin(), sEntries.begin() + (ptrdiff_t)keep);
    sIntervals.erase(sIntervals.begin(), it);
    for (Interval& v : sIntervals) {
        v.first -= keep;
    }
    if (sOverflowAt != 0xFFFFFFFF && (sIntervals.empty() || sIntervals.front().label > sOverflowAt)) {
        sOverflowAt = 0xFFFFFFFF; // the incomplete interval is gone
    }
}

u64 faults() { return sFaults; }
bool overflowed() { return sOverflowAt != 0xFFFFFFFF; }
size_t logged() { return sEntries.size(); }

} // namespace mp5rb
