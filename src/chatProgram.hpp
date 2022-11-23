#pragma once

#include <iostream>
#include <csignal>
#include <exception>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <thread>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifdef DEBUG
#define debug std::cout << "[DEBUG] "
#endif

#define EXIT_FAILURTE 1
#define EXIT_SUCCESS 0

#define MAX_MSG_LENGTH 0x1000

enum RECV_RETURN {
    RECV_SUCCESS, RECV_FAILURE, RECV_END
};

RECV_RETURN recvMessage(const int sockfd, std::string& msg);
bool sendMessage(const int sockfd, const std::string& msg);

#ifdef DEBUG
void printSocketError();
#endif