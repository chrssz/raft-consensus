#include "protocol.hpp"

Protocol::Protocol(){}
uint8_t Protocol::extractBits(uint8_t data, bitWidth amnt){
    return data & ((1 << amnt) - 1);
}

RaftMessage Protocol::unpack(uint8_t data[]){
    RaftMessage msg;
    msg.sender = this->extractBits(data[0], OPCODE);
    data[0] >> OPCODE;
    msg.opcode = this->bytesToOp[this->extractBits(data[0], SENDER)];

    msg.term[0] = data[1];
    msg.term[1] = data[2];
    msg.term[2] = data[3];

    //Payload will be here.

}

uint32_t Protocol::pack(RaftMessage& msg){
    
}

Protocol::~Protocol(){}
