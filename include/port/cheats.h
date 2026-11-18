#ifndef MP5_NATIVE_CHEATS_H
#define MP5_NATIVE_CHEATS_H

#ifdef __cplusplus
#include <filesystem>
#include <string>

namespace mp5native::cheats {
std::filesystem::path file_path();
void reload();
const std::string &status();
} // namespace mp5native::cheats

extern "C" {
#endif

/* once per game tick (main.c): the built-in cheats and the cheat code file */
void Mp5Native_CheatsTick(void);
int mp5native_settings_unlock_bowsers_gnarly_party_c(void);

#ifdef __cplusplus
}
#endif

#endif
