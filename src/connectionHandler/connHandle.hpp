#pragma once


#include "../protocol/protocol.hpp"

class ConnHandle{
    ConnHandle();
    bool handleConnection(RaftMessage msg);
    ~ConnHandle();
};