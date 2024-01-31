#include "client.h"

void Client::enterCredential()
{
    char username[1024];
    char password[1024];

    cout << "\033[1mEnter Username:\033[0m ";
    cin.getline(username, sizeof(username));
    send(sock, username, strlen(username), 0);

    cout << "\033[1mEnter Password:\033[0m ";
    cin.getline(password, sizeof(password));
    send(sock, password, strlen(password), 0);
    cout << endl;
}
