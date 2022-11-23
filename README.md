# echo-client-server
## Objective
* To comprehend how the TCP specification is implemented in Berkeley Software Distribution (BSD) sockets (i.e., Berkeley sockets)
* To learn how to implement a network application using the socket API
## Component
* chat server
    * create a TCP socket
    * waiting for an incoming connection from the client
    * when the TCP connection is successfully connected, the client’s IP address and port number are printed
    * make new thread for managing connection with each clients
* chat client
    * establish a TCP connection to the server
    * make new thread for receiving message from server
## Requirements
* Use socket-related functions (socket, connect, send, recv, bind, listen, accept, etc.).
* The echo-client (hereinafter referred to as the client) makes a TCP connection to the echo-server (server).
* The client receives a message from the user and delivers the message to the server.
* The server outputs the received message to the screen and sends it to the client as it is given the "-e" (echo) option.
* The server sends a message to all connected clients when given the "-b" (broadcast) option.
* When the client receives a message from the server, it outputs it to the screen.
* The server should be capable of requesting access from multiple clients and processing data (hint - thread).