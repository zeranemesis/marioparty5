// test bench: MP5_TEST_PROF=<seconds> samples every thread of the process each ~1 ms (suspend, copy the stack,
// resume, unwind the copy) and prints "PROF:" lines every <seconds>: per busy thread, the top functions by self
// and by inclusive samples, symbols from each module's PDB. Windows only; unset, nothing runs.
// MP5_TEST_PROF_HITCH=<ms> samples the main thread only and prints "HITCH:" lines, the call stacks seen during
// each frame longer than <ms> (frame_interp.cpp reports them).
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#include <tlhelp32.h>
#include <winternl.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "winmm.lib")

namespace {

constexpr int kMaxDepth = 48;
constexpr size_t kStackCopy = 256 * 1024;

struct Loc {
    int module; // index into sModules, -1 unknown
    DWORD rva;
    bool operator==(const Loc &o) const { return module == o.module && rva == o.rva; }
};
struct LocHash {
    size_t operator()(const Loc &l) const { return (size_t)l.module * 0x9E3779B97F4A7C15ull ^ l.rva; }
};

struct ThreadStat {
    std::string name;
    unsigned samples = 0;
    std::unordered_map<Loc, unsigned, LocHash> self, incl;
    std::map<std::string, unsigned> stacks; // leaf-first call chains (raw Locs), for the top stacks listing
};

std::vector<std::wstring> sModules;
std::map<DWORD, ThreadStat> sThreads;
std::unordered_map<HMODULE, int> sModCache; // emptied each thread scan: a REL dll may unload and another load there
DWORD sMainTid = 0;
std::mutex sLock; // sModules, the hitch ring; never held while a thread is suspended
double sHitchMs = 0.0;
constexpr int kHitchDepth = 12;
struct HitchSample {
    LONGLONG t;
    int n;
    Loc chain[kHitchDepth];
};
std::vector<HitchSample> sRing(8192);
size_t sRingPos = 0;

int module_of(DWORD64 addr, DWORD *rva)
{
    HMODULE mod = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)addr, &mod)) {
        return -1;
    }
    *rva = (DWORD)(addr - (DWORD64)mod);
    auto cached = sModCache.find(mod);
    if (cached != sModCache.end()) {
        return cached->second;
    }
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(mod, path, MAX_PATH);
    auto it = std::find(sModules.begin(), sModules.end(), path);
    if (it == sModules.end()) {
        it = sModules.insert(sModules.end(), path);
    }
    return sModCache[mod] = (int)(it - sModules.begin());
}

std::string symbol(const Loc &l)
{
    static std::unordered_map<int, DWORD64> bases;
    static bool init = false;
    HANDLE proc = GetCurrentProcess();
    if (!init) {
        init = true;
        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
        SymInitialize(proc, nullptr, FALSE);
    }
    if (l.module < 0) {
        return "?";
    }
    const std::wstring &path = sModules[l.module];
    auto it = bases.find(l.module);
    if (it == bases.end()) {
        const DWORD64 base = 0x100000000000ull + (DWORD64)l.module * 0x40000000ull;
        SymLoadModuleExW(proc, nullptr, path.c_str(), nullptr, base, 0x3FFFFFFF, nullptr, 0);
        it = bases.emplace(l.module, base).first;
    }
    char buf[sizeof(SYMBOL_INFO) + 256];
    auto *sym = (SYMBOL_INFO *)buf;
    sym->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym->MaxNameLen = 255;
    std::string modName(path.begin() + path.find_last_of(L"\\/") + 1, path.end());
    DWORD64 disp = 0;
    if (SymFromAddr(proc, it->second + l.rva, &disp, sym)) {
        return modName + "!" + sym->Name;
    }
    char tmp[32];
    std::snprintf(tmp, sizeof(tmp), "+0x%x", l.rva);
    return modName + tmp;
}

void dump(double seconds)
{
    std::printf("PROF: ---- %.0f s\n", seconds);
    for (auto &[tid, ts] : sThreads) {
        if (ts.samples < 50) {
            continue;
        }
        // threads parked in a wait most of the time are not interesting
        std::vector<std::pair<unsigned, Loc>> self, incl;
        for (auto &[l, n] : ts.self) {
            self.push_back({n, l});
        }
        for (auto &[l, n] : ts.incl) {
            incl.push_back({n, l});
        }
        std::sort(self.begin(), self.end(), [](auto &a, auto &b) { return a.first > b.first; });
        std::sort(incl.begin(), incl.end(), [](auto &a, auto &b) { return a.first > b.first; });
        unsigned waiting = 0;
        std::unordered_map<std::string, unsigned> selfSym;
        for (auto &[n, l] : self) {
            selfSym[symbol(l)] += n;
        }
        std::vector<std::pair<unsigned, std::string>> ss;
        for (auto &[s, n] : selfSym) {
            ss.push_back({n, s});
            if (s.find("ntdll.dll!Nt") != std::string::npos || s.find("ntdll.dll!Zw") != std::string::npos
                || s.find("win32u.dll!") != std::string::npos) {
                waiting += n;
            }
        }
        std::sort(ss.begin(), ss.end(), [](auto &a, auto &b) { return a.first > b.first; });
        std::printf("PROF: thread %lu%s '%s' samples %u busy %.1f%%\n", tid, tid == sMainTid ? " (main)" : "", ts.name.c_str(),
            ts.samples, 100.0 * (ts.samples - waiting) / ts.samples);
        if (tid != sMainTid && ts.samples - waiting < ts.samples / 20) {
            continue;
        }
        for (size_t i = 0; i < ss.size() && i < 30; i++) {
            std::printf("PROF:   self %5.1f%% %s\n", 100.0 * ss[i].first / ts.samples, ss[i].second.c_str());
        }
        std::unordered_map<std::string, unsigned> inclSym;
        for (auto &[n, l] : incl) {
            std::string s = symbol(l);
            inclSym[s] += n; // one symbol at several addresses of a stack: may overcount (recursion)
        }
        std::vector<std::pair<unsigned, std::string>> is;
        for (auto &[s, n] : inclSym) {
            is.push_back({n, s});
        }
        std::sort(is.begin(), is.end(), [](auto &a, auto &b) { return a.first > b.first; });
        std::vector<std::pair<unsigned, const std::string *>> st;
        for (auto &[k, n] : ts.stacks) {
            st.push_back({n, &k});
        }
        std::sort(st.begin(), st.end(), [](auto &a, auto &b) { return a.first > b.first; });
        for (size_t i = 0; i < st.size() && i < 25; i++) {
            std::printf("PROF:   stack %5.1f%%", 100.0 * st[i].first / ts.samples);
            const Loc *c = (const Loc *)st[i].second->data();
            for (size_t k = 0; k < st[i].second->size() / sizeof(Loc); k++) {
                std::printf(" < %s", symbol(c[k]).c_str());
            }
            std::printf("\n");
        }
        for (size_t i = 0; i < is.size() && i < 60; i++) {
            std::printf("PROF:   incl %5.1f%% %s\n", 100.0 * is[i].first / ts.samples, is[i].second.c_str());
        }
    }
    std::fflush(stdout);
}

void sampler(double period)
{
    timeBeginPeriod(1);
    const DWORD self = GetCurrentThreadId();
    const DWORD pid = GetCurrentProcessId();
    std::map<DWORD, HANDLE> handles;
    std::vector<unsigned char> stack(kStackCopy);
    ULONGLONG lastEnum = 0, start = GetTickCount64(), lastDump = start;
    for (;;) {
        const ULONGLONG now = GetTickCount64();
        if (now - lastEnum > 500) {
            lastEnum = now;
            sModCache.clear();
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            THREADENTRY32 te{sizeof(te)};
            std::unordered_set<DWORD> alive;
            for (BOOL ok = Thread32First(snap, &te); ok; ok = Thread32Next(snap, &te)) {
                if (te.th32OwnerProcessID != pid || te.th32ThreadID == self) {
                    continue;
                }
                alive.insert(te.th32ThreadID);
                if (!handles.count(te.th32ThreadID)) {
                    HANDLE h = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
                    if (h) {
                        handles[te.th32ThreadID] = h;
                        PWSTR desc = nullptr;
                        std::string name;
                        if (SUCCEEDED(GetThreadDescription(h, &desc)) && desc) {
                            for (PWSTR p = desc; *p; p++) {
                                name += (char)*p;
                            }
                            LocalFree(desc);
                        }
                        sThreads[te.th32ThreadID].name = name;
                    }
                }
            }
            CloseHandle(snap);
            for (auto it = handles.begin(); it != handles.end();) {
                if (!alive.count(it->first)) {
                    CloseHandle(it->second);
                    it = handles.erase(it);
                } else {
                    ++it;
                }
            }
        }
        for (auto &[tid, h] : handles) {
            if (sHitchMs > 0.0 && tid != sMainTid) {
                continue;
            }
            if (SuspendThread(h) == (DWORD)-1) {
                continue;
            }
            CONTEXT ctx{};
            ctx.ContextFlags = CONTEXT_FULL;
            size_t copied = 0;
            NT_TIB *tib = nullptr;
            if (GetThreadContext(h, &ctx)) {
                struct {
                    NTSTATUS ExitStatus;
                    PVOID TebBaseAddress;
                    CLIENT_ID ClientId;
                    ULONG_PTR AffinityMask;
                    LONG Priority;
                    LONG BasePriority;
                } tbi{};
                if (NtQueryInformationThread(h, (THREADINFOCLASS)0, &tbi, sizeof(tbi), nullptr) == 0) {
                    tib = (NT_TIB *)tbi.TebBaseAddress;
                    DWORD64 top = (DWORD64)tib->StackBase;
                    if (ctx.Rsp >= top || ctx.Rsp < (DWORD64)tib->StackLimit) {
                        // a libco coroutine (a game thread) runs on its own stack: copy to the end of its region
                        MEMORY_BASIC_INFORMATION mbi{};
                        top = VirtualQuery((void *)ctx.Rsp, &mbi, sizeof(mbi)) ? (DWORD64)mbi.BaseAddress + mbi.RegionSize : 0;
                    }
                    if (ctx.Rsp < top) {
                        copied = (size_t)std::min<DWORD64>(top - ctx.Rsp, kStackCopy);
                        std::memcpy(stack.data(), (void *)ctx.Rsp, copied);
                    }
                }
            }
            ResumeThread(h);
            LARGE_INTEGER sampleTime;
            QueryPerformanceCounter(&sampleTime);
            if (ctx.Rip == 0) {
                continue;
            }
            std::lock_guard lock(sLock);
            ThreadStat &ts = sThreads[tid];
            ts.samples++;
            // unwind on the copy: rsp (and a frame register inside the copied range) relocated into it
            const DWORD64 lo = ctx.Rsp, hi = ctx.Rsp + copied, delta = (DWORD64)stack.data() - lo;
            auto reloc = [&](DWORD64 &r) {
                if (r >= lo && r < hi) {
                    r += delta;
                }
            };
            reloc(ctx.Rsp);
            reloc(ctx.Rbp);
            Loc seen[kMaxDepth];
            int depth = 0;
            Loc chain[kHitchDepth];
            int chainLen = 0;
            for (int d = 0; d < kMaxDepth && ctx.Rip; d++) {
                DWORD64 imageBase = 0;
                const DWORD64 pc = ctx.Rip - (d ? 1 : 0);
                PRUNTIME_FUNCTION fn = RtlLookupFunctionEntry(pc, &imageBase, nullptr);
                // keyed by function start, so a function counts once per stack
                Loc l{};
                l.module = module_of(fn ? imageBase + fn->BeginAddress : pc, &l.rva);
                if (d == 0) {
                    ts.self[l]++;
                }
                if (chainLen < kHitchDepth) {
                    chain[chainLen++] = l;
                }
                if (std::find(seen, seen + depth, l) == seen + depth) {
                    seen[depth++] = l;
                }
                if (!fn) {
                    // leaf: return address on top of the stack
                    if (ctx.Rsp < (DWORD64)stack.data() || ctx.Rsp + 8 > (DWORD64)stack.data() + copied) {
                        break;
                    }
                    ctx.Rip = *(DWORD64 *)ctx.Rsp;
                    ctx.Rsp += 8;
                } else {
                    void *handlerData = nullptr;
                    DWORD64 establisher = 0;
                    RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, ctx.Rip, fn, &ctx, &handlerData, &establisher, nullptr);
                }
                if (ctx.Rsp < (DWORD64)stack.data() || ctx.Rsp >= (DWORD64)stack.data() + copied) {
                    break;
                }
            }
            for (int i = 0; i < depth; i++) {
                ts.incl[seen[i]]++;
            }
            if (sHitchMs > 0.0) {
                HitchSample &hs = sRing[sRingPos++ % sRing.size()];
                hs.t = sampleTime.QuadPart;
                hs.n = chainLen;
                std::copy(chain, chain + chainLen, hs.chain);
            } else if (chainLen > 0) {
                ts.stacks[std::string((const char *)chain, std::min(chainLen, 8) * sizeof(Loc))]++;
            }
        }
        const ULONGLONG t = GetTickCount64();
        if (sHitchMs == 0.0 && (t - lastDump) / 1000.0 >= period) {
            std::lock_guard lock(sLock);
            lastDump = t;
            dump((t - start) / 1000.0);
        }
        Sleep(1);
    }
}

} // namespace

extern "C" void Mp5Test_ProfStart(void)
{
    static bool started = false;
    const char *env = std::getenv("MP5_TEST_PROF");
    const char *hitch = std::getenv("MP5_TEST_PROF_HITCH");
    if (started || ((!env || !*env) && (!hitch || !*hitch))) {
        return;
    }
    started = true;
    sMainTid = GetCurrentThreadId();
    sHitchMs = hitch && *hitch ? std::max(1.0, std::atof(hitch)) : 0.0;
    const double period = env && *env ? std::max(1.0, std::atof(env)) : 1e9;
    std::thread(sampler, period).detach();
}

// a frame from t0 to t1 (seconds of the performance counter) was long: print what the main thread did meanwhile
extern "C" void Mp5Test_ProfFrame(double t0, double t1)
{
    if (sHitchMs <= 0.0 || (t1 - t0) * 1e3 < sHitchMs) {
        return;
    }
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    const LONGLONG a = (LONGLONG)(t0 * freq.QuadPart), b = (LONGLONG)(t1 * freq.QuadPart);
    std::lock_guard lock(sLock);
    std::map<std::string, unsigned> stacks;
    unsigned total = 0;
    for (const HitchSample &hs : sRing) {
        if (hs.t >= a && hs.t <= b && hs.n > 0) {
            stacks[std::string((const char *)hs.chain, hs.n * sizeof(Loc))]++;
            total++;
        }
    }
    std::vector<std::pair<unsigned, const std::string *>> st;
    for (auto &[k, n] : stacks) {
        st.push_back({n, &k});
    }
    std::sort(st.begin(), st.end(), [](auto &x, auto &y) { return x.first > y.first; });
    std::printf("HITCH: frame %.1f ms, %u samples\n", (t1 - t0) * 1e3, total);
    for (size_t i = 0; i < st.size() && i < 6; i++) {
        std::printf("HITCH:   %3u", st[i].first);
        const Loc *c = (const Loc *)st[i].second->data();
        for (size_t k = 0; k < st[i].second->size() / sizeof(Loc); k++) {
            std::printf(" < %s", symbol(c[k]).c_str());
        }
        std::printf("\n");
    }
}
#else
extern "C" void Mp5Test_ProfStart(void) {}
extern "C" void Mp5Test_ProfFrame(double, double) {}
#endif
