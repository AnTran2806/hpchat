#include "server.h"

Connection::Connection(class Server& server) : server(server) {}

void Connection::start(int port)  {
    //create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Unable to create socket! Cancel..." << endl;
        return;
    }

    // establish IP server
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    int reuse = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        cerr << "Error setting socket option SO_REUSEADDR" << endl;
        close(serverSocket);
        return;
    }
    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Unable to bind socket." << endl;
        close(serverSocket);
        return;
    }

    // Listen
    if (listen(serverSocket, SOMAXCONN) == -1) {
        cerr << "Error while listening for connection." << endl;
        close(serverSocket);
        return;
    }


    cout << "Waiting for connection from client..." << endl;

    while (true) {
        // accept new connection
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (newClientSocket == -1) {
            cerr << "Error accepting connection." << endl;
            continue;
        }

        // Recieve the Option
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(newClientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            cerr << "Error receiving data from client." << endl;
            close(newClientSocket);
            continue;
        }

        string option(buffer, 0, bytesReceived);

        // Goto the handleAuthentication
        thread authenThread(&Server::handleAuthentication, &server, newClientSocket, option);
        authenThread.detach();
    }
}

// #include "server.h"

// Connection::Connection(class Server& server) : server(server) {}

// void Connection::start(int port) 
// {
//     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1)
//     {
//         cerr << "Unable to create socket! Cancel..." << endl;
//         // return false;
//         return;
//     }

//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(port);
//     inet_pton(AF_INET, "0.0.0.0", &serverAddr.sin_addr);

//     // Add the following code for socket reuse
//     // int reuse = 1;
//     // if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
//     //     cerr << "Error setting socket option SO_REUSEADDR" << endl;
//     //     close(serverSocket);
//     //     return;
//     // }

//     if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
//     {
//         cerr << "Unable to bind socket." << endl;
//         close(serverSocket);
//         // return false;
//         return;
//     }

//     // Listen for connections
//     if (listen(serverSocket, MAX_CLIENTS) == -1)
//     {
//         cerr << "Error while listening for connection." << endl;
//         close(serverSocket);
//         // return false;
//         return;
//     }

//     cout << "Waiting for connection from client..." << endl;

//     while (true)
//     {
//         FD_ZERO(&readfds);
//         FD_SET(serverSocket, &readfds);
//         maxSocket = serverSocket;

//         for (const int &clientSocket : clientSockets)
//         {
//             FD_SET(clientSocket, &readfds);
//             if (clientSocket > maxSocket)
//             {
//                 maxSocket = clientSocket;
//             }
//         }

//         // Use select to wait for events on sockets
//         int activity = select(maxSocket + 1, &readfds, nullptr, nullptr, nullptr);

//         if (activity < 0)
//         {
//             cerr << "Error when using select." << endl;
//             break;
//         }

//         // New connection
//         if (FD_ISSET(serverSocket, &readfds))
//         {
//             sockaddr_in clientAddr;
//             socklen_t clientAddrLen = sizeof(clientAddr);
//             int newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

//             if (newClientSocket < 0)
//             {
//                 cerr << "Error accepting connection." << endl;
//             }
//             else
//             {
//                 char clientIP[INET_ADDRSTRLEN];
//                 inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

//                 //cout << "Connected to the client with IP: " << "\033[1;32m" << clientIP << "\033[0m"<<endl;
//                 //printColoredIP(clientIP);
//                 //cout << endl;

//                 clientSockets.push_back(newClientSocket);
//             }
//         }
        
//         for (auto it = clientSockets.begin(); it != clientSockets.end();)
//         {
//             int clientSocket = *it;

//             if (FD_ISSET(clientSocket, &readfds))
//             {
//                 char buffer[BUFFER_SIZE];
//                 memset(buffer, 0, sizeof(buffer));
//                 int bytesReceived = recv(clientSocket, buffer, 4096, 0);

//                 if (bytesReceived <= 0)
//                 {
//                     cerr << "Connection closed." << endl;
//                     close(clientSocket);
//                     it = clientSockets.erase(it);
//                     continue; // Skip the rest of the loop and go back to the beginning.
//                 }
//                 else
//                 {
//                     string option;
//                     try {
//                         option = string(buffer, 0, bytesReceived);
//                     } catch (const invalid_argument& e) {
//                         cerr << "Invalid option received: " << e.what() << endl;
//                         // Handle the error, possibly by sending an error message to the client
//                         // and asking for input again.
//                         continue; // Skip the rest of the loop and go back to the beginning.
//                     }

//                     thread authenThread(&Server::handleAuthentication, &server, clientSocket, option);
//                     authenThread.join();
//                     ++it;
//                 }
//             }
//             else
//             {
//                 ++it;
//             }
//         }
//     }
// }