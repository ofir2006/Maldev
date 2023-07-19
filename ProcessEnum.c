#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>


//This code enumerates all of the running processes using an alternative method to using CreateToolhelp32Snapshot function



int main() {

	DWORD processes[1024];
	DWORD returned;
	BOOL eProcesses = EnumProcesses(processes, sizeof(processes), &returned);

	if (!eProcesses) {
	
		printf("Error getting proccesses! %d", GetLastError());

	}

	size_t NumberOfProcesses = returned / sizeof(DWORD);
	
	for (DWORD i = 0; i < (returned / sizeof(DWORD)); i++) {
		HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		if (hProc == INVALID_HANDLE_VALUE) {
			printf("Invalid handle! Error: %d", GetLastError());
		}
		else {

			LPSTR name[MAX_PATH];
			DWORD size = sizeof(name) / sizeof(WCHAR);
			if (GetModuleBaseNameW(hProc, NULL, name, sizeof(name) / sizeof(WCHAR)) > 0) {
				printf("%ls PID: %d\n", name, processes[i]);
				CloseHandle(hProc);
			
			}
		}
	}
	
	

		return 0;

}