#include "client.h"    
 
void Client::startReceiving() {
    // Create a new thread to handle receiving messages from the server
    receivingThread = std::thread([this]() {
        char buffer[4096];
        // Continuously receive messages from the server
        while (true) {
            int bytesReceived = recv(sock, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                std::cerr << "Error: Server is down" << std::endl;
                close(sock);
                break;
            }
            // Check if the received message indicates a file is being sent
            if (strcmp(buffer, "sendfile") == 0) {
                std::cout<<"Receiving a file from another client!";
                receiveFile();
            }
            //check P2p server
            else if(strcmp(buffer,"P2P")==0){
                
                cout<<"Enter Y to accept the private chat and N to decline\n";
                string Y;getline(cin,Y);
                if(Y=="y"||Y=="Y"){
                    char IP_Server[256];
                    read(sock, IP_Server,sizeof(IP_Server));
                    string IP_SErver(IP_Server);
                    cout<<IP_SErver<<"\n";
                    AClient client1;
                    client1.AconnectToServer(IP_SErver,54001);
                    client1.AstartReceiving();
                   // bool exitChat1 = false;
                    while(true){
                        string IPUT;getline(cin,IPUT);
                        if (IPUT=="exit"){
                            cout <<"You exited the Chat!\n";
                            break;
                        }else {
                            client1.AsendToServer(0,IPUT);
                            client1.AsendToServer(1,IPUT);
                        }
                    }
                }else if (Y=="N"||Y=="n") {
                    cout<<"You decline the Chat1-1\n";
                }
                
                /// gửi đi
               
            }
            else {
                std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
            }
        }
    });
}