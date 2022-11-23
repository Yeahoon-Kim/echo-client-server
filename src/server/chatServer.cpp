#include "chatServer.hpp"

int serverSocketDescriptorForSignal;

/**
 * @brief Signal handler function
 * 
 * @param signo signal number
 */
void serverInterruptHandler(const int signo) {
    switch(signo) {
        case SIGINT:
            std::cout << "Keyboard Interrupt\n";
            break;
        case SIGTERM:
            std::cout << "Terminate signal\n";
            break;
        default: break;
    }

    isServerTerminated.store(true);

#ifdef DEBUG
    debug << "Close welcoming socket descriptor\n";
#endif
    close(serverSocketDescriptorForSignal);

#ifdef DEBUG
    debug << "Close each socket descriptor connected with clients\n";
#endif
    [](std::unordered_set<int>& clientList) {
        std::lock_guard<std::mutex> lock(m);

        for(auto it = clientList.begin(); it != clientList.end(); it++) {
            close(*it);
            clientList.erase(it);
        }
    }(clientList);

    exit(EXIT_SUCCESS);
}

/**
 * @brief Print connection successful message
 * 
 * @param clientSocket Include IP and port number from connected server
 */
void printConnnectionSuccessful(const struct sockaddr_in& clientSocket) {
    uint16_t port = ntohs(clientSocket.sin_port);
    char IP[32] = { 0 };
    
    inet_ntop(PF_INET, &clientSocket.sin_addr, IP, sizeof(IP));

    std::cout << "Connection from " << IP << ":" << port << std::endl;
}

/**
 * @brief Thread function connecting and receive/send messages
 * 
 * @param clientSocketDescriptor socket descriptor for client
 * @param clientSocket socket structure
 * @param isEcho If isEcho is on, server retransmits client's message to that client
 * @param isBroadcast If isBroadcast is on, server sends client's message to all client connected
 */
void threadConnection(int clientSocketDescriptor, const struct sockaddr_in& clientSocket, bool isEcho, bool isBroadcast) {
#ifdef DEBUG
    debug << "threadConnection begin" << std::endl;
    debug << "clientSocketDescriptor: " << clientSocketDescriptor << std::endl;
    debug << "isEcho: " << (isEcho ? "True": "False") << '\n';
    debug << "isBroadcast: " << (isBroadcast ? "True": "False") << '\n';
#endif
    std::string nickname, msg;
    std::ostringstream oss;

    printConnnectionSuccessful(clientSocket);
    recvMessage(clientSocketDescriptor, nickname);

#ifdef DEBUG
    debug << "Nickname: " << nickname << '\n';
#endif
    
    while(not isServerTerminated.load()) {
#ifdef DEBUG
        sleep(1);
#endif
        switch(recvMessage(clientSocketDescriptor, msg)) {
        case RECV_FAILURE:
            std::cerr << "Error: Error while receive message from client\n";
            break;
        case RECV_END:
            std::cout << nickname << " is disconnected\n";

            [clientSocketDescriptor](std::unordered_set<int>& clientList) {
                std::lock_guard<std::mutex> lock(m);
#ifdef DEBUG
                debug << "Set mutex and close the client socket descriptor\n";
#endif
                close(clientSocketDescriptor);
                clientList.erase(clientList.find(clientSocketDescriptor));
            }(clientList);

            return;
        case RECV_SUCCESS:
            oss.str(""), oss.clear();
            oss << nickname << ": " << msg << '\n';
            std::cout << oss.str();
            if(isBroadcast) {
#ifdef DEBUG
                debug << "Broadcasting to each clients\n";
#endif
                broadcast(clientSocketDescriptor, oss.str(), isEcho);
            }
            else if(isEcho) {
#ifdef DEBUG
                debug << "Resending message to the client\n";
#endif
                sendMessage(clientSocketDescriptor, oss.str());
            }
            break;
        default: break;
        }
    }
}

/**
 * @brief Initialize socket structure(Protocol type, IP, port)
 * 
 * @param sockAddr socket structure want to initialize
 * @param port port number want to bind
 * @return true 
 * @return false 
 */
bool initSocket(struct sockaddr_in& sockAddr, const int port) {
#ifdef DEBUG
    debug << "initSocket begin\n";
#endif

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(port);

    return true;
} 

/**
 * @brief Initialize, bind, and listen socket
 * 
 * @param serverSocketDescriptor Server's Welcoming socket
 * @param port port number want to bind
 * @return true 
 * @return false 
 */
bool serverSocketSetting(int& serverSocketDescriptor, const int port) {
#ifdef DEBUG
    debug << "serverSocketSetting begin\n";
#endif
    struct sockaddr_in serverSocket = {0, 0, 0, 0};
    errno = 0;

    // Create Socket
    serverSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocketDescriptor == -1) {
        std::cerr << "Error: Error while create socket\n";
        
#ifdef DEBUG
        printSocketError();
#endif

        return false;
    }

    serverSocketDescriptorForSignal = serverSocketDescriptor;

    // Initialize socket
    if(not initSocket(serverSocket, port)) {
        std::cerr << "Error: Error while initialize socket" << std::endl;
        return false;
    }

    // Bind a name to a server socket
    if(bind(serverSocketDescriptor, (struct sockaddr*)&serverSocket, (socklen_t)sizeof(serverSocket))) {
        std::cerr << "Error: Error while bind socket to specified address\n";

#ifdef DEBUG
        printBindError();
#endif

        close(serverSocketDescriptor);
        return false;
    }

    // Listen for connections on a socket
    if(listen(serverSocketDescriptor, BACKLOG)) {
        std::cerr << "Error: Error while listening\n";

#ifdef DEBUG
        printListenError();
#endif

        close(serverSocketDescriptor);
        return false;
    }

    return true;
}

/**
 * @brief wait for client and accept client's connection requests
 * 
 * @param serverSocketDescriptor server's welcoming socket
 * @param clientSocketPtr socket structure pointer
 * @return int connection socket descriptor number
 */
int acceptConnection(const int serverSocketDescriptor, struct sockaddr* clientSocketPtr) {
    socklen_t clientSocketLength = (socklen_t)sizeof(*clientSocketPtr);
    int clientSocketDescriptor;
    errno = 0;

    clientSocketDescriptor = accept(serverSocketDescriptor, clientSocketPtr, &clientSocketLength);

    if(clientSocketDescriptor == -1) {
        std::cerr << "Error: Error while accept connection\n";

#ifdef DEBUG
        printAcceptError();
#endif

        return -1;
    }
    
    return clientSocketDescriptor;
}

/**
 * @brief broadcast client's message
 * 
 * @param senderSocketDescriptor socket descriptor connected with sender client
 * @param msg message want to broadcast
 * @param isMirrored if isMirrored is on, resend client's message to that client
 * @return true 
 * @return false 
 */
bool broadcast(const int senderSocketDescriptor, const std::string& msg, const bool isMirrored = false) {
#ifdef DEBUG
    debug << "broadcast begin'\n";
    debug << "message: " << msg << std::endl;
#endif

    for(auto sockfd : clientList) {
        if(sockfd == senderSocketDescriptor and not isMirrored) continue;

        while(not sendMessage(sockfd, msg)) {
#ifdef DEBUG
            debug << "Retransmission...\n";
#endif
        }
    }

    return true;
}

#ifdef DEBUG
void printBindError() {
    switch(errno) {
    case EACCES: 
        std::cerr << "The address is protected, and the user is not the superuser.\n";
        break;
    case EADDRINUSE:
        std::cerr << "The given address is already in use.\n";
        break;
    case EBADF:
        std::cerr << "sockfd is not a valid file descriptor.\n";
        break;
    case EINVAL:
        std::cerr << "The socket is already bound to an address.\n";
        std::cerr << "addrlen is wrong, or addr is not a valid address for this socket's domain.\n";
        break;
    case ENOTSOCK:
        std::cerr << "The file descriptor sockfd does not refer to a socket.\n";
        break;
    default:
        std::cerr << "Unknown error occured.\n";
        break;
    }
}

void printListenError() {
    switch(errno) {
    case EADDRINUSE:
        std::cerr << "Another socket is already listening on the same port.\n";
        break;
    case EBADF:
        std::cerr << "The argument sockfd is not a valid file descriptor.\n";
        break;
    case ENOTSOCK:
        std::cerr << "The file descriptor sockfd does not refer to a socket.\n";
        break;
    case EOPNOTSUPP:
        std::cerr << "The socket is not of a type that supports the listen() operation.\n";
        break;
    default:
        std::cerr << "Unknown error occured.\n";
        break;
    }
}

void printAcceptError() {
    switch(errno) {
    case EAGAIN:
        std::cerr << "The socket is marked nonblocking and no connections are present to be accepted.\n";
        break;
    case EBADF:
        std::cerr << "sockfd is not an open file descriptor.\n";
        break;
    case ECONNABORTED:
        std::cerr << "A connection has been aborted.\n";
        break;
    case EFAULT:
        std::cerr << "The addr argument is not in a writable part of the user address space.\n";
    case EINTR:
        std::cerr << "The system call was interrupted by a signal that was caught before a valid connection arrived.\n";
        break;
    case EINVAL:
        std::cerr << "Socket is not listening for connections, or addrlen is invalid.\n";
        break;
    case EMFILE:
        std::cerr << "The per-process limit on the number of open file descriptors has been reached.\n";
        break;
    case ENFILE:
        std::cerr << "The system-wide limit on the total number of open files has been reached\n";
        break;
    case ENOBUFS: 
    case ENOMEM:
        std::cerr << "Not  enough  free  memory.\n";
        std::cerr << "This often means that the memory allocation is limited by the socket buffer limits, not by the system memory.\n";
        break;
    case ENOTSOCK:
        std::cerr << "The file descriptor sockfd does not refer to a socket.\n";
        break;
    case EOPNOTSUPP:
        std::cerr << "The referenced socket is not of type SOCK_STREAM.\n";
        break;
    case EPROTO:
        std::cerr << "Protocol error.\n";
        break;
    default:
        std::cerr << "Unknown error occured.\n";
        break;
    }
}
#endif
