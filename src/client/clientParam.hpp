#pragma once

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "ip.hpp"

class Param {
private:
    IPv4 IP;
    uint16_t port;
public:
    Param() = default;

    bool parse(int argc, char* argv[]) {
        char* endptr;

        if(argc < 3) {
            this->printUsageError();
            return false;
        }

        port = strtol(argv[2], &endptr, 10); 
        IP = IPv4(argv[1]);

        return (port != 0);
    }

    int getPort() const { return this->port; }
    IPv4 getIP() const { return this->IP; }

    void printUsageError() {
        std::cerr << "syntax : echo-client <ip> <port>\n";
        std::cerr << "echo-client 192.168.10.2 1234" << std::endl;
    }
};