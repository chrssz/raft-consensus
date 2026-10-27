#pragma once
#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <string>
#include "payload.hpp"
/*
    Protocols are the defined rules for what type of data our raft nodse can receive.

    Keep protcol as a pure translation. 


    Fields              Size (Total size will be Mandatory Header 9 bytes + OpCodeDefinedSize)
    -----------------------------------------------------------------------------------------
    Opcode | SenderID       8 bits (1Byte) - Lower 4 bits of 1st byte OPCODE ; Upper 4 of byte SENDER ID
    Term:                   32 bits(4 bytes)
    PayloadSize in bytes:   32 bits(4 bytes)     
    Payload:                Opcode-Defined
    ---------------------------------

    Opcodes(Rough Draft):
    ------------------------------------------------------------
        - RequestVote : 0000        ; Node tries to become a leader (Requires payLoad)
        - Vote Response : 0001      ; Sends back binary 1 for vote given, 0 for vote denied
        - AppendEntries / Send HeartBeat : 0010     ; Leader only* sends a heartbeat to followers (with term?)
    ------------------------------------------------------------

*/

struct RaftMessage {
    friend std::ostream& operator <<(std::ostream& os, const RaftMessage& obj);
    
    std::string opcode; //data[0]
    uint8_t sender; //data[0]
    uint32_t term; //data[1..4]
    
    uint32_t payLoadSize; //data[5...8]

    std::unique_ptr<PayLoad> payload; //data[9 ...]
    
};


enum class OpCode : uint8_t {
    RequestVote = 0,
    VoteResponse = 1,
    AppendEntries = 2
};
enum bitWidth : int {
    OPCODE = 4,
    SENDER = 4,
    LAST_EIGHT = 8
};

class Protocol {
    private:
        uint8_t extractBits(uint8_t, bitWidth amnt);

        std::unordered_map<uint8_t, std::string> bytesToOp = {
            {0, "requestVote"},
            {1, "voteResponse"},
            {2, "AppendEntries"}
        };
        std::unordered_map<std::string, uint8_t> opToBytes = {
            {"requestVote", 0},
            {"voteResponse", 1},
            {"AppendEntries", 2}
        };

    public:
        Protocol();
        RaftMessage unpack(uint8_t* data, int size);
        std::vector<uint8_t> pack(RaftMessage& msg);
        ~Protocol();
};