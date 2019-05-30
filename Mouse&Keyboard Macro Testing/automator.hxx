#pragma once

#include <windows.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <map>

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1200

namespace IEvents {
	struct EventParameters {
		unsigned pollingRate = 2;
		EventParameters(){}
	};

	class InputEvent {
	protected:
		EventParameters m_eventParameters;

		InputEvent(EventParameters eventParameters) : m_eventParameters(eventParameters) {}
		InputEvent() {}
	public:
		virtual void execute() {
			std::cout << "ERROR: Superclass method was called." << std::endl;
		}
	};

	class MouseClick : public InputEvent {
	private:
		INPUT m_simInputConfig; 
	public:
		void execute() override {
			SendInput(1, &m_simInputConfig, sizeof(INPUT));
		}

		MouseClick(unsigned mouseInputVKID) {
			m_simInputConfig.type = INPUT_MOUSE;
			m_simInputConfig.mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
			m_simInputConfig.mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
			m_simInputConfig.mi.dwExtraInfo = NULL;
			m_simInputConfig.mi.mouseData = NULL;
			m_simInputConfig.mi.time = NULL;
			m_simInputConfig.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
			m_simInputConfig.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | mouseInputVKID);
			
		};
	};

	class KeyboardStroke : public InputEvent {
	private:
		INPUT m_simInputConfig; 
		unsigned m_waitPeriod;
	public:
		void execute() override {
			if (m_waitPeriod) {
				std::cout << "Sleeping for " << m_waitPeriod << " milliseconds." << std::endl;
				Sleep(m_waitPeriod);

			}
			SendInput(1, &m_simInputConfig, sizeof(INPUT));
		}

		KeyboardStroke(unsigned keyboardInputVKID, unsigned released, unsigned waitPeriod=0.0) {
			m_simInputConfig.type = INPUT_KEYBOARD;
			m_simInputConfig.ki.wScan = NULL;
			m_simInputConfig.ki.time = NULL;
			m_simInputConfig.ki.dwExtraInfo = NULL;
			m_simInputConfig.ki.dwFlags = released;
			m_simInputConfig.ki.wVk = keyboardInputVKID;
			m_waitPeriod = waitPeriod;
		}
	};

	class MouseMove : public InputEvent {
	private:
		std::pair<int, int> m_cursorPosition;
	public:
		void execute() override {
			SetCursorPos(m_cursorPosition.first, m_cursorPosition.second);
			Sleep(m_eventParameters.pollingRate);
		}

		MouseMove(std::pair<int,int> cursorPosition, EventParameters eventParameters) : 
		InputEvent(eventParameters), m_cursorPosition(cursorPosition) {
		
		}
	};

	class TimeDelay : public InputEvent {
	private:
		__int64 m_milliseconds;
	public:
		void setDelay(__int64 milliseconds) {
			m_milliseconds = milliseconds;
		}
		
		void execute() {
			Sleep((unsigned)m_milliseconds);
		}

		TimeDelay(__int64 milliseconds) : m_milliseconds(milliseconds) {}
	};
}

class Macro {
private:
	std::vector<IEvents::InputEvent*> m_inputEvents;

	bool getKeyState_(unsigned VKID) {
		return ((GetAsyncKeyState(VKID) & 0x8000) != 0);
	}

	__int64 unixTimestamp_() {
		std::chrono::milliseconds timeStamp;
		timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch());
		return timeStamp.count();
	}

public:
	std::pair<int, int> getCursorPos() {
		POINT pointerData;
		GetCursorPos(&pointerData);
		return std::make_pair(pointerData.x, pointerData.y);
	}

	void record(unsigned stopKeyVKID, unsigned pollingRate = 1) {
		Sleep(1000);
		m_inputEvents.clear();

		IEvents::EventParameters eventParameters;
		eventParameters.pollingRate = pollingRate;

		std::map<unsigned, std::pair<bool, unsigned>> beingPressed;

		for (unsigned i = 0x01; i < 0xFF; i++) beingPressed.insert(std::make_pair(i, std::make_pair(0, 0)));

		while (true) {
			if (getKeyState_(stopKeyVKID)) break;
			m_inputEvents.push_back(new IEvents::MouseMove(getCursorPos(), eventParameters));
			__int64 previousKeypress = -1;

			for (unsigned i = 0x01; i < 0xFF; i++) {
				if (i == stopKeyVKID) continue;

				if (getKeyState_(i) && !(beingPressed.at(i).first) && i != stopKeyVKID) {
					if (i == 0x01 || i == 0x02) {
						unsigned translatedCode = i == 0x01 ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
						m_inputEvents.push_back(new IEvents::MouseClick(translatedCode));
					}
					else {
						m_inputEvents.push_back(new IEvents::KeyboardStroke(i, 0x0000));
					}

					beingPressed.at(i) = std::make_pair(1, unixTimestamp_());

					if (previousKeypress == -1) previousKeypress = unixTimestamp_();
					else {
						m_inputEvents.push_back(new IEvents::TimeDelay(unixTimestamp_() - previousKeypress));
						previousKeypress = unixTimestamp_();
					}

				} else if (!getKeyState_(i) && beingPressed.at(i).first && i != stopKeyVKID) {

					if (i == 0x01 || i == 0x02) {
						unsigned translatedCode = i == 0x01 ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
						m_inputEvents.push_back(new IEvents::MouseClick(translatedCode));
					}
					else {
						m_inputEvents.push_back(new IEvents::KeyboardStroke(i, 0x0002));
					}

					beingPressed.at(i) = std::make_pair(0, 0);

					if (previousKeypress == -1) previousKeypress = unixTimestamp_();
					else {
						m_inputEvents.push_back(new IEvents::TimeDelay(unixTimestamp_() - previousKeypress));
						previousKeypress = unixTimestamp_();
					}
				}
			}

			Sleep(pollingRate);
		}
	}

	void clearInputEvents() {
		m_inputEvents.clear();
	}

	void addInputEvent(IEvents::InputEvent* inputEvent) {
		m_inputEvents.push_back(inputEvent);
	}

	void play() {
		for (std::size_t i = 0; i < m_inputEvents.size(); i++) {
			m_inputEvents.at(i)->execute();
		}
	}

};