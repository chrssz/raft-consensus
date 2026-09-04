#pragma once
#include "../../../cpp-load-balancer/src/socket/socket.hpp" //Imported from previous cpp-load-balancer-project
//Socket.hpp also already imports winsock for us.

#include <vector>
class RaftNode{
    private:
        std::string portNumber;
        std::vector<ConnectedSocket> conns;
    public:
        RaftNode();
        std::string getPort();
        void start(std::string port);
        void addConnection(SOCKET newConn);
        ~RaftNode();
};
