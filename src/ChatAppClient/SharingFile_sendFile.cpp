#include "client.h"

void Client::sendFile(int clientSocket) {

        string filePath;
            cout << "Enter the file path to send: ";
            getline(cin, filePath);
            sendToServer(clientSocket, filePath);
            cout << "File Name: " << filePath << endl;
            // thread sendFileThread(&SharingFile::sendFile,this, clientSocket, filePath);
            // sendFileThread.detach();

        ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            cerr << "Error opening file for reading: " << filePath << std::endl;
            return;
        }

        // send the byte of the file before sending the data
        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        send(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

        char buffer[4096];
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            send(clientSocket, buffer, file.gcount(), 0);
        }
        file.close();
        cout << " File sent successfully, please enter the 'exit' to end this.\n";       
}