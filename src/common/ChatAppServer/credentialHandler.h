#include "library.h"

class CredentialHandler {
public:
    void receiveCredential(int clientSocket, string& username, string& password);
    void sendResponse(int clientSocket, const string &response);
};