#include "protocol.hpp"

std::ostream& operator<<(std::ostream& os, const RaftMessage& obj){
    os << "Opcode: " << obj.opcode << std::endl;
    os << "Sender: " << obj.sender << std::endl;
    os << "Term: " << obj.term << std::endl;
    os << "Payload Size: " << obj.payLoadSize << std::endl;

    return os;
}

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

    //Payload Size in [5]..[8]
    msg.payLoadSize = 0;
    shiftAmnt = 0;
    for(int i = 8; i >= 5; --i){
        msg.payLoadSize = msg.payLoadSize | (this->extractBits(data[i], LAST_EIGHT) << shiftAmnt);
        shiftAmnt += 8;
    }
    
    //Payload data resides in data [9] .... [payloadSize]
    int n = msg.payLoadSize;
    //Payload data ends a 9 + msg.payLoadSize 
    PayLoadDecode decode;
    std::unique_ptr<PayLoad> payLoad;
    //Payload will be here.
    switch (static_cast<OpCode>(opCodeNumeric)){
        case OpCode::AppendEntries:{
            payLoad = decode.appendEntries(data + 9, msg.payLoadSize);
            break;
        }

    }   
    
    msg.payload = std::move(payLoad);

    return msg;
}

std::vector<uint8_t> Protocol::pack(RaftMessage& msg){
    std::vector<uint8_t> data; 
    //Data[0] should contain Sender+Opcode; left to right
    uint8_t opCodeSender = 0;
    opCodeSender |= (msg.sender << SENDER);
    opCodeSender |= (this->opToBytes[msg.opcode]);
    
    data.push_back(opCodeSender);

    //4 Bytes for term and payload size
    std::vector<uint8_t> termData = {0, 0, 0, 0};
    std::vector<uint8_t> payloadSizeData = {0, 0, 0, 0};

    uint32_t mutable_payloadSizeVar = msg.payLoadSize;
    uint32_t mutable_termSizeVar = msg.term;

    for(int i = 0; i < 4; ++i){
        payloadSizeData[payloadSizeData.size() - i - 1] = static_cast<uint8_t>(mutable_payloadSizeVar);
        termData[termData.size() - i - 1] = static_cast<uint8_t>(mutable_termSizeVar);

        mutable_payloadSizeVar >>= LAST_EIGHT;
        mutable_termSizeVar >>= LAST_EIGHT;
    }
    for(int i = 0; i < termData.size(); ++i){
        data.push_back(termData[i]);
    }
    
    for(int i = 0; i < payloadSizeData.size(); ++i){
        data.push_back(payloadSizeData[i]);
        
    }

    //Pack payload data.
    PayLoadEncode encoder;
    std::vector<uint8_t> pData;
    switch(static_cast<OpCode>(opToBytes[msg.opcode])){
        case OpCode::AppendEntries:{
            pData = encoder.appendEntries(dynamic_cast<AppendEntries&>(*msg.payload));
            break;
        }
    }
    
    for(int i = 0; i < pData.size(); ++i){
        data.push_back(pData[i]);
    }

    return data;
}

Protocol::~Protocol(){}
