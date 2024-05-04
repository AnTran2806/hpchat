#include "server.h"

Server::~Server() {
    stopServer();
}

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

void Server::handleP2PMessages(int clientSock,string clientName) {
    Client client;
    char buffer[4096];
    while (true) {
        int bytesReceived = recv(clientSock, buffer, 4096, 0);
        if (bytesReceived <= 0) {
            cerr <<clientName<< " disconnected." << endl;
            close(clientSock);
            break;
        }
        if (strcmp(buffer, "out") == 0) {
            string message = "Bye!";
            sendToServer(clientSock, message);
            cout <<clientName<< " exited the chat! Enter 'out' to leave the chat" << endl;
            close(clientSock);
            break;
        }
        if (strcmp(buffer, "sendfile") == 0) {
                cout<<"Receiving a file from another client!";
                client.receiveFile(clientSock);
            }
        cout << clientName <<":" << string(buffer, bytesReceived) << endl;
    }
}

void Server::stopServer() {
    close(serverSock);
    close(clientSock);

    if (serverThread.joinable()) {
        serverThread.join();
    }   
}
