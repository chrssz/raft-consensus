#pragma once
#include "../socket/socket.hpp" //Imported from previous cpp-load-balancer-project
//Socket.hpp also already imports winsock for us.
#include <iostream>
#include <vector>
#include <cmath>
enum SOCKET_STATUS : int {
    CON_DROPPED = -1,
    CON_GOOD = 1,
    DATA_READY = 2
};


class RaftNode{
    private:
        int id;
        uint8_t votedFor;
        uint32_t term;
        //Log; to be implemented later.
        std::string portNumber;
        std::vector<ConnectedSocket> conns;
    public:
        RaftNode(int id);
        std::string getPort();

        void addConnection(SOCKET newConn);
        void handleMessage(const RaftMessage& msg);
        
        void pollSockets(std::vector<WSAPOLLFD> &poll, short revent);
        void start(std::string port);
        
        ~RaftNode();
};
