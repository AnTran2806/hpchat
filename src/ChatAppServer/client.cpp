#include "client.h"

Client::Client(int socket, const string& name, const string& roomName) : socket(socket), name(name), roomName(roomName) {}

int Client::getSocket() const {
    return socket;
}

const string& Client::getName() const {
    return name;
}

const string& Client::getRoomName() const {
    return roomName;
}
