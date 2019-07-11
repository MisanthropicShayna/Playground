#include <iostream>
#include <random>
#include <string>

uintmax_t RandomNumber(uintmax_t from, uintmax_t to) {
    static std::random_device device;
    static std::mt19937 mersene_twister(device());
    
    std::uniform_int_distribution<uintmax_t> uniform_distributer(from, to);
    uintmax_t generated = uniform_distributer(mersene_twister);
    
    return generated;
}

uint8_t RandomByte() { 
    return static_cast<uint8_t>(RandomNumber(0, 255)); 
}

int main(int argc, char** argv) {
    if(argc == 2) {
        uintmax_t supplied_number;
        
        try { supplied_number = std::stoll(argv[1]); }
        catch(const std::invalid_argument& exception) {
            std::cout << "The provided argument failed to convert to a number." << std::endl;
            return 1;
        }
        
        for(uintmax_t i=0; i<supplied_number; ++i) {
            std::cout << RandomByte();
        }
    } else {
        std::cout << RandomByte();
    }
    
    return 0;
}