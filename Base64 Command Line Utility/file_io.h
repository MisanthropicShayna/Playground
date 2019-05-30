#pragma once

#include <iostream>
#include <fstream>
#include <string>

bool read_file(std::string file_name, std::string* output) {
	std::ifstream input_stream(file_name, std::ios::binary);
	std::string input_stream_data;
	unsigned __int64 input_stream_size;

	if (!input_stream.good()) {
		std::cout << "Unable to open file \"" << file_name << "\". Input steam isn't good." << std::endl;
		return false;
	}

	try {
		input_stream.seekg(0, std::ifstream::end);
		input_stream_size = input_stream.tellg();
		input_stream.seekg(0, std::ifstream::beg);
	} catch (std::exception& exc) {
		std::cout << "Exception encountered when seeking through input stream." << std::endl;
		std::cout << "Exception: " << exc.what() << std::endl;
		input_stream.close();
		return false;
	}

	try {
		char input_stream_buffer[1];
		for (unsigned __int32 i = 0; i < input_stream_size; i++) {
			input_stream.read(input_stream_buffer, 1);
			input_stream_data.push_back(input_stream_buffer[0]);
		}
	} catch (std::exception& exc) {
		std::cout << "Exception encountered when reading file \"" << file_name << "\"" << std::endl;
		std::cout << "Exception: " << exc.what() << std::endl;
		return false;
	}
	
	input_stream.close();
	*output = input_stream_data;
	return true;
}

bool write_file(std::string file_name, std::string data) {
	std::ofstream output_stream(file_name, std::ios::binary);

	if (!output_stream.good()) {
		std::cout << "Unable to open file \"" << file_name << "\". Output steam isn't good." << std::endl;
		return false;
	}

	try {
		for (unsigned __int32 i = 0; i < data.size(); i++) {
			char write_buffer[1] = { data.at(i) };
			output_stream.write(write_buffer, 1);
		}
	} catch (std::exception& exc) {
		std::cout << "Exception encountered when writing to file \"" << file_name << "\"" << std::endl;
		std::cout << "Exception: " << exc.what() << std::endl;
		return false;
	}

	output_stream.close();
	return true;
}
