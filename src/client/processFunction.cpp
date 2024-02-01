#include "client.h"

void Client::processFunction(){
    cout << "Enter 'help' to get the guide\n";
    while (true) {
        string userInput;
        cout << "You: ";
        getline(cin, userInput);

        if (userInput == "sendfile")
        {
            sendToServer("sendfile");

            // Enter the recipient's name or room name and send it to the server
            string nameOrRoom;
            cout << "The name to sendFile: ";
            getline(cin, nameOrRoom);
            sendToServer(nameOrRoom);

            // Enter the file path and send it to the server
            string filePath;
            cout << "Enter the file path to send: ";
            getline(cin, filePath);
            sendToServer(filePath);
            cout << "File Name: " << filePath << endl;

                // Start a new thread to send the file to the server
            thread sendFileThread(&Client::sendFile, this, filePath);
            sendFileThread.detach();
        }
        // Check if the user wants to see the guide
        else if (userInput == "help") 
        {
            cout << "1. Chat Group: Entering the message then Enter\n"
                        << "2. Private chat: Format:/pm + User's name + your message \n"
                        << "3. Server: /sv + Your message\n"
                        << "4. Sendfile: Entering the sendfile.\n"
                        << "5. /pm + any word to Display the list user's name online"
                        << endl;
        }
        else {
            sendToServer(userInput);
        }
    }
}