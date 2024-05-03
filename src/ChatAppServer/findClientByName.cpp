#include "chatService.h"

Client* ChatService::findClientByName(const string& name) {
    // ... (findClientByName implementation)
    lock_guard<mutex> guard(clientsMutex);
    auto it = find_if(clients.begin(), clients.end(), [this,name](const Client& client) {
        return trim(client.getName()) == trim(name);
    });
    return (it != clients.end()) ? &(*it) : nullptr;
}
