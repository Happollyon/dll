#include <windows.h>
#include <stdio.h>
#include<tlhelp32.h>


void getPorcessInfo(){

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // create a snapshot of all runing processes 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot (of processes) failed with error %d \n", GetLastError());
        return;
    }
    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,	
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32)) {
        printf("Process32First failed with error %d \n", GetLastError());
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return;
    }
    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do {
        printf("\n\n=====================================================");
        printf("\nPROCESS NAME:  %s", pe32.szExeFile);
        printf("\n-------------------------------------------------------");
        printf("\n  Process ID        = 0x%08X", pe32.th32ProcessID);
        printf("\n  Thread count      = %d", pe32.cntThreads);
        printf("\n  Parent process ID = 0x%08X", pe32.th32ParentProcessID);
        printf("\n  Priority base     = %d", pe32.pcPriClassBase);
        printf("\n  Flags             = 0x%08X", pe32.dwFlags);
        printf("\n-------------------------------------------------------");
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}   
int main (void){

    getPorcessInfo();
    return 0;
}