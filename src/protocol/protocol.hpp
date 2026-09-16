#pragma once
#include <unordered_map>
#include <cstdint>
#include <string>
/*
    Protocols are the defined rules for what type of data our raft nodse can receive.

    The protcols will take inspiration from computer architecture IS formats where we will define 
    a buffer of some amount of bytes. ASsign some amount of bytes to specific opcodes.

    Protcol will receive bytes, represented as u_int.
    These bytes will point towards some operation in our op table.
    I will have protocol be responsible for translating the bytes. return as string maybe,
        perform the operations after we translate.

    Architecture?: Socket---RecievesData(Bytes)---> ProtcolTranslation--(TranslatedMsg)--> HandleOperations -->RaftNodes.

    Keep protcol as a pure translation helper. 

    TODO: Write translation rules. 
          SetUp recieve data constraints.
    
    Opcodes(Rough Draft):
        - RequestVote : 0000        ; Requests data incoming query (Requires payLoad)
        - Vote Response : 0001      ; Sends back binary 1 for vote given, 0 for vote denied
        - Send HeartBeat : 0010     ; Leader only* sends a heartbeat to followers
        

    #16 bit sized buffer

    #Buffer Read from right to left:  [payLodData(12bits)] [opCode (4bits)]
    #                                      Data               operation
    #Little Endian Format
*/
struct RaftMessage {
    uint8_t opCode;
    uint16_t payload;
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
        char* unpack(RaftMessage& msg);
        ~Protocol();
};