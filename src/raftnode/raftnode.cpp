#include "raftnode.hpp"

RaftNode::RaftNode(int id) : id(id){
    
}

std::string RaftNode::getPort(){
    return this->portNumber;
}

void RaftNode::addConnection(SOCKET newConn){
    this->conns.emplace_back(ConnectedSocket(newConn));
}

void RaftNode::pollSockets(std::vector<WSAPOLLFD> &fdArray, short revent){
    //Polling
    for(int i = 1; i < fdArray.size(); ++i){
        int conn_i = i - 1;
        if(fdArray[i].revents & (POLLHUP | POLLERR | POLLNVAL)){
            conns.erase(conns.begin() + conn_i);
            continue;
        }

        if(fdArray[i].revents & revent){
            if(revent == POLLWRNORM){
                //Send rest of buffer data here; Handle message is responsible for snd init response.
                //fdArray[i].sendBuffer()
                continue;
            }
            int response = this->conns[conn_i].receive();
            
            if(response == CON_DROPPED){
                conns.erase(conns.begin() + conn_i);
                continue;
            }
            
            if(revent == POLLRDNORM & response == DATA_READY){
                int HEADER_SIZE = 9;
                //We have all the data we need
                Protocol protocol;
                std::vector<uint8_t>& buffer = this->conns[conn_i].getReadBuffer();
                RaftMessage msg = protocol.unpack(buffer.data(), buffer.size());
                
                int end = static_cast<int>(msg.payLoadSize);
                
                //Clear buffer from [0, 9 + end]
                this->conns[conn_i].clearBuffer(HEADER_SIZE + end, buffer);
                
                std::cout << msg << std::endl;
                //Handle the message.
                //this->handleMessage(msg);
                
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
        std::vector<WSAPOLLFD> readArray;
        std::vector<WSAPOLLFD> writeArray;

        readArray.push_back(listen_poll);
        for(int i = 0; i < this->conns.size(); ++i){
            WSAPOLLFD read_fdNew{};
            WSAPOLLFD write_fdNew{};


            read_fdNew.fd = this->conns[i].getSocket();
            read_fdNew.events = POLLRDNORM;
            read_fdNew.revents = 0;
            
            write_fdNew.fd = this->conns[i].getSocket();
            write_fdNew.events = POLLWRNORM;
            write_fdNew.revents = 0;

            writeArray.push_back(write_fdNew);
            readArray.push_back(read_fdNew);
        }

        //Thread waits here for connection
        int event = WSAPoll(readArray.data(), static_cast<ULONG>(readArray.size()), timeoutMS);
        
        if(event == SOCKET_ERROR){
            std::cout << "WSAPOll failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
        
        if(event == 0){
            continue;
        }

        if(readArray[0].revents & POLLRDNORM){
            SOCKET new_conn = accept(listen.getSocket(), nullptr, nullptr);
            this->conns.emplace_back(ConnectedSocket(new_conn));
        } 

        pollSockets(readArray, POLLRDNORM);
        pollSockets(writeArray, POLLWRNORM);
    }
}
RaftNode::~RaftNode(){}