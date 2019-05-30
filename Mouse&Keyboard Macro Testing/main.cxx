#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <sstream>

#include "automator.hxx"

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1600

__int64 unixTimestamp_() {
	std::chrono::milliseconds timeStamp;
	timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch());
	return timeStamp.count();
}

int main() {
	Macro movementMacro;
	IEvents::EventParameters eventParameters;
	eventParameters.pollingRate = 1;

	movementMacro.addInputEvent(new IEvents::MouseMove(std::make_pair<int, int>(0, SCREEN_HEIGHT / 2), eventParameters));

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		std::pair<int, int> coordinates;
		coordinates.first = i;
		coordinates.second = SCREEN_HEIGHT / 2;

		movementMacro.addInputEvent(new IEvents::MouseMove(coordinates, eventParameters));
		movementMacro.addInputEvent(new IEvents::MouseMove(coordinates, eventParameters));
		movementMacro.addInputEvent(new IEvents::MouseMove(coordinates, eventParameters));
	}

	movementMacro.play();

	std::cout << "Done playing macro." << std::endl;
	std::cin.get();
	return 0;
}