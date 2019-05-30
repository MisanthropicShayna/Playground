#pragma once

#include <Windows.h>
#include <vector>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <thread>

#define TARGET_WINDOW_TITLE "Prey"
#define TARGET_PROCESS_NAME "Prey.exe"

#define	TARGET_STATIC_OFFSET  0x02416E40
#define TARGET_OFFSET_CHAIN   {0x138, 0x0, 0x8, 0x668, 0x408}

class Trainer {
private: 
	HANDLE			m_processHandle;
	DWORD			m_processID;
	HWND			m_processWindow;

	uintptr_t		mp_processBaseAddr;
	uintptr_t		mp_psiAddress;

	std::string		m_moduleName;
	std::string		m_windowTitle;

	std::thread		mt_memoryWriter;

	bool			m_toggleThread;
	bool			m_threadTerminationSignal;

	unsigned int	m_trainerStatus;

	unsigned int	m_memoryWriterStatus;
	float			m_memoryWriterValue;

public:
	struct RegenSettings_DataStructure {
		unsigned int regenMax = 100;
		unsigned int regenRate = 2000;
		unsigned int regenDelay	= 5000;
		unsigned int regenAmount = 1;
	} mc_regenSettings;

	enum e_mwStatuses {
		STATUS_DRAINING,
		STATUS_REGENERATING,
		STATUS_AT_MCAPACITY
	};
private:
	uintptr_t f_resolveBaseAddress(DWORD processID, std::string moduleName) const {
		uintptr_t baseAddress = NULL;

		HANDLE handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
		if (handleSnapshot == INVALID_HANDLE_VALUE) return EXIT_FAILURE;
		
		MODULEENTRY32 moduleEntryVector;
		moduleEntryVector.dwSize = sizeof(moduleEntryVector);
		if (!Module32First(handleSnapshot, &moduleEntryVector)) return EXIT_FAILURE;
		do {
			if (!_stricmp(moduleEntryVector.szModule, moduleName.c_str())) {
				baseAddress = (uintptr_t)moduleEntryVector.modBaseAddr;
				break;
			}
		} while (Module32Next(handleSnapshot, &moduleEntryVector));
		CloseHandle(handleSnapshot);

		return baseAddress;
	}

	HWND f_findProcessWindow(std::string windowTitle) const {
		HWND processWindow = FindWindowA(NULL, windowTitle.c_str());
		return processWindow;
	}

	DWORD f_fetchProcessID(HWND processWindow) const {
		DWORD processID;
		GetWindowThreadProcessId(processWindow, &processID);
		return processID;
	}

	HANDLE f_createHandle(DWORD processID) const {
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	}

	uintptr_t f_followPointerChain(HANDLE processHandle, uintptr_t baseAddress, uintptr_t staticOffset, std::vector<uintptr_t> offsets) const {
		uintptr_t pointerBuffer = NULL;
		uintptr_t trailingOffset = offsets.at(offsets.size()-1);

		ReadProcessMemory(processHandle, (LPVOID*)(baseAddress + staticOffset), &pointerBuffer, sizeof(pointerBuffer), NULL);

		for (std::size_t i = 0; i < offsets.size()-1; i++) {
			uintptr_t& currentOffset = offsets.at(i);
			ReadProcessMemory(processHandle, (LPVOID*)(pointerBuffer + currentOffset), &pointerBuffer, sizeof(pointerBuffer), NULL);
		}

		pointerBuffer = pointerBuffer == NULL ? NULL : pointerBuffer + trailingOffset;
		return pointerBuffer;
	}

	void tf_memoryWriter() {
		float previousAddressValue = NULL;

		while (true) {
			if (m_threadTerminationSignal) std::terminate();
			if (!m_toggleThread) { Sleep(1000); continue; }
			
			mp_psiAddress = f_followPointerChain(m_processHandle, mp_processBaseAddr, TARGET_STATIC_OFFSET, TARGET_OFFSET_CHAIN);

			if (previousAddressValue == NULL)
				ReadProcessMemory(m_processHandle, (LPVOID*)mp_psiAddress, &previousAddressValue, sizeof(previousAddressValue), NULL);
			

			float currentAddressValue;
			ReadProcessMemory(m_processHandle, (LPVOID*)mp_psiAddress, &currentAddressValue, sizeof(currentAddressValue), NULL);

			m_memoryWriterValue = currentAddressValue;

			if (currentAddressValue >= mc_regenSettings.regenMax) { m_memoryWriterStatus = e_mwStatuses::STATUS_AT_MCAPACITY; Sleep(100); continue; }
			if (currentAddressValue < previousAddressValue) {
				m_memoryWriterStatus = e_mwStatuses::STATUS_DRAINING;
				Sleep(mc_regenSettings.regenDelay);
				previousAddressValue = currentAddressValue;
				continue;
			}

			m_memoryWriterStatus = e_mwStatuses::STATUS_REGENERATING;
			float incrementedAddressValue = currentAddressValue + mc_regenSettings.regenAmount;
			WriteProcessMemory(m_processHandle, (LPVOID*)mp_psiAddress, &incrementedAddressValue, sizeof(incrementedAddressValue), NULL);
			Sleep(mc_regenSettings.regenRate);
			previousAddressValue = currentAddressValue;
		}
	}

public:
	float GetCurrentValue()	{ return m_memoryWriterValue;  }
	unsigned int GetCurrentStatus() { return m_memoryWriterStatus; }
	void SetRegenSettings(RegenSettings_DataStructure settings) { mc_regenSettings = settings; }
	uintptr_t GetPsiAddress() { return mp_psiAddress; }
	void ToggleRegeneration() { m_toggleThread = !m_toggleThread; }

	Trainer() {
		m_moduleName = TARGET_PROCESS_NAME;
		m_windowTitle = TARGET_WINDOW_TITLE;
		m_threadTerminationSignal = FALSE;
		m_toggleThread = FALSE;
	
		std::cout << "[STATUS] Waiting for window to hook." << std::endl;
		do { m_processWindow = f_findProcessWindow(m_windowTitle); Sleep(10); } while (m_processWindow == NULL);
		system("cls");
		
		std::cout << "[STATUS] Waiting for valid process ID." << std::endl;
		do { m_processID = f_fetchProcessID(m_processWindow); Sleep(10); } while (m_processID == NULL);
		system("cls");

		std::cout << "[STATUS] Waiting for valid handle." << std::endl;
		do { m_processHandle = f_createHandle(m_processID); Sleep(10); } while (m_processHandle == NULL);
		system("cls");

		std::cout << "[STATUS] Waiting for base address pointer." << std::endl;
		do { mp_processBaseAddr = f_resolveBaseAddress(m_processID, m_moduleName); Sleep(10); } while (mp_processBaseAddr == NULL);
		system("cls");

		std::cout << "[STATUS] Waiting for resolved pointer chain." << std::endl;
		do { mp_psiAddress = f_followPointerChain(m_processHandle, mp_processBaseAddr, TARGET_STATIC_OFFSET, TARGET_OFFSET_CHAIN); } while (mp_psiAddress == NULL);
		system("cls");
		
		mt_memoryWriter = std::thread(&Trainer::tf_memoryWriter, this);
	}
};
