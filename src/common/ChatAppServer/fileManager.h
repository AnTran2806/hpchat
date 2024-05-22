#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "library.h"
#include "user.h"

class FileManager {
public:
    bool readUsersFromFile(const string& filePath, vector<User>& users);
    bool writeUsersToFile(const string& filePath, const vector<User>& users);
    bool appendUserToFile(const string& filePath, const User& user);
};

#endif // FILEMANAGER_H