#include "client.h"

string AgetIPAddress();

void Client::processFunction(){
    string ipAddress = AgetIPAddress();

    Roomchat:
    cout<<"\t\t\tWelcome!\n";
    while (true) {
        
        string userInput;
        cout << "You: ";
        getline(cin, userInput);

        if (userInput == "sendfile")
        {
            sendToServer("sendfile");

            // Enter the recipient's name or room name and send it to the server
            string nameOrRoom;
            cout << "Enter the receiver name: ";
            getline(cin, nameOrRoom);
            sendToServer(nameOrRoom);

            // Enter the file path and send it to the server
            string filePath;
            cout << "Enter the file path to send: ";
            getline(cin, filePath);
            sendToServer(filePath);
            cout << "File Name: " << filePath << endl;

                // Start a new thread to send the file to the server
            thread sendFileThread(&Client::sendFile, this, filePath);
            sendFileThread.detach();
        }

        //Check if the user wants to chat Privte
        else if (userInput== "P2P"){
            string infP2P= "P2P ";
            cout<<"The client that you wanna to chat: \t";
            string userNameP2P; getline(cin,userNameP2P);
            string lenhP2P =infP2P+userNameP2P;
            sendToServer(lenhP2P);
            //cout<<" Da gửi "<<lenhP2P<<"\n";
            sendToServer(ipAddress);
            cout<<"Waiting to client accept your connect....\n";
            AServer server1;
            server1.AstartServer(54001);
            while (true) {
                    std::string inPutServer;
                    std::getline(std::cin,inPutServer);
                    if(inPutServer=="exit"){
                        server1.sendMessageToAllClients("Server Fake exited the chat Private!");
                        server1.stopServer();
                        //break;
                        goto Roomchat;
                    }
                    server1.sendMessageToAllClients(inPutServer);
                    
                    
            }


        }

        // Check if the user wants to see the guide
        
        
        else if (userInput == "help") 
        {
            cout << "1. Chat Group: Enter Room then create the new room\n"
                        << "2. Private chat:/pm + User's name + your message.\n"
                        << "3. Server: /sv + Your message\n"
                        << "4. Sendfile: Enter the sendfile to start forward the file.\n"
                        << "5. /pm + any word to display the list of users currently online"
                        << endl;
        }
        else {
            sendToServer(userInput);
        }
    }
}


















          
            


            //get IP addr
            
            // AClient Aclient;
            // Aclient.AconnectToServer("10.188.9.19",54002);
            // Aclient.AconnectToServer("127.0.0.1",54001);
            // Aclient.AstartReceiving();
            // cout<<"\t\tWelcome to the PRIVATE CHAT\n";
            // while (true){
            //     cout<<"s1 để server 1, s2 để server 2\n";
            //     string Iput;getline(cin,Iput);
            //     if (Iput=="s2"){
            //         while (true){
            //             string Amessage; cout<<"You:";
            //             getline(cin,Amessage);
            //             if(Amessage=="exit"){
            //                 cout<<"You exited chat with s2\n";
            //                 break;
            //             }
            //             Aclient.AsendToServer(1,Amessage);
            //         }
            //     }else if (Iput=="s1") {
            //         while (true)
            //         {
            //             string Amessage1; cout<<"You:";
            //             getline(cin,Amessage1);
            //             if(Amessage1=="exit"){
            //                 cout<<"You exited chat with s1\n";
            //                 break;
            //             }
            //             Aclient.AsendToServer(0,Amessage1);
            //         }
                    
            //     }

            //     if (Iput=="exit"){
            //         cout<<"You exited chat with private\n";
                    
            //         break;
                    
                    
            //     }
            // }
            // goto Roomchat;
            