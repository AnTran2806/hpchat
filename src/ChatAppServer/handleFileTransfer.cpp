#include "chatService.h"

void ChatService::handleFileTransfer(int clientSocket,string clientName,string roomID) {
    
    
    string abc = clientName;
    for (const auto &pair : checkRoomIDs) {
        if (pair.second == roomID && pair.first != clientSocket) {
            send(pair.first, "sendfile", strlen("sendfile") + 1, 0);
        }
    }
    char buffer[4096];
    int bytesReceived;
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    string filename = string(buffer, 0, bytesReceived);
    //send the file name
    for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID && pair.first != clientSocket) {
                send(pair.first, filename.c_str(), filename.size() + 1, 0);
            }
        }

    // Forward the data
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break;
        }
        if (strcmp(buffer, "exit") == 0) {
            for (const auto &pair : checkRoomIDs) {
                if (pair.second == roomID && pair.first != clientSocket) {
                    send(pair.first, "exit", strlen("exit") + 1, 0);
                }
            }
            break;
        }

        for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID && pair.first != clientSocket) {
                send(pair.first, buffer, bytesReceived, 0);
            }
        }
    }
    
    cout << "Successfully forwarded the file\n";
}
