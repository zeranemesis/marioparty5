/*
 * _ctors and _dtors, one per REL module.
 *
 * On GameCube these are linker-generated tables of static C++ constructor
 * and destructor pointers, terminated by a NULL entry, that a module's
 * _prolog/_epilog walk before and after ObjectSetup runs. The game's REL
 * modules are C, so the real tables only ever hold the terminator; this file
 * gives every REL module the same empty tables directly, since the host
 * linker does not generate them.
 *
 * Compiled once into each REL target (see the REL loop in CMakeLists.txt),
 * never into dol: main.dol has its own copy, built the same way by the
 * matching compiler, and does not need one on the host.
 */

typedef void (*VoidFunc)(void);

const VoidFunc _ctors[] = { (VoidFunc)0 };
const VoidFunc _dtors[] = { (VoidFunc)0 };
