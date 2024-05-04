#include "client.h"

Client::Client(int sock):sock(sock){}

Client::~Client(){
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

int Client::getSocket() const {
    return sock;
}

void Client::stopClient(int clientSocket){
    close(clientSocket);
}


Client::Client() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error: Create the socket" << endl;
        return;
    }
}

void sendToServer(int socket,const string& message) {
    send(socket, message.c_str(), message.size() + 1, 0);
}