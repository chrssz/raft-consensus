#include "socket.hpp"

ConnectedSocket::ConnectedSocket(){
    this->setNonBlocking();
}
ConnectedSocket::ConnectedSocket(SOCKET created) : SocketWrapper(created) {
    this->setNonBlocking();
}

std::vector<uint8_t>& ConnectedSocket::getReadBuffer() {
    return this->readBuffer;
}

/*  -1: Closed Socket, 1: socket is good, but buffer data is not ready. 2: buffer data is ready to be extracted*/
int ConnectedSocket::receive(){
    const int HEADER_SIZE = 9; //9 Bytes
    char tempBuffer[1024];

    int dataSize = recv(
        this->s,
        tempBuffer,
        sizeof(tempBuffer),
        0
    );

    if(dataSize == 0 | (dataSize == SOCKET_ERROR)){
        std::cout << "Connection Lost. Error: " << WSAGetLastError() << std::endl;
        return -1;
    }
        
    this->readBuffer.insert(this->readBuffer.end(), tempBuffer, tempBuffer + dataSize);
    
    //Payload
    if(this->readBuffer.size() >= 9){
        const uint32_t PAYLOAD_SIZE =
            (static_cast<uint32_t>(this->readBuffer[5]) << 24) |
            (static_cast<uint32_t>(this->readBuffer[6]) << 16) |
            (static_cast<uint32_t>(this->readBuffer[7]) << 8)  |
            (static_cast<uint32_t>(this->readBuffer[8]));

        const int TOTAL_NEEDED = HEADER_SIZE + PAYLOAD_SIZE;

        
        return (this->readBuffer.size() >= TOTAL_NEEDED) ? 2 : 1;
    }

    return 1;
}

int ConnectedSocket::snd(std::vector<uint8_t> &data){
    //Sends data to this->s
    int res = send(this->s, 
                reinterpret_cast<const char*>(data.data()), 
                static_cast<int>(data.size()), 
                0);
    
    if(res == SOCKET_ERROR){
        std::cout << "Connection Lost. Error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    
    return 1;
}

void ConnectedSocket::clearBuffer(int end, std::vector<uint8_t>& buffer){
    //end is inclusive.
    if(end + 1 > buffer.size()){
        std::cout << "Buffer not cleared. Given end is larger than buffer size";
        return;
    }
    
    auto begin = buffer.begin();
    buffer.erase(begin, begin + end + 1);
    
}


ConnectedSocket::~ConnectedSocket(){
}