#include "client.h"

Client::Client(int clientSocket):clientSocket(clientSocket){}

Client::~Client(){
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

int Client::getSocket() const {
    return clientSocket;
}

void Client::stopClient(int clientSocket){
    close(clientSocket);
}


Client::Client() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Error: Create the socket" << endl;
        return;
    }
}

void sendToServer(int socket,const string& message) {
    send(socket, message.c_str(), message.size() + 1, 0);
}