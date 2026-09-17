#ifndef PORT_REL_LOADER_H_
#define PORT_REL_LOADER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*PortRelFunc)(void);

/* Loads the shared library built for a REL, from its disc path ("dll/bootdll.rel"). */
void *PortRelOpen(const char *discPath);
/* Exported function of a loaded module, NULL if absent. */
PortRelFunc PortRelSymbol(void *handle, const char *name);
void PortRelClose(void *handle);

#ifdef __cplusplus
}
#endif

#endif
