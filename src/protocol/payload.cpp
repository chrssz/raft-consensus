#include "payload.hpp"

PayLoadDecode::PayLoadDecode(){

}

std::unique_ptr<PayLoad> PayLoadDecode::appendEntries(const uint8_t* data, int size){
    AppendEntries msg;
    msg.type = "AppendEntries";
    
    return std::make_unique<AppendEntries>(msg);
}

std::unique_ptr<PayLoad> PayLoadDecode::entriesResponse(const uint8_t* data, int size){
    EntriesResponse msg;
    msg.type = "EntriesResponse";
    
    return std::make_unique<EntriesResponse>(msg);
}

PayLoadDecode::~PayLoadDecode(){
}


/* # ---------------------ENCODE into bytes-----------------------#*/

PayLoadEncode::PayLoadEncode(){

}

std::vector<uint8_t> PayLoadEncode::appendEntries(const AppendEntries& obj){
    std::vector<uint8_t> output;

    return output;
}

PayLoadEncode::~PayLoadEncode() {

}