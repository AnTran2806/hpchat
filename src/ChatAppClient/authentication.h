#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "../common/ChatAppClient/library.h"
#include "client.h"

class Authentication {
public:
    void enterCredential(int clientSocket);

    void handleUserInteraction(int clientSocket);
};

#endif // AUTHENTICATION_H
