#undef UNICODE

#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>

//This code opens a new process with a spoofed parent ID
//Usage: spoofer.exe <Full path to file> <PID of parent process>


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
	

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, strtol(argv[2],NULL,10));
	if (hProcess == NULL) {
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
