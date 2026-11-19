#pragma once
#include <memory>
#include <vector>
#include <string>

struct PayLoad {
    std::string type;
    virtual ~PayLoad() = default;
};


struct AppendEntries : public PayLoad {
    
};

struct EntriesResponse : public PayLoad{
    bool success;
};

class PayLoadDecode {
    public:
        PayLoadDecode();

        std::unique_ptr<PayLoad> appendEntries(const uint8_t* data, int size);
        std::unique_ptr<PayLoad> entriesResponse(const uint8_t* data, int size);

        ~PayLoadDecode();
};

class PayLoadEncode {
   
    public:
        PayLoadEncode();

        std::vector<uint8_t> appendEntries(const AppendEntries& obj);
        
        ~PayLoadEncode();
};