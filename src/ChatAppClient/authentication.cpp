#include "authentication.h"

void Authentication::enterCredential(int clientSocket){
    string username,password;
    cout << "\033[1mEnter Username:\033[0m ";
    getline(cin,username);
    sendToServer(clientSocket,username);
    cout << "\033[1mEnter Password:\033[0m ";
    getline(cin,password);
    sendToServer(clientSocket,password);
    cout << endl;
}

void Authentication::handleUserInteraction(int clientSocket) {
    string option;

    cout << "\t\033[1;34mWelcome to Chat Application!\033[0m" << endl;
    cout << "\033[1;34mA:\033[0m";cout<< "Sign Up" << endl;
    cout << "\033[1;34mB:\033[0m";cout<<"Sign In" << endl;
    cout << "\033[1;34mC:\033[0m";cout<<"Change The Password" << endl;
    cout << "\033[1;34mD:\033[0m";cout<<"Delete Your Account" << endl;

    // Use a loop to ask the user to re-enter if the selection is invalid
    bool validOption = false;
    do {
        cout << "\033[1mYour Choice:\033[0m ";
        cin >> option;
        cout << endl;
        cin.ignore();

        if (option == "A" || option == "B" || option == "C" || option == "D") {
            validOption = true;
        } else {
            cout << "\033[1;31mInvalid option\033[0m\nPlease try again...\n" ;
        }
    } while (!validOption);

    // After having a valid selection, send it to the server and process it
    sendToServer(clientSocket, option);

    bool success = false;
    do {
        if (option == "A") {
            bool registrationSuccess = false;
            do {
                cout << "\t\033[3mSign Up\033[0m" << endl;
                enterCredential(clientSocket);
                char buffer[1024] = {0};
                read(clientSocket, buffer, 1024);
                cout << buffer << endl;

                if (strcmp(buffer, "Registration successful. Please log in with your new credentials.") == 0) {
                    registrationSuccess = true;
                    option = "B"; // Set option to "B" for login
                } 
            } while (!registrationSuccess);
        } else if (option == "B") {
            cout << "\t\033[3mSign In\033[0m" << endl;
            enterCredential(clientSocket);
            char buffer[1024] = {0};
            read(clientSocket, buffer, 1024);
            cout << buffer << endl;

            if (strcmp(buffer, "Login successful.") == 0) {
                success = true;
            }
        }else if (option == "C") {
            cout << "\t\033[3mSign In\033[0m" << endl;
            enterCredential(clientSocket);
            char buffer[1024] = {0};
            read(clientSocket, buffer, 1024);
            cout << buffer << endl;

            if (strcmp(buffer, "Login successful.") == 0) {
                bool passwordChangeSuccess = false;

                do {
                    cout << endl;
                    char oldPassword[1024];

                    cout << "\033[1mOld Password:\033[0m ";
                    cin.getline(oldPassword, sizeof(oldPassword));
                    send(clientSocket, oldPassword, strlen(oldPassword), 0);
                    cout << endl;

                    char newPassword[1024];

                    cout << "\033[1mCreate New password:\033[0m ";
                    cin.getline(newPassword, sizeof(newPassword));
                    send(clientSocket, newPassword, strlen(newPassword), 0);
                    cout << endl;

                    char confirmNewPassword[1024];

                    cout << "\033[1mConfirm New Password:\033[0m ";
                    cin.getline(confirmNewPassword, sizeof(confirmNewPassword));
                    send(clientSocket, confirmNewPassword, strlen(confirmNewPassword), 0);
                    cout << endl;

                    char buffer[1024] = {0};
                    read(clientSocket, buffer, 1024);
                    cout << buffer << endl;

                    if (strcmp(buffer, "\033[1;32mPassword changed successfully.\033[0m") == 0) {
                        passwordChangeSuccess = true;
                    } 
                } while (!passwordChangeSuccess);

                success = true;
            }
        } else if (option == "D") {
            cout << "\t\033[3mSign In\033[0m" << endl;
            enterCredential(clientSocket);
            char buffer[1024] = {0};
            read(clientSocket, buffer, 1024);
            cout << buffer << endl;

            if (strcmp(buffer, "Login successful.") == 0) {
                bool deleteAccountSuccess = false;
                do {
                    char retypePassword[1024];

                    cout << "\033[1mPlease retype Your Password Before Deleting:\033[0m ";
                    cin.getline(retypePassword, sizeof(retypePassword));
                    send(clientSocket, retypePassword, strlen(retypePassword), 0);

                    char buffer[1024] = {0};
                    read(clientSocket, buffer, 1024);
                    cout << buffer << endl;

                    if (strcmp(buffer, "\033[1;32mAccount deleted successfully.\033[0m") == 0) {
                        deleteAccountSuccess = true;
                        handleUserInteraction(clientSocket);
                    } 
                } while (!deleteAccountSuccess);
                success = true;
            }
        }
    } while (!success);
}