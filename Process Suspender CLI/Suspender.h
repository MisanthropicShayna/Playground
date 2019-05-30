#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

#include "cprocinfo.h"  /* 
This custom header comes from Elmue @ https://bit.ly/2Cfx28A // https://bit.ly/2TEWKKD
It contains a class designed to query process information.
This will be used later on to check if a thread is currently suspended or not.
*/ 

class Suspender {
public:
	/* This finds the process ID of the first process matching the supplied title */
	unsigned __int32 discover_process_id(std::string process_title) const {

		/* This creates a new process entry point, and assigns the size */
		tagPROCESSENTRY32 process_entry; 
		process_entry.dwSize = sizeof process_entry;

		/* Creates a snapshot of every running process */
		void* process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		/* Checks if the returned snapshot is valid or not. (Exits) */
		if (process_snapshot == INVALID_HANDLE_VALUE) {
			std::cout << "Could not create process snapshot, returned invalid handle value." << std::endl;
			return EXIT_FAILURE;
		}

		/* Gets the process entry of the first process in the snapshot.*/
		Process32First(process_snapshot, &process_entry);

		
		/* Loops over every single process entry beyond the first one */
		do {
			/* Returns the process ID of the current process if the 
			title of the process matches the supplied title. */
			if (process_entry.szExeFile == process_title) {
				CloseHandle(process_snapshot);
				return process_entry.th32ProcessID; 
			}

		} while (Process32Next(process_snapshot, &process_entry)); 


		/* If the end of the method is reached without having returned anything previously,
		then it's safe to say that no process that was looped over had a matching title. */
		CloseHandle(process_snapshot);
		std::cout << "Could not find a matching process with the supplied title \"" << process_title << "\"." << std::endl;
		return EXIT_FAILURE;
	}

	/* This method attempts to suspend the main thread of the supplied process.
	It will return true if the process was suspended. */
	bool suspend_thread(unsigned __int32 process_id) const {

		/* This is the aforementioned class to query process information */
		cProcInfo process_info; 

		/* This is a pseudo-setup phase for the cProcInfo class. It will return positive if there has been an error.*/
		if (process_info.Capture()) {
			std::cout << "Error encountered when running capture for cProcInfo." << std::endl;
			return EXIT_FAILURE;
		}

		/* This is a struct containing process information, initialized with PID */
		SYSTEM_PROCESS* system_process_information = process_info.FindProcessByPid(process_id);

		/* This is a subset of SYSTEM_PROCESS. It will contain thread information, including thread state later on. */
		SYSTEM_THREAD* system_thread_information = nullptr;

		/* This creates a snapshot of all running threads */
		void* thread_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		/* This creates a new thread entry point and assigns the size. */
		tagTHREADENTRY32 thread_entry; 
		thread_entry.dwSize = sizeof thread_entry;

		/* Gets the thread entry of the first thread in the snapshot.*/
		Thread32First(thread_snapshot, &thread_entry);

		/* This may be flipped on later on to indicate that the thread should be resumed rather than suspended. */
		bool resume_mode = false;

		/* Loops over every single thread entry beyond the first one */
		do {

			/* Checks if the owner of the thread has a matching process ID to the one supplied.*/
			if (thread_entry.th32OwnerProcessID == process_id) {

				/* As we know the current thread belongs to the process that we're trying to suspend, we initialize
				the aformentioned SYSTEM_THREAD* with information about the current thread. */
				system_thread_information = process_info.FindThreadByTid(system_process_information, thread_entry.th32ThreadID);

				/* Stores the current suspend state of the thread. */
				BOOL current_thread_suspended = false;
				process_info.IsThreadSuspended(system_thread_information, &current_thread_suspended);

				/* Checks if the thread is currently suspended and sets resume mode to on if it is. */
				if (current_thread_suspended ) resume_mode = true;
				
				/* Creates a new handle to the thread which will be used to communicate with the thread. */
				void* thread_handle = OpenThread(THREAD_ALL_ACCESS, false, thread_entry.th32ThreadID);

				/* Thread suspension count */
				unsigned __int32 suspension_count;

				/* Sends resume signals until suspension count reaches zero */
				if (resume_mode) {
					do { suspension_count = ResumeThread(thread_handle); } while (suspension_count != 0);
					std::cout << "Resumed thread " << thread_entry.th32ThreadID << std::endl;
				} else {
					suspension_count = SuspendThread(thread_handle);
					std::cout << "Suspended thread " << thread_entry.th32ThreadID << std::endl;
				}

				/* Closes the thread handle now that communication is complete. */
				CloseHandle(thread_handle);
			}
			
		} while (Thread32Next(thread_snapshot, &thread_entry));

		/* Closes the thread snapshot handle that was made before */
		CloseHandle(thread_snapshot);

		return true;
	}

	Suspender() {

	}
};