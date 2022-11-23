#include "chatServer.hpp"

using namespace std;

atomic<bool> isServerTerminated;
unordered_set<int> clientList;
mutex m;

int main(int argc, char* argv[]) {
    signal(SIGINT, serverInterruptHandler);
    signal(SIGTERM, serverInterruptHandler);

    int serverSocketDescriptor, clientSocketDescriptor;
    struct sockaddr_in clientSocket{0, 0, 0, 0};
    Param param;

    if(not param.parse(argc, argv)) exit(EXIT_FAILURE);
    if(not serverSocketSetting(serverSocketDescriptor, param.getPort())) exit(EXIT_FAILURE);
    isServerTerminated.store(false);

    while( true ) {
        clientSocketDescriptor = acceptConnection(serverSocketDescriptor, (struct sockaddr*)&clientSocket);
        if(clientSocketDescriptor < 0) exit(EXIT_FAILURE);

        [clientSocketDescriptor]() {
            lock_guard<mutex> lock(m);
            clientList.insert(clientSocketDescriptor);
        }();
        
        thread connectThread(threadConnection, clientSocketDescriptor, clientSocket, param.isEcho(), param.isBroadcast());

        connectThread.detach();
    }

    return 0;
}
