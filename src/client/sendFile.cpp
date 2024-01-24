  #include "client.h"
  
   void Client::sendFile(const std::string& filePath) {
        send(createSocketClient, "sendfile", strlen("sendfile") + 1, 0);
        send(createSocketClient,filePath.c_str(),filePath.size()+1,0);

        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file for reading: " << filePath << std::endl;
            return;
        }

        char buffer[4096];
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            send(createSocketClient, buffer, file.gcount(), 0);
        }
        send(createSocketClient, "exit", strlen("exit") + 1, 0);
        file.close();
        std::cout << "File sent successfully.\n";
    }