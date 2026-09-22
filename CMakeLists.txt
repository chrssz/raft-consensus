cmake_minimum_required(VERSION 3.16)
project(raft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(raft
    src/main.cpp
    src/socket/socket.cpp
    src/socket/connectedsocket.cpp
    src/socket/listeningsocket.cpp
    src/raftnode/raftnode.cpp
    src/protocol/payload.cpp
    src/protocol/protocol.cpp
    src/connectionHandler/connHandle.cpp
)

target_include_directories(raft
    PRIVATE
        raftnode
        socket
        protocol
        connectionHandler
        cluster
    
)

if(WIN32)
    target_link_libraries(raft PRIVATE ws2_32)
endif()