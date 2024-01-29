#include "client.h"

void Client::receiveFile()
{
    char filename[256];
    read(sock, filename, sizeof(filename));
    //
    std::cout << "File Name: " << filename << std::endl;
    std::cout << "Receiving file..." << std::endl;

    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    char buffer[4096];
    int bytesReceived;
    while (true)
    {
        bytesReceived = read(sock, buffer, sizeof(buffer));
        if (bytesReceived <= 0) {
            break;
        }

        if (strcmp(buffer, "endsendfile") == 0) {
            break;
        }
        file.write(buffer, bytesReceived);
    }
    if (bytesReceived < 0) {
        std ::cerr << "Read Error " << std::endl;
    }

    file.close();
    std::cout << "File received successfully: " << filename << std::endl;
}