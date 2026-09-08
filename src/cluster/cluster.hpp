#pragma once

#include "src/raftnode/raftnode.hpp" 
#include <thread>

class Cluster{
    private:
        u_int CLUSTER_SIZE;
    public:
        Cluster(int size);
        SOCKET connectTo(const std::string PORT);
        void start();
        ~Cluster();
};