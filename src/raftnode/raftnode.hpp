#pragma once
#include "../socket/socket.hpp" //Imported from previous cpp-load-balancer-project
//Socket.hpp also already imports winsock for us.
#include <iostream>
#include <vector>
class RaftNode{
    private:
        int id;
        std::string portNumber;
        std::vector<ConnectedSocket> conns;
    public:
        RaftNode(int id);
        std::string getPort();
        void pollSockets(std::vector<WSAPOLLFD> &poll);
        void start(std::string port);
        void addConnection(SOCKET newConn);
        ~RaftNode();
};
