#include <iostream>
#include <string>
#include <map>

#include "trainer.hxx"


class BlackList : public Trainer {
private: virtual void __interface__() {}
public:
	std::map<std::string, unsigned __int64> address_map;

	BlackList() {
		m_window_title = (char*)"Blacklist";
		m_process_title = (char*)"Blacklist_DX11_game.exe";
		trainer_setup_();

		address_map = std::map<std::string, unsigned __int64> {
			{"money", discover_dynamic_address_(0x01CB4EB0,{0x98, 0xE8, 0x18, 0x8, 0x28})}
		};
	}

	void set_money(unsigned __int64 value) const {
		write_address_value_(address_map.at("money"), value);
	}

	unsigned __int32 get_money() const {
		return (unsigned __int32)read_address_data_(address_map.at("money"));
	}
};

int main() {
	BlackList trainer = BlackList();

	while (true) {
		system("cls");
		std::cout << "Current money: " << trainer.get_money() << std::endl;
		std::cout << "Input new value: ";
		std::string input; std::getline(std::cin, input);
		unsigned __int32 value = 0;
		try { value = std::stoi(input); }
		catch (...) { std::cout << std::endl << "Invalid input." << std::endl; continue; }
		trainer.set_money(value);
	}

	return EXIT_SUCCESS;
}