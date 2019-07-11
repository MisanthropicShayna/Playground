#include <iostream>
#include <random>
#include <string>
#include <cmath>

uintmax_t RandomNumber(uintmax_t from, uintmax_t to) {
    static std::random_device device;
    static std::mt19937 mersene_twister(device());
    
    std::uniform_int_distribution<uintmax_t> uniform_distributer(from, to);
    uintmax_t generated = uniform_distributer(mersene_twister);
    
    return generated;
}

int main(int argc, char** argv) {
    uintmax_t range_from = 0;
    uintmax_t range_to = std::pow(255, sizeof(uintmax_t));
    
    for(int32_t i=0; i<argc; ++i) {
        const char* argument = argv[i];
        const char* next_argument = i+1 < argc ? argv[i+1] : nullptr;
        
        if((!strcmp(argument, "--from") || !strcmp(argument, "-f")) && next_argument != nullptr) {
            try { range_from = std::stoll(next_argument); }
            catch(const std::invalid_argument& exception) {
                std::cout << "From argument failed to convert to a number." << std::endl;
                std::cout << "Exception: " << exception.what() << std::endl;
                return 1;
            }
        } else if((!strcmp(argument, "--to") || !strcmp(argument, "-t")) && next_argument != nullptr) {
            try { range_to = std::stoll(next_argument); }
            catch(const std::invalid_argument& exception) {
                std::cout << "To argument failed to convert to a number." << std::endl;
                std::cout << "Exception: " << exception.what() << std::endl;
                return 1;
            }            
        }
    }
    
    std::cout << RandomNumber(range_from, range_to) << std::endl;

    return 0;    
}