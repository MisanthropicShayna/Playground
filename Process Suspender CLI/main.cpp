#include "Suspender.h"
 
int main(int argc, char* argv[]) {
	std::string process_title = argc >= 2 ? argv[1] : ([]()->std::string {
		std::cout << "Please supply a process." << std::endl;
		std::string input; std::getline(std::cin, input);
		return input;
	})();

	Suspender suspender = Suspender();
	unsigned __int32 process_id = suspender.discover_process_id(process_title);
	if (process_id != EXIT_FAILURE) {
		suspender.suspend_thread(process_id);
	} else {
		std::cout << "Could not find process ID." << std::endl;
	}
	return EXIT_SUCCESS;
}