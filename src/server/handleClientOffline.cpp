#include "server.h"

void Server::handleClientOffline(int clientSocket, const string &clientName)
{
    cerr << "Client " << clientName << " offline!" << endl;

    // Lock the clients vector to ensure thread safety
    lock_guard<mutex> guard(clientsMutex);

    // Remove the disconnected client from the clients vector
    clients.erase(remove_if(clients.begin(), clients.end(), [clientSocket](const Client& client) {
        return client.getSocket() == clientSocket;
    }), clients.end());
    close(clientSocket);
}
