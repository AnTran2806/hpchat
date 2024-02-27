#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>
#include <cctype>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fstream>
#include <vector>
#include <ifaddrs.h>


using namespace std;

const int PORT = 55000;

class Client {
private:
    int sock;
    thread receivingThread;

    
public:
    Client();

    ~Client();
    
    bool connectToServer(const string& ipAddress, int port);
    
    void runMessageReceiver();

    void startReceiving();

    void sendToServer(const string& message);

    void handleUserInteraction();

    void enterCredential();

    void enterRoomName();

    void processFunction();

    string trim(const string& str);
    
    void sendFile(const string& filePath);

    void receiveFile();

};


//
class AServer{
    private:
        int AserverSock;
        vector<int> AclientSockets;
        thread AserverThread;
        bool AserverRunning=false;
    public:
        AServer(){

        }
        ~AServer(){
            stopServer();
        }
        void stopServer() {
            if (AserverRunning) {
                for (int clientSock : AclientSockets) {
                    close(clientSock);
                }
                close(AserverSock);
                if (AserverThread.joinable()) {
                    AserverThread.join();
                }
                AserverRunning = false;
            }
        }
        bool AstartServer(int port) {
            if (AserverRunning) {
                cerr << "Error: Server is already running" << endl;
                return false;
            }

            AserverSock = socket(AF_INET, SOCK_STREAM, 0);
            if (AserverSock == -1) {
                cerr << "Error: Create the server socket" << endl;
                return false;
            }

            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(port);
            hint.sin_addr.s_addr = INADDR_ANY;

            if (bind(AserverSock, (sockaddr*)&hint, sizeof(hint)) == -1) {
                cerr << "Error: Cannot bind to port" << endl;
                close(AserverSock);
                return false;
            }

            if (listen(AserverSock, SOMAXCONN) == -1) {
                cerr << "Error: Cannot listen on port" << endl;
                close(AserverSock);
                return false;
            }

            AserverRunning = true;

            AserverThread = thread([this, port]() {
                while (true) {
                    int AclientSock = accept(AserverSock, nullptr, nullptr);
                    if (AclientSock == -1) {
                        cerr << "Error: Cannot accept client connection" << endl;
                        continue;
                    }
                    cout << "Client connected to server on port " << port << endl;

                    AclientSockets.push_back(AclientSock);
                    thread AclientThread(&AServer::AhandleClientMessages, this, AclientSock);
                    AclientThread.detach();
                }
            });

            return true;
        }
        void AhandleClientMessages(int clientSock) {
            char buffer[4096];
            while (true) {
                int bytesReceived = recv(clientSock, buffer, 4096, 0);
                if (bytesReceived <= 0) {
                    cerr << "Error: Client disconnected" << endl;
                    close(clientSock);
                    break;
                }
                cout << "Client: " << string(buffer, bytesReceived) <<  endl;
            }
        }
        void sendMessageToAllClients(const std::string& message) {
            for (int clientSock : AclientSockets) {
                send(clientSock, message.c_str(), message.size() + 1, 0);
            }
        }
        
};

class AClient{
    private:
        vector<int> AserverSockets;
        thread AreceivingThread;
        bool AreceivingRunning = false;
    public:
        AClient(){}
        ~AClient(){
            if (AreceivingThread.joinable()) {
                AreceivingThread.join();
            }
        }

        bool AconnectToServer(const string& ipAddress, int port) {
            int Asock = socket(AF_INET, SOCK_STREAM, 0);
            if (Asock == -1) {
                cerr << "Error: Create the socket" << endl;
                return false;
            }

            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(port);
            inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

            int connectRes = connect(Asock, (sockaddr*)&hint, sizeof(hint));
            if (connectRes == -1) {
                cerr << "Error: Cannot connect to Server at " << ipAddress << ":" << port << endl;
                close(Asock);
                return false;
            }

            AserverSockets.push_back(Asock);
            return true;
        }

        void AstartReceiving(){
            if (AreceivingRunning){
                cerr<<"""Error: Receiving thread is already running\n"; 
            }
            AreceivingThread = thread([this]() {
                char buffer[4096];
                while (true) {
                    for (int AserverSock : AserverSockets) {
                        int bytesReceived = recv(AserverSock, buffer, 4096, 0);
                        if (bytesReceived <= 0) {
                            //cerr << "Error: Server is down" << endl;
                            close(AserverSock);
                            break;
                        }
                        cout << "Private chat: " << string(buffer, bytesReceived) << endl;
                    }
                }
            });

            AreceivingRunning = true;

        }

        void AsendToServer(int serverIndex, const string& message) {
            int AserverSock = AserverSockets[serverIndex];
            send(AserverSock, message.c_str(), message.size() + 1, 0);
        }
            

};

#endif // CLIENT_H