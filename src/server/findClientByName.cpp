#include "server.h"

Client *Server::findClientByName(const std::string &name) {
    std::lock_guard<std::mutex> guard(clientsMutex);
    auto it = std::find_if(clients.begin(), clients.end(), [this, name](const Client &client)
                           { return trim(client.getName()) == trim(name); });
    return (it != clients.end()) ? &(*it) : nullptr;
}