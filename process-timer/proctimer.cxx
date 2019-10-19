#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "headers/json.hpp"

using Json = nlohmann::json;

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
    bool background_process = false;
    bool verbose_output = false;
    bool initialize = false;

    for(int32_t i=0; i<argc; ++i) {
        if(!strcmp("--start", argv[i]) || !strcmp("-s", argv[i])) {
            initialize = true;
        }

        if(!strcmp("--background", argv[i]) || !strcmp("-b", argv[i])) {
            ShowWindow(GetConsoleWindow(), SW_HIDE);
        }

        if(!strcmp("--verbose", argv[i]) || !strcmp("-v", argv[i])) {
            verbose_output = true;
        }
    }

    if(initialize) {
        Json proctimes_json;

        std::ifstream input_stream("./proctimes.json", std::ios::binary);

        if(input_stream.good()){
            std::string plain_file_data((std::istreambuf_iterator<char>(input_stream)), (std::istreambuf_iterator<char>()));
            proctimes_json = Json::parse(plain_file_data);
            input_stream.close();
        } else {
            std::cerr << "Stream to proctimes.json was bad, and overwriting it would risk corruption -- so please solve the problem manually." << std::endl;
            return 1;
        }

        for(;;Sleep(1000)) {
            for(std::pair<uint32_t, std::string>& process : RunningProcesses()) {
                const std::string image = process.second;

                if(proctimes_json.contains(image)) {
                    const uint64_t reference_time = std::time(0);
                    const uint64_t last_checked = proctimes_json[image]["last_checked"];
                    const uint64_t time_elapsed = proctimes_json[image]["elapsed"];

                    const uint64_t time_delta = reference_time - last_checked;
                
                    proctimes_json[image]["last_checked"] = reference_time;
                    proctimes_json[image]["elapsed"] = time_elapsed + time_delta;
                    
                    if(verbose_output) std::cout << "Updated entry for process with image name: " << image << std::endl;
                } else {
                    proctimes_json[image] = {
                        {"last_checked", std::time(0)},
                        {"elapsed", 1}
                    };

                    if(verbose_output) std::cout << "Created new entry for process with image name: " << image << std::endl;
                }

                Sleep(1);
            }

            std::string plain_json_output = proctimes_json.dump(4);

            std::ofstream output_stream("./proctimes.json", std::ios::binary);
            if(output_stream.good()) {
                output_stream.write(plain_json_output.data(), plain_json_output.size());
                output_stream.close();

                if(verbose_output) std::cout << "Updated proctimes.json" << std::endl;
            } else {
                std::cerr << "Output stream to proctimes.json was bad, aborting." << std::endl;
                return 1;
            }        
        }
    } else {
        for(;;) {
            std::cout << "Image to query >> ";
            std::string image_query; std::getline(std::cin, image_query);

            Json proctimes_json;

            std::ifstream input_stream("./proctimes.json", std::ios::binary);

            if(input_stream.good()) {
                std::string plain_file_data((std::istreambuf_iterator<char>(input_stream)), (std::istreambuf_iterator<char>()));
                proctimes_json = Json::parse(plain_file_data);
                input_stream.close();
            } else {
                std::cerr << "Bad input stream to proctimes.json, cannot query." << std::endl;
                return 1;
            }

            if(proctimes_json.contains(image_query)) {
                float elapsed_seconds = proctimes_json[image_query]["elapsed"];
                float last_checked = proctimes_json[image_query]["last_checked"];

                std::cout << std::fixed << "Image name: " << image_query << std::endl;
                std::cout << std::fixed << "Elapsed seconds: " << elapsed_seconds << std::endl;
                std::cout << std::fixed << "Elapsed minutes: " << elapsed_seconds / 60 << std::endl;
                std::cout << std::fixed << "Elapsed hours: " << elapsed_seconds / 60 / 60 << std::endl;
                std::cout << std::fixed << "Elapsed days: " << elapsed_seconds / 60 / 60 / 24 << std::endl;
                std::cout << std::fixed << "Last checked: " << last_checked << std::endl;
            }
        }
    }

    return 0;
}