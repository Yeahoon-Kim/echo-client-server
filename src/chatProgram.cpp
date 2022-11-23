#include "chatProgram.hpp"

/**
 * @brief Receive message from socket
 * 
 * @param sockfd socket file descriptor
 * @param msg buffer for storing message from the socket
 * @return RECV_RETURN consist of RECV_SUCCESS, RECV)FAILURE, RECV_END
 */
RECV_RETURN recvMessage(const int sockfd, std::string& msg) {
#ifdef DEBUG
    debug << "recvMessage begin\n";
#endif
    char buffer[MAX_MSG_LENGTH] = { 0 };
    int recvLen = recv(sockfd, buffer, MAX_MSG_LENGTH - 1, 0);

    if(recvLen < 0) return RECV_FAILURE;
    else if(recvLen == 0) return RECV_END;

    buffer[recvLen] = '\0';

#ifdef DEBUG
    debug << "Received message: " << buffer << '\n';
#endif  
    msg = buffer;

    return RECV_SUCCESS;
}

/**
 * @brief Send message to socket
 * 
 * @param sockfd socket file descriptor
 * @param msg message for sending to the socket
 * @return true 
 * @return false 
 */
bool sendMessage(const int sockfd, const std::string& msg) {
#ifdef DEBUG
    debug << "sendMessage begin\n";
    debug << "Send message: " << msg << '\n';
#endif
    if(send(sockfd, msg.c_str(), msg.size(), 0) == -1) {
#ifdef DEBUG
        printSendError();
#endif

        return false;
    }
    return true;
}

#ifdef DEBUG
void printSocketError() {
    switch(errno) {
    case EACCES: 
        std::cerr << "Permission to create a socket of the specified type and/or protocol is denied." << std::endl;
        break;
    case EAFNOSUPPORT: 
        std::cerr << "The implementation does not support the specified address family." << std::endl;
        break;
    case EINVAL: 
        std::cerr << "Unknown protocol, or protocol family not available." << std::endl;
        break;
    case EMFILE: 
        std::cerr << "The per-process limit on the number of open file descriptors has been reached." << std::endl;
        break;
    case ENFILE: 
        std::cerr << "The system-wide limit on the total number of open files has been reached." << std::endl;
        break;
    case ENOBUFS:
    case ENOMEM:
        std::cerr << "Insufficient memory is available.  The socket cannot be created until sufficient resources are freed." << std::endl;
        break;
    case EPROTONOSUPPORT:
        std::cerr << "The protocol type or the specified protocol is not supported within this domain." << std::endl;
    default: 
        std::cerr << "Unknown error occured" << std::endl;
        break;
    }
}

void printSendError() {
    switch(errno) {
    case EACCES:
        std::cerr << "Write permission is denied on the destination socket file, or search permission is denied for one of the directories the path prefix." << std::endl;
        break;
    case EAGAIN:
        std::cerr << "The socket is marked nonblocking and the requested operation would block." << std::endl;
        break;
    case EALREADY:
        std::cerr << "Another Fast Open is in progress." << std::endl;
        break;
    case EBADF:
        std::cerr << "sockfd is not a valid open file descriptor." << std::endl;
        break;
    case ECONNRESET:
        std::cerr << "Connection reset by peer." << std::endl;
        break;
    case EDESTADDRREQ:
        std::cerr << "The socket is not connection-mode, and no peer address is set." << std::endl;
        break;
    case EFAULT:
        std::cerr << "An invalid user space address was specified for an argument." << std::endl;
        break;
    case EINTR:
        std::cerr << "A signal occurred before any data was transmitted." << std::endl;
        break;
    case EINVAL:
        std::cerr << "Invalid argument passed." << std::endl;
        break;
    case EISCONN:
        std::cerr << "The connection-mode socket was connected already but a recipient was specified." << std::endl;
        break;
    case EMSGSIZE:
        std::cerr << "The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible." << std::endl;
        break;
    case ENOBUFS:
        std::cerr << "The output queue for a network interface was full." << std::endl;
        break;
    case ENOMEM:
        std::cerr << "No memory available." << std::endl;
        break;
    case ENOTCONN:
        std::cerr << "The socket is not connected, and no target has been given." << std::endl;
        break;
    case ENOTSOCK:
        std::cerr << "The file descriptor sockfd does not refer to a socket." << std::endl;
        break;
    case EOPNOTSUPP:
        std::cerr << "Some bit in the flags argument is inappropriate for the socket type." << std::endl;
        break;
    case EPIPE:
        std::cerr << "The local end has been shut down on a connection oriented socket." << std::endl;
        break;
    default:
        std::cerr << "Unknown error occured" << std::endl;
        break;
    }
}
#endif