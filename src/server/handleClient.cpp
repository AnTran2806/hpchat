#include "server.h"

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
  
