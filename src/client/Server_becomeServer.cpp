#include "client.h"

bool Server::becomeServer(int port, string clientName) {
    AserverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (AserverSock == -1) {
        cerr << "Error: Create the server socket" << endl;
        return false;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(AserverSock, (sockaddr*)&hint, sizeof(hint)) == -1) {
        cerr << "Error: Cannot bind to port" << endl;
        close(AserverSock);
        return false;
    }

    if (listen(AserverSock, SOMAXCONN) == -1) {
        cerr << "Error: Cannot listen on port" << endl;
        close(AserverSock);
        return false;
    }
    
    AserverThread = thread([this, port, clientName]() {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        AclientSock = accept(AserverSock, (sockaddr*)&clientAddr, &clientSize);
        if (AclientSock == -1) {
            cerr << "Error: Cannot accept client connection" << endl;
            return;
        }
        cout <<clientName<< " accepted your request and connected on port  " << port << endl;

        handleP2PMessages(AclientSock, clientName);
    });

    return true;
}
