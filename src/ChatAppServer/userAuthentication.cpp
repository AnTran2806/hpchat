#include "server.h"
#include "../common/ChatAppServer/credentialReceiver.cpp"
#include "../common/ChatAppServer/fileManager.cpp"
#include "../common/ChatAppServer/user.cpp"


UserAuthentication::UserAuthentication(Server* server) : server(server), username(""), password("") {}

bool UserAuthentication::isUserRegistered(const string& checkUsername) {
    vector<User> users;
    FileManager fileManager; // Create an instance of FileManager
    if (!fileManager.readUsersFromFile(filePath, users)) {
        return false;
    }

    for (const auto& user : users) {
        if (user.username == checkUsername) {
            return true;
        }
    }

    return false;
}

bool UserAuthentication::registerUser(const string& enteredUsername, const string& enteredPassword) {
    FileManager fileManager;
    if (isUserRegistered(enteredUsername)) {
        cerr << "Error: Username already exists" << endl;
        return false;
    }

    User newUser(enteredUsername, enteredPassword);
    return fileManager.appendUserToFile(filePath, newUser);
}

bool UserAuthentication::isLoggedIn(const string& enteredUsername, const string& enteredPassword) {
    vector<User> users;
    FileManager fileManager;

    if (!fileManager.readUsersFromFile(filePath, users)) {
        return false;
    }

    for (const auto& user : users) {
        if (user.username == enteredUsername && user.password == enteredPassword) {
            return true;
        }
    }

    return false;
}

bool UserAuthentication::changePassword(const string& enteredUsername, const string& oldPassword, const string& newPassword, const string& confirmNewPassword) {
    FileManager fileManager;
    if (newPassword != confirmNewPassword) {
        cerr << "Error: Confirm password does not match the new password" << endl;
        return false;
    }

    vector<User> users;
    if (!fileManager.readUsersFromFile(filePath, users)) {
        return false;
    }

    bool passwordChanged = false;
    for (auto& user : users) {
        if (user.username == enteredUsername && user.password == oldPassword) {
            if (newPassword == oldPassword) {
                cerr << "Error: New password is the same as the old password" << endl;
                return false;
            }
            user.password = newPassword;
            passwordChanged = true;
        }
    }

    if (passwordChanged) {
        return fileManager.writeUsersToFile(filePath, users);
    } else {
        cerr << "Error: Username or old password not found" << endl;
        return false;
    }
}

bool UserAuthentication::deleteAccount(const string& enteredUsername, const string& retypePassword) {
    vector<User> users;
    FileManager fileManager;
    if (!fileManager.readUsersFromFile(filePath, users)) {
        return false;
    }

    auto it = remove_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == enteredUsername && user.password == retypePassword;
    });

    if (it != users.end()) {
        users.erase(it, users.end());
        return fileManager.writeUsersToFile(filePath, users);
    } else {
        cerr << "Error: Username or password not found" << endl;
        return false;
    }
}

void UserAuthentication::handleAuthentication(int clientSocket, const string& option) {
    bool check = false;
    string username;
    while(!check)
    {   
        // char buffer[4096];
        if (option == "A")
        {
            check = handleRegistration(clientSocket);
        }
        else if (option == "B")
        {
            check = handleLogin(clientSocket, server);
        }
        else if (option == "C")
        {
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
    thread processThread(&Server::processClient, server, clientSocket);
    processThread.join();
}

bool UserAuthentication::handleRegistration(int clientSocket) {
    string username, password;
    CredentialReceiver credentialReceiver;
    credentialReceiver.receiveCredential(clientSocket, username, password);

    bool status = isUserRegistered(username);
    status = !status && registerUser(username, password);
    const char *response;
    if (status)
    {
        response = "Registration successful. Please log in with your new credentials.\n";
        send(clientSocket, response, strlen(response), 0);
        handleLogin(clientSocket, server);
    }
    else
    {
        response = "Username already exists. Choose a different username.\nPlease try again.";
        send(clientSocket, response, strlen(response), 0);

    }
    // send(clientSocket, response, strlen(response), 0);
    return status;
}

bool UserAuthentication::handleLogin(int clientSocket, class Server* server) {
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen);

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

    string username, password;
    CredentialReceiver credentialReceiver;
    credentialReceiver.receiveCredential(clientSocket, username, password);

    bool status = isLoggedIn(username, password);

    const char *response = status ? "Login successful." : "Login failed.\nPlease try again.\n";
    if(strcmp(response, "Login successful.") == 0){
        cout <<username << " signed successful at IP address " << "\033[1;32m" << clientIP << "\033[0m"<<endl; 
    } 

    send(clientSocket, response, strlen(response), 0);  

    // After confirming successful login, save the username to the list
    loggedInUsers[clientSocket] = username;
    loggedInPasswords[clientSocket] = password;

    // this->username = username;
    // this->password = password;
    server->setLoggedInUsers(loggedInUsers);

    return status;
}

bool UserAuthentication::handleChangePassword(int clientSocket) {
    if (!handleLogin(clientSocket, server))
    {
        // If handleLogin fails, send a message to the client
        return false;
    }    

    bool passwordChangeSuccess = false;

    do {
        // Read the old password from the client
        char oldPasswordBuffer[1024] = {0};
        recv(clientSocket, oldPasswordBuffer, sizeof(oldPasswordBuffer), 0);
        string oldPassword(oldPasswordBuffer);

        // Read the new password from the client
        char newPasswordBuffer[1024] = {0};
        recv(clientSocket, newPasswordBuffer, sizeof(newPasswordBuffer), 0);
        string newPassword(newPasswordBuffer);

        // Read the confirm new password from the client
        char confirmNewPasswordBuffer[1024] = {0};
        recv(clientSocket, confirmNewPasswordBuffer, sizeof(confirmNewPasswordBuffer), 0);
        string confirmNewPassword(confirmNewPasswordBuffer);

        // Get the username of the logged-in user from loggedInUsers map
        string enteredUsername = loggedInUsers[clientSocket];

        // Call the changePassword function
        passwordChangeSuccess = changePassword(enteredUsername, oldPassword, newPassword, confirmNewPassword);

        // Send the response back to the client
        const char *changePasswordResponse = passwordChangeSuccess ? "\033[1;32mPassword changed successfully.\033[0m" : "\033[1;31mNew password is the same as the old password or passwords do not match. Please try again.\033[0m\n";
        
        send(clientSocket, changePasswordResponse, strlen(changePasswordResponse), 0);

    } while (!passwordChangeSuccess);

    return passwordChangeSuccess;
}

bool UserAuthentication::handleDeleteAccount(int clientSocket) {
    if (!handleLogin(clientSocket, server))
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