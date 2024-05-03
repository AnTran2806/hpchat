#include "chatService.h"
#include "connection.h"

int main() {
    ChatService chatService;
    UserAuthentication auth(&chatService);
    Connection connection(auth);
    connection.start(PORT);
    return 0;
}