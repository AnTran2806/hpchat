#include "chatService.h"   

// Server::Server(class UserAuthentication& auth) : auth(auth) {}
ChatService::ChatService() {}
UserAuthentication::UserAuthentication(){}
string ChatService::trim(const string& str) {
    // ... (trim implementation)
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == string::npos || last == string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}

string ChatService::receiveString(int clientSocket) {
    // ... (receiveString implementation)
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, 4096, 0);

    // Check if data reception was successful
    if (bytesReceived <= 0) {
        cerr << "Error: Received data from client failed!" << endl;
        return "";
    }
    return string(buffer, 0, bytesReceived);
}

void ChatService::setLoggedInUsers(unordered_map<int, string> loggedInUsers){
        this->loggedInUsers = loggedInUsers;
}
