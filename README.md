## hpchat

## Requirements: Building a chat application to explore various aspects of software development, including networking, user interfaces, and real-time communication. Here are some ideas for chat-related projects in C/C++:
1.	Console-Based Chat Application: -> done
    - Create a simple console-based chat application where users can send and receive messages. This project will help you understand basic socket programming and message handling.
2.	TCP/IP Chat Server and Client: -> done
    - Build a chat server that supports multiple clients using the TCP/IP protocol. Develop a separate client application that connects to the server for messaging.
3.	Multi-Room Chat Application: -> done
    - Enhance your chat application to support multiple chat rooms. Users can join different rooms and communicate within those rooms.
4.	Secure Chat Application: -> in-progress
    - Implement secure communication using encryption. You can explore cryptographic libraries and protocols to ensure that messages are transmitted securely.
5.	Peer-to-Peer Chat: -> done
    - Develop a peer-to-peer chat application where users can connect directly without a centralized server. This project will involve more complex networking concepts.
6.	File Sharing in Chat: -> in-progress
    - Extend your chat application to support file sharing between users. This involves implementing file transfer functionality over the network.
7.	Implement User Authentication: -> in-progress
    - Add user authentication to your chat application to ensure secure access. Users might need to create accounts and log in before joining chat rooms.

## How to run:
``` bash
cd hpchat
make # requires make and g++ with C++14 support installed
```

### To start the server:

``` bash
./server
# or ./server PORT 
# like: ./server 
```

Defaults:
- `PORT`: ``
- `BACKLOG`: ``

### To start a client:

``` bash
./client
# or ./client HOST PORT
```

Defaults:
- `HOST`: ``
- `PORT`: ``

You can get the `HOST` for your server through `ifconfig` command.
