#include "client.h"    
 
void Client::startReceiving(int clientSocket) {
    // Create a new thread to handle receiving messages from the server
    receivingThread = thread([this,clientSocket]() {
        char buffer[4096]; 
        while (true) {
            int bytesReceived = recv(sock, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                cerr << "Error: Server is down" << endl;
                close(sock);
                break;
            }
            // Check if the received message indicates a file is being sent
            if (strcmp(buffer, "sendfile") == 0) {
                cout<<"Receiving a file from another client!";
                receiveFile(sock);
            }
            else if (strcmp(buffer, "out") == 0) {
                string message = "Bye!";
                sendToServer(clientSocket, message);
                close(clientSocket);
                break;
            }
            else {
                cout << string(buffer, 0, bytesReceived) << endl;
            }
        }
    });
}
