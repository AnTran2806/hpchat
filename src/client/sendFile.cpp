#include "client.h"

void Client:: sendFile(const string& filePath) {
        ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            cerr << "Error opening file for reading: " << filePath << std::endl;
            return;
        }

        // send the byte of the file before sending the data
        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        send(sock, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

        char buffer[4096];
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            send(sock, buffer, file.gcount(), 0);
        }
        file.close();
        cout << " File sent successfully, please press the endsendfile to end this.\n";
        
    }