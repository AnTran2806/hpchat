#include "client.h"

void Client::enterRoomName(){
    string roomName;
    cout << "Enter the ID: ";
    getline(cin, roomName);
    sendToServer(roomName);
}