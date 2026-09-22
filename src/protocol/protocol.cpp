#include "protocol.hpp"

Protocol::Protocol(){}
uint8_t Protocol::extractBits(uint8_t data, bitWidth amnt){
    return data & ((1 << amnt) - 1);
}

RaftMessage Protocol::unpack(uint8_t* data, int size){
    RaftMessage msg;
    
    uint8_t opCodeNumeric = this->extractBits(data[0], OPCODE);
    msg.opcode = this->bytesToOp[opCodeNumeric];

    data[0] = data[0] >> OPCODE;
    
    msg.sender = this->extractBits(data[0], SENDER);
    
    msg.term[0] = data[1];
    msg.term[1] = data[2];
    msg.term[2] = data[3];

    int sizeForPayload = size - 4;
    
    //Payload will be here.
    switch (static_cast<OpCode>(opCodeNumeric)){
        case OpCode::RequestVote:
            break;

        case OpCode::VoteResponse:
            break;
        
        case OpCode::HeartBeat:
            break;

    }
    

    return msg;
}

uint32_t Protocol::pack(RaftMessage& msg){
    
}

Protocol::~Protocol(){}
