#include "client.h"

void Authen::enterCredential(int clientSocket){
    string username,password;
    cout << "\033[1mEnter Username:\033[0m ";
    getline(cin,username);
    sendToServer(clientSocket,username);
    cout << "\033[1mEnter Password:\033[0m ";
    getline(cin,password);
    sendToServer(clientSocket,password);
    cout << endl;
}
