#include "client.h"

void Client::handleUserInteraction() {
    string option;

    cout << "\t\033[1;34mWelcome to Chat Application!\033[0m" << endl;
    cout << "\033[1;34mA:\033[0m";cout<< "Sign Up" << endl;
    cout << "\033[1;34mB:\033[0m";cout<<"Sign In" << endl;

// Use a loop to ask the user to re-enter if the selection is invalid
    bool validOption = false;
    do {
        cout << "\033[1mYour Choice:\033[0m ";
        cin >> option;
        cout << endl;
        cin.ignore();

        if (option == "A" || option == "B") {
            validOption = true;
        } else {
            cout << "\033[1;31mInvalid option\033[0m\nPlease try again...\n" ;
        }
    } while (!validOption);

// After having a valid selection, send it to the server and process it
    sendToServer(option);
    bool success = false;
    do {
        if (option == "A") {
            bool registrationSuccess = false;
            do {
                cout << "\t\033[3mSign Up\033[0m" << endl;
                enterCredential();
                char buffer[1024] = {0};
                read(sock, buffer, 1024);
                cout << buffer << endl;

                if (strcmp(buffer, "Registration successful. Please log in with your new credentials.") == 0) {
                    registrationSuccess = true;
                    option = "B"; // Set option to "B" for login
                } 
                // else {
                //     If registration was not successful, handle it accordingly.
                //     cout << "Registration failed. Please try again." << endl;
                // }
            } while (!registrationSuccess);
        } else if (option == "B") {
            cout << "\t\033[3mSign In\033[0m" << endl;
            enterCredential();
            char buffer[1024] = {0};
            read(sock, buffer, 1024);
            cout << buffer << endl;

            if (strcmp(buffer, "Login successful.") != 0) {
                // If login was not successful, handle it accordingly.
                cout << "Please try again." << endl;
            } else {
                // Login was successful.
                //cout << "Operation successful." << endl;
                success = true;
            }
        }
    } while (!success);
}
