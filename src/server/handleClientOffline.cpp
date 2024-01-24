#include "server.h"

void Server::handleClientOffline(int clientSocket, const std::string &clientName)
{
    std::cerr << "Client " << clientName << " offline!" << std::endl;
    std::lock_guard<std::mutex> guard(clientsMutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(), [clientSocket](const Client &client)
                                 { return client.getSocket() == clientSocket; }),
                  clients.end());
    close(clientSocket);
}
