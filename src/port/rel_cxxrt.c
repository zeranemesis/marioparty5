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
