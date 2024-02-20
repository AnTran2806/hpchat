#include "server.h"

bool Server::handleLogin(int clientSocket)
{
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen);

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

    char usernameBuffer[1024] = {0};
    char passwordBuffer[1024] = {0};

    // read(clientSocket, usernameBuffer, 1024);
    //cout << "Here is handleLogin Fucntion" << endl;
    // read(clientSocket, passwordBuffer, 1024);
    recv(clientSocket, usernameBuffer, sizeof(usernameBuffer), 0);
    recv(clientSocket, passwordBuffer, sizeof(passwordBuffer), 0);

    string username(usernameBuffer);
    string password(passwordBuffer);
    bool status = auth.isLoggedIn(username, password);

    const char *response = status ? "Login successful." : "Login failed.";
    if(strcmp(response, "Login successful.") == 0){
        cout <<username << " signed successful at IP address " << "\033[1;32m" << clientIP << "\033[0m"<<endl; 
    } 

    send(clientSocket, response, strlen(response), 0);  

    // After confirming successful login, save the username to the list
    loggedInUsers[clientSocket] = username;

    return status;
}