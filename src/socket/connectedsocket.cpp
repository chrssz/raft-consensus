#include "socket.hpp"

ConnectedSocket::ConnectedSocket(){
    this->setNonBlocking();
}
ConnectedSocket::ConnectedSocket(SOCKET created) : SocketWrapper(created) {
    this->setNonBlocking();
}
void ConnectedSocket::snd(std::vector<uint8_t> &data){
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

std::vector<uint8_t> ConnectedSocket::receive(){
    //Receive data to this ->s
    
    std::vector<uint8_t> output;
    uint8_t buffer[4096]; //512 Bytes absolute maximum

    //Extract headers
    int HEADER_SIZE = 8; //8 Bytes
    int bytesRecv = 0;

    while(bytesRecv < 8){
        char dataGotten;
        int dataSize = recv(
            this->s,
            reinterpret_cast<char*>(buffer),
            sizeof(buffer),
            0
        );
    }
    
    //Payload size resides at BytesRecv[4]// Payload size is a 32 bit integer. 4 bytes
    //[ByesRecv[4], BytesRecv[8]]
    //TODO WORK ON RECIEVING PAYLOAD,  THINK ABOUT SOME EVENT DRIVEN MODEL SO A RECIEV FUNC KNOWS WHEN DATA IS READY.
    //Payload Here
    
    return output;
}

ConnectedSocket::~ConnectedSocket(){
}