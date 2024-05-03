#include "connection.h"
#include "chatService.h"
#include "userAuthentication.h"

// Connection::Connection(class Server& server) : server(server) {}
Connection::Connection(class UserAuthentication& auth) : auth(auth) {}

void Connection::start(int port)  {
    //create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Unable to create socket! Cancel..." << endl;
        return;
    }

    // establish IP server
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    int reuse = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        cerr << "Error setting socket option SO_REUSEADDR" << endl;
        close(serverSocket);
        return;
    }
    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Unable to bind socket." << endl;
        close(serverSocket);
        return;
    }

    // Listen
    if (listen(serverSocket, SOMAXCONN) == -1) {
        cerr << "Error while listening for connection." << endl;
        close(serverSocket);
        return;
    }


    cout << "Waiting for connection from client..." << endl;

    while (true) {
        // accept new connection
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (newClientSocket == -1) {
            cerr << "Error accepting connection." << endl;
            continue;
        }

        // Recieve the Option
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(newClientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            cerr << "Error receiving data from client." << endl;
            close(newClientSocket);
            continue;
        }

        string option(buffer, 0, bytesReceived);

        // Goto the handleAuthentication
        thread authenThread(&UserAuthentication::handleAuthentication, &auth, newClientSocket, option);
        authenThread.detach();
    }
}