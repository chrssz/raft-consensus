#include "socket.hpp"

ConnectedSocket::ConnectedSocket(){

}
ConnectedSocket::ConnectedSocket(SOCKET created) : SocketWrapper(created) {
}
void ConnectedSocket::snd(std::string& data){

}

std::string ConnectedSocket::receive(){
    
}

ConnectedSocket::~ConnectedSocket(){
}