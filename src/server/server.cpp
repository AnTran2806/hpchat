#include "server.h"

UserAuthentication::UserAuthentication() : username(""), password("") {}

bool UserAuthentication::isUserRegistered(const string& checkUsername) {
    // Implementation here
    ifstream inputFile("data\\users.txt");
    string username;
    while (getline(inputFile, username)) {
        // Check if the current line is the username
        if (username == checkUsername) {
            inputFile.close();
            return true; // User is already registered
        }

        // Skip the next line (password)
        if (!getline(inputFile, username)) {
            break;  // Break if unable to read the next line (no password found)
        }
    }

    inputFile.close();
    return false; // User is not registered
}

bool UserAuthentication::registerUser(const string& enteredUsername, const string& enteredPassword) {
    // Implementation here
    username = enteredUsername;
    password = enteredPassword;

    // ofstream myFile("data\\" + username + ".txt");
    ofstream myFile("data\\users.txt", ios::app);  // Open the file to continue writing to the last file
    if (myFile.is_open()) {
        myFile << username << endl << password << endl;
        myFile.close();
        return true;
    } else {
        cerr << "Unable to open file" << endl;
        return false;
    }
}

bool UserAuthentication::isLoggedIn(const string& enteredUsername, const string& enteredPassword) {
    string storedUsername, storedPassword;

    // ifstream inputFile("./data/data_users/users.txt");
    ifstream inputFile("data\\users.txt");
    if (inputFile.is_open()) {
        while (getline(inputFile, storedUsername)) {
            getline(inputFile, storedPassword);

            if (storedUsername == enteredUsername && storedPassword == enteredPassword) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
    }
    return false;
}

Client::Client(int socket, const string& name, const string& roomName) : socket(socket), name(name), roomName(roomName) {}

int Client::getSocket() const {
    return socket;
}

const string& Client::getName() const {
    return name;
}

const string& Client::getRoomName() const {
    return roomName;
}

// Server::Server() {
//     // ... (constructor implementation)
//     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         cerr << "Can't create the socket" << endl;
//         return;
//     }

//     sockaddr_in hint;
//     hint.sin_family = AF_INET;
//     hint.sin_port = htons(54000);
//     hint.sin_addr.s_addr = INADDR_ANY;

//     if (bind(serverSocket, (sockaddr*)&hint, sizeof(hint)) == -1) {
//         cerr << "Bind failed!" << endl;
//         close(serverSocket);
//         return;
//     }

//     if (listen(serverSocket, SOMAXCONN) == -1) {
//         cerr << "Listen failed!" << endl;
//         close(serverSocket);
//         return;
//     }
// }

Client* Server::findClientByName(const string& name) {
    // ... (findClientByName implementation)
    lock_guard<mutex> guard(clientsMutex);
    auto it = find_if(clients.begin(), clients.end(), [this,name](const Client& client) {
        return trim(client.getName()) == trim(name);
    });
    return (it != clients.end()) ? &(*it) : nullptr;
}

void Server::handlePrivateMessage(const string& senderName, const string& receiverName, const string& message) {
    // ... (handlePrivateMessage implementation)
    // Lock the clientsMutex to ensure thread safety
    lock_guard<mutex> guard(clientsMutex);

    // Find the sender in the clients array
    Client* sender = nullptr;
    for (auto& client : clients) {
        if (client.getName() == senderName) {
            sender = &client;
        }
    }

    // If the sender is found, iterate through all clients to find the receiver
    if (sender) {
        for (auto& client : clients) {
            if (client.getName() == receiverName) {
                
                //int sendResult = sendPrivateMessage(client.getSocket(), message);Sai
                send(client.getSocket(),message.c_str(),message.size()+1,0);
                
                //cout << "Send result: " << sendResult << endl;
                // Print the private message sent from sender to receiver
                // cout << "Private message sent from " << senderName << " to " << receiverName << ": " << message << endl;
                return;  // Exit the function after sending the message to one receiver
            }
        }
        // Print an error message if the receiver is not found
        cerr << "Error: Receiver not found!" << endl;
    } else {
        // Print an error message if the sender is not found
        cerr << "Error: Sender not found!" << endl;
    }
}

bool Server::handleRegistration(int clientSocket)
    {
        char usernameBuffer[1024] = {0};
        char passwordBuffer[1024] = {0};

        recv(clientSocket, usernameBuffer, sizeof(usernameBuffer), 0);
        recv(clientSocket, passwordBuffer, sizeof(passwordBuffer), 0);

        string username(usernameBuffer);
        string password(passwordBuffer);

        bool status = auth.isUserRegistered(username);
        status = !status && auth.registerUser(username, password);
        const char *response;
        if (status)
        {
            response = "Registration successful.";
        }
        else
        {
            response = "Username already exists. Choose a different username.";
        }
        send(clientSocket, response, strlen(response), 0);
        return status;
    }

bool Server::handleLogin(int clientSocket)
{
    char usernameBuffer[1024] = {0};
    char passwordBuffer[1024] = {0};

    read(clientSocket, usernameBuffer, 1024);
    read(clientSocket, passwordBuffer, 1024);

    string username(usernameBuffer);
    string password(passwordBuffer);
    bool status = auth.isLoggedIn(username, password);
    const char *response = status ? "Login successful." : "Login failed.";
    send(clientSocket, response, strlen(response), 0);
    return status;
}

void Server::handleAuthentication(int clientSocket, int option)
    {
        bool check = false;
        cout << option << endl;
        string username;
        while(!check)
        {   
            
            cout << "Need to sign in or sign up" << endl;
            char buffer[4096];
            // int option;
            // try {
            //     option = stoi(string(buffer, 0, bytesReceived));
            // } catch (const std::invalid_argument& e) {
            //     cerr << "Invalid option received: " << e.what() << endl;
            //     // Handle the error, possibly by sending an error message to the client
            //     // and asking for input again.
            //     continue; // Skip the rest of the loop and go back to the beginning.
            // }

            if (option == 1)
            {
                // Register
                cout<<"Registration"<<endl;
                check = handleRegistration(clientSocket);
            }
            else if (option == 2)
            {
                // Login
                cout<<"Login"<<endl;
                check = handleLogin(clientSocket);
                if (check) {
                    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                    username = string(buffer, 0, bytesReceived); // Store the username
                }
            }
        }
        string roomName = receiveString(clientSocket);
        if (roomName.empty()) {
            close(clientSocket);
            // continue;
        }

        // Receive client name from client
        // string clientName = receiveString(clientSocket);
        // if (clientName.empty()) {
        //     close(clientSocket);
        //     continue;
        // }

        lock_guard<mutex> guard(clientsMutex);
        // clients.push_back(Client(clientSocket, clientName, roomName));
        clients.push_back(Client(clientSocket, username, roomName)); // Pass the username

        // thread clientThread(&Server::handleClient, this, clientSocket, clientName, roomName);
        thread clientThread(&Server::handleClient, this, clientSocket, username, roomName); // Pass the username
        clientThread.detach();
    }

Server::Server(){}

void Server::start(int port) {
    // ... (start implementation)
    // while (true) {
    //     sockaddr_in clientAddr;
    //     socklen_t clientSize = sizeof(clientAddr);
    //     int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    //     if (clientSocket == -1) {
    //         cerr << "Error when create the socket from client" << endl;
    //         continue;
    //     }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Unable to create socket! Cancel..." << endl;
        // return false;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &serverAddr.sin_addr);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Unable to bind socket." << endl;
        close(serverSocket);
        // return false;
        return;
    }

    // Listen for connections
    if (listen(serverSocket, MAX_CLIENTS) == -1)
    {
        cerr << "Error while listening for connection." << endl;
        close(serverSocket);
        // return false;
        return;
    }

    cout << "Waiting for connection from client..." << endl;

    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        maxSocket = serverSocket;

        for (const int &clientSocket : clientSockets)
        {
            FD_SET(clientSocket, &readfds);
            if (clientSocket > maxSocket)
            {
                maxSocket = clientSocket;
            }
        }

        // Use select to wait for events on sockets
        int activity = select(maxSocket + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity < 0)
        {
            cerr << "Error when using select." << endl;
            break;
        }

        // New connection
        if (FD_ISSET(serverSocket, &readfds))
        {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

            if (newClientSocket < 0)
            {
                cerr << "Error accepting connection." << endl;
            }
            else
            {
                cout << "Connected to the client." << endl;
                clientSockets.push_back(newClientSocket);
            }
        }
        
        for (auto it = clientSockets.begin(); it != clientSockets.end();)
        {
            cout <<"abc"<<endl;
            int clientSocket = *it;

            if (FD_ISSET(clientSocket, &readfds))
            {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer));
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

                cout << "----------" << bytesReceived << endl;
                if (bytesReceived <= 0)
                {
                    cerr << "Connection closed." << endl;
                    close(clientSocket);
                    it = clientSockets.erase(it);
                }
                else
                {
                    thread processThread(&Server::handleAuthentication, this, clientSocket, bytesReceived);
                    cout <<"dô luồng" << endl;
                    processThread.detach();
                    // cout << "Client " << clientSocket << ": " << buffer << endl;

                    // // Send data to client
                    // cout << "Server: ";
                    // cin.getline(buffer, sizeof(buffer));
                    // send(clientSocket, buffer, strlen(buffer), 0);
                    // handleAuthentication(clientSocket);   
                    // handleAuthentication(clientSocket, bytesReceived); // Get the username
                    // Receive room name from client
                    
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }
    }
}

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

void Server::handleClient(int clientSocket, const string& clientName, const string& roomName) {
    // ... (handleClient implementation)
    char buffer[4096];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived <= 0) {
                cerr << "Client " << clientName << " offline!" << endl;
                lock_guard<mutex> guard(clientsMutex);
                clients.erase(remove_if(clients.begin(), clients.end(), [clientSocket](const Client& client) {
                    return client.getSocket() == clientSocket;
                }), clients.end());
                close(clientSocket);
                return;
            
        }

        string receivedMessage(buffer, 0, bytesReceived);


        // Check the user that want to send the privare Message
        if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/pm") {
            //handleSendPrivateMessage(receivedMessage,clientSocket,bytesReceived,clientName, buffer);
            //format "/pm receiverName message"
            size_t spacePos = receivedMessage.find(' ', 4);
            if (spacePos != string::npos) {
                string receiverName = trim(receivedMessage.substr(4, spacePos - 4));
                string privateMessage = trim(receivedMessage.substr(spacePos + 1));
        
                // Handle to send the private file
                while (true) {
                    if (privateMessage.substr(0, 4) == "exit") {
                        string NotificationExitPrivateChat ="\033[1;32mYou exited the private chat and came back to the room chat hiiiiihihihihihihihihihihi\033[0m";
                        for (const auto& client : clients) {
                            if ( client.getSocket()== clientSocket) {
                                send(client.getSocket(), NotificationExitPrivateChat.c_str(), NotificationExitPrivateChat.size() + 1, 0);
                            }
                        }
                        break;
                    } else {
                        string Private = "\033[1;31mPrivate chat from>>\033[0m";
                        string clientNameColor = "\033[1;34m" + clientName + "\033[0m";
                        string messageColor = "\033[1;37m" + privateMessage + "\033[0m";

                        handlePrivateMessage(clientName, receiverName, Private + " " + clientNameColor + ": " + messageColor);
                        bytesReceived = recv(clientSocket, buffer, 4096, 0);
                        //handleClientOffline(clientSocket,clientName,roomName,bytesReceived);
                        if (bytesReceived <= 0) {
                            cerr << "Client " << clientName << " offline!" << endl;
                            return;
                        }
                        privateMessage = string(buffer, 0, bytesReceived);
                    }
                }
            } else {
                string Notification1 ="\033[1;33m Error: Incorrect private message format!\n Correct is : /pm + User's name + your message!\n The list users are online : \n\033[0m";
                string clientNamesList1;
                for (const auto& client : clients) {
                    clientNamesList1 += client.getName() + "\n"; 
                }
                string A=Notification1 + clientNamesList1;
                for (const auto& client : clients) {
                    if ( client.getSocket()== clientSocket) {
                        send(client.getSocket(), A.c_str(), A.size() + 1, 0);
                    }
                }
            }
        } 
            ///chat with server and client
            //////Duc is added // c

        else if (receivedMessage.size() > 3 && receivedMessage.substr(0, 3) == "/sv") {
                size_t spacePos = receivedMessage.find(' ');
                if (spacePos != string::npos) {
                    string privateClientMessage = receivedMessage.substr(spacePos + 1);
                    string clientNameMessage;

                    for (const auto& client : clients) {
                        if (client.getSocket() == clientSocket) {
                            clientNameMessage = "\033[1;34m" + client.getName() + "\033[0m";
                                //cout << clientNameMessage << ":" << privateClientMessage << endl;

                            }
                        }
                    cout << clientNameMessage << ":" << privateClientMessage << endl;

                    string YouLocation = "\033[1;32m A new client want to chat >>\033[0m";
                    for (const auto& client : clients) {
                        if (client.getSocket() == clientSocket) {
                            cout << YouLocation << client.getName() << endl;
                             break; // Thoát khỏi vòng lặp khi client đã thoát
                            }
                    }

                        
                    string userInput;
                     bool exitChat = false;
                    // bool exitChatRequested = false;

                        while (!exitChat) {
                            fd_set readSet;
                            FD_ZERO(&readSet);
                            FD_SET(STDIN_FILENO, &readSet);
                            FD_SET(clientSocket, &readSet);

                            struct timeval timeout;
                            timeout.tv_sec = 0;
                            timeout.tv_usec = 0;

                            int ready = select(max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
                            string NotificationExitPrivateChatServer = "\033[1;32mYou exited the private chat with SERVER\033[0m";
                            if (ready > 0) {
                                if (FD_ISSET(clientSocket, &readSet)) {
                                    bytesReceived = recv(clientSocket, buffer, 4096, 0);
                                    if (bytesReceived <= 0) {
                                        cerr << "Client " << clientName << " offline!" << endl;
                                        exitChat = true; // Set exitChat khi client offline
                                    } else {
                                        privateClientMessage = string(buffer, 0, bytesReceived);
                                        if (privateClientMessage.compare(0, 4, "exit") == 0) {
                                            exitChat = true;
                                            cout << "\033[1;32mYou exited the private chat with \033[0m"  <<clientName<< endl;
                                            send(clientSocket, NotificationExitPrivateChatServer.c_str(), NotificationExitPrivateChatServer.size() + 1, 0);
                                        }
                                        cout << clientNameMessage << ":" << privateClientMessage << endl;
                                    }
                                }

                                
                    if (FD_ISSET(STDIN_FILENO, &readSet)) {
                    getline(cin, userInput);
                    if (userInput.compare(0, 4, "exit") == 0 ) {
                        exitChat = true;
                        send(clientSocket, NotificationExitPrivateChatServer.c_str(), NotificationExitPrivateChatServer.size() + 1, 0);
                        cout << "\033[1;32mYou exited the private chat with \033[0m" << clientNameMessage << endl;
                    } else {
                        size_t spacePos1 = userInput.find(' ');

                        if (spacePos1 != string::npos) {
                            string firstPart = trim(userInput.substr(0, spacePos1));
                            string secondPart = trim(userInput.substr(spacePos1 + 1));

                            string privateServer = "\033[1;31mServer: \033[0m";
                            string messageOfServer = privateServer + secondPart;

                            // Only one client want to chat with server
                            if (clients.size() == 1) {
                                auto& client = clients[0];
                                if (firstPart == client.getName() ) {
                                    cout << "You sent a message to: " << firstPart << endl;
                                    send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                                }
                            } else {
                                // if have more one client want to chat with server
                                for (auto& client : clients) {
                                    if (firstPart == client.getName() ) {
                                        cout << "You sent a message to: " << firstPart << endl;
                                        send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                                        
                                    }
                                }
                            }
                        }
                    }
                }

            }
            // Sleep to reduce CPU
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }

}
            
            ////////added
        //send file huihu dw
        else if(receivedMessage.substr(0, 9) == "/sendfile") {
            std ::string SendFile ="File is being sent from ";
            cout<< SendFile <<endl;
        }
        
        else {
            // Handle to send message to the same roomName
            string groupMessage = "\033[1;34m" + clientName +"\033[0m" + ": " + receivedMessage;
            lock_guard<mutex> guard(clientsMutex);
            for (const auto& client : clients) {
                if (client.getRoomName() == roomName && client.getSocket() != clientSocket) {
                    string tenPhong = "\033[1;35m" + client.getRoomName()+ "\033[0m" +">>" + groupMessage ;
                    send(client.getSocket(), tenPhong.c_str(), tenPhong.size() + 1, 0);
                }
            }
        }
    }
}