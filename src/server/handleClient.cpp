#include "server.h"

void Server::handleClient(int clientSocket, const string& clientName, const string& roomName) 
{
    // ... (handleClient implementation)
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
            handleFileTransfer(clientSocket, clientName);
        }
        else if (receivedMessage.substr(0,3)=="P2P"){
            cout<<"Reeceive the request to chat P2P\n";
            string p2pName = receivedMessage.substr(4);
            cout<<p2pName<<"\n";
            bytesReceived =recv(clientSocket,buffer,4096,0);
            string IPClient1(buffer,0,bytesReceived);
           //cout<<IPClient1<<" Ip of client chat 1-1\n";
            for (auto& client:clients){
                if (client.getName()==p2pName){
                    send(client.getSocket(),"P2P",strlen("P2P")+1,0);
                }
            }
            for (auto& client:clients){
                if (client.getName()==p2pName){
                    send(client.getSocket(),IPClient1.c_str(),IPClient1.size()+1,0);
                    //cout<<IPClient1<<" sent 1-1\n";
                }
            }

        }
        else if (receivedMessage.substr(0,4)=="Room"||receivedMessage.substr(0,4)=="room")
        {
            handleGroupMessage(clientName, roomName, receivedMessage, clientSocket);
        }
    }
}
  
