#include "chatService.h"

void ChatService::handlePrivateMessage(int clientSocket, const string &clientName, [[maybe_unused]] const string &roomName, string receivedMessage)
{
    char buffer[4096];
    int bytesReceived;
    // Find the position of the first space in the received message
    size_t spacePos = receivedMessage.find(' ', 4);
    if (spacePos != string::npos)
    {
        string receiverName = trim(receivedMessage.substr(4, spacePos - 4));
        string privateMessage = trim(receivedMessage.substr(spacePos + 1));
        // Loop to handle private chat
        while (true)
        {
            if (privateMessage.substr(0, 4) == "exit")
            {
                // Notify the client that they exited the private chat
                string exitNotification  = "\033[1;32mYou exit the private chats and come back to the room chats!\033[0m";
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
                string privateChatHeader = "\033[1;31mPrivate chat from>>\033[0m";
                string clientNameColor = "\033[1;34m" + clientName + "\033[0m";
                string messageColor = "\033[1;37m" + privateMessage + "\033[0m";

                sendPrivateMessage(clientName, 
                                   receiverName, 
                                   privateChatHeader + " " + 
                                   clientNameColor + ": " + 
                                   messageColor);
                bytesReceived = recv(clientSocket, buffer, 4096, 0);

                if (bytesReceived <= 0) {
                    cerr << "Client " << clientName << " offline!" << endl;
                    return;
                }
                // Update privateMessage with the received response
                privateMessage = string(buffer, 0, bytesReceived);
            }
        }
    }
    else {
        // Notify the client about incorrect private message format
        string errorMsgFormat = "\033[1;33m Error: Incorrect private message format!\n"
                                     "Correct is : /pm + User's name + your message!\n"
                                     "The list users are online : \n\033[0m";
        string clientNamesList;
        for (const auto &client : clients)
        {
            clientNamesList += client.getName() + "\n";
        }
        string errMsgWithClientNamesLst = errorMsgFormat + clientNamesList;
        for (const auto &client : clients)
        {
            if (client.getSocket() == clientSocket)
            {
                send(client.getSocket(), errMsgWithClientNamesLst.c_str(), errMsgWithClientNamesLst.size() + 1, 0);
            }
        }
    }
}