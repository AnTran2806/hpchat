#include "client.h"

bool Server::becomeServer(int port, string clientName) {
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        cerr << "Error: Create the server socket" << endl;
        return false;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&hint, sizeof(hint)) == -1) {
        cerr << "Error: Cannot bind to port" << endl;
        close(serverSock);
        return false;
    }

    if (listen(serverSock, SOMAXCONN) == -1) {
        cerr << "Error: Cannot listen on port" << endl;
        close(serverSock);
        return false;
    }
    
    serverThread = thread([this, port, clientName]() {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientSize);
        if (clientSock == -1) {
            cerr << "Error: Cannot accept client connection" << endl;
            return;
        }
        cout <<clientName<< " accepted your request and connected on port  " << port << endl;

        handleP2PMessages(clientSock, clientName);
    });

    return true;
}
