#include "server.h"

void Server::handlePrivateMessage(int clientSocket, const std::string &clientName, [[maybe_unused]] const std::string &roomName, std::string receivedMessage)
{
    char buffer[4096];
    int bytesReceived;
    // Find the position of the first space in the received message
    size_t spacePos = receivedMessage.find(' ', 4);
    if (spacePos != std::string::npos)
    {
        std::string receiverName = trim(receivedMessage.substr(4, spacePos - 4));
        std::string privateMessage = trim(receivedMessage.substr(spacePos + 1));
        // Loop to handle private chat
        while (true)
        {
            if (privateMessage.substr(0, 4) == "exit")
            {
                // Notify the client that they exited the private chat
                std::string exitNotification  = "\033[1;32mYou exit the private chats and come back to the room chats!\033[0m";
                for (const auto &client : clients)
                {
                    if (client.getSocket() == clientSocket) {
                        send(client.getSocket(), exitNotification.c_str(), exitNotification.size() + 1, 0);
                    }
                }
                break;
            }
            else {
                // Prepare private chat message with colors
                std::string privateChatHeader = "\033[1;31mPrivate chat from>>\033[0m";
                std::string clientNameColor = "\033[1;34m" + clientName + "\033[0m";
                std::string messageColor = "\033[1;37m" + privateMessage + "\033[0m";

                sendPrivateMessage(clientName, 
                                   receiverName, 
                                   privateChatHeader + " " + 
                                   clientNameColor + ": " + 
                                   messageColor);
                bytesReceived = recv(clientSocket, buffer, 4096, 0);

                if (bytesReceived <= 0) {
                    std::cerr << "Client " << clientName << " offline!" << std::endl;
                    return;
                }
                // Update privateMessage with the received response
                privateMessage = std::string(buffer, 0, bytesReceived);
            }
        }
    }
    else {
        // Notify the client about incorrect private message format
        std::string errorMsgFormat = "\033[1;33m Error: Incorrect private message format!\n"
                                     "Correct is : /pm + User's name + your message!\n"
                                     "The list users are online : \n\033[0m";
        std::string clientNamesList;
        for (const auto &client : clients)
        {
            clientNamesList += client.getName() + "\n";
        }
        std::string errMsgWithClientNamesLst = errorMsgFormat + clientNamesList;
        for (const auto &client : clients)
        {
            if (client.getSocket() == clientSocket)
            {
                send(client.getSocket(), errMsgWithClientNamesLst.c_str(), errMsgWithClientNamesLst.size() + 1, 0);
            }
        }
    }
}