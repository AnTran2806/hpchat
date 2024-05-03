#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "../common/library.h"
#include "userAuthentication.h"
#include "client.h"

// using namespace std;

const int PORT = 55000;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class ChatService {
public:
    ChatService();

    Client* findClientByName(const string& name);

    void sendPrivateMessage(const string& senderName, const string& receiverName, const string& message);

    string trim(const string& str);

    string receiveString(int clientSocket);

    void handlePrivateMessage(int clientSocket, const string& clientName, const string& roomName, string receivedMessage);

    void handleServerChat(int clientSocket, const string &clientName);

    void handleFileTransfer(int clientSocket, string clientName,string roomID);

    void handleGroupMessage(const string& clientName, int clientSocket);

    void handleClientOffline(int clientSocket, const string &clientName);

    void processClient(int clientSocket);
    
    void handleClient(int clientSocket, const string& clientName, const string& roomName);

    void setLoggedInUsers(unordered_map<int, string> loggedInUsers);

private:
    int PORT;
    vector<Client> clients;
    mutex clientsMutex;
    unordered_map<int, string> loggedInUsers;
    UserAuthentication auth;
    vector<pair<int,string>> checkRoomIDs;
};

#endif // CHATSERVICE_H