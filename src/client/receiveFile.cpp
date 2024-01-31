#include "client.h"

void Client::receiveFile() {
        char filename[256];
        read(sock, filename, sizeof(filename));
        cout << "File Name: " << filename << endl;

        // Create the time to receive the file
        time_t currentTime = time(nullptr);
        tm* localTime = localtime(&currentTime);
        char timeBuffer[32];
        strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d%H%M%S", localTime);

        // instert the time with the file name
        string newFilename = string(timeBuffer) + "_" + string(filename);

        cout << "Receiving file..." << endl;

        ofstream file(newFilename,ios::binary);
        if (!file.is_open()) {
            cerr << "Error opening file for writing: " << newFilename << endl;
            return;
        }

        // Nhận số byte của file
        int fileSize;
        recv(sock, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

        char buffer[4096];
        int totalReceived = 0;
        while (totalReceived < fileSize) {
            int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                cerr << "Error: Server is down" << endl;
                close(sock);
                return;
            }
            if (strcmp(buffer,"exit")==0){
                break;
            }
            file.write(buffer, bytesReceived);
            totalReceived += bytesReceived;
        }

        file.close();
        cout << "File received successfully: " << newFilename << endl;
    }