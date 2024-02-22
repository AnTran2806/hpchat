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
            check = handleLogin(clientSocket);
        }
        else if (option == "D")
        {
            // Login
            //cout<<"Client has logged in."<<endl;
            check = handleLogin(clientSocket);
        }
    }
    // Detach the thread before proceeding
    thread processThread(&Server::processClient, this, clientSocket);
    processThread.join();
}    