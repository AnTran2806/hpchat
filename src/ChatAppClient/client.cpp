#include "client.h"

Client::Client(int clientSocket) : clientSocket(clientSocket), connection(*this) {}

Client::~Client(){
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

int Client::getSocket() const {
    return clientSocket;
}

void Client::stopClient(int clientSocket){
    close(clientSocket);
}

Client::Client() : connection(*this) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Error: Create the socket" << endl;
        return;
    }
}

void sendToServer(int socket,const string& message) {
    send(socket, message.c_str(), message.size() + 1, 0);
}

string Client:: getIPAddress() {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            if (strcmp(tmp->ifa_name, "lo") != 0) { // Ignore loopback interface
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(pAddr->sin_addr), ip, INET_ADDRSTRLEN);
                freeifaddrs(addrs);
                return string(ip);
            }
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
    return "";
}

void Client::startReceiving(int clientSocket) {
    // Create a new thread to handle receiving messages from the server
    receivingThread = thread([this,clientSocket]() {
        char buffer[4096]; 
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                cerr << "Error: Server is down" << endl;
                close(clientSocket);
                break;
            }
            // Check if the received message indicates a file is being sent
            if (strcmp(buffer, "sendfile") == 0) {
                cout<<"Receiving a file from another client!";
                receiveFile(clientSocket);
            }
            else if (strcmp(buffer, "out") == 0) {
                string message = "Bye!";
                sendToServer(clientSocket, message);
                close(clientSocket);
                break;
            }
            else {
                cout << string(buffer, 0, bytesReceived) << endl;
            }
        }
    });
}

void Client::processFunction() {
    string ipAddress = getIPAddress();
    cout << "\t\t\tWelcome!\n";

    while (true) {
        string userInput;
        cout << "You: ";
        getline(cin, userInput);

        if (userInput == "sendfile") {
            sendToServer(clientSocket, "sendfile");
            sendFile(clientSocket);
        }
        else if (userInput == "P2P") {
            string P2P_Info = "P2P ";
            cout << "The client that you want to chat: \t";
            string P2P_UserName;
            getline(cin, P2P_UserName);
            string P2P = P2P_Info + P2P_UserName;
            sendToServer(clientSocket, P2P);
            sendToServer(clientSocket, ipAddress);
            cout << "Waiting for the client to accept your connection....\n";
            server->becomeServer(PORT + 1, P2P_UserName);

            bool exitServer = false;
            while (!exitServer) {
                string inputServer;
                getline(cin, inputServer);
                
                if (inputServer == "out") {
                    sendToServer(server->clientSock, "\nYour friend exited the chat. Enter 'out' to leave the chat!\n");
                    server->stopServer();
                    exitServer = true;
                }
                else if(inputServer=="sendfile") {
                    sendToServer(server->clientSock, "sendfile");
                    sendFile(server->clientSock);
                }else {
                    sendToServer(server->clientSock, inputServer);
                }
            }
            continue;
        }
        else if (userInput == "N" || userInput == "n" || userInput == "Y" || userInput == "y") {
            if (userInput == "N" || userInput == "n") {
                cout << "Enter 'Y' at any time to accept 1-1 Chat.\n";
            }
            else {
                cout << "Enter the new IP_Server: ";
                string IP_SErver;
                getline(cin, IP_SErver);
                connection.connectToServer(IP_SErver, 54001);
                startReceiving(getSocket());
                cout << "\n\t\t\tWelcome to the Private chat\n";
                while (true) {
                    string IPUT;
                    getline(cin, IPUT);
                    if (IPUT == "out") {
                        cout << "You exited the Chat!\n";
                        sendToServer(getSocket(), IPUT);
                        stopClient(getSocket());
                        break;
                    }
                    else if (IPUT == "sendfile"){
                        sendToServer(getSocket(), "sendfile");
                        sendFile(getSocket());
                    }
                    else {
                        sendToServer(getSocket(), IPUT);
                    }
                }
            }
            continue;
        }
        else if (userInput == "help") {
            cout << "1. Chat Group: Enter Room then create the new room\n"<< "2. Private chat:/pm + User's name + your message.\n"<< "3. Server: /sv + Your message\n"<< "4. Sendfile: Enter the sendfile to start forwarding the file.\n"<< "5. /pm + any word to display the list of users currently online"<< endl;
        }
        else {
            sendToServer(clientSocket, userInput);
        }
    }
}

void Client::sendFile(int clientSocket) {
    string filePath;
        cout << "Enter the file path to send: ";
        getline(cin, filePath);
        sendToServer(clientSocket, filePath);
        cout << "File Name: " << filePath << endl;
        // thread sendFileThread(&SharingFile::sendFile,this, clientSocket, filePath);
        // sendFileThread.detach();

    ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << filePath << std::endl;
        return;
    }

    // send the byte of the file before sending the data
    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    send(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

    char buffer[4096];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        send(clientSocket, buffer, file.gcount(), 0);
    }
    file.close();
    cout << " File sent successfully, please enter the 'exit' to end this.\n";       
}

void Client::receiveFile(int clientSocket) {
    char filename[256];
    read(clientSocket, filename, sizeof(filename));
    cout << "File Name: " << filename << endl;

    // Create the time to receive the file
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char timeBuffer[32];
    strftime(timeBuffer, sizeof(timeBuffer), "%d%H%M%S", localTime);

    // instert the time with the file name
    string newFilename = string(timeBuffer) + "_" + string(filename);

    cout << "Receiving file..." << endl;

    ofstream file(newFilename,ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << newFilename << endl;
        return;
    }

    int fileSize;
    recv(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

    char buffer[4096];
    int totalReceived = 0;
    while (totalReceived < fileSize) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cerr << "Error: Server is down" << endl;
            close(clientSocket);
            return;
        }
        if (strcmp(buffer,"exit")==0){
            break;
        }
        file.write(buffer, bytesReceived);
        totalReceived += bytesReceived;
    }

    file.close();
    cout << "File received successfully: " << newFilename << endl;
}
