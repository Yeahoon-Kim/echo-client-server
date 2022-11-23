#include "chatClient.hpp"

using namespace std;

atomic<bool> isConnectionTerminated;

int main(int argc, char* argv[]) {
    signal(SIGINT, clientInterruptHandler);
    signal(SIGTERM, clientInterruptHandler);

    int clientSocketDescriptor;
    string nickName;
    Param param;

    if(not param.parse(argc, argv)) exit(EXIT_FAILURE);

    if(clientSocketSetting(clientSocketDescriptor, param.getIP(), param.getPort())) {
        exit(EXIT_FAILURE);
    }

    cout << "Type username: ";
    cin >> nickName;

    if(not chattingWithServer(clientSocketDescriptor, nickName)) exit(EXIT_FAILURE);

    if(close(clientSocketDescriptor)) {
        cerr << "Error: Error while closing socket" << endl;
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
