#pragma once
#include <memory>

struct PayLoad {
    virtual ~PayLoad() = default;
};

struct RequestVotePayLoad : public PayLoad {
    
};

struct VoteResponse : public PayLoad {
    bool voteGranted;
    
};

struct heartbeat : public PayLoad {
    
};

enum PAYLOAD_SIZE {
    VoteResponseSize = 1
};

class PayLoadDecode {
    public:
        PayLoadDecode();

        std::unique_ptr<PayLoad> decodeRequestVote(const uint8_t* data, int size);
        std::unique_ptr<PayLoad> decodeVoteResponse(const uint8_t* data, int size);
        std::unique_ptr<PayLoad> heartbeat(const uint8_t* data, int size);
        
        ~PayLoadDecode();
};