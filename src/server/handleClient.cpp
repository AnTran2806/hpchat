#include "server.h"

void Server::handleClient(int acceptConnection, const std::string &clientName, const std::string &roomName) {
    char buffer[4096];
    int bytesReceived;
    while (true)
    {
        bytesReceived = recv(acceptConnection, buffer, 4096, 0);
        if (bytesReceived <= 0)
        {
            std::cerr << "Client " << clientName << " offline!" << std::endl;
            std::lock_guard<std::mutex> guard(clientsMutex);
                                                            
            clients.erase(std::remove_if(
                              clients.begin(), clients.end(), [acceptConnection](const Client &client)
                              { return client.getSocket() == acceptConnection; }),
                          clients.end());
            close(acceptConnection);
            return;
        }

        std::string receivedMessage(buffer, 0, bytesReceived);

        // Check the user that want to send the privare Message
        if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/pc")
        {
            size_t _spacePos = receivedMessage.find(' ', 4);
            if (_spacePos != std::string::npos)
            {
                std::string receiverName = trim(receivedMessage.substr(4, _spacePos - 4));
                std::string privateMessage = trim(receivedMessage.substr(_spacePos + 1));

                // Handle to send the private file
                while (true)
                {
                    if (privateMessage.substr(0, 4) == "exit")
                    {
                        std::string NotifyExitPrivateChat = "\033[1;32mYou have exited the private chat and returned to the room chat\033[0m";
                        for (const auto &client : clients) {
                            if (client.getSocket() == acceptConnection)
                            {
                                send(client.getSocket(), NotifyExitPrivateChat.c_str(), NotifyExitPrivateChat.size() + 1, 0);
                            }
                        }
                        break;
                    }
                    else
                    {
                        std::string privateChatNotify = "\033[1;31mPrivate chat from>>\033[0m";
                        std::string clientNameColor = "\033[1;34m" + clientName + "\033[0m";
                        std::string messageColor = "\033[1;37m" + privateMessage + "\033[0m";

                        handlePrivateMessage(clientName, receiverName, privateChatNotify + " " + clientNameColor + ": " + messageColor);
                        bytesReceived = recv(acceptConnection, buffer, 4096, 0);
                        if (bytesReceived <= 0)
                        {
                            std::cerr << "Client " << clientName << " offline!" << std::endl;
                            return;
                        }
                        privateMessage = std::string(buffer, 0, bytesReceived);
                    }
                }
            }
            else
            {
                std::string typedErrorCmd = "\033[1;33m Error: Incorrect private message format!\n Correct is : /pm + User's name + your message!\n The list users are online : \n\033[0m";
                std::string clientNamesList;
                for (const auto &client : clients)
                {
                    clientNamesList += client.getName() + "\n";
                }
                std::string errMsgWithUserList = typedErrorCmd + clientNamesList;
                for (const auto &client : clients) {
                    if (client.getSocket() == acceptConnection)
                    {
                        send(client.getSocket(), errMsgWithUserList.c_str(), errMsgWithUserList.size() + 1, 0);
                    }
                }
            }
        }

        // Server-Client Cross-Communication
        else if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/sc")
        {
            size_t _spacePos = receivedMessage.find(' ');
            if (_spacePos != std::string::npos)
            {
                std::string privateClientMessage = receivedMessage.substr(_spacePos + 1);
                std::string clientNameMessage;

                for (const auto &client : clients) {
                    if (client.getSocket() == acceptConnection)
                    {
                        clientNameMessage = "\033[1;34m" + client.getName() + "\033[0m";
                    }
                }
                std::cout << clientNameMessage << ":" << privateClientMessage << std::endl;

                std::string newClientChat = "\033[1;32m A new client wants to chat >>\033[0m";
                for (const auto &client : clients) {
                    if (client.getSocket() == acceptConnection)
                    {
                        std::cout << newClientChat << client.getName() << std::endl;
                        break;
                    }
                }

                std::string userInput;
                bool exitChat = false;

                while (!exitChat)
                {
                    fd_set eventSet;
                    FD_ZERO(&eventSet);
                    FD_SET(STDIN_FILENO, &eventSet);
                    FD_SET(acceptConnection, &eventSet);

                    struct timeval timeout;
                    timeout.tv_sec = 0;
                    timeout.tv_usec = 0;

                    int numReadyEvents = select(std::max(STDIN_FILENO, acceptConnection) + 1, &eventSet, NULL, NULL, &timeout);
                    std::string exitPrivateChatNotify = "\033[1;32mYou exited the private chat with SERVER\033[0m";
                    if (numReadyEvents > 0) {
                        if (FD_ISSET(acceptConnection, &eventSet))
                        {
                            bytesReceived = recv(acceptConnection, buffer, 4096, 0);
                            if (bytesReceived <= 0)
                            {
                                std::cerr << "Client " << clientName << " offline!" << std::endl;
                                exitChat = true;
                            }
                            else
                            {
                                privateClientMessage = std::string(buffer, 0, bytesReceived);
                                if (privateClientMessage.compare(0, 4, "exit") == 0)
                                {
                                    exitChat = true;
                                    std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << clientName << std::endl;
                                    send(acceptConnection, exitPrivateChatNotify.c_str(), exitPrivateChatNotify.size() + 1, 0);
                                }
                                std::cout << clientNameMessage << ":" << privateClientMessage << std::endl;
                            }
                        }
                        if (FD_ISSET(STDIN_FILENO, &eventSet)) {
                            std::getline(std::cin, userInput);
                            if (userInput.compare(0, 4, "exit") == 0) {
                                exitChat = true;
                                send(acceptConnection, exitPrivateChatNotify.c_str(), exitPrivateChatNotify.size() + 1, 0);
                                std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << clientNameMessage << std::endl;
                            }
                            else {
                                size_t __spacePos = userInput.find(' ');

                                if (__spacePos != std::string::npos) 
                                {
                                    std::string firstPart = trim(userInput.substr(0, __spacePos));
                                    std::string secondPart = trim(userInput.substr(__spacePos + 1));
                                    // std::string privateServer = "\033[1;31mServer: \033[0m";
                                    std::string messageOfServer = "\033[1;31mServer: \033[0m" + secondPart;

                                    // Only one client want to chat with server
                                    if (clients.size() == 1) {
                                        auto &client = clients[0];
                                        if (firstPart == client.getName())
                                        {
                                            std::cout << "You sent a message to: " << firstPart << std::endl;
                                            send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                                        }
                                    }
                                    else {
                                        // if have more one client want to chat with server
                                        for (auto &client : clients) {
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
                    // Sleep to reduce CPU
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
        }
        // send file
        else if (receivedMessage.substr(0, 9) == "/sendfile") {
            std ::string SendFile = "File is being sent from ";
            std::cout << SendFile << std::endl;
        }
        else {
            // Handle to send message to the same roomName
            std::string groupMessage = "\033[1;34m" + clientName + "\033[0m" + ": " + receivedMessage;
            std::lock_guard<std::mutex> guard(clientsMutex);
            for (const auto &client : clients) {
                if (client.getRoomName() == roomName && client.getSocket() != acceptConnection)
                {
                    std::string roomMessage = "\033[1;35m" + client.getRoomName() + "\033[0m" + ">>" + groupMessage;
                    send(client.getSocket(), roomMessage.c_str(), roomMessage.size() + 1, 0);
                }
            }
        }
    }
}