#include "client.h"

void Client::enterCredential()
{
    char username[1024];
    char password[1024];
    
    cout << "Enter Username: ";
    cin.getline(username, sizeof(username));
    send(sock, username, strlen(username), 0);

    cout << "Enter Password: ";
    cin.getline(password, sizeof(password));
    send(sock, password, strlen(password), 0);
}
