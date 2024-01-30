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

    // if (option == "A" || option == "B") {
    //     enterCredential();
    //     char buffer[1024] = {0};
    //     read(sock, buffer, 1024);
    //     cout << buffer << endl;
    //     if (option == "A" && strcmp(buffer, "Registration successful.") != 0) {
    //         // If registration was not successful, handle it accordingly.
    //         cout << "Please try again." << endl;
    //         // handleUserInteraction();
    //         enterCredential();
    //     } else if (option == "B" && strcmp(buffer, "Login successful.") != 0) {
    //         // If login was not successful, handle it accordingly.
    //         cout << "Please try again." << endl;
    //         // handleUserInteraction();
    //         enterCredential();
    //     } else {
    //         // Registration or login was successful.
    //         cout << "Operation successful." << endl;
    //     }
    // } else {
    //     cout << "Invalid option" << endl;
    // }


    if (option == "A" || option == "B") {
        bool success = false;
        do {
            enterCredential();
            char buffer[1024] = {0};
            read(sock, buffer, 1024);
            cout << buffer << endl;

            if (option == "A" && strcmp(buffer, "Registration successful.") != 0) {
                // If registration was not successful, handle it accordingly.
                cout << "Please try again." << endl;
            } else if (option == "B" && strcmp(buffer, "Login successful.") != 0) {
                // If login was not successful, handle it accordingly.
                cout << "Please try again." << endl;
            } else {
                // Registration or login was successful.
                cout << "Operation successful." << endl;
                success = true;
            }
        } while (!success);
    } else {
        cout << "Invalid option" << endl;
    }
}

