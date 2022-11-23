#include "chatClient.hpp"

/**
 * @brief Signal handler function
 * 
 * @param signo signal number
 */
void clientInterruptHandler(const int signo) {
    switch(signo) {
    case SIGINT:
        std::cout << "Keyboard Interrupt\n";
        break;
    case SIGTERM:
        std::cout << "Terminate signal\n";
        break;
    default: break;
    }

    isConnectionTerminated.store(true);
}

/**
 * @brief Initialize socket structure(IP, port)
 * 
 * @param sockAddr socket structure
 * @param IP IP number want to connect
 * @param port port number want to connect
 */
void initSocket(struct sockaddr_in& sockAddr, const IPv4& IP, const uint16_t port) {
#ifdef DEBUG
    debug << "initSocket begin\n";
#endif

    sockAddr.sin_port = htons(port);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(IP);
}

/**
 * @brief Create, initialize socket, and try to connect
 * 
 * @param clientSocketDescriptor client socket descriptor
 * @param IP IP number want to connect
 * @param port port number want to connect
 * @return true
 * @return false 
 */
bool clientSocketSetting(int clientSocketDescriptor, const IPv4& IP, const uint16_t port) {
    struct sockaddr_in serverSocket = { 0 };
    errno = 0;

#ifdef DEBUG
    debug << "clientSocketSetting begin\n";
#endif

    clientSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocketDescriptor == -1) {
        std::cerr << "Error: Error while create socket\n";

#ifdef DEBUG
        printSocketError();
#endif

        return false;
    }

    initSocket(serverSocket, IP, port);

    if(connect(clientSocketDescriptor, (struct sockaddr*)&serverSocket, sizeof(serverSocket))) {
        std::cerr << "Error: Error while connecting to server\n";

#ifdef DEBUG
        printConnectError();
#endif

        return false;
    }

    isConnectionTerminated.store(false);

    return true;
}

/**
 * @brief Receive message from server and print to standard display
 * 
 * @param clientSocketDescriptor client socket description number
 * @return true
 * @return false
 */
bool clientConnection(int clientSocketDescriptor) {
#ifdef DEBUG
    debug << "threadConnection begin\n";
#endif
    std::string msg;

    while(not isConnectionTerminated.load()) {
        switch(recvMessage(clientSocketDescriptor, msg)) {
        case RECV_FAILURE:
            std::cerr << "Error: Error while receive message from server\n";
            break;
        case RECV_END:
            std::cout << "Disconnected\n";
            isConnectionTerminated.store(true);
            break; 
        case RECV_SUCCESS:
            std::cout << msg << '\n';
            break;
        default: break;
        }
    }
}

/**
 * @brief send client's nickname and messages to server 
 * 
 * @param clientSocketDescriptor socket description number for connecting server
 * @param nickname user nickname
 * @return true
 * @return false
 */
bool chattingWithServer(int clientSocketDescriptor, std::string nickname) {
#ifdef DEBUG
    debug << "chattingWithServer begin\n";
#endif

    std::string msg;

    std::thread(clientConnection, clientSocketDescriptor);

    if(sendMessage(clientSocketDescriptor, nickname)) {
        std::cerr << "Error: Error while sending nickname to server\n";
        return false;
    }

    while(not isConnectionTerminated.load()) {
        std::cin >> msg;
        if(not sendMessage(clientSocketDescriptor, msg)) {
            std::cerr << "Error: Error while sending message to server\n";
            return false;
        }
    }

    std::cout << "**Server Terminated!\n";

    return true;
}

#ifdef DEBUG
void printConnectError() {
    switch(errno) {
    case EACCES:
    case EPERM:
        std::cerr << "Write permission is denied on the socket file." << std::endl;
        break;
    case EADDRINUSE:
        std::cerr << "Local address is already in use." << std::endl;
        break;
    case EADDRNOTAVAIL:
        std::cerr << "The socket referred to by sockfd had not previously been bound to an address." << std::endl;
        std::cerr << "All port numbers in the ephemeral port range are currently in use." << std::endl;
        break;
    case EAFNOSUPPORT:
        std::cerr << "The passed address didn't have the correct address family in its sa_family field." << std::endl;
        break;
    case EAGAIN: 
        std::cerr << "For nonblocking UNIX domain sockets, the socket is nonblocking, and the connection cannot be completed immediately." << std::endl;
        break;
    case EALREADY: 
        std::cerr << "The socket is nonblocking and a previous connection attempt has not yet been completed." << std::endl;
        break;
    case EBADF:
        std::cerr << "sockfd is not a valid open file descriptor." << std::endl;
        break;
    case ECONNREFUSED:
        std::cerr << "A connect() on a stream socket found no one listening on the remote address." << std::endl;
        break;
    case EFAULT:
        std::cerr << "The socket structure address is outside the user's address space." << std::endl;
        break;
    case EINPROGRESS:
        std::cerr << " The socket is nonblocking and the connection cannot be completed immediately." << std::endl;
        break;
    case EINTR:
        std::cerr << "The system call was interrupted by a signal that was caught; see signal(7)." << std::endl;
        break;
    case EISCONN:
        std::cerr << "The socket is already connected." << std::endl;
        break;
    case ENETUNREACH:
        std::cerr << "Network is unreachable." << std::endl;
        break;
    case ENOTSOCK:
        std::cerr << "The file descriptor sockfd does not refer to a socket." << std::endl;
        break;
    case EPROTOTYPE:
        std::cerr << "The socket type does not support the requested communications protocol." << std::endl;
        break;
    case ETIMEDOUT:
        std::cerr << "Timeout while attempting connection." << std::endl;
        break;
    default:
        std::cerr << "Unknown error occured" << std::endl;
        break;
    }
}
#endif