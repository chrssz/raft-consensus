#include "socket.hpp"

ConnectedSocket::ConnectedSocket(){
    this->setNonBlocking();
}
ConnectedSocket::ConnectedSocket(SOCKET created) : SocketWrapper(created) {
    this->setNonBlocking();
}

std::vector<uint8_t>& ConnectedSocket::getBuffer() {
    return this->buffer;
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
        
    this->buffer.insert(this->buffer.end(), tempBuffer, tempBuffer + dataSize);
    
    //Payload
    if(this->buffer.size() >= 9){
        const uint32_t PAYLOAD_SIZE =
            (static_cast<uint32_t>(this->buffer[5]) << 24) |
            (static_cast<uint32_t>(this->buffer[6]) << 16) |
            (static_cast<uint32_t>(this->buffer[7]) << 8)  |
            (static_cast<uint32_t>(this->buffer[8]));

        const int TOTAL_NEEDED = HEADER_SIZE + PAYLOAD_SIZE;

        
        return (this->buffer.size() >= TOTAL_NEEDED) ? 2 : 1;
    }

    return 1;
}

void ConnectedSocket::clearBuffer(int end){
    //end is inclusive.
    if(end + 1 > buffer.size()){
        std::cout << "Buffer not cleared. Given end is larger than buffer size";
        return;
    }
    
    auto begin = this->buffer.begin();
    this->buffer.erase(begin, begin + end + 1);

}

void ConnectedSocket::snd(std::vector<uint8_t> &data){
    //Sends data to this->s
}


ConnectedSocket::~ConnectedSocket(){
}