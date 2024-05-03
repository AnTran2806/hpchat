#ifndef SERVER_H
#define SERVER_H

#include "../common/library.h"
#include "userAuthentication.h"
#include "client.h"

const int PORT = 55000;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class Server { 
public:
    Server();

    string trim(const string& str);
    
    string receiveString(int clientSocket);
    
    Client* findClientByName(const string& name);

    void processClient(int clientSocket);

    void setLoggedInUsers(unordered_map<int, string> loggedInUsers);

    void handleServerChat(int clientSocket, const string &clientName);

    void handleGroupMessage(const string& clientName, int clientSocket);

    void handleClientOffline(int clientSocket, const string &clientName);

    void handleFileTransfer(int clientSocket, string clientName,string roomID);

    void handleClient(int clientSocket, const string& clientName, const string& roomName);
    
    void sendPrivateMessage(const string& senderName, const string& receiverName, const string& message);

    void handlePrivateMessage(int clientSocket, const string& clientName, const string& roomName, string receivedMessage);

private:
    int PORT;
    mutex clientsMutex;
    vector<Client> clients;
    UserAuthentication auth;
    vector<pair<int,string>> checkRoomIDs;
    unordered_map<int, string> loggedInUsers;
};

#endif // SERVER_H