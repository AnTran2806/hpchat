#include "client.h"

int main() {
    Client client;
    Authen authen;
    string IPOFSERVER; 
    while (true) {
        cout << "IP server: ";
        getline(cin, IPOFSERVER);
        if (client.connectToServer(IPOFSERVER, PORT)) {
            authen.handleUserInteraction(client.getSocket());
            client.startReceiving(client.getSocket());
            sendToServer(client.getSocket(),"R1");
            client.processFunction();
            break; 
        } else {
            cout << "Please try again...\n";
        }
    }
    return 0;
}