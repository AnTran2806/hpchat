#include "server.h"

void Server::handleServerChat(int clientSocket, const std::string &clientName)
{
    std::string privateClientMessage;

    std::string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
    std::cout << clientNameMessage << " wants to chat with the server." << std::endl;

    // Loop for handling server chat
    std::string userInput;
    char buffer[4096];
    while (true)
    {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO, &readSet);
        FD_SET(clientSocket, &readSet);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready = select(std::max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
        std::string NotificationExitPrivateChatServer = "\033[1;32mYou exited the private chat with SERVER\033[0m";

        if (ready > 0)
        {
            if (FD_ISSET(clientSocket, &readSet))
            {
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

                if (bytesReceived <= 0)
                {
                    std::cerr << "Client " << clientName << " offline!" << std::endl;
                    break; // Set exitChat khi client offline
                }
                else
                {
                    privateClientMessage = std::string(buffer, 0, bytesReceived);
                    if (privateClientMessage.compare(0, 4, "exit") == 0)
                    {
                        std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
                        send(clientSocket, NotificationExitPrivateChatServer.c_str(), NotificationExitPrivateChatServer.size() + 1, 0);
                        break;
                    }
                    std::cout << clientNameMessage << ": " << privateClientMessage << std::endl;
                }
            }

            if (FD_ISSET(STDIN_FILENO, &readSet))
            {
                std::getline(std::cin, userInput);
                if (userInput.compare(0, 4, "exit") == 0)
                {
                    std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
                    send(clientSocket, NotificationExitPrivateChatServer.c_str(), NotificationExitPrivateChatServer.size() + 1, 0);
                    break;
                }
                else
                {
                    // Handle user input and send messages to the server
                    // You can implement this part according to your requirements
                    // For example, you can send the userInput to the server
                    // or perform any other actions based on the user input
                    size_t spacePos1 = userInput.find(' ');
                    if (spacePos1 != std::string::npos)
                    {
                        std::string firstPart = trim(userInput.substr(0, spacePos1));
                        std::string secondPart = trim(userInput.substr(spacePos1 + 1));

                        std::string privateServer = "\033[1;31mServer: \033[0m";
                        std::string messageOfServer = privateServer + secondPart;

                        // Kiểm tra nếu chỉ có một client
                        if (clients.size() == 1)
                        {
                            auto &client = clients[0];
                            if (firstPart == client.getName())
                            {
                                std::cout << "You sent a message to: " << firstPart << std::endl;
                                send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                            }
                        }
                        else
                        {
                            //
                            for (auto &client : clients)
                            {
                                if (firstPart == client.getName())
                                {
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