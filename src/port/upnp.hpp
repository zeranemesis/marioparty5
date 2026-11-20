// UPnP port mapping for hosting online play (upnp.cpp).
#pragma once
#include <dolphin/types.h>

#include <string>

namespace mp5upnp {

// asks the router, in the background, to forward this TCP port to this machine and to tell the public address
void open(u16 port);
// removes the mapping (waits for open() to finish first)
void close();
// a line for the lobby: what happened, and the address friends join (empty: nothing asked)
std::string status();

} // namespace mp5upnp
