#include "payload.hpp"

PayLoadDecode::PayLoadDecode(){

}

std::unique_ptr<PayLoad> PayLoadDecode::appendEntries(const uint8_t* data, int size){
    AppendEntries msg;
    msg.type = "AppendEntries";

    return std::make_unique<AppendEntries>(msg);
}

PayLoadDecode::~PayLoadDecode(){

}


/* # ---------------------ENCODE-----------------------#*/

PayLoadEncode::PayLoadEncode(){

}

std::vector<uint8_t> PayLoadEncode::appendEntries(const AppendEntries& obj){
    std::vector<uint8_t> output;

    return output;
}

PayLoadEncode::~PayLoadEncode() {

}