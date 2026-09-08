#include <vector>
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
*/
class Protocol{
    private:

    public:
        Protocol();
        
        ~Protocol();
};