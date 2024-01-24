#include "client.h"

int main() {
    Client client;
    if (client.connectToServer("10.188.9.19", 54000)) {
        std::string roomName;
        std::cout << "Enter the Room or Your ID:  ";
        std::getline(std::cin, roomName);
        client.runMessageReceiver();

        std::string clientName;
        std::cout << "Enter Your name: ";
        std::getline(std::cin, clientName);
        client.sendToServer(clientName);
        abc:
        while (true) {
            std::string userInput;
            std::getline(std::cin, userInput);

            if (userInput == "exitchat") {
                break;
            } else if (userInput == "sendfile") {
                std::cout << "Enter the file path to send: ";
                std::string filePath;
                std::getline(std::cin, filePath);
                if (filePath == "exitchat") {
                    goto abc;
                }
                client.sendFile(filePath);
                
            } else {
                client.sendToServer(userInput);
            }
        }
    }
    return 0;
}
