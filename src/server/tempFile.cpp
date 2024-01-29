// #include <iostream>
// #include <thread>
// #include <cstring>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <fstream>
// #include <chrono>
// #include <ctime>

// class Client {
// private:
//     int sock;
//     std::thread receivingThread;
//     std::string clientName;
//     std::string roomName;

// public:
//     Client() {
//         sock = socket(AF_INET, SOCK_STREAM, 0);
//         if (sock == -1) {
//             std::cerr << "Error: Create the socket" << std::endl;
//             return;
//         }
//     }

//     ~Client() {
//         close(sock);
//         if (receivingThread.joinable()) {
//             receivingThread.join();
//         }
//     }

//     bool connectToServer(const std::string& ipAddress, int port) {
//             sockaddr_in hint;
//         hint.sin_family = AF_INET;
//         hint.sin_port = htons(port);
//         inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

//         int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
//         if (connectRes == -1) {
//             std::cerr << "Error: Cannot connect to Server!" << std::endl;
//             return false;
//         }

//         return true;
//     }
//     void receiveFile() {
//         char filename[256];
//         read(sock, filename, sizeof(filename));
//         //
//         std::cout<<"File Name: "<<filename<<std::endl;
//         std::cout << "Receiving file..." << std::endl;

//         std::ofstream file( filename, std::ios::binary| std::ios::app);
//         if (!file.is_open()) {
//             std::cerr << "Error opening file for writing: " << filename << std::endl;
//             return;
//         }

//         char buffer[4096];
//         int bytesReceived;
//         while (true) {
//             bytesReceived = read(sock, buffer, sizeof(buffer));
//             if (bytesReceived <= 0) {
//             break;
//             }
            
//             if (strcmp(buffer,"endsendfile")==0){
//                 break;
//             }
//             file.write(buffer, bytesReceived);
//         }
//         if (bytesReceived<0){
//             std ::cerr <<"Read Error "<<std::endl;
//         }

//         file.close();
//         std::cout << "File received successfully: " << filename << std::endl;
//     }

//     void startReceiving() {
//         receivingThread = std::thread([this]() {
//             char buffer[4096];
//             while (true) {
//                 int bytesReceived = recv(sock, buffer, 4096, 0);
//                 if (bytesReceived <= 0) {
//                     std::cerr << "Error: Server is down" << std::endl;
//                     close(sock);
//                     break;
//                 }

//                 // Kiểm tra xem có phải là lệnh gửi file không
//                 if (strcmp(buffer, "sendfile") == 0) {
//                     std::cout<<"Receiving a file from another client!";
//                     receiveFile();
//                 } else {
//                     std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
//                 }
//             }
//         });
//     }

//     void sendToServer(const std::string& message) {
//         send(sock, message.c_str(), message.size() + 1, 0);
//     }

//     void sendFile(const std::string& filePath) {
//         // send(sock, "sendfile", strlen("sendfile") + 1, 0);
//         // send(sock,filePath.c_str(),filePath.size()+1,0);

//         std::ifstream file(filePath, std::ios::binary);
//         if (!file.is_open()) {
//             std::cerr << "Error opening file for reading: " << filePath << std::endl;
//             return;
//         }
//         //std::cerr << "tên File trong Hàm gửi file: "<<filePath<<std::endl;
//         char buffer[4096];
//         while (!file.eof()) {
//             file.read(buffer, sizeof(buffer));
//             send(sock, buffer, file.gcount(), 0);
//         }
//         send(sock,"endsendfile", strlen("endsendfile") + 1, 0);
//         file.close();
//         std::cout << "File sent successfully.\n";
//         //send(sock, "endsendfile", strlen("endsendfile") + 1, 0);
//     }



// };

// int main() {
//     Client client;

//     if (client.connectToServer("10.188.9.19", 54000)) {
//         std::string roomName;
//         std::cout << "Enter the Room: ";
//         std::getline(std::cin, roomName);
//         client.sendToServer(roomName);

//         std::string clientName;
//         std::cout << "Enter Your name: ";
//         std::getline(std::cin, clientName);
//         client.sendToServer(clientName);
//         std::cout<<"Enter help to get the guide\n";
        
//         client.startReceiving();

//         while (true) {
//             std::string userInput;
//             std::cout << "You: ";
//             std::getline(std::cin, userInput);

//             if (userInput == "sendfile") {
//                 client.sendToServer("sendfile");

//                 std::string nameOrRoom;
//                 std::cout << "The name to sendfile: ";
//                 std::getline(std::cin, nameOrRoom);
//                 client.sendToServer(nameOrRoom);
                

//                 std::string filePath;
//                 std::cout << "Enter the file path to send: ";
//                 std::getline(std::cin, filePath);
//                 client.sendToServer(filePath);
//                 std::cout << "tên File: "<<filePath<<std::endl;

                
//                 std::thread sendFileThread(&Client::sendFile, &client, filePath);
//                 sendFileThread.detach();
//             }else if (userInput=="help"){
//                 std::cout<<"1.Chat Group: Entering the message then Enter"<<"\n"<<
//                 "2.Private chat: Format:/pm + User's name + your message "<<"\n"<<
//                 "3.Server: /sv + Your message\n"<<
//                 "4.Sendfile: Entering the sendfile.\n"<<
//                 "5./pm + any word to Display the list user's name online"<<std::endl;

//             }
//              else {
//                 client.sendToServer(userInput);
//             }
//         }
//     }

//     return 0;
// }