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