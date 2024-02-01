#include "client.h"

int main() {
    Client client;

    if (client.connectToServer(IP_SERVER, PORT)) {
        client.handleUserInteraction();
        client.startReceiving();
        client.enterRoomName();
        client.processFunction();
    }

    return 0;
}