#include "client.h"
#include "authentication.h"

int main() {
    Client client;
    Authentication authentication;
    string IPOFSERVER; 
    while (true) {
        cout << "IP server: ";
        getline(cin, IPOFSERVER);
        if (client.connectToServer(IPOFSERVER, PORT)) {
            authentication.handleUserInteraction(client.getSocket());
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