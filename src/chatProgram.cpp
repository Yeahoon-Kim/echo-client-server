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
    if(send(sockfd, msg.c_str(), msg.size(), 0) == -1) return false;
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
#endif