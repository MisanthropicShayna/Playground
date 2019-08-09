// Windows libraries.
#include <windows.h>
#include <conio.h>
#include <cstdint>

// Standard libraries.
#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

uint32_t Randint(const uint32_t& from, const uint32_t& to) {
    static std::random_device randomness_device;
    static std::mt19937 mersene_twister(randomness_device());
    
    std::uniform_int_distribution<uint32_t> uniform_distributer(from, to);
    
    return uniform_distributer(mersene_twister);
}

std::string MakeDisplayMessageString(const std::pair<std::string, std::string>& strings) {
    std::stringstream display_message_stream;
    display_message_stream << "\r" << strings.first << ": " << strings.second;
    std::string display_message = display_message_stream.str();
    display_message.resize(50, '-');
    return display_message;
}

int main() {
    std::ifstream input_stream("dictionary.txt", std::ios::binary);
    std::vector<std::string> dictionary;
    
    for(std::string line; std::getline(input_stream, line);) {
        if(line.size()) {
            line.pop_back();
            dictionary.push_back(line);
        }
    }
        
    input_stream.close();
    
    std::string target_word = dictionary.at(Randint(0, dictionary.size()-1));
    std::string word_buffer;
        
    double words_typed = 0;
    double delta_sum = 0;
    
    std::clock_t begin_time = std::clock();
        
    for(char last_character; last_character != 27;) {
        bool first_char_of_word = (last_character == 0x00);
        
        std::cout << MakeDisplayMessageString({target_word, word_buffer});
        
        last_character = _getch();
        
        if(first_char_of_word) begin_time = clock();
        
        if(last_character == 0x08) {
            if(word_buffer.size()) {
                word_buffer.pop_back();
            }
        } else {
            word_buffer.push_back(last_character);
        }
        
        std::cout << MakeDisplayMessageString({target_word, word_buffer});
                
        if(word_buffer == target_word) {
            target_word = dictionary.at(Randint(0, dictionary.size()-1));
            last_character = 0x00;
            word_buffer.clear();
            std::cout << std::endl;
            ++words_typed;
            delta_sum += (std::clock() - begin_time) / CLOCKS_PER_SEC;
        }
    }
    
    std::cout << std::endl << std::endl << words_typed << " Words typed in " << delta_sum << " seconds." << std::endl;
    std::cout << (words_typed / delta_sum) << " Words per second." << std::endl;
    std::cout << (words_typed / delta_sum) * 60 << " Words per minute." << std::endl;
        
    return 0;
}