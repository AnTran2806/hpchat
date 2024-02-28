#include "client.h"

void Server::handleP2PMessages(int clientSock,string clientName) {
    SharingFile sharingFile;
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
                sharingFile.receiveFile(clientSock);
            }
        cout << clientName <<":" << string(buffer, bytesReceived) << endl;
    }
}
