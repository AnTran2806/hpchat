// #include "server.h"

// void Server::handleServerChat(int clientSocket, const std::string &clientName)
// {
//     // Display a message indicating that the client wants to chat with the server
//     std::string clientNameMessage = "\033[1;34m" + clientName + "\033[0m";
//     std::cout << clientNameMessage << " wants to chat with the server." << std::endl;

//     // Loop for handling server chat
//     std::string privateClientMessage;
//     std::string userInput;
//     char buffer[4096];
//     while (true)
//     {
//         // Prepare the file descriptor set for select
//         fd_set readSet;
//         FD_ZERO(&readSet);
//         FD_SET(STDIN_FILENO, &readSet);
//         FD_SET(clientSocket, &readSet);

//         // Set the timeout for select
//         struct timeval timeout;
//         timeout.tv_sec = 0;
//         timeout.tv_usec = 0;

//         // Call select to check for ready file descriptors
//         int ready = select(std::max(STDIN_FILENO, clientSocket) + 1, &readSet, NULL, NULL, &timeout);
//         std::string exitNotification = "\033[1;32mYou exited the private chat with SERVER\033[0m";

//         // If there is a file descriptor ready for reading
//         if (ready > 0)
//         {
//             // Check if clientSocket is ready to read
//             if (FD_ISSET(clientSocket, &readSet))
//             {
//                 // Receive data from the client
//                 int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//                 if (bytesReceived <= 0)
//                 {
//                     // If no data is received, the client is considered offline
//                     std::cerr << "Client " << clientName << " offline!" << std::endl;
//                     break;
//                 }
//                 else {
//                     // Process the received message from the client
//                     privateClientMessage = std::string(buffer, 0, bytesReceived);
//                     if (privateClientMessage.compare(0, 4, "exit") == 0)
//                     {
//                         // If the client sends an "exit" command, exit the chat loop
//                         std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
//                         send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
//                         break;
//                     }
//                     std::cout << clientNameMessage << ": " << privateClientMessage << std::endl;
//                 }
//             }

//             // Check if STDIN_FILENO is ready to read
//             if (FD_ISSET(STDIN_FILENO, &readSet))
//             {
//                 // Read user input from stdin
//                 std::getline(std::cin, userInput);
//                 if (userInput.compare(0, 4, "exit") == 0)
//                 {
//                     // If the user types "exit", exit the chat loop
//                     std::cout << "\033[1;32mYou exited the private chat with SERVER\033[0m" << std::endl;
//                     send(clientSocket, exitNotification.c_str(), exitNotification.size() + 1, 0);
//                     break;
//                 }
//                 else {
//                     // Handle user input and send messages to the server or perform other actions
//                     size_t spacePos1 = userInput.find(' ');
//                     if (spacePos1 != std::string::npos)
//                     {
//                         // Parse the user input to extract receiver's name and message
//                         std::string firstPart = trim(userInput.substr(0, spacePos1));
//                         std::string secondPart = trim(userInput.substr(spacePos1 + 1));

//                         // Construct the message to be sent to the server
//                         std::string privateServer = "\033[1;31mServer: \033[0m";
//                         std::string messageOfServer = privateServer + secondPart;

//                         // Send the message to the specified client
//                         if (clients.size() == 1)
//                         {
//                             auto &client = clients[0];
//                             if (firstPart == client.getName())
//                             {
//                                 std::cout << "You sent a message to: " << firstPart << std::endl;
//                                 send(client.getSocket(), messageOfServer.c_str(), messageOfServer.size() + 1, 0);
//                             }
//                         }
//                         else {
         
