#pragma once

#include "../chatProgram.hpp"
#include "clientParam.hpp"

#include <atomic>

extern std::atomic<bool> isConnectionTerminated;

void clientInterruptHandler(const int signo);

void initSocket(struct sockaddr_in& sockAddr, const IPv4& IP, const uint16_t port);
bool clientSocketSetting(int& clientSocketDescriptor, const IPv4& IP, const uint16_t port);
bool clientConnection(int clientSocketDescriptor);
bool chattingWithServer(int clientSocketDescriptor, std::string nickname);

#ifdef DEBUG
/**
 * @brief Print connection error message
 */
void printConnectError();
#endif