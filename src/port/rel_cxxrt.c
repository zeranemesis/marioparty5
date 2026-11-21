/*
 * C++ runtime pieces MWCC emits calls to from a REL's static initialisers.
 * Every REL module gets its own copy (see CMakeLists.txt), like rel_ctors.c.
 *
 * __register_global_object chains a global object's destructor into regmem, a
 * buffer the module sizes for three 32-bit words: on 64-bit it would overflow.
 * The port never runs those destructors anyway (a module is unloaded by freeing
 * its memory, as on GameCube where __destroy_global_chain runs only at exit), so
 * registering is a no-op.
 * shortcut: global destructors of REL modules never run, as on hardware.
 */
#ifndef TARGET_DOL
void *__register_global_object(void *object, void *destructor, void *regmem)
{
    (void)destructor;
    (void)regmem;
    return object;
}
#endif

/* MWCC pointer-to-member runtime, for the layout the decomp declares
 * ({this delta, vtable offset, function}). __ptmf_scall cannot be generic (MWCC
 * passes the pmf in r12), so callers do the call themselves under TARGET_PC.
 * shortcut: test looks at the function only (the decomp's null pmf is {0, -1, 0}),
 * virtual pmfs are not supported. */
#ifndef TARGET_DOL
typedef struct PortPtmf {
    /* 32-bit, as MWCC lays them out and as the decomp's structs declare them: `long` is 8 bytes on Android
       (LP64) and moved func to offset 16, so __ptmf_test read the next field and every pmf looked null there
       (m568's sleds never started) */
    int thisDelta;
    int vtblOffset;
    void *func;
} PortPtmf;

const PortPtmf __ptmf_null = { 0, 0, 0 };

int __ptmf_test(const PortPtmf *pmf)
{
    return pmf->func != 0;
}
#endif

/* MSL's float constants, read by the sqrt that the REL modules inline (in the DOL on GameCube). */
#ifndef TARGET_DOL
int __float_nan[] = { 0x7FFFFFFF };
int __float_huge[] = { 0x7F800000 };
#endif

/* MSL's array construction helpers (Runtime NMWException.cp): constructors get 1, destructors -1,
 * destroyed back to front. */
#ifndef TARGET_DOL
typedef void *(*PortCtorDtor)(void *obj, short flag);

void *__construct_array(void *block, PortCtorDtor ctor, PortCtorDtor dtor, unsigned long size, unsigned long n)
{
    char *p = (char *)block;
    unsigned long i;
    (void)dtor;
    for (i = 0; i < n; i++, p += size) {
        ctor(p, 1);
    }
    return block;
}

void __destroy_arr(void *block, PortCtorDtor dtor, unsigned long size, unsigned long n)
{
    char *p = (char *)block + size * n;
    for (; n > 0; n--) {
        p -= size;
        dtor(p, -1);
    }
}
#endif

/* Hook data of a 3D model, for REL units that declare Hu3DData with the GameCube stride. */
#ifndef TARGET_DOL
#include "game/hu3d.h"
void *PortHu3DModelHookData(int modelId)
{
    return Hu3DData[modelId].hookData;
}

void PortHu3DModelHookDataSet(int modelId, void *data)
{
    Hu3DData[modelId].hookData = data;
}

void *PortHu3DModelMtx(int modelId)
{
    return Hu3DData[modelId].mtx;
}
#endif
