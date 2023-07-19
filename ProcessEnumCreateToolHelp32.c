#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <Dbghelp.h>


// This code outputs all of the processes running on the machine using the classic CreateToolhelp32Snapshot function



int main() {

    int pid = 0;
    HANDLE hSnapshot;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL first = Process32First(hSnapshot, &entry);
    printf("%ls PID: %d\n", entry.szExeFile, entry.th32ProcessID);

    while (Process32Next(hSnapshot, &entry)) {
        printf("%ls PID: %d\n", entry.szExeFile, entry.th32ProcessID);
        }


    return 0;

}
