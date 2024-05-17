#include "library.h"

class CredentialReceiver {
public:
    void receiveCredential(int clientSocket, string& username, string& password); 
};