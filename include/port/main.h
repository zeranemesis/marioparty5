#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

extern bool Mp5Native_IsRunning;
extern bool Mp5Native_IsShuttingDown;
extern bool Mp5Native_IsGameLaunched;
extern bool Mp5Native_RestartRequested;

#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || \
  (defined(TARGET_OS_TV) && TARGET_OS_TV)
#define SUPPORTS_PROCESS_RESTART FALSE
#else
#define SUPPORTS_PROCESS_RESTART TRUE
#endif

void Mp5Native_RequestRestart(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" std::filesystem::path Mp5Native_ConfigPath;
#endif