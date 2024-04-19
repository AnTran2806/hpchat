#include "server.h"

bool UserAuthentication::handleChangePassword(int clientSocket)
{
    if (!handleLogin(clientSocket))
    {
        // If handleLogin fails, send a message to the client
        return false;
    }    


    bool passwordChangeSuccess = false;

    do {
        // Read the new password from the client
        char oldPasswordBuffer[1024] = {0};
        recv(clientSocket, oldPasswordBuffer, sizeof(oldPasswordBuffer), 0);
        string oldPassword(oldPasswordBuffer);

        char newPasswordBuffer[1024] = {0};
        recv(clientSocket, newPasswordBuffer, sizeof(newPasswordBuffer), 0);
        string newPassword(newPasswordBuffer);

        // Get the username of the logged-in user from loggedInUsers map
        string enteredUsername = loggedInUsers[clientSocket];

        // Call the changePassword function
        passwordChangeSuccess = changePassword(enteredUsername, oldPassword, newPassword);

        // Send the response back to the client
        const char *changePasswordResponse = passwordChangeSuccess ? "\033[1;32mPassword changed successfully.\033[0m" : "\033[1;31mNew password is the same as the old password. Please try again.\033[0m\n";
        send(clientSocket, changePasswordResponse, strlen(changePasswordResponse), 0);

    } while (!passwordChangeSuccess);

    return passwordChangeSuccess;
}