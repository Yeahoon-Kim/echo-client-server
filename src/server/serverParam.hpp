#pragma once

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>

class Param {
private:
    bool echo, broadcast;
    uint16_t port;
public:
    Param() = default;

    bool parse(int argc, char* argv[]) {
        char* endptr;

        if(argc < 2) {
            this->printUsageError();
            return false;
        }

        port = strtol(argv[1], &endptr, 10); 

        for(int i = 2; i < argc; i++) {
            if(not strncmp(argv[i], "-e", 2)) this->echo = true;
            else if(not strncmp(argv[i], "-b", 2)) this->broadcast = true;
        }

        return (port != 0);
    }

    bool isEcho() const { return this->echo; }
    bool isBroadcast() const { return this->broadcast; }
    int getPort() const { return this->port; }

    void printUsageError() {
        std::cerr << "syntax : echo-server <port> [-e[-b]]\n";
        std::cerr << "sample : echo-server 1234 -e -b" << std::endl;
    }
};