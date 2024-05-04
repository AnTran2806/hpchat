#include "client.h"

void Client::processFunction() {
    string ipAddress = getIPAddress();
    cout << "\t\t\tWelcome!\n";

    while (true) {
        string userInput;
        cout << "You: ";
        getline(cin, userInput);

        if (userInput == "sendfile") {
            sendToServer(sock, "sendfile");
            sendFile(sock);
        }
        else if (userInput == "P2P") {
            string infP2P = "P2P ";
            cout << "The client that you want to chat: \t";
            string userNameP2P;
            getline(cin, userNameP2P);
            string lenhP2P = infP2P + userNameP2P;
            sendToServer(sock, lenhP2P);
            sendToServer(sock, ipAddress);
            cout << "Waiting for the client to accept your connection....\n";
            Server server1;
            server1.becomeServer(PORT + 1,userNameP2P);

            bool exitServer = false;
            while (!exitServer) {
                string inPutServer;
                getline(cin, inPutServer);
                
                
                if (inPutServer == "out") {
                    sendToServer(server1.AclientSock, "\nYour friend exited the chat. Enter 'out' to leave the chat!\n");
                    server1.stopServer();
                    exitServer = true;
                }
                else if(inPutServer=="sendfile") {
                    sendToServer(server1.AclientSock, "sendfile");
                    sendFile(server1.AclientSock);
                }else {
                    sendToServer(server1.AclientSock, inPutServer);
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
                Client client1;
                client1.connectToServer(IP_SErver, 54001);
                client1.startReceiving(client1.getSocket());
                cout << "\n\t\t\tWelcome to the Private chat\n";
                while (true) {
                    string IPUT;
                    getline(cin, IPUT);
                    if (IPUT == "out") {
                        cout << "You exited the Chat!\n";
                        sendToServer(client1.getSocket(), IPUT);
                        stopClient(client1.getSocket());
                        break;
                    }
                    else if (IPUT == "sendfile"){
                        sendToServer(client1.getSocket(), "sendfile");
                        sendFile(client1.getSocket());
                    }
                    else {
                        sendToServer(client1.getSocket(), IPUT);
                    }
                }
            }
            continue;
        }
        else if (userInput == "help") {
            cout << "1. Chat Group: Enter Room then create the new room\n"<< "2. Private chat:/pm + User's name + your message.\n"<< "3. Server: /sv + Your message\n"<< "4. Sendfile: Enter the sendfile to start forwarding the file.\n"<< "5. /pm + any word to display the list of users currently online"<< endl;
        }
        else {
            sendToServer(sock, userInput);
        }
    }
}
