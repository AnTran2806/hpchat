#include "server.h"

void Server::handlePrivateMessage(int clientSocket, const std::string &clientName, const std::string &roomName, std::string receivedMessage)
{
    char buffer[4096];
    int bytesReceived;
    size_t spacePos = receivedMessage.find(' ', 4);
    if (spacePos != std::string::npos)
    {
        std::string receiverName = trim(receivedMessage.substr(4, spacePos - 4));
        std::string privateMessage = trim(receivedMessage.substr(spacePos + 1));
        while (true)
        {
            if (privateMessage.substr(0, 4) == "exit")
            {
                std::string NotificationExitPrivateChat = "\033[1;32mYou exited the private chat and came back to the room chat hiiiiihihihihihihihihihihi\033[0m";
                for (const auto &client : clients)
                {
                    if (client.getSocket() == clientSocket)
                    {
                        send(client.getSocket(), NotificationExitPrivateChat.c_str(), NotificationExitPrivateChat.size() + 1, 0);
                    }
                }
                break;
            }
            else
            {
                std::string Private = "\033[1;31mPrivate chat from>>\033[0m";
                std::string clientNameColor = "\033[1;34m" + clientName + "\033[0m";
                std::string messageColor = "\033[1;37m" + privateMessage + "\033[0m";

                sendPrivateMessage(clientName, receiverName, Private + " " + clientNameColor + ": " + messageColor);
                bytesReceived = recv(clientSocket, buffer, 4096, 0);
                // handleClientOffline(clientSocket,clientName,roomName,bytesReceived);
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
        std::string Notification1 = "\033[1;33m Error: Incorrect private message format!\n Correct is : /pm + User's name + your message!\n The list users are online : \n\033[0m";
        std::string clientNamesList1;
        for (const auto &client : clients)
        {
            clientNamesList1 += client.getName() + "\n";
        }
        std::string A = Notification1 + clientNamesList1;
        for (const auto &client : clients)
        {
            if (client.getSocket() == clientSocket)
            {
                send(client.getSocket(), A.c_str(), A.size() + 1, 0);
            }
        }
    }
}