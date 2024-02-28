#include "server.h"

int main() {
    Server server;
    Connection connection(server); // Pass the Server object to the Connection constructor
    connection.start(PORT);
    
    return 0;
}