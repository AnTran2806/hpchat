#include "client.h"

Client::Client() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error: Create the socket" << endl;
        return;
    }
}

Client::~Client() {
    close(sock);
    if (receivingThread.joinable()) {
        receivingThread.join();
    }
}

bool Client::connectToServer(const string& ipAddress, int port) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        cerr << "Error: Cannot connect to Server!" << endl;
        return false;
    }

    return true;
}

void Client::startReceiving() {
    receivingThread = thread([this]() {
        char buffer[4096];
        while (true) {
            int bytesReceived = recv(sock, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                cerr << "Error: Server is down" << endl;
                close(sock);
                break;
            }
            cout << string(buffer, 0, bytesReceived) << endl;
        }
    });
}

void Client::sendToServer(const string& message) {
    send(sock, message.c_str(), message.size() + 1, 0);
}

void Client::handleUserInteraction() {
    int option;
    // cout << "Press Enter to continue" << endl;
    // cin.get();
    cout << "1: Register" << endl << "2: Login" << endl << "Your Choice: ";
    cin >> option;
    cin.ignore();

    // send(sock, to_string(option).c_str(), to_string(option).size() + 1, 0);
    sendToServer(to_string(option));

    if (option == 1 || option == 2) {
        char username[1024];
        char password[1024];

        cout << "Enter Username: ";
        cin.getline(username, sizeof(username));
        send(sock, username, strlen(username), 0);

        cout << "Enter Password: ";
        cin.getline(password, sizeof(password));
        send(sock, password, strlen(password), 0);
        
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        cout << buffer << endl;
        if (option == 1 && strcmp(buffer, "Registration successful.") != 0) {
            // If registration was not successful, handle it accordingly.
            cout << "Please try again." << endl;
            handleUserInteraction();
        } else if (option == 2 && strcmp(buffer, "Login successful.") != 0) {
            // If login was not successful, handle it accordingly.
            cout << "Please try again." << endl;
            handleUserInteraction();
        } else {
            // Registration or login was successful.
            cout << "Operation successful." << endl;
        }
    } else {
        cout << "Invalid option" << endl;
    }
}


string Client::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");

    if (first == string::npos || last == string::npos)
        return "";
    else
        return str.substr(first, last - first + 1);
}