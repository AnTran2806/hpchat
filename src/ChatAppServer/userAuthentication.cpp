#include "server.h"

UserAuthentication::UserAuthentication(Server* server) : server(server), username(""), password("") {}

bool UserAuthentication::isUserRegistered(const string& checkUsername) 
{
    // Implementation here
    ifstream inputFile("data\\users.txt");
    string username;
    while (getline(inputFile, username)) {
        // Check if the current line is the username
        if (username == checkUsername) {
            inputFile.close();
            return true; // User is already registered
        }

        // Skip the next line (password)
        if (!getline(inputFile, username)) {
            break;  // Break if unable to read the next line (no password found)
        }
    }

    inputFile.close();
    return false; // User is not registered
}

bool UserAuthentication::registerUser(const string& enteredUsername, const string& enteredPassword) {
    // Implementation here
    username = enteredUsername;
    password = enteredPassword;

    // ofstream myFile("data\\" + username + ".txt");
    ofstream myFile("data\\users.txt", ios::app);  // Open the file to continue writing to the last file
    if (myFile.is_open()) {
        myFile << username << endl << password << endl;
        myFile.close();
        return true;
    } else {
        cerr << "Unable to open file" << endl;
        return false;
    }
}

bool UserAuthentication::isLoggedIn(const string& enteredUsername, const string& enteredPassword) {
    string storedUsername, storedPassword;

    // ifstream inputFile("./data/data_users/users.txt");
    ifstream inputFile("data\\users.txt");
    if (inputFile.is_open()) {
        while (getline(inputFile, storedUsername)) {
            getline(inputFile, storedPassword);

            if (storedUsername == enteredUsername && storedPassword == enteredPassword) {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
    }
    return false;
}

bool UserAuthentication::changePassword(const string& enteredUsername, const string& oldPassword, const string& newPassword) {
    string storedUsername, storedPassword;

    // Open the input/output file to read and write user data
    fstream file("data\\users.txt", ios::in | ios::out);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file" << endl;
        return false;
    }

    // Flag to check if the password is changed
    bool passwordChanged = false;

    while (getline(file, storedUsername)) {
        getline(file, storedPassword);


        // Check if the current line contains the username and old password
        if (storedUsername == enteredUsername && storedPassword == oldPassword) {
            // Check if the new password is different from the old password
            if (newPassword != oldPassword) {
                // Update the password directly in the file
                file.seekp(file.tellg());  // Move the write cursor to the current position
                file << enteredUsername << std::endl << newPassword << std::endl;

                passwordChanged = true;
            } else {
                // cerr << "Error: New password is the same as the old password" << endl;
            }

            break;
        }
    }

    // Close the file
    file.close();

    if (passwordChanged) {
        // cout << "Password changed successfully" << endl;
        return true;
    } else {
        // cerr << "Error: Username or old password not found" << endl;
        return false;
    }
}

bool UserAuthentication::deleteAccount(const std::string& enteredUsername, const std::string& retypePassword) {
    std::string storedUsername, storedPassword;

    // Open the input/output file to read and write user data
    std::fstream file("data\\users.txt", std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file" << std::endl;
        return false;
    }

    // Get the current position in the file for later seeking
    std::streampos currentPosition = file.tellg();

    // Flag to check if the account is deleted
    bool accountDeleted = false;

    // Create a stringstream to store the contents of the file
    std::stringstream updatedContents;

    while (getline(file, storedUsername)) {
        getline(file, storedPassword);

        // Check if the current line contains the username to be deleted
        if (storedUsername == enteredUsername && storedPassword == retypePassword) {
            accountDeleted = true;
        } else {
            // Write data to the stringstream for all other usernames
            updatedContents << storedUsername << std::endl << storedPassword << std::endl;
        }

        // Check for the end of the file
        if (file.eof()) {
            break;
        }
    }

    // Clear the end-of-file flag and seek back to the beginning of the file
    file.clear();
    file.seekg(currentPosition);

    // Truncate the file
    file.close();
    file.open("data\\users.txt", std::ios::out | std::ios::trunc);

    // Write the updated contents to the file
    file << updatedContents.str();

    // Close the file
    file.close();

    if (accountDeleted) {
        std::cout << "Account deleted successfully" << std::endl;
        return true;
    } else {
        std::cerr << "Error: Username or password not found" << std::endl;
        return false;
    }
}

void UserAuthentication::handleAuthentication(int clientSocket, const string& option)
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
            check = handleLogin(clientSocket, server);
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
    thread processThread(&Server::processClient, server, clientSocket);
    processThread.join();
}    

bool UserAuthentication::handleRegistration(int clientSocket)
{
    char usernameBuffer[1024] = {0};
    char passwordBuffer[1024] = {0};

    recv(clientSocket, usernameBuffer, sizeof(usernameBuffer), 0);
    recv(clientSocket, passwordBuffer, sizeof(passwordBuffer), 0);

    string username(usernameBuffer);
    string password(passwordBuffer);

    bool status = isUserRegistered(username);
    status = !status && registerUser(username, password);
    const char *response;
    if (status)
    {
        response = "Registration successful. Please log in with your new credentials.";
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

bool UserAuthentication::handleLogin(int clientSocket, class Server* server)
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
    bool status = isLoggedIn(username, password);

    const char *response = status ? "Login successful." : "Login failed.\nPlease try again.";
    if(strcmp(response, "Login successful.") == 0){
        cout <<username << " signed successful at IP address " << "\033[1;32m" << clientIP << "\033[0m"<<endl; 
    } 

    send(clientSocket, response, strlen(response), 0);  

    // After confirming successful login, save the username to the list
    loggedInUsers[clientSocket] = username;
    server->setLoggedInUsers(loggedInUsers);

    return status;
}

bool UserAuthentication::handleChangePassword(int clientSocket)
{
    if (!handleLogin(clientSocket, server))
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

bool UserAuthentication::handleDeleteAccount(int clientSocket)
{
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