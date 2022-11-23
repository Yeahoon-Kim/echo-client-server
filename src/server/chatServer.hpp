#pragma once

#include "../chatProgram.hpp"
#include "serverParam.hpp"

#include <unordered_set>
#include <vector>
#include <mutex>

#define BACKLOG 5
#define MAX_NICKNAME_LENGTH 20

extern std::unordered_set<int> clientList;
extern std::mutex m;

void serverInterruptHandler(int signo);

bool initSocket(struct sockaddr_in& sockAddr, const int port);
bool serverSocketSetting(int& serverSocketDescriptor, const int port);
int acceptConnection(const int serverSocketDescriptor, struct sockaddr* clientSocketPtr);

void printConnnectionSuccessful(const struct sockaddr_in& clientSocket);
void threadConnection(int clientSocketDescriptor, const struct sockaddr_in& clientSocket, bool isEcho, bool isBroadcast);

bool broadcast(const int senderSocketDescriptor, const std::string& msg, const bool isMirrored);

#ifdef DEBUG
void printBindError();
void printListenError();
void printAcceptError();
#endif
