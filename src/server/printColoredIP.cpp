#include "server.h"

void Server::printColoredIP(const char* ipAddress) {
    cout << "\033[1;32m" << ipAddress << "\033[0m"; // Set color to green
}  
