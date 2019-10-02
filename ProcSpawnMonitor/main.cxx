#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <ctime>

std::vector<std::pair<uint32_t, std::string>> RunningProcesses() {
    std::vector<std::pair<uint32_t, std::string>> running_processes;

    HANDLE running_processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(running_processes_snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create snapshot of running processes, aborting." << std::endl;
    } else {
        PROCESSENTRY32 process_entry_32;
        process_entry_32.dwSize = sizeof(PROCESSENTRY32);

        if(Process32First(running_processes_snapshot, &process_entry_32)) {
            do {
                running_processes.emplace_back(process_entry_32.th32ProcessID, process_entry_32.szExeFile);
            } while(Process32Next(running_processes_snapshot, &process_entry_32));
        }
    }

    return running_processes; 
}

int main(int argc, char** argv) {
    uint32_t polling_rate_ms = 1000;

    uint32_t maximum_differences = 0;
    uint32_t time_to_break = 0;

    for(int i=0; i<argc; ++i) {
        const std::string& argument(argv[i]);
        
        if(argument == "--polling-rate" && i + 1 < argc) {
            try {
                polling_rate_ms = std::stoi(argv[i + 1]);
            } catch(const std::invalid_argument& exception) {
                std::cout << "Invalid type. Argument \"" << argv[i+1] << "\" is not a number." << std::endl;
            }
        }

        if(argument == "--break-maximum" && i + 1 < argc) {
            try {
                maximum_differences = std::stoi(argv[i + 1]);
            } catch(const std::invalid_argument& exception) {
                std::cout << "Invalid type. Argument \"" << argv[i + 1] << "\" is not a number." << std::endl;
            }
        }

        if(argument == "--break-abstime" && i + 1 < argc) {
            try {
                time_to_break = std::stoi(argv[i + 1]);
            } catch(const std::invalid_argument& exception) {
                std::cout << "Invalid type. Argument \"" << argv[i + 1] << "\" is not a number." << std::endl;
            }
        }

        if(argument == "--break-reltime" && i + 1 < argc) {
            try {
                time_to_break = std::time(nullptr) + std::stoi(argv[i + 1]);
            } catch(const std::invalid_argument& exception) {
                std::cout << "Invalid type. Argument \"" << argv[i + 1] << "\" is not a number." << std::endl;
            }
        }
    }

    uint32_t difference_count = 0;

    for(auto previous_snapshot = RunningProcesses();;Sleep(polling_rate_ms)) {
        auto current_snapshot = RunningProcesses();

        for (const auto& process : current_snapshot) {
            if (std::find(previous_snapshot.begin(), previous_snapshot.end(), process) == previous_snapshot.end()) {
                std::cout << " + " << process.second << " PID " << process.first << std::endl;
                ++difference_count;
            }
        }

        for (const auto& process : previous_snapshot) {
            if (std::find(current_snapshot.begin(), current_snapshot.end(), process) == current_snapshot.end()) {
                std::cout << " - " << process.second << " PID " << process.first << std::endl;
                ++difference_count;
            }
        }

        if (maximum_differences && (difference_count >= maximum_differences)) break;
        if (time_to_break && std::time(nullptr) >= time_to_break) break;

        previous_snapshot = current_snapshot;
    }

    return 0;
}