#include "client.h"

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
                connectToServer(IP_SErver, 54001);
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
