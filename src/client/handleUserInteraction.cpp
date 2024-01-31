#include "client.h"

void Client::handleUserInteraction() {
    string option;
    // cout << "Press Enter to continue" << endl;
    // cin.get();
    cout << "\t\033[1;34mWelcome to Chat Application!\033[0m" << endl;
    cout << "\033[1mA:\033[0m Sign Up" << endl;
    cout << "\033[1mB:\033[0m Sign In" << endl;
    cout << "\033[1mYour Choice:\033[0m ";
    cin >> option;
    cout << endl;
    cin.ignore();

    // send(sock, to_string(option).c_str(), to_string(option).size() + 1, 0);
    sendToServer(option);

    if (option == "A" || option == "B") {
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

                    if (strcmp(buffer, "Registration successful.") == 0) {
                        cout << "Please log in with your new credentials." << endl;
                        registrationSuccess = true;
                        option = "B"; // Set option to "B" for login
                    } else {
                        // If registration was not successful, handle it accordingly.
                        cout << "Registration failed. Please try again." << endl;
                    }
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
                    cout << "Operation successful." << endl;
                    success = true;
                }
            }
        } while (!success);
    } else {
        cout << "Invalid option" << endl;
    }

}

