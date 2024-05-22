#include "fileManager.h"

bool FileManager::readUsersFromFile(const string& filePath, vector<User>& users) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file" << endl;
        return false;
    }

    string username, password;
    while (getline(inputFile, username)) {
        if (getline(inputFile, password)) {
            users.emplace_back(username, password);
        }
    }

    inputFile.close();
    return true;
}

bool FileManager::writeUsersToFile(const string& filePath, const vector<User>& users) {
    ofstream outputFile(filePath, ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open file for writing" << endl;
        return false;
    }

    for (const auto& user : users) {
        outputFile << user.username << endl;
        outputFile << user.password << endl;
    }

    outputFile.close();
    return true;
}

bool FileManager::appendUserToFile(const string& filePath, const User& user) {
    ofstream outputFile(filePath, ios::app);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open file for writing" << endl;
        return false;
    }

    outputFile << user.username << endl;
    outputFile << user.password << endl;
    outputFile.close();
    return true;
}
