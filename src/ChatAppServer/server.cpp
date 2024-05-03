#include "server.h"   

Server::Server() {}

UserAuthentication::UserAuthentication(){}

string Server::trim(const string& str) {
    // ... (trim implementation)
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == string::npos || last == string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}

string Server::receiveString(int clientSocket) {
    // ... (receiveString implementation)
    char buffer[4096];
    int bytesReceived = recv(clientSocket, buffer, 4096, 0);

    // Check if data reception was successful
    if (bytesReceived <= 0) {
        cerr << "Error: Received data from client failed!" << endl;
        return "";
    }
    return string(buffer, 0, bytesReceived);
}

void Server::setLoggedInUsers(unordered_map<int, string> loggedInUsers){
        this->loggedInUsers = loggedInUsers;
}

Client* Server::findClientByName(const string& name) {
    // ... (findClientByName implementation)
    lock_guard<mutex> guard(clientsMutex);
    auto it = find_if(clients.begin(), clients.end(), [this,name](const Client& client) {
        return trim(client.getName()) == trim(name);
    });
    return (it != clients.end()) ? &(*it) : nullptr;
}

void Server::handleClient(int clientSocket, const string& clientName, const string& roomName) 
{
    char buffer[4096];
    int bytesReceived;
    while (true) 
    {
        bytesReceived = recv(clientSocket, buffer, 4096, 0);

        if (bytesReceived <= 0) {
            handleClientOffline(clientSocket, clientName);
            return;
        }

        string receivedMessage(buffer, 0, bytesReceived);
        // Check the user that want to send the privare Message
        if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/pm") 
        {
            handlePrivateMessage(clientSocket, clientName, roomName, receivedMessage);
        } 

        else if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/sv")
        {
            string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
            cout << clientNameMessage << " wants to chat with the you." << endl;
            string messagedau=receivedMessage.substr(4);
            cout<<clientNameMessage<<": "<<messagedau<<endl;
            string Noti="\033[1;34m\n\n\t\tYou are in conversation with the server\033[0m";
            if (clientSocket){
                send(clientSocket,Noti.c_str(),Noti.size()+1,0);
            }
            handleServerChat(clientSocket, clientName);
        
        }
        else if(receivedMessage.substr(0, 9) == "sendfile") {
            handleFileTransfer(clientSocket, clientName,"R1");
        }
        else if (receivedMessage.substr(0,3)=="P2P"){
            string p2pName = receivedMessage.substr(4);
            cout<<"Reeceived the request to chat P2P from "<<clientName <<" to "<<p2pName<<endl;
            bytesReceived =recv(clientSocket,buffer,4096,0);
            string IPClient1(buffer,0,bytesReceived);
           string clientNameAndIP = "\033[1;34m" + clientName + " wants to chat with you. IP: " + IPClient1 + "\033[0m\nPlease reply with 'Y' to accept or 'N' to wait:   ";
           for (auto& client:clients){
                if (client.getName()==p2pName&&clientSocket!=client.getSocket()){
                    send(client.getSocket(),clientNameAndIP.c_str(),clientNameAndIP.size()+1,0);
                }
            }
        }
        else if (receivedMessage.substr(0,4)=="Room"||receivedMessage.substr(0,4)=="room")
        {
            handleGroupMessage(clientName, clientSocket);
        }
    }
}
  
void Server::handleClientOffline(int clientSocket, const string &clientName)
{
    cerr << "Client " << clientName << " offline!" << endl;

    // Lock the clients vector to ensure thread safety
    lock_guard<mutex> guard(clientsMutex);

    // Remove the disconnected client from the clients vector
    clients.erase(remove_if(clients.begin(), clients.end(), [clientSocket](const Client& client) {
        return client.getSocket() == clientSocket;
    }), clients.end());
    close(clientSocket);
}

void Server::handleGroupMessage(const string &clientName,  int clientSocket)
{
   
    char buffer[4096];
    int bytesReceived;
    if (clientSocket) {
        send(clientSocket, "Enter the RoomID to join the chat Room", strlen("Enter the RoomID to join the chat Room") + 1, 0);
    }

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        
        return;
    }
    string roomID = string(buffer, 0, bytesReceived);
    checkRoomIDs.emplace_back(clientSocket, roomID);
 
    while (true) {


        bool clientExists = false;
        for (auto &pair : checkRoomIDs) {
            if (pair.first == clientSocket) {
                // Upate the client 
                pair.second = roomID;
                clientExists = true;
                break;
            }
        }

        if (!clientExists) {
            checkRoomIDs.emplace_back(clientSocket, roomID);
        }
        vector<int> roomClients;
        for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID) {
                roomClients.push_back(pair.first);
            }
        }
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        
        string notiExit="\033[1;34m" + clientName  + ": " + "disconnected from the room" + "\033[0m";
        if (bytesReceived <= 0) {
            
            for (int socket : roomClients) {
                if (socket != clientSocket) {
                    send(socket,notiExit.c_str(), notiExit.size() + 1, 0);
                }
            }
              
            roomClients.erase(std::remove(roomClients.begin(), roomClients.end(), clientSocket), roomClients.end());
            break;
        }
        string groupMessage = "\033[1;31m" +roomID+"\033[0m" +"-"+"\033[1;34m" + clientName + "\033[0m" + ": " + string(buffer, 0, bytesReceived);

        //send the message to all client in the room
        if (strcmp(buffer, "sendfile") == 0){
            handleFileTransfer(clientSocket,clientName,roomID);
        }else if (strcmp(buffer, "sendfile") != 0&&strcmp(buffer, "out") != 0) {
            for (const auto &pair : checkRoomIDs) {
                if (pair.second == roomID && pair.first != clientSocket) {
                    send(pair.first, groupMessage.c_str(), groupMessage.size() + 1, 0);
                }
            }
        }
        
        if (strcmp(buffer, "out") == 0) {
            
            for (int socket : roomClients) {
                if (socket != clientSocket) {
                    send(socket, notiExit.c_str(), notiExit.size() + 1, 0);
                }
                string notiExit1="\033[1;34m\t\t\tYou are now out of the chat room\033[0m";
                if (socket == clientSocket) {
                    send(socket, notiExit1.c_str(), notiExit1.size() + 1, 0);
                }
            }

            //erase the client if the client exit the chat room
            auto iter = std::find_if(checkRoomIDs.begin(), checkRoomIDs.end(), 
                                        [clientSocket](const auto &pair) {
                                        return pair.first == clientSocket;
                                    });
            if (iter != checkRoomIDs.end()) {
                checkRoomIDs.erase(iter);
            }

            
            break;
        }

    }
}

void Server::handlePrivateMessage(int clientSocket, const string &clientName, [[maybe_unused]] const string &roomName, string receivedMessage)
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

void Server::handleServerChat(int clientSocket, const string &clientName)
{
    // Display a message indicating that the client wants to chat with the server
    string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
    // cout << clientNameMessage << " wants to chat with the you." << endl;

    // Loop for handling server chat
    string privateClientMessage;
    string userInput;
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

        int ready = select(max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
        string exitNotification = "\033[1;32m\n\t\tYou exited the private chat with SERVER\033[0m";

        if (ready > 0)
        {
            if (FD_ISSET(clientSocket, &readSet))
            {
                // Receive data from the client
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0)
                {
                    cerr << "Client " << clientName << " offline!" << endl;
                    break;
                }
                else {
                    // Process the received message from the client
                    privateClientMessage = string(buffer, 0, bytesReceived);
                    if (privateClientMessage.compare(0, 4, "exit") == 0)
                    {
                        cout <<clientName<< "\033[1;32m exited the private chat with You\033[0m" << endl;
                        send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
                        break;
                    }
                    cout << clientNameMessage << ": " << privateClientMessage << endl;
                }
            }

            if (FD_ISSET(STDIN_FILENO, &readSet))
            {
                getline(cin, userInput);
                if (userInput.compare(0, 4, "exit") == 0)
                {
                    cout << "\033[1;32m\n\t\tYou exited the private chat with SERVER\033[0m" << endl;
                    send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
                    break;
                }
                else {
                    // Handle user input and send messages to the server
                    size_t spacePos1 = userInput.find(' ');
                    if (spacePos1 != string::npos)
                    {
                        // Extract receiver's name and message
                        string firstPart = trim(userInput.substr(0, spacePos1));
                        string secondPart = trim(userInput.substr(spacePos1 + 1));
                        // Construct the message to be sent to the server
                        string privateServer = "\033[1;31mServer: \033[0m";
                        string messageOfServer = privateServer + secondPart;

                        // Send the message to the specified client
                        if (clients.size() == 1)
                        {
                            auto &client = clients[0];
                            if (firstPart == client.getName())
                            {
                                cout << "You sent a message to: " << firstPart << endl;
                                send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                            }
                        }
                        else {
                            for (auto &client : clients)
                            {
                                if (firstPart == client.getName())
                                {
                                    cout << "You sent a message to: " << firstPart << endl;
                                    send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
        // Sleep for a short interval to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void Server::handleFileTransfer(int clientSocket,string clientName,string roomID) {
    string abc = clientName;
    for (const auto &pair : checkRoomIDs) {
        if (pair.second == roomID && pair.first != clientSocket) {
            send(pair.first, "sendfile", strlen("sendfile") + 1, 0);
        }
    }
    char buffer[4096];
    int bytesReceived;
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    string filename = string(buffer, 0, bytesReceived);
    //send the file name
    for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID && pair.first != clientSocket) {
                send(pair.first, filename.c_str(), filename.size() + 1, 0);
            }
        }

    // Forward the data
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break;
        }
        if (strcmp(buffer, "exit") == 0) {
            for (const auto &pair : checkRoomIDs) {
                if (pair.second == roomID && pair.first != clientSocket) {
                    send(pair.first, "exit", strlen("exit") + 1, 0);
                }
            }
            break;
        }

        for (const auto &pair : checkRoomIDs) {
            if (pair.second == roomID && pair.first != clientSocket) {
                send(pair.first, buffer, bytesReceived, 0);
            }
        }
    }
    
    cout << "Successfully forwarded the file\n";
}

void Server::sendPrivateMessage(const string &senderName, const string &receiverName, const string &message)
{
    // Lock the clientsMutex to ensure thread safety
    lock_guard<mutex> guard(clientsMutex);

    // Find the sender in the clients array
    Client *sender = nullptr;
    for (auto &client : clients)
    {
        if (client.getName() == senderName) {
            sender = &client;
        }
    }

    // If the sender is found, iterate through all clients to find the receiver
    if (sender) {
        for (auto &client : clients)
        {
            if (client.getName() == receiverName)
            {
                send(client.getSocket(), message.c_str(), message.size() + 1, 0);
                return;
            }
        }
        cerr << "Error: Receiver not found!" << endl;
    }
    else {
        cerr << "Error: Sender not found!" << endl;
    }
}

void Server::processClient(int clientSocket)
{
    string roomName = receiveString(clientSocket);
    if (roomName.empty()) {
        cerr << "Error: Unable to receive room name. Closing connection." << endl;
        close(clientSocket);
        // continue;
    }

    // Check if the client is logged in
    if (loggedInUsers.find(clientSocket) == loggedInUsers.end()) {
        cerr << "Error: Client not logged in. Closing connection." << endl;
        close(clientSocket);
        return;
    }

    string clientName = loggedInUsers[clientSocket];

    lock_guard<mutex> guard(clientsMutex);
    clients.push_back(Client(clientSocket, clientName, roomName)); // Pass the username
    
    thread clientThread(&Server::handleClient, this, clientSocket, clientName, roomName); // Pass the username
    clientThread.detach();
}