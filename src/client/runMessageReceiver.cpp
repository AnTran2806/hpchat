#include "client.h"

void Client::runMessageReceiver() {
    receivingThread = thread([this]() {
        char buffer[4096];
        while (true) {
            int bytesReceived = recv(sock, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                cerr << "Error: Server is down" << endl;
                close(sock);
                break;
            }
            cout << string(buffer, 0, bytesReceived) << endl;
        }
    });
}