//Program is for testing functionality.
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>
#include <string>

#include "../protocol/protocol.hpp"

int main()
{
    RaftMessage original{};

    original.sender = 1;
    original.opcode = "AppendEntries";
    original.term = 5;
    std::cout << static_cast<int>(original.sender) << std::endl;
    Protocol p;

    auto bytes = p.pack(original);

    auto reconstructed = p.unpack(bytes.data(), bytes.size());

    assert(reconstructed.sender == original.sender);
    assert(reconstructed.opcode == original.opcode);
    assert(reconstructed.term == original.term);

    std::cout << "Protocol test passed!\n";
    std::cout << original << std::endl;
    std::cout << reconstructed << std::endl;
    
    return 0;
}
