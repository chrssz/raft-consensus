#include "payload.hpp"

PayLoadDecode::PayLoadDecode(){

}
std::unique_ptr<PayLoad> decodeVoteResponse(const uint8_t* data, int size){
    if (size < VoteResponseSize){
        return nullptr;
    }

    VoteResponse msg;
    msg.voteGranted = data[0];
    
    return std::make_unique<VoteResponse>(msg);
}

PayLoadDecode::~PayLoadDecode(){

}