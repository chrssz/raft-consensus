#pragma once
#include <unordered_map>
#include <cstdint>
#include <string>
#include "payload.hpp"
/*
    Protocols are the defined rules for what type of data our raft nodse can receive.

    Keep protcol as a pure translation. 


    Fields              Size (Total size will be Mandatory Header 4 bytes + OpCodeDefinedSize)
    -----------------------------------------------------------------------------------------
    Opcode:             4 bits - Lower 4 bits of 1st byte
    SenderID:           4 Bits - Upper 4 bits of 1st byte
    Term:               24 bits (3bytes)
    Payload:            Opcode-Defined
    
    ---------------------------------
    Opcodes(Rough Draft):
    ------------------------------------------------------------
        - RequestVote : 0000        ; Node tries to become a leader (Requires payLoad)
        - Vote Response : 0001      ; Sends back binary 1 for vote given, 0 for vote denied
        - Send HeartBeat : 0010     ; Leader only* sends a heartbeat to followers (with term?)
    ------------------------------------------------------------

    Format Payload
    ------------------------
        RequestVote:
            candidateId - Same as above.
            
        VoteResponse:
            voteGranted
        
        Send HeartBeat:
            term
        
    -----------------------
*/

struct RaftMessage {
    std::string opcode; //4 Bits
    uint8_t sender; //4 Bits
    uint8_t term[3]; // 24 bits
    
    uint32_t payLoadSize;

    std::unique_ptr<PayLoad> payload;
    
};

enum class OpCode : uint8_t {
    RequestVote = 0,
    VoteResponse = 1,
    HeartBeat = 2
};
enum bitWidth {
    OPCODE = 4,
    SENDER = 4
};

class Protocol{
    private:
        std::unordered_map<uint8_t, std::string> bytesToOp = {
            {0, "requestVote"},
            {1, "voteResponse"},
            {2, "sendHeartBeat"}
        };
        std::unordered_map<std::string, uint8_t> opToBytes = {
            {"requestVote", 0},
            {"voteResponse", 1},
            {"sendHeartBeat", 2}
        };
        
        uint8_t extractBits(uint8_t, bitWidth amnt);

        
    public:
        Protocol();
        RaftMessage unpack(uint8_t* data, int size);
        uint32_t pack(RaftMessage& msg);
        ~Protocol();
};