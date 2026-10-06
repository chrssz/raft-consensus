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
    
    msg.term = 0;
    

    //Term in [1]..[4]
    int shiftAmnt = 0;
    for(int i = 4; i >= 1; --i){
        msg.term = msg.term | (this->extractBits(data[i], LAST_EIGHT) << shiftAmnt);
        shiftAmnt += 8;
    }
    
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

std::vector<uint8_t> Protocol::pack(RaftMessage& msg){
    std::vector<uint8_t> data; 
    //Data[0] should contain Sender+Opcode; left to right
    uint8_t opCodeSender = 0;
    opCodeSender |= (msg.sender << SENDER);
    opCodeSender |= (this->opToBytes[msg.opcode]);
    

    //4 Bytes for term
    std::vector<uint8_t> termData;
    
    
    std::vector<uint8_t> payloadSizeData = {0, 0, 0, 0};

    //Payload size  field is 32 bits; need 4 bytes

    uint32_t mutable_payloadSizeVar = msg.payLoadSize;
    for(int i = 0; i < 4; ++i){
        payloadSizeData[payloadSizeData.size() - i - 1] = static_cast<uint8_t>(mutable_payloadSizeVar);
        mutable_payloadSizeVar >>= LAST_EIGHT;
        
    }


    

}

Protocol::~Protocol(){}
