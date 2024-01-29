#include "server.h"

Client *Server::findClientByName(const std::string &name)
{
    std::lock_guard<std::mutex> guard(clientsMutex);
    // auto it = std::find_if(clients.begin(), clients.end(), [this, name](const Client &client)
    //                        { return trim(client.getName()) == trim(name); });
    auto it = clients.end();
    for (auto iter = clients.begin(); iter != clients.end(); ++iter) {
        if (trim(iter->getName()) == trim(name)) {
            it = iter;
            break; // Kết thúc vòng lặp khi tìm thấy
        }
    }

    return (it != clients.end()) ? &(*it) : nullptr;
    
}

