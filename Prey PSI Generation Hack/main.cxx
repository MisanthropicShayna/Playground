#include <conio.h>
#include <sstream>
#include <map>
#include "regenerator.hxx"

bool alphaNumericalStr(std::string string) {
	if (string.size() == 0) return false;
	for (std::size_t i = 0; i < string.size(); i++) {
		char& ichar = string.at(i);
		if (! ( (ichar >= 48) & (ichar <= 57) ) ) return false;
	}
	return true;
}

std::string capitalize(std::string string) {
	std::string buffer;
	for (std::size_t i = 0; i < string.size(); i++) {
		char& ichar = string.at(i);
		if (ichar >= 97 && ichar <= 121) ichar -= 32;
		 buffer.push_back(ichar);
	}
	return buffer;
}

void moveCursor(unsigned int column, unsigned int row) {
	COORD newPosition = { column, row };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(outputHandle, newPosition);
}

bool settingsConfirmed = false;

int main() {
	std::string	psiPool, drainDelay, incrementAmount, regenerationRate;

	while (!settingsConfirmed) {
		psiPool.clear(); drainDelay.clear(); incrementAmount.clear(); regenerationRate.clear();

		system("cls");
		std::cout << "At what rate should your PSI pool be incremented in milliseconds?" << std::endl;
		std::cout << "(default:2000) > ";
		std::getline(std::cin, regenerationRate);

		system("cls");
		std::cout << "By what amount should your PSI pool be incremented per the specified rate?" << std::endl;
		std::cout << "(default:1) > ";
		std::getline(std::cin, incrementAmount);

		system("cls");
		std::cout << "What should the delay be in milliseconds after using an aptitude until PSI regeneration starts?" << std::endl;
		std::cout << "(default:5000) > ";
		std::getline(std::cin, drainDelay);

		system("cls");
		std::cout << "What is the maximum value of your current PSI pool?" << std::endl;
		std::cout << "(default:100) > ";
		std::getline(std::cin, psiPool);
		system("cls");

		psiPool = alphaNumericalStr(psiPool) ? psiPool : "100";
		drainDelay = alphaNumericalStr(drainDelay) ? drainDelay : "5000";
		incrementAmount = alphaNumericalStr(incrementAmount) ? incrementAmount : "1";
		regenerationRate = alphaNumericalStr(regenerationRate) ? regenerationRate : "2000";

		std::cout << "Are these settings correct? [Y/N]" << std::endl << std::endl;
		std::cout << "PSI Pool >> " << psiPool << " PSI" << std::endl;
		std::cout << "Regeneration Rate >> " << regenerationRate << " MS" << std::endl;
		std::cout << "Aptitude Delay >>  " << drainDelay << " MS" << std::endl;
		std::cout << "Increment Amount >> " << incrementAmount << " PSI" << std::endl << std::endl;

		char C1, C2;
		do {
			C1 = _getch();
			C2 = _getch();
		} while (C1 != 'y' && C1 != 'n');

		if (C1 == 'y') { system("cls");  break; }
	}
	
	Trainer::RegenSettings_DataStructure regenerationSettings;
	regenerationSettings.regenAmount = std::stoi(incrementAmount);
	regenerationSettings.regenDelay = std::stoi(drainDelay);
	regenerationSettings.regenRate = std::stoi(regenerationRate);
	regenerationSettings.regenMax = std::stoi(psiPool);
	
	Trainer trainer = Trainer();
	trainer.SetRegenSettings(regenerationSettings);
	
	std::cout << "Ready to initialize. Hit enter when ready. " << std::endl;
	std::cin.get(); system("cls");

	std::map<unsigned int, std::string> statusCodes = {
		{trainer.STATUS_AT_MCAPACITY, "PSI AT MAXIMUM CAPACITY"},
		{trainer.STATUS_DRAINING, "PSI BEING DRAINED"},
		{trainer.STATUS_REGENERATING, "PSI BEING REGENERATED"},
		{NULL, "UNKNOWN"}
	};

	trainer.ToggleRegeneration();
	system("cls");
	while (true) {
		
		std::stringstream stream;
		stream << std::hex << trainer.GetPsiAddress();

		moveCursor(0, 0);
		std::cout << "[PSI-STATUS] >> " << statusCodes.at(trainer.GetCurrentStatus()) << "             ";
		moveCursor(0, 1);
		std::cout << "[PSI-ADDRESS] >> " << capitalize(stream.str()) << "             ";
		moveCursor(0, 2);
		std::cout << "[PSI] >> " << trainer.GetCurrentValue() << "             ";
		moveCursor(0, 3);
		stream.clear();
		Sleep(500);
	}


	std::cin.get();
	return 0;
}