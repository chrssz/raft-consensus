#include "payload.hpp"

// ** Decoder ** //
PayLoadDecode::PayLoadDecode(){

}
std::unique_ptr<PayLoad> PayLoadDecode::decodeVoteResponse(const uint8_t* data, int size){
    if (size < VoteResponseSize){
        return nullptr;
    }
    VoteResponse msg;
    msg.type = "VoteResponse";

    msg.voteGranted = data[0];
    
    return std::make_unique<VoteResponse>(msg);
}

PayLoadDecode::~PayLoadDecode(){

}

// ** Encoder ** //

PayLoadEncode::PayLoadEncode(){

}
std::vector<uint8_t> PayLoadEncode::encodeVoteResponse(const VoteResponse& obj){
    
    std::vector<uint8_t> output;
    
    output.push_back(static_cast<uint8_t>(obj.voteGranted));

    return output;

}

PayLoadEncode::~PayLoadEncode() {

}