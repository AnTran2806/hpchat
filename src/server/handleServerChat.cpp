#include "server.h"

void Server::handleServerChat(int clientSocket, const std::string &clientName)
{
    // Display a message indicating that the client wants to chat with the server
    std::string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
    std::cout << clientNameMessage << " wants to chat with the server." << std::endl;

    // Loop for handling server chat
    std::string privateClientMessage;
    std::string userInput;
    char buffer[4096];
    while (true)
    {
        // Prepare the file descriptor set for select
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO, &readSet);
        FD_SET(clientSocket, &readSet);

        // Set the timeout for select
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready = select(std::max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
        std::string exitNotification = "\033[1;32mYou exited the private chat with SERVER\033[0m";

        if (ready > 0)
        {
            if (FD_ISSET(clientSocket, &readSet))
            {
                // Receive data from the client
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0)
                {
                    std::cerr << "Client " << clientName << " offline!" << std::endl;
                    break;
                }
                else {
                    // Process the received message from the client
                    privateClientMessage = std::string(buffer, 0, bytesReceived);
                    if (privateClientMessage.compare(0, 4, "exit") == 0)
                    {
                        std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
                        send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
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
                    send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
                    break;
                }
                else {
                    // Handle user input and send messages to the server
                    size_t spacePos1 = userInput.find(' ');
                    if (spacePos1 != std::string::npos)
                    {
                        // Extract receiver's name and message
                        std::string firstPart = trim(userInput.substr(0, spacePos1));
                        std::string secondPart = trim(userInput.substr(spacePos1 + 1));
                        // Construct the message to be sent to the server
                        std::string privateServer = "\033[1;31mServer: \033[0m";
                        std::string messageOfServer = privateServer + secondPart;

                        // Send the message to the specified client
                        if (clients.size() == 1)
                        {
                            auto &client = clients[0];
                            if (firstPart == client.getName())
                            {
                                std::cout << "You sent a message to: " << firstPart << std::endl;
                                send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                            }
                        }
                        else {
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
        // Sleep for a short interval to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}