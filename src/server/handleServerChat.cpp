#include "server.h"

void Server::handleServerChat(int clientSocket, const string &clientName)
{
    // Display a message indicating that the client wants to chat with the server
    string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
    cout << clientNameMessage << " wants to chat with the server." << endl;

    // Loop for handling server chat
    string privateClientMessage;
    string userInput;
    char buffer[4096];
    while (true)
    {
        // Prepare the file descriptor set for select
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO, &readSet);
        FD_SET(clientSocket, &readSet);

        // Set the timeout for select
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ready = select(max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
        string exitNotification = "\033[1;32mYou exited the private chat with SERVER\033[0m";

        if (ready > 0)
        {
            if (FD_ISSET(clientSocket, &readSet))
            {
                // Receive data from the client
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0)
                {
                    cerr << "Client " << clientName << " offline!" << endl;
                    break;
                }
                else {
                    // Process the received message from the client
                    privateClientMessage = string(buffer, 0, bytesReceived);
                    if (privateClientMessage.compare(0, 4, "exit") == 0)
                    {
                        cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << endl;
                        send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
                        break;
                    }
                    cout << clientNameMessage << ": " << privateClientMessage << endl;
                }
            }

            if (FD_ISSET(STDIN_FILENO, &readSet))
            {
                getline(cin, userInput);
                if (userInput.compare(0, 4, "exit") == 0)
                {
                    cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << endl;
                    send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
                    break;
                }
                else {
                    // Handle user input and send messages to the server
                    size_t spacePos1 = userInput.find(' ');
                    if (spacePos1 != string::npos)
                    {
                        // Extract receiver's name and message
                        string firstPart = trim(userInput.substr(0, spacePos1));
                        string secondPart = trim(userInput.substr(spacePos1 + 1));
                        // Construct the message to be sent to the server
                        string privateServer = "\033[1;31mServer: \033[0m";
                        string messageOfServer = privateServer + secondPart;

                        // Send the message to the specified client
                        if (clients.size() == 1)
                        {
                            auto &client = clients[0];
                            if (firstPart == client.getName())
                            {
                                cout << "You sent a message to: " << firstPart << endl;
                                send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                            }
                        }
                        else {
                            for (auto &client : clients)
                            {
                                if (firstPart == client.getName())
                                {
                                    cout << "You sent a message to: " << firstPart << endl;
                                    send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
        // Sleep for a short interval to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}