#ifndef CLIENT_H
#define CLIENT_H

// #include <string>
// using namespace std;
#include "../common/library.h"

class Client {
public:
    Client(int socket, const string& name, const string& roomName);

    int getSocket() const;

    const string& getName() const;

    const string& getRoomName() const;

private:
    int socket;
    string name;
    string roomName;
};

#endif // CLIENT_H