#include "server.h"

void Server::processClient(int clientSocket)
{
    string roomName = receiveString(clientSocket);
    if (roomName.empty()) {
        cerr << "Error: Unable to receive room name. Closing connection." << endl;
        close(clientSocket);
        // continue;
    }
    cout << "Room name is " << roomName << endl;

    // Check if the client is logged in
    if (loggedInUsers.find(clientSocket) == loggedInUsers.end()) {
        cerr << "Error: Client not logged in. Closing connection." << endl;
        close(clientSocket);
        return;
    }

    string clientName = loggedInUsers[clientSocket];
    cout << "Username is " << clientName << endl;

    lock_guard<mutex> guard(clientsMutex);
    clients.push_back(Client(clientSocket, clientName, roomName)); // Pass the username
    cout << "Client " << clientName << " joined room " << roomName << endl;
    
    thread clientThread(&Server::handleClient, this, clientSocket, clientName, roomName); // Pass the username
    clientThread.detach();
}