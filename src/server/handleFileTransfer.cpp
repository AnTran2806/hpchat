#include "server.h"

void Server::handleFileTransfer(int clientSocket, const std::string& clientName) {
    char buffer[4096];
    int bytesReceived;

    // Receive the name of the receiver from the client
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        std::cerr << "Error receiving the name to send file to from client " << clientName << std::endl;
        return;
    }
    std::string receiverName = std::string(buffer, 0, bytesReceived);
    //std::cout << "Receiver's name: " << receiverName << std::endl;

    // Receive the file name from the client
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        std::cerr << "Error receiving file name from client " << clientName << std::endl;
        return;
    }
    std::string filename = std::string(buffer, 0, bytesReceived);
    //std::cout << "Received file name: " << filename << std::endl;

    // Send the command "sendfile" to the receiver
    std::string lenh = "sendfile";
    for (auto& client : clients) {
        bool isRoomNameMatch = client.getRoomName() == receiverName && client.getSocket() != clientSocket;
        bool isNameMatch = client.getName() == receiverName && client.getSocket() != clientSocket;
        if (isRoomNameMatch || isNameMatch) {
        // if (client.getRoomName() == receiverName && client.getSocket() != clientSocket || client.getName() == receiverName && client.getSocket() != clientSocket) {
            send(client.getSocket(), lenh.c_str(), lenh.size() + 1, 0);
            //std::cout << "Sent 'sendfile' command to client" << std::endl;
        }
    }
    //if invalid recieved name
    std::string lenh1 = "Please enter the correct receiver name. If it is correct, you will receive the message as below:";
    for (auto& client : clients) {
        if ( client.getSocket() == clientSocket) {
            send(client.getSocket(), lenh1.c_str(), lenh1.size() + 1, 0);
            
        }
    }
  

    // Send the file name to the receiver
    for (auto& client : clients) {
        bool isRoomNameMatch = client.getRoomName() == receiverName && client.getSocket() != clientSocket;
        bool isNameMatch = client.getName() == receiverName && client.getSocket() != clientSocket;
        if (isRoomNameMatch || isNameMatch) {

        // if (client.getRoomName() == receiverName && client.getSocket() != clientSocket || client.getName() == receiverName && client.getSocket() != clientSocket) {
            send(client.getSocket(), filename.c_str(), filename.size() + 1, 0);
            //std::cout << "Sent received file name to client: " << filename << std::endl;
        }
    }

    // Open the file for writing
    std::ofstream fileDuc("abc"+filename, std::ios::binary | std::ios::app);
    if (!fileDuc.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    //std::cout << "Receiving file..." << std::endl;

    // Receive and write file data
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            // Exit the loop when receiving "endsendfile" or if there's an error
            break;
        }

        for (auto& client : clients) {
        bool isRoomNameMatch = client.getRoomName() == receiverName && client.getSocket() != clientSocket;
        bool isNameMatch = client.getName() == receiverName && client.getSocket() != clientSocket;
            if (isRoomNameMatch || isNameMatch) {

            // if (client.getRoomName() == receiverName && client.getSocket() != clientSocket || client.getName() == receiverName && client.getSocket() != clientSocket) {
                send(client.getSocket(), buffer, bytesReceived, 0);
            }
        }

        //fileDuc.write(buffer, bytesReceived);
        //std::cout << std::string(buffer, 0, bytesReceived);

        if (strcmp(buffer,"endsendfile")==0) {
            // Exit the loop when receiving "endsendfile"
            //std::cout<<"Có thấy lệnh EndSF mà"<<std::endl;
            for (auto& client : clients) {
                bool isRoomNameMatch = client.getRoomName() == receiverName && client.getSocket() != clientSocket;
                bool isNameMatch = client.getName() == receiverName && client.getSocket() != clientSocket;
                if (isRoomNameMatch || isNameMatch) {
                // if (client.getRoomName() == receiverName && client.getSocket() != clientSocket || client.getName() == receiverName && client.getSocket() != clientSocket) {
                    send(client.getSocket(), "endsendfile", strlen("endsendfile") + 1, 0);
                    //std::cout<<"Đã gửi endsF cho client"<<std::endl;
                }
            }
            break; // Add break statement here
        }
        fileDuc.write(buffer, bytesReceived);
        
        
    }

    if (bytesReceived < 0) {
        std::cerr << "Read Error" << std::endl;
    }

    
    fileDuc.close();
    std::cout << "successfull to write the file\n";
    
}
