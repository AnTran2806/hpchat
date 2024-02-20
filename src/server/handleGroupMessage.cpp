#include "server.h"

void Server::handleGroupMessage(const string &clientName, 
                                const string &roomName, 
                                const string &receivedMessage, 
                                int clientSocket)
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
        
        for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID && pair.first != clientSocket) {
                send(pair.first, groupMessage.c_str(), groupMessage.size() + 1, 0);
            }
        }

        if (strcmp(buffer, "exit") == 0) {
            
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





   // while (true)
    // {
    //     /* code */
    //     bytesReceived =recv(clientSocket,buffer,sizeof(buffer),0);
    //     string groupMessage ="\033[1;34m" + clientName + "\033[0m" + ": " + string(buffer,0,bytesReceived);
    //     string notiExitRoom= "\033[1;35m" + clientName + " exited the chat room" + "\033[0m";
    //     if (bytesReceived<=0){
    //         for (auto & client:clients){
    //             if (client.getRoomName() == roomName && client.getSocket() != clientSocket){
    //                 send(client.getSocket(),notiExitRoom.c_str(),notiExitRoom.size()+1,0);
    //             }
    //         }
    //         break;
    //     }
    //     if (strcmp(buffer,"exit")==0){
    //         for (auto & client:clients){
    //             if (client.getRoomName() == roomName && client.getSocket() != clientSocket){
    //                 send(client.getSocket(),notiExitRoom.c_str(),notiExitRoom.size()+1,0);
    //             }
    //         }
    //         break;
    //     }
    //     for (const auto &client : clients) {
    //         // Check if the client is in the same room and is not the sender
    //         if (client.getRoomName() == roomName && client.getSocket() != clientSocket)
    //         {
    //             string roomMessage = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
    //             // Send the room message to the client's socket
    //             send(client.getSocket(), roomMessage.c_str(), roomMessage.size() + 1, 0);
    //         }
    //     }
    // }

 // string groupMessage = "\033[1;34m" + clientName + "\033[0m" + ": " + receivedMessage;
    // // Lock the clientsMutex to ensure thread safety
    // lock_guard<mutex> guard(clientsMutex);

    // for (const auto &client : clients) {
    //     // Check if the client is in the same room and is not the sender
    //     if (client.getRoomName() == roomName && client.getSocket() != clientSocket)
    //     {
    //         string roomMessage = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
    //         // Send the room message to the client's socket
    //         send(client.getSocket(), roomMessage.c_str(), roomMessage.size() + 1, 0);
    //     }
    // }