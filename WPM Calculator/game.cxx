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

inline std::clock_t CurrentTime() {
    return (std::clock() / CLOCKS_PER_SEC);
}

int32_t ProcessWord(std::string const& word, bool const& erorr_tracking = false) {

}

int32_t ProcessPhrase(std::string const& phrase, bool const& error_tracking = false) {

}

int main(int argc, char const* argv[]) {
    bool phrase_mode, error_tracking = false;
    std::vector<std::string> dictionary;

    /* 
    A pointer to the stream where the wordlist for the dictionary is located.
    This stream will be read line-by-line and each line will be added to
    the dictionary. */
    std::istream* input_data_stream = nullptr;
    bool memory_allocated = false;

    for(uint32_t i=0; i<argc; ++i) {
        char const* argument = argv[i];

        if(!strcmp(argument, "--stdin") || !strcmp(argument, "-s")) {
            freopen(NULL, "rb", stdin);
            input_data_stream = &(std::cin);
        } else if((!strcmp(argument, "--file") || !strcmp(argument, "-f")) && (i+1) < argc) {
            char const* file_path = argv[i+1];
            input_data_stream = new std::ifstream(file_path, std::ios::binary);
            memory_allocated = true;
        } else if(!strcmp(argument, "--phrases") || !strcmp(argument, "-p")) {
            large_phrase_mode = true;
        } else if(!strcmp(argument, "--error-tracking") || !strcmp(argument, "-t")) {
            error_tracking = true;
        }
    }

    for(std::string line; std::getline(*data_stream, line);) {
        if(line.size()) {
            line.pop_back(); // Removes the newline byte from the line.
            dictionary.push_back(line); // Adds the modified line to the dictionary.
        }
    }
    
    if(memory_allocated) delete input_data_stream;

    std::clock_t processing_time_sum = 0;
    
    std::clock_t total_time_start = std::clock() / CLOCKS_PER_SEC;

    for(;;) {
        std::string random_entry = dictionary.at(Randint(0, dictionary.size()-1));
        int32_t process_time = false;

        if(phrase_mode) {
            process_time = ProcessPhrase(random_entry, error_tracking);
        } else {
            process_time = ProcessWord(random_entry, error_tracking);
        }

        if(process_time < 0) break;
        else processing_time_sum += process_time;
    }

    std::clock_t total_time_end = std::clock() / CLOCKS_PER_SEC;
    std::clock_t total_time = (total_time_end - total_time_start);

    // Results here

    return 0;
}