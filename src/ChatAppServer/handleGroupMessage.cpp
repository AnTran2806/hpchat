#include "chatService.h"

void ChatService::handleGroupMessage(const string &clientName,  int clientSocket)
{
   
    char buffer[4096];
    int bytesReceived;
    if (clientSocket) {
        send(clientSocket, "Enter the RoomID to join the chat Room", strlen("Enter the RoomID to join the chat Room") + 1, 0);
    }

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        
        return;
    }
    string roomID = string(buffer, 0, bytesReceived);
    checkRoomIDs.emplace_back(clientSocket, roomID);
 
    while (true) {


        bool clientExists = false;
        for (auto &pair : checkRoomIDs) {
            if (pair.first == clientSocket) {
                // Upate the client 
                pair.second = roomID;
                clientExists = true;
                break;
            }
        }

        if (!clientExists) {
            checkRoomIDs.emplace_back(clientSocket, roomID);
        }
        vector<int> roomClients;
        for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID) {
                roomClients.push_back(pair.first);
            }
        }
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        
        string notiExit="\033[1;34m" + clientName  + ": " + "disconnected from the room" + "\033[0m";
        if (bytesReceived <= 0) {
            
            for (int socket : roomClients) {
                if (socket != clientSocket) {
                    send(socket,notiExit.c_str(), notiExit.size() + 1, 0);
                }
            }
              
            roomClients.erase(std::remove(roomClients.begin(), roomClients.end(), clientSocket), roomClients.end());
            break;
        }
        string groupMessage = "\033[1;31m" +roomID+"\033[0m" +"-"+"\033[1;34m" + clientName + "\033[0m" + ": " + string(buffer, 0, bytesReceived);

        //send the message to all client in the room
        if (strcmp(buffer, "sendfile") == 0){
            handleFileTransfer(clientSocket,clientName,roomID);
        }else if (strcmp(buffer, "sendfile") != 0&&strcmp(buffer, "out") != 0) {
            for (const auto &pair : checkRoomIDs) {
                if (pair.second == roomID && pair.first != clientSocket) {
                    send(pair.first, groupMessage.c_str(), groupMessage.size() + 1, 0);
                }
            }
        }
        
        if (strcmp(buffer, "out") == 0) {
            
            for (int socket : roomClients) {
                if (socket != clientSocket) {
                    send(socket, notiExit.c_str(), notiExit.size() + 1, 0);
                }
                string notiExit1="\033[1;34m\t\t\tYou are now out of the chat room\033[0m";
                if (socket == clientSocket) {
                    send(socket, notiExit1.c_str(), notiExit1.size() + 1, 0);
                }
            }

            //erase the client if the client exit the chat room
            auto iter = std::find_if(checkRoomIDs.begin(), checkRoomIDs.end(), 
                                        [clientSocket](const auto &pair) {
                                        return pair.first == clientSocket;
                                    });
            if (iter != checkRoomIDs.end()) {
                checkRoomIDs.erase(iter);
            }

            
            break;
        }

    }
}
