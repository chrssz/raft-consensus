#pragma once
#include <memory>

struct PayLoad {

};
struct RequestVotePayLoad : PayLoad {

};

struct VoteResponse : PayLoad {

};

struct heartbeat : PayLoad {
    
};

class PayLoadDecode {
    public:
        PayLoadDecode();

        std::unique_ptr<PayLoad> decodeRequestVote(const uint8_t* data, int size);
        std::unique_ptr<PayLoad> decodeVoteResponse(const uint8_t* data, int size);
        std::unique_ptr<PayLoad> heartbeat(const uint8_t* data, int size);
        
        ~PayLoadDecode();
};