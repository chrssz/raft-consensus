#pragma once
#include <unordered_map>
#include <cstdint>
#include <string>
/*
    Protocols are the defined rules for what type of data our raft nodse can receive.


    Architecture?: Socket---RecievesData(Bytes)---> ProtcolTranslation--(TranslatedMsg)--> HandleOperations -->RaftNodes.

    Keep protcol as a pure translation helper. 

    TODO: Write translation rules. 
          SetUp recieve data constraints.
        

    Fields              Size
    ---------------------------------
    SenderID:              4 Bits
    Opcode:             4 bits
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
            term
            candidateId
        
        VoteResponse:
            term
            voteGranted
        
        Send HeartBeat:
            term
        
    -----------------------
    #1 + 3 + payLoad = Buffer Size: 4bytes + payload

    #Buffer Read from right to left:
    #Little Endian Format

    #All machines understand 8 bits so 1 byte is mandatory (8 bits)

    #We can do bit packing
    #2 Bytes (Subject to change)
    #Byte1 : 00000000; Lower 4 bits - Sender Node,  upper 4 Bits - OpCode
    #Byte 2-4: 00000000; Term
    #Byte 5+?; 00000000; PayLoad OpCode Defined
    
*/
struct RaftMessage {
    std::string operation;
    uint32_t payload;
};

class Protocol{
    private:
        const int BUFFER_SIZE = 16; //May be subject to change.

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

    public:
        Protocol();
        RaftMessage pack(char data[]);
        uint32_t unpack(RaftMessage& msg);
        ~Protocol();
};