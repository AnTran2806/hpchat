#include "client.h"

void Server::stopServer() {
    close(AserverSock);
    close(AclientSock);

    if (AserverThread.joinable()) {
         AserverThread.join();
    }   
}