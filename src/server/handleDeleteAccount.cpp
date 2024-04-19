#include "server.h"

bool UserAuthentication::handleDeleteAccount(int clientSocket)
{
    if (!handleLogin(clientSocket))
    {
        // If handleLogin fails, send a message to the client
        return false;
    }    
    bool deleteAccountSuccess = false;

    do {
        string enteredUsername = loggedInUsers[clientSocket];

        char retypePasswordBuffer[1024] = {0};
        recv(clientSocket, retypePasswordBuffer, sizeof(retypePasswordBuffer), 0);
        string retypePassword(retypePasswordBuffer);

        deleteAccountSuccess = deleteAccount(enteredUsername, retypePassword);

        // If account deletion is successful, remove the username from loggedInUsers
        if (deleteAccountSuccess) {
            loggedInUsers.erase(clientSocket);

            // Send the response back to the client
            const char *deleteAccountResponse = "\033[1;32mAccount deleted successfully.\033[0m";
            send(clientSocket, deleteAccountResponse, strlen(deleteAccountResponse), 0);
        } else {
            // Send the response back to the client
            const char *deleteAccountResponse = "\033[1;31mCan not delete your account. Please try again.\033[0m\n";
            send(clientSocket, deleteAccountResponse, strlen(deleteAccountResponse), 0);
        }

    } while (!deleteAccountSuccess);
    return deleteAccountSuccess;
}