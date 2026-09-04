#include "raftnode.hpp"

RaftNode::RaftNode(){}
std::string RaftNode::getPort(){
    return this->portNumber;
}
void RaftNode::start(std::string port){
    this->portNumber = port;
    ListeningSocket listen(port);
    
    if(listen.setup() == -1){
        std::cout << "Error starting server" << std::endl;
        return;
    }   
    
    WSAPOLLFD poll_fd{};
    poll_fd.fd = listen.getSocket();
    poll_fd.events = POLLRDNORM;
    
    while(true){
        //Thread waits here for connection
        int event = WSAPoll(&poll_fd, 1, -1);
        
        if(event == SOCKET_ERROR){
            std::cout << "WSAPOll failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
        
        if(poll_fd.revents & POLLRDNORM){
            SOCKET new_conn = accept(listen.getSocket(), nullptr, nullptr);
            this->conns.emplace_back(ConnectedSocket(new_conn));
            
        }
    }
}
void RaftNode::addConnection(SOCKET newConn){
    this->conns.emplace_back(ConnectedSocket(newConn));
    
}
RaftNode::~RaftNode(){}