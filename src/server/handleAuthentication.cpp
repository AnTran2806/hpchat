#include "server.h"

void Server::handleAuthentication(int clientSocket, const string& option)
{
    bool check = false;
    string username;
    while(!check)
    {   
        // char buffer[4096];
        if (option == "A")
        {
            // Register
            //cout<<"Client is registering."<<endl;
            check = handleRegistration(clientSocket);
        }
        else if (option == "B")
        {
            // Login
            //cout<<"Client has logged in."<<endl;
            check = handleLogin(clientSocket);
        }
        else if (option == "C")
        {
            // Login
            //cout<<"Client has logged in."<<endl;
            check = handleChangePassword(clientSocket);
        }
        else if (option == "D")
        {
            do{
                check = handleDeleteAccount(clientSocket);
            }while(!check);
            // Continue to wait for new connections
            return;
        }
    }
    // Detach the thread before proceeding
    thread processThread(&Server::processClient, this, clientSocket);
    processThread.join();
}    