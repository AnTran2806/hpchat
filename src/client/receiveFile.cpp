#include "client.h"

void Client::receiveFile()
{
    char filename[256];
    read(sock, filename, sizeof(filename));
    //
    cout << "File Name: " << filename << endl;
    cout << "Receiving file..." << endl;

    ofstream file(filename, ios::binary | ios::app);
    if (!file.is_open())
    {
        cerr << "Error opening file for writing: " << filename << endl;
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
        std ::cerr << "Read Error " << endl;
    }

    file.close();
    cout << "File received successfully: " << filename << endl;
}