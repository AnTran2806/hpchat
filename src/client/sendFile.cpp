#include "client.h"

void Client::sendFile(const string &filePath)
{
    // Open the file for reading in binary mode
    ifstream file(filePath, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error opening file for reading: " << filePath << endl;
        return;
    }
    char buffer[4096];
    // Read data from the file until the end of file is reached
    while (!file.eof())
    {
        file.read(buffer, sizeof(buffer));
        send(sock, buffer, file.gcount(), 0);
    }
    // Send a termination message indicating the end of file transmission
    send(sock, "endsendfile", strlen("endsendfile") + 1, 0);
    file.close();
    cout << "File sent successfully.\n";
}