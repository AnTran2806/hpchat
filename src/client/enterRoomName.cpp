#include "client.h"

void Client::enterRoomName(){
    string roomName;
    cout << "Enter the ID or RoomName: ";
    getline(cin, roomName);
    sendToServer(roomName);
}