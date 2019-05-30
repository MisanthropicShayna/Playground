#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

class ArgumentProcessor {
private:
	std::map<std::string, std::string>		m_argument_map;
	std::vector<std::string>				m_arguments;

	std::vector<std::string> split_string(std::string input, char wedge, unsigned __int32 maximum=0) const {
		std::vector<std::string> vector_buffer;
		std::string vetor_entry_buffer;

		for (std::size_t i = 0; i < input.size(); i++) {
			char& current_character = input.at(i);
			if (current_character == wedge && maximum && vector_buffer.size() < maximum) {
				vector_buffer.push_back(vetor_entry_buffer);
				vetor_entry_buffer.clear();
			} else vetor_entry_buffer.push_back(current_character);
		}

		if (vetor_entry_buffer.size() > 0) {
			vector_buffer.push_back(vetor_entry_buffer);
			vetor_entry_buffer.clear();
		}

		return vector_buffer;
	}

	void map_arguments_(std::vector<std::string>& arguments, std::map<std::string, std::string>* argument_map) const {
		for (std::size_t i = 0; i < arguments.size(); i++) {
			std::string& current_argument = arguments.at(i);

			std::vector<std::string> split_argument = split_string(current_argument, '=');

			std::string argument_key = split_argument.at(0);
			std::string argument_value = split_argument.size() == 2 ? split_argument.at(1) : "";
			
			argument_map->insert(std::make_pair(argument_key, argument_value));
		}
	}

public:
	ArgumentProcessor(char* arguments[], int argument_count) {
		for (std::size_t i = 0; i < (unsigned __int32)argument_count; i++) 
			m_arguments.push_back(std::string(arguments[i]));

		map_arguments_(m_arguments, &m_argument_map);
	}

	std::string argument_value(std::string argument) const {
		if (!m_argument_map.count(argument)) return "";
		else { return m_argument_map.at(argument); }
	}

	std::string argument_number(unsigned __int32 index) const { return m_arguments.at(index); }
	bool argument_supplied(std::string argument) const { return m_argument_map.count(argument); }
	std::size_t command_count() const { return m_arguments.size(); }
};
