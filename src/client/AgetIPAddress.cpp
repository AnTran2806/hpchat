#include "client.h"


string AgetIPAddress() {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            if (strcmp(tmp->ifa_name, "lo") != 0) { // Ignore loopback interface
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(pAddr->sin_addr), ip, INET_ADDRSTRLEN);
                freeifaddrs(addrs);
                return string(ip);
            }
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
    return "";
}