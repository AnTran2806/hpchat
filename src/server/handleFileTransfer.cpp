#include "server.h"

void Server::handleFileTransfer(int clientSocket, const string& clientName) {
    char buffer[4096];
    int bytesReceived;

    // Receive the name of the receiver from the client
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving the name to send file to from client " << clientName << endl;
        return;
    }
   string receiverName = string(buffer, 0, bytesReceived);
    cout << "Receiver's name: " << receiverName << std::endl;

    // Send the command "sendfile" to the receiver
    bool receiverFound = false;
    for (auto& client : clients) {
        if ((client.getRoomName() == receiverName || client.getName() == receiverName) && client.getSocket() != clientSocket) {
            send(client.getSocket(), "sendfile", strlen("sendfile") + 1, 0);
            receiverFound = true;
        }
    }

    // If receiver not found, send a notification to the sender
    if (!receiverFound) {
        string notification = "Please enter the correct receiver name. If it is correct, you will receive the message as below:";
        send(clientSocket, notification.c_str(), notification.size() + 1, 0);
        return;
    }

    // Receive the file name from the client
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        cerr << "Error receiving file name from client " << clientName << endl;
        return;
    }
    string filename = string(buffer, 0, bytesReceived);
    //std::cout << "Received file name: " << filename << std::endl;

    // Send the file name to the receiver
    for (auto& client : clients) {
        if ((client.getRoomName() == receiverName || client.getName() == receiverName) && client.getSocket() != clientSocket) {
            send(client.getSocket(), filename.c_str(), filename.size() + 1, 0);
        }
    }

    // Open the file for writing
    ofstream file("abc" + filename, ios::binary |ios::app);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    // Receive and write file data
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break;
        }
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        file.write(buffer, bytesReceived);

        // Forward the received file data to the receiver
        for (auto& client : clients) {
            if ((client.getRoomName() == receiverName || client.getName() == receiverName) && client.getSocket() != clientSocket) {
                send(client.getSocket(), buffer, bytesReceived, 0);
            }
        }
    }

    if (bytesReceived < 0) {
        cerr << "Read Error" << endl;
    }

    file.close();
    cout << "Successfully forwarded the file\n";

    // Send "endsendfile" command to the receiver
    for (auto& client : clients) {
        if ((client.getRoomName() == receiverName || client.getName() == receiverName) && client.getSocket() != clientSocket) {
            send(client.getSocket(), "exit", strlen("exit") + 1, 0);
        }
    }
}