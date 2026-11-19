#pragma once

// Netplay (src/port/netplay.cpp): every machine runs the whole game, the host relays the pads frame by frame.

#include <dolphin/pad.h>

#ifdef __cplusplus
#include <string>

// Lobby, before the game boots (src/port/ui/netplay.cpp drives it; MP5_NET_* do the same from the environment).
enum class NetLobby { Off, Hosting, Joined, Started, Failed };
bool Mp5Net_Host(u16 port, int delay);
bool Mp5Net_Join(const std::string& address); // "ip[:port]"
NetLobby Mp5Net_Poll();                       // call every UI frame while hosting or joined
bool Mp5Net_Start();                          // host: everyone boots the game now
void Mp5Net_Leave();
const std::string& Mp5Net_Status();
int Mp5Net_Players(); // host: players who joined
std::string Mp5Net_FindLan(); // "ip:port" of a host on the local network, or ""
// Right before the game boots: turns a started lobby (or MP5_NET_HOST / MP5_NET_JOIN) into the session.
// Returns false when the session failed and the game should not start.
bool Mp5Net_Boot();
void Mp5Net_SetConfigPath(const char* utf8Path); // where the cards and desync dumps are
extern "C" {
#endif

int Mp5Net_Active(void);
// From the pad VSync callback: replaces the 4 local pads by the pads every machine agreed on for this frame.
void Mp5Net_PadHook(PADStatus* status);
// In a session, msmSysInit calls this instead of starting MusyX's audio thread.
void Mp5Net_AudioStart(void);
// In a session: the host's window size, frozen at the start (widescreen.cpp); 0 outside one.
int Mp5Net_SessionAspect(u32* width, u32* height);
// The game's OSGetTick / OSGetTime (renamed by CMake): the real clock, or the session's in netplay.

#ifdef __cplusplus
}
#endif
