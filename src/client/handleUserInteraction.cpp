#include "client.h"

void Client::handleUserInteraction() {
    string option;
    // cout << "Press Enter to continue" << endl;
    // cin.get();
    cout << "A: Register" << endl << "B: Login" << endl << "Your Choice: ";
    cin >> option;
    cin.ignore();

    // send(sock, to_string(option).c_str(), to_string(option).size() + 1, 0);
    sendToServer(option);

    if (option == "A" || option == "B") {
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
        if (option == "A" && strcmp(buffer, "Registration successful.") != 0) {
            // If registration was not successful, handle it accordingly.
            cout << "Please try again." << endl;
            handleUserInteraction();
        } else if (option == "B" && strcmp(buffer, "Login successful.") != 0) {
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