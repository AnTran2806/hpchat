#include "client.h"

int main() {
    Client client;
    string IPOFSERVER; 
    while (true) {
        cout << "IP server: ";
        getline(cin, IPOFSERVER);
        if (client.connectToServer(IPOFSERVER, PORT)) {
            client.handleUserInteraction();
            client.startReceiving();
            client.enterRoomName();
            client.processFunction();
            break; 
        } else {
            cout << "Please try again...\n";
        }
    }
    return 0;
}