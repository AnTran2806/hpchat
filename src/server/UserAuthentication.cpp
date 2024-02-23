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

// bool UserAuthentication::changePassword(const string& enteredUsername, const string& oldPassword, const string& newPassword) {
//     string storedUsername, storedPassword;

//     // Open the input/output file to read and write user data
//     fstream file("data\\users.txt", ios::in | ios::out);
//     if (!file.is_open()) {
//         cerr << "Error: Unable to open file" << endl;
//         return false;
//     }

//     // Flag to check if the password is changed
//     bool passwordChanged = false;

//     while (getline(file, storedUsername)) {
//         getline(file, storedPassword);

//         // cout << "Username: " << storedUsername << " | Old Password: " << oldPassword << " | Stored Password: " << storedPassword << endl;
//         cout << "Old Pwd" << oldPassword << endl;
//         // Check if the current line contains the username and old password
//         if (storedUsername == enteredUsername && storedPassword == oldPassword) {
//             // Update the password directly in the file
//             streampos pos = file.tellg(); // Get the position after reading storedPassword
//             file.seekp(pos - static_cast<ios::off_type>(storedPassword.length()) - static_cast<ios::off_type>(1)); // Move the cursor back to the start of the storedPassword
//             file << newPassword; // Overwrite the old password
//             file.seekg(pos); // Move the cursor back to the end of the current line

//             passwordChanged = true;
//             break;
//         }
//     }

//     // Close the file
//     file.close();

//     if (passwordChanged) {
//         cout << "Password changed successfully" << endl;
//         return true;
//     } else {
//         cerr << "Error: Username or old password not found" << endl;
//         return false;
//     }
// }



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
                streampos pos = file.tellg(); // Get the position after reading storedPassword
                file.seekp(pos - static_cast<ios::off_type>(storedPassword.length()) - static_cast<ios::off_type>(1)); // Move the cursor back to the start of the storedPassword
                file << newPassword; // Overwrite the old password
                file.seekg(pos); // Move the cursor back to the end of the current line

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



