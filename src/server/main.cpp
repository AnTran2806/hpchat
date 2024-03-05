#include "server.h"

int main() {
    Server server;
    UserAuthentication auth(&server);
    Connection connection(auth);
    connection.start(PORT);

    
    return 0;
}