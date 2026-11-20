// UPnP IGD port mapping for hosting online play (see upnp.hpp): the host's router forwards the session port to it,
// and tells its public address, so a friend joins over the Internet without opening a port by hand.
//
// The protocol in short: SSDP finds the router (an M-SEARCH to 239.255.255.250:1900, the answer's LOCATION is its
// description), the description names the WANIPConnection (or WANPPPConnection) service's control URL, and SOAP
// calls on it add or delete a mapping and read the external address.
#include "upnp.hpp"

#include <atomic>
#include <chrono>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>

#include <fmt/format.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
using sock_t = SOCKET;
static void closeSock(sock_t s) { closesocket(s); }
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
using sock_t = int;
static constexpr sock_t INVALID_SOCKET = -1;
static void closeSock(sock_t s) { close(s); }
#endif

namespace mp5upnp {
namespace {

std::mutex sLock;
std::string sStatus;       // for the lobby
std::string sControlUrl;   // the router's mapping service, once found
std::string sServiceType;
std::string sLocalIp;
u16 sMapped = 0;           // the port mapped on the router (0: none)
std::thread sThread;
std::atomic<bool> sBusy{false};

void setStatus(std::string s) {
    fmt::println("upnp: {}", s); // in the log too: what the router did is the first question when a friend cannot join
    std::lock_guard g(sLock);
    sStatus = std::move(s);
}

void setTimeout(sock_t s, int ms) {
#ifdef _WIN32
    DWORD t = (DWORD)ms;
#else
    timeval t{ms / 1000, (ms % 1000) * 1000};
#endif
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&t, sizeof(t));
    setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&t, sizeof(t));
}

// "http://host:port/path" -> its parts
bool splitUrl(const std::string& url, std::string& host, u16& port, std::string& path) {
    const std::string pre = "http://";
    if (url.compare(0, pre.size(), pre) != 0) {
        return false;
    }
    const size_t start = pre.size();
    const size_t slash = url.find('/', start);
    const std::string hostPort = url.substr(start, slash == std::string::npos ? std::string::npos : slash - start);
    path = slash == std::string::npos ? "/" : url.substr(slash);
    const size_t colon = hostPort.find(':');
    host = hostPort.substr(0, colon);
    port = colon == std::string::npos ? 80 : (u16)atoi(hostPort.c_str() + colon + 1);
    return !host.empty() && port != 0;
}

// one HTTP request to the router; the body of the answer (empty on failure), and the address this machine reached it
// from (the address the router must forward to)
std::string http(const std::string& url, const std::string& method, const std::string& headers, const std::string& body,
                 int* status = nullptr) {
    std::string host, path;
    u16 port;
    if (!splitUrl(url, host, port, path)) {
        return {};
    }
    sock_t s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        return {};
    }
    setTimeout(s, 3000);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1 || connect(s, (sockaddr*)&addr, sizeof(addr)) != 0) {
        closeSock(s);
        return {};
    }
    sockaddr_in local{};
    socklen_t len = sizeof(local);
    if (getsockname(s, (sockaddr*)&local, &len) == 0) {
        char ip[INET_ADDRSTRLEN] = {};
        inet_ntop(AF_INET, &local.sin_addr, ip, sizeof(ip));
        sLocalIp = ip;
    }
    const std::string req = fmt::format("{} {} HTTP/1.1\r\nHost: {}:{}\r\nConnection: close\r\n{}Content-Length: {}\r\n\r\n{}",
                                        method, path, host, port, headers, body.size(), body);
    send(s, req.data(), (int)req.size(), 0);
    std::string resp;
    char buf[4096];
    for (int n; (n = recv(s, buf, sizeof(buf), 0)) > 0;) {
        resp.append(buf, (size_t)n);
        if (resp.size() > (1 << 20)) {
            break;
        }
    }
    closeSock(s);
    if (status != nullptr) {
        *status = resp.size() > 12 ? atoi(resp.c_str() + 9) : 0;
    }
    const size_t bodyAt = resp.find("\r\n\r\n");
    return bodyAt == std::string::npos ? std::string() : resp.substr(bodyAt + 4);
}

// the text of the first <tag>...</tag> at or after from
std::string tagText(const std::string& xml, const std::string& tag, size_t from = 0, size_t* end = nullptr) {
    const size_t a = xml.find("<" + tag + ">", from);
    if (a == std::string::npos) {
        return {};
    }
    const size_t b = xml.find("</" + tag + ">", a);
    if (b == std::string::npos) {
        return {};
    }
    if (end != nullptr) {
        *end = b;
    }
    return xml.substr(a + tag.size() + 2, b - a - tag.size() - 2);
}

// the address of the interface that leads to the Internet (a UDP connect sends nothing): on a PC with VPN or
// virtual adapters, multicast would otherwise leave through whichever the system picks
in_addr primaryAddress() {
    in_addr ip{};
    sock_t s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in to{};
    to.sin_family = AF_INET;
    to.sin_port = htons(53);
    inet_pton(AF_INET, "1.1.1.1", &to.sin_addr);
    if (s != INVALID_SOCKET && connect(s, (sockaddr*)&to, sizeof(to)) == 0) {
        sockaddr_in local{};
        socklen_t len = sizeof(local);
        if (getsockname(s, (sockaddr*)&local, &len) == 0) {
            ip = local.sin_addr;
        }
    }
    if (s != INVALID_SOCKET) {
        closeSock(s);
    }
    return ip;
}

// SSDP: the router's description URL
std::string discover() {
    sock_t s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET) {
        return {};
    }
    setTimeout(s, 500);
    const in_addr local = primaryAddress();
    if (local.s_addr != 0) {
        sockaddr_in bindTo{};
        bindTo.sin_family = AF_INET;
        bindTo.sin_addr = local;
        bind(s, (sockaddr*)&bindTo, sizeof(bindTo));
        setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, (const char*)&local, sizeof(local));
    }
    sockaddr_in to{};
    to.sin_family = AF_INET;
    to.sin_port = htons(1900);
    inet_pton(AF_INET, "239.255.255.250", &to.sin_addr);
    // routers also answer an M-SEARCH sent to them directly: the usual gateway, x.x.x.1 of this network
    sockaddr_in gateway = to;
    gateway.sin_addr.s_addr = (local.s_addr & htonl(0xFFFFFF00)) | htonl(1);
    std::string location;
    for (int attempt = 0; attempt < 4 && location.empty(); attempt++) {
        for (const char* st : {"urn:schemas-upnp-org:device:InternetGatewayDevice:1",
                               "urn:schemas-upnp-org:device:InternetGatewayDevice:2",
                               "urn:schemas-upnp-org:service:WANIPConnection:1"}) {
            const std::string msg = fmt::format(
                "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 2\r\nST: {}\r\n\r\n", st);
            sendto(s, msg.data(), (int)msg.size(), 0, (sockaddr*)&to, sizeof(to));
            if (local.s_addr != 0) {
                sendto(s, msg.data(), (int)msg.size(), 0, (sockaddr*)&gateway, sizeof(gateway));
            }
        }
        const auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(800);
        while (location.empty() && std::chrono::steady_clock::now() < until) {
            char buf[2048];
            const int n = recv(s, buf, sizeof(buf) - 1, 0);
            if (n <= 0) {
                continue;
            }
            buf[n] = 0;
            std::string answer(buf);
            for (char& c : answer) { // headers are case-insensitive
                c = (char)tolower((unsigned char)c);
            }
            const size_t at = answer.find("\r\nlocation:");
            if (at != std::string::npos) {
                size_t v = at + 11;
                while (v < answer.size() && answer[v] == ' ') {
                    v++;
                }
                const size_t e = answer.find("\r\n", v);
                location = std::string(buf + v, (e == std::string::npos ? answer.size() : e) - v); // original case
            }
        }
    }
    closeSock(s);
    return location;
}

bool findService(const std::string& location) {
    const std::string xml = http(location, "GET", "", "");
    if (xml.empty()) {
        return false;
    }
    std::string base = tagText(xml, "URLBase");
    if (base.empty()) {
        std::string host, path;
        u16 port;
        if (!splitUrl(location, host, port, path)) {
            return false;
        }
        base = fmt::format("http://{}:{}", host, port);
    }
    while (!base.empty() && base.back() == '/') {
        base.pop_back();
    }
    for (const char* kind : {"WANIPConnection", "WANPPPConnection"}) {
        for (size_t at = 0; (at = xml.find("<serviceType>", at)) != std::string::npos; at++) {
            size_t end;
            const std::string type = tagText(xml, "serviceType", at, &end);
            if (type.find(kind) == std::string::npos) {
                continue;
            }
            std::string control = tagText(xml, "controlURL", end);
            if (control.empty()) {
                continue;
            }
            sServiceType = type;
            sControlUrl = control.rfind("http://", 0) == 0 ? control : base + (control[0] == '/' ? "" : "/") + control;
            return true;
        }
    }
    return false;
}

std::string soap(const std::string& action, const std::string& args, int* status) {
    const std::string body = fmt::format(
        "<?xml version=\"1.0\"?>\r\n<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
        "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:{0} xmlns:u=\"{1}\">{2}</u:{0}>"
        "</s:Body></s:Envelope>\r\n",
        action, sServiceType, args);
    const std::string headers = fmt::format("Content-Type: text/xml; charset=\"utf-8\"\r\nSOAPAction: \"{}#{}\"\r\n",
                                            sServiceType, action);
    return http(sControlUrl, "POST", headers, body, status);
}

bool addMapping(u16 port) {
    // a lease of 0 is permanent (removed when the session ends); some routers only take a limited one
    for (const char* lease : {"0", "7200"}) {
        int status = 0;
        soap("AddPortMapping",
             fmt::format("<NewRemoteHost></NewRemoteHost><NewExternalPort>{0}</NewExternalPort><NewProtocol>TCP</NewProtocol>"
                         "<NewInternalPort>{0}</NewInternalPort><NewInternalClient>{1}</NewInternalClient>"
                         "<NewEnabled>1</NewEnabled><NewPortMappingDescription>MP5 Native online play"
                         "</NewPortMappingDescription><NewLeaseDuration>{2}</NewLeaseDuration>",
                         port, sLocalIp, lease),
             &status);
        if (status == 200) {
            return true;
        }
    }
    return false;
}

void run(u16 port) {
    setStatus("Opening the port on the router (UPnP)...");
    const std::string location = discover();
    if (location.empty()) {
        setStatus(fmt::format("No UPnP router answered: for the Internet, forward TCP port {} to this machine by hand", port));
        return;
    }
    if (!findService(location)) {
        setStatus(fmt::format("The router does not offer port mapping (UPnP): forward TCP port {} by hand", port));
        return;
    }
    if (!addMapping(port)) {
        setStatus(fmt::format("The router refused to open port {} (UPnP may be off in its settings)", port));
        return;
    }
    {
        std::lock_guard g(sLock);
        sMapped = port;
    }
    int status = 0;
    const std::string ip = tagText(soap("GetExternalIPAddress", "", &status), "NewExternalIPAddress");
    if (ip.empty()) {
        setStatus(fmt::format("Port {} is open on the router (UPnP); friends join this network's public address", port));
    } else {
        setStatus(fmt::format("Friends on the Internet join {}:{} (opened on the router with UPnP)", ip, port));
    }
}

} // namespace

void open(u16 port) {
    if (sBusy.exchange(true)) {
        return;
    }
    if (sThread.joinable()) {
        sThread.join();
    }
    sThread = std::thread([port] {
        run(port);
        sBusy = false;
    });
}

void close() {
    if (sThread.joinable()) {
        sThread.join(); // at most a few seconds: every network call has a timeout
    }
    u16 port;
    {
        std::lock_guard g(sLock);
        port = sMapped;
        sMapped = 0;
        sStatus.clear();
    }
    if (port != 0) {
        int status = 0;
        soap("DeletePortMapping",
             fmt::format("<NewRemoteHost></NewRemoteHost><NewExternalPort>{}</NewExternalPort><NewProtocol>TCP</NewProtocol>",
                         port),
             &status);
    }
}

std::string status() {
    std::lock_guard g(sLock);
    return sStatus;
}

} // namespace mp5upnp
