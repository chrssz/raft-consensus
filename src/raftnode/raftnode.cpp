#include "raftnode.hpp"

RaftNode::RaftNode(int id) : id(id){
    
}

std::string RaftNode::getPort(){
    return this->portNumber;
}
void RaftNode::pollSockets(std::vector<WSAPOLLFD> &poll){
    //Polling
    for(int i =0; i < poll.size(); ++i){
        
    }
    
}
void RaftNode::start(std::string port){
    this->portNumber = port;
    ListeningSocket listen(port);
    
    if(listen.setup() == -1){
        std::cout << "Error starting server" << std::endl;
        return;
    }   
    
    WSAPOLLFD poll_fd{};
    std::vector<WSAPOLLFD> fdArray; 
    
    poll_fd.fd = listen.getSocket();
    poll_fd.events = POLLRDNORM;
    poll_fd.revents = 0;

    std::vector<WSAPOLLFD> fdArray;
    fdArray.push_back(poll_fd);

    int timeoutMS = 5000;

    while(true){
        //Thread waits here for connection
        int event = WSAPoll(fdArray.data(), static_cast<ULONG>(fdArray.size()), timeoutMS);
        
        if(event == SOCKET_ERROR){
            std::cout << "WSAPOll failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
        
        if(event == 0){
            continue;
        }

        if(fdArray[0].revents & POLLRDNORM){
            SOCKET new_conn = accept(listen.getSocket(), nullptr, nullptr);
            this->conns.emplace_back(ConnectedSocket(new_conn));
            WSAPOLLFD poll_fdNew{};
            poll_fd.fd = new_conn;
            poll_fd.events = POLLRDNORM;
            poll_fd.revents = 0;

            fdArray.push_back(poll_fdNew);
        }

        pollSockets(fdArray);
    }
}
void RaftNode::addConnection(SOCKET newConn){
    this->conns.emplace_back(ConnectedSocket(newConn));
    
}
RaftNode::~RaftNode(){}