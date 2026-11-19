/*
 * Netplay deep check (MP5_NET_DEEP=1, src/port/netplay.cpp): the game state a CRC of GwCommon/GwSystem/GwPlayer
 * misses, laid out the same on every platform (no pointers, no padding): every live 3D model's position,
 * rotation, scale and motion time, plus the camera. Compared every frame, it shows the first frame and the first
 * model where two machines part, which a desync of the RNG alone does not.
 */
#include "game/hu3d.h"

/* one record: model index, attr, then 13 floats */
typedef struct {
    s32 index;
    u32 attr;
    float v[13];
} PROBEREC;

/* fills out with up to max records; returns how many */
int Mp5Net_ProbeModels(void *out, int max)
{
    PROBEREC *rec = (PROBEREC *)out;
    int n = 0;
    int i;
    for (i = 0; i < HU3D_MODEL_MAX && n < max; i++) {
        HU3DMODEL *m = &Hu3DData[i];
        if (m->hsf == NULL) {
            continue;
        }
        rec[n].index = i;
        rec[n].attr = m->attr;
        rec[n].v[0] = m->pos.x;
        rec[n].v[1] = m->pos.y;
        rec[n].v[2] = m->pos.z;
        rec[n].v[3] = m->rot.x;
        rec[n].v[4] = m->rot.y;
        rec[n].v[5] = m->rot.z;
        rec[n].v[6] = m->scale.x;
        rec[n].v[7] = m->scale.y;
        rec[n].v[8] = m->scale.z;
        rec[n].v[9] = m->motWork.time;
        rec[n].v[10] = m->motWork.speed;
        rec[n].v[11] = m->motOvlWork.time;
        rec[n].v[12] = m->motShiftWork.time;
        n++;
    }
    return n;
}

int Mp5Net_ProbeRecordSize(void)
{
    return (int)sizeof(PROBEREC);
}

/* For a desync report: "module+offset" of a hook model's draw function (symbolize with addr2line / the PDB). */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define _GNU_SOURCE
#include <dlfcn.h>
#endif
#include <stdio.h>
#include <string.h>

void Mp5Net_AddrName(const void *fn, char *buf, int size)
{
    buf[0] = 0;
#ifdef _WIN32
    {
        HMODULE mod = NULL;
        char name[MAX_PATH] = "?";
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (LPCSTR)fn, &mod)) {
            GetModuleFileNameA(mod, name, sizeof(name));
        }
        snprintf(buf, size, "%s+0x%llx", strrchr(name, '\\') ? strrchr(name, '\\') + 1 : name,
                 (unsigned long long)((const char *)fn - (const char *)mod));
    }
#else
    {
        Dl_info info;
        if (dladdr(fn, &info) && info.dli_fname != NULL) {
            const char *base = strrchr(info.dli_fname, '/');
            snprintf(buf, size, "%s+0x%llx %s", base ? base + 1 : info.dli_fname,
                     (unsigned long long)((const char *)fn - (const char *)info.dli_fbase),
                     info.dli_sname ? info.dli_sname : "");
        }
    }
#endif
}

void Mp5Net_ProbeHookName(int index, char *buf, int size)
{
    buf[0] = 0;
    if ((Hu3DData[index].attr & HU3D_ATTR_HOOKFUNC) && Hu3DData[index].hookFunc != NULL) {
        Mp5Net_AddrName((const void *)Hu3DData[index].hookFunc, buf, size);
    }
}
