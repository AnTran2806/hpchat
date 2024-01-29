#include "server.h"

UserAuthentication::UserAuthentication() : username(""), password("") {}

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