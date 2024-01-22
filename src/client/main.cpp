#include "client.h"

int main()
{
    Client client;

    if (client.connectToServer("10.188.9.19", 54000)) {
        /// void login (){goto :abc}
        //abc:
        string roomName;
        cout << "Enter the ID or RoomName: ";
        getline(cin, roomName);
        client.sendToServer(roomName);

        client.startReceiving();

        // string clientName;
        // cout << "Enter Your name: ";
        // getline(cin, clientName);

        // // Sử dụng hàm trim thông qua đối tượng client
        // clientName = client.trim(clientName);

        // client.sendToServer(clientName);

        std::cout << "Enter 'help' to get the guide\n";
        client.startReceiving();
        // Main loop to input and send messages from the user
        while (true) {
            string userInput;
            getline(cin, userInput);

            if (userInput == "sendfile")
            {
                client.sendToServer("sendfile");

                // Enter the recipient's name or room name and send it to the server
                std::string nameOrRoom;
                std::cout << "The name to sendFile: ";
                std::getline(std::cin, nameOrRoom);
                client.sendToServer(nameOrRoom);

                // Enter the file path and send it to the server
                std::string filePath;
                std::cout << "Enter the file path to send: ";
                std::getline(std::cin, filePath);
                client.sendToServer(filePath);
                std::cout << "File Name: " << filePath << std::endl;

                 // Start a new thread to send the file to the server
                std::thread sendFileThread(&Client::sendFile, &client, filePath);
                sendFileThread.detach();
            }
            // Check if the user wants to see the guide
            else if (userInput == "help") 
            {
                std::cout << "1. Chat Group: Entering the message then Enter\n"
                          << "2. Private chat: Format:/pm + User's name + your message \n"
                          << "3. Server: /sv + Your message\n"
                          << "4. Sendfile: Entering the sendfile.\n"
                          << "5. /pm + any word to Display the list user's name online"
                          << std::endl;
            }
            else {
                client.sendToServer(userInput);
            }
        }
    }
    return 0;
}