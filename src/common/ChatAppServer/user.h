#ifndef USER_H
#define USER_H

#include "library.h"

class User {
public:
    string username;
    string password;

    User(const string& user, const string& pass);
};

#endif // USER_H