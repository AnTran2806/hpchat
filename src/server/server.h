#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cctype>
#include <chrono>
#include <fstream>








class Client {
public:
    Client(int socket, const std::string& name, const std::string& roomName);

    int getSocket() const;

    const std::string& getName() const;

    const std::string& getRoomName() const;

private:
    int socket;
    std::string name;
    std::string roomName;
};

class Server {
public:
    Server();

    Client* findClientByName(const std::string& name);

    void sendPrivateMessage(const std::string& senderName, const std::string& receiverName, const std::string& message);

    void start();
    std::string trim(const std::string& str);
    
    std::string receiveString(int clientSocket);

    void handlePrivateMessage(int clientSocket, const std::string& clientName, const std::string& roomName,std::string receivedMessage )
       {
        char buffer[4096];
        int bytesReceived;
        size_t spacePos = receivedMessage.find(' ', 4);
        if (spacePos != std::string::npos) {
            std::string receiverName = trim(receivedMessage.substr(4, spacePos - 4));
            std::string privateMessage = trim(receivedMessage.substr(spacePos + 1));
            while (true) {
                        if (privateMessage.substr(0, 4) == "exit") {
                            std::string NotificationExitPrivateChat ="\033[1;32mYou exited the private chat and came back to the room chat hiiiiihihihihihihihihihihi\033[0m";
                            for (const auto& client : clients) {
                                if ( client.getSocket()== clientSocket) {
                                    send(client.getSocket(), NotificationExitPrivateChat.c_str(), NotificationExitPrivateChat.size() + 1, 0);
                                }
                            }
                            break;
                        } else {
                            std::string Private = "\033[1;31mPrivate chat from>>\033[0m";
                            std::string clientNameColor = "\033[1;34m" + clientName + "\033[0m";
                            std::string messageColor = "\033[1;37m" + privateMessage + "\033[0m";

                            sendPrivateMessage(clientName, receiverName, Private + " " + clientNameColor + ": " + messageColor);
                            bytesReceived = recv(clientSocket, buffer, 4096, 0);
                            //handleClientOffline(clientSocket,clientName,roomName,bytesReceived);
                            if (bytesReceived <= 0) {
                                std::cerr << "Client " << clientName << " offline!" << std::endl;
                                return;
                            }
                            privateMessage = std::string(buffer, 0, bytesReceived);
                        }
                    }
        
    } 
        else {
                    std::string Notification1 ="\033[1;33m Error: Incorrect private message format!\n Correct is : /pm + User's name + your message!\n The list users are online : \n\033[0m";
                    std::string clientNamesList1;
                    for (const auto& client : clients) {
                        clientNamesList1 += client.getName() + "\n"; 
                    }
                    std::string A=Notification1 + clientNamesList1;
                    for (const auto& client : clients) {
                        if ( client.getSocket()== clientSocket) {
                            send(client.getSocket(), A.c_str(), A.size() + 1, 0);
                        }
                    }
                }
    }


    void handleServerChat(int clientSocket, const std::string& clientName) {
    std::string privateClientMessage;

    std::string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
    std::cout << clientNameMessage << " wants to chat with the server." << std::endl;

    // Loop for handling server chat
    std::string userInput;
    char buffer[4096];
    while (true) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO, &readSet);
        FD_SET(clientSocket, &readSet);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready = select(std::max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
        std::string NotificationExitPrivateChatServer = "\033[1;32mYou exited the private chat with SERVER\033[0m";

        if (ready > 0) {
            if (FD_ISSET(clientSocket, &readSet)) {
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

                if (bytesReceived <= 0) {
                    std::cerr << "Client " << clientName << " offline!" << std::endl;
                    break; // Set exitChat khi client offline
                } else {
                    privateClientMessage = std::string(buffer, 0, bytesReceived);
                    if (privateClientMessage.compare(0, 4, "exit") == 0) {
                        std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
                        send(clientSocket, NotificationExitPrivateChatServer.c_str(), NotificationExitPrivateChatServer.size() + 1, 0);
                        break;
                    }
                    std::cout << clientNameMessage << ": " << privateClientMessage << std::endl;
                }
            }

            if (FD_ISSET(STDIN_FILENO, &readSet)) {
                std::getline(std::cin, userInput);
                if (userInput.compare(0, 4, "exit") == 0) {
                    std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
                    send(clientSocket, NotificationExitPrivateChatServer.c_str(), NotificationExitPrivateChatServer.size() + 1, 0);
                    break;
                } else {
                    // Handle user input and send messages to the server
                    // You can implement this part according to your requirements
                    // For example, you can send the userInput to the server
                    // or perform any other actions based on the user input
                    size_t spacePos1 = userInput.find(' ');
                    if (spacePos1 != std::string::npos) {
                        std::string firstPart = trim(userInput.substr(0, spacePos1));
                        std::string secondPart = trim(userInput.substr(spacePos1 + 1));

                        std::string privateServer = "\033[1;31mServer: \033[0m";
                        std::string messageOfServer = privateServer + secondPart;

                        // Kiểm tra nếu chỉ có một client
                        if (clients.size() == 1) {
                            auto& client = clients[0];
                            if (firstPart == client.getName() ) {
                                std::cout << "You sent a message to: " << firstPart << std::endl;
                                send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                            }
                        } else {
                            // 
                            for (auto& client : clients) {
                                if (firstPart == client.getName() ) {
                                    std::cout << "You sent a message to: " << firstPart << std::endl;
                                    send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                                    
                                }
                            }
                        }
                    }
                }
            }
        }

        // Sleep để giảm tải CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


    void handleFileTransfer(int clientSocket, const std::string& clientName) {
    char buffer[4096];
    int bytesReceived;

    // Receive the file name from the client
    bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Error receiving file name from client " << clientName << std::endl;
        return;
    }
    std::string filename = std::string(buffer, 0, bytesReceived);

    // Open the file for writing
    std::ofstream fileDuc(filename, std::ios::binary | std::ios::app);
    if (!fileDuc.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    std::cout << "Receiving file..." << std::endl;

    // Receive and write file data
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            // Exit the loop when receiving "endsendfile" or if there's an error
            break;
        }

        fileDuc.write(buffer, bytesReceived);
        std::cout << std::string(buffer, 0, bytesReceived);

        if (std::string(buffer, 0, bytesReceived) == "endsendfile") {
            // Exit the loop when receiving "endsendfile"
            break;
        }
    }

    if (bytesReceived < 0) {
        std::cerr << "Read Error" << std::endl;
    }

    // Close the file
    fileDuc.close();
    std::cout << "File received successfully: " << filename << std::endl;
}


    void handleGroupMessage(const std::string& clientName, const std::string& roomName, const std::string& receivedMessage,int clientSocket) {
        std::string groupMessage = "\033[1;34m" + clientName + "\033[0m" + ": " + receivedMessage;
        std::lock_guard<std::mutex> guard(clientsMutex);
        
        for (const auto& client : clients) {
            if (client.getRoomName() == roomName && client.getSocket() != clientSocket) {
                std::string tenPhong = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
                send(client.getSocket(), tenPhong.c_str(), tenPhong.size() + 1, 0);
            }
        }
    }
    
    void handleClientOffline(int clientSocket, const std::string& clientName) {
        std::cerr << "Client " << clientName << " offline!" << std::endl;
        std::lock_guard<std::mutex> guard(clientsMutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(), [clientSocket](const Client& client) {
            return client.getSocket() == clientSocket;
        }), clients.end());
        close(clientSocket);
    }


private:
    int createSocket;
    std::vector<Client> clients;
    std::mutex clientsMutex;


    void handleClient(int clientSocket, const std::string& clientName, const std::string& roomName);
};

#endif // SERVER_H
