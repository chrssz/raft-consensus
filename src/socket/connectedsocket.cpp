#include "socket.hpp"

ConnectedSocket::ConnectedSocket(){

}
ConnectedSocket::ConnectedSocket(SOCKET created) : SocketWrapper(created) {
}
void ConnectedSocket::snd(std::vector<int8_t> &data){
    //Sends data to this->s
    int bufferSize = static_cast<int>(data.size());
    size_t bytesSent = 0;

    while(bytesSent < bufferSize){
        int byte = send(
            this->s,  
            reinterpret_cast<const char*>(data.data() + bytesSent), 
            static_cast<int>(data.size() - bytesSent), 
            0
        );
        
        bytesSent += byte;
    }
}

std::vector<int8_t> ConnectedSocket::receive(){
    //Receive data to this ->s
    //How much data? Hm.
    std::vector<int8_t> output;
    
    return output;
}

ConnectedSocket::~ConnectedSocket(){
}