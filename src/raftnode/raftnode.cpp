#include "raftnode.hpp"
#include "../protocol/protocol.hpp"
RaftNode::RaftNode(int id) : id(id){
    
}

std::string RaftNode::getPort(){
    return this->portNumber;
}
void RaftNode::pollSockets(std::vector<WSAPOLLFD> &fdArray){

    //Polling
    for(int i = 1; i < fdArray.size(); ++i){
        int conn_i = i - 1;
        if(fdArray[i].revents & (POLLHUP | POLLERR | POLLNVAL)){
            conns.erase(conns.begin() + conn_i);
            continue;
        }

        if(fdArray[i].revents & POLLRDNORM){

            int response = this->conns[conn_i].receive();
            if(response == CON_DROPPED){
                conns.erase(conns.begin() + conn_i);
                continue;
            }

            if(response == DATA_READY){
                int HEADER_SIZE = 9;
                //We have all the data we need
                Protocol protocol;
                std::vector<uint8_t>& buffer = this->conns[i].getBuffer();
                RaftMessage msg = protocol.unpack(buffer.data(), buffer.size());
                
                int end = static_cast<int>(msg.payLoadSize);
                //Clear buffer from [0, 9 + end]
                this->conns[i].clearBuffer(HEADER_SIZE);

                //Handle the operation.
            }

        }
    }
    
}
void RaftNode::start(std::string port){
    this->portNumber = port;
    ListeningSocket listen(port);
    
    if(listen.setup() == -1){
        std::cout << "Error starting server" << std::endl;
        return;
    }   
    
    WSAPOLLFD listen_poll{};
    
    listen_poll.fd = listen.getSocket();
    listen_poll.events = POLLRDNORM;
    listen_poll.revents = 0;

    

    int timeoutMS = 5000;

    while(true){
        //Because sockets can disconnect, and I want to maintain RAII. Rebuild fdArray everyTime.
        std::vector<WSAPOLLFD> fdArray;
        fdArray.push_back(listen_poll);
        for(int i = 0; i < this->conns.size(); ++i){
            WSAPOLLFD poll_fdNew{};
            poll_fdNew.fd = this->conns[i].getSocket();
            poll_fdNew.events = POLLRDNORM;
            poll_fdNew.revents = 0;
            
            fdArray.push_back(poll_fdNew);
        }

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
        }
        
        pollSockets(fdArray);
    }
}
void RaftNode::addConnection(SOCKET newConn){
    this->conns.emplace_back(ConnectedSocket(newConn));
    
}
RaftNode::~RaftNode(){}