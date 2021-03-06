/*
	The process functions are used to gather information about processes, modules, etc.
	Code by Z0F.
*/
#include "Process.h"

// Get PID given name:
DWORD GetProcID(const std::string& procName) {
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(hSnapshot, &procEntry)) {
		do {
			if (procEntry.szExeFile == procName) {
				CloseHandle(hSnapshot);
				return procEntry.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &procEntry));
		CloseHandle(hSnapshot);
	}
	return 0;	
}

// Get ModuleEntry from module name, using toolhelp32snapshot:
MODULEENTRY32 GetModule(const DWORD &procID, const std::string& modName) {
	MODULEENTRY32 modEntry = {0};

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 curr = {0};
		curr.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &curr)) {
			do {
				if (curr.szModule == modName) {
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}
