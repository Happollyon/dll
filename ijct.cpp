#include <windows.h>
#include <iostream>

int main(int argc, char* argv[])
{if (argc < 2) {
        printf("(--) usage: dll.exe <pid>");
        return EXIT_FAILURE;
    }

    DWORD PID = atoi(argv[1]);
    // Path to the target process
    const char* targetProcess = "C:\\Windows\\System32\\notepad.exe";

    // Path to the DLL to inject
    const char* dllPath = "C:\\Users\\fagner\\Desktop\\windowsM\\dll\\dlli.dll";

    // Load the target process
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE,PID);// if I replace process ID with GetCurrentProcessId() it works 
   
    if (hProcess == NULL)
    {
        std::cout << GetLastError() << std::endl;
        std::cout<<"error opening process"<<std::endl;
        return 1;
    }
    
    
    std::cout<<"id: "<<hProcess<<std::endl;

    // Allocate memory for the DLL path in the target process
    LPVOID dllPathAddress = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    
    if (dllPathAddress == NULL)
    {
        std::cout << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Write the DLL path into the target process
    if (!WriteProcessMemory(hProcess, dllPathAddress, dllPath, strlen(dllPath) + 1, NULL))
    {   std::cout << GetLastError() << std::endl;
        
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Get the address of the LoadLibraryA function
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    
    if (hKernel32 == NULL)
    {
        std::cout << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    LPTHREAD_START_ROUTINE loadLibraryAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryA");
    
    if (loadLibraryAddr == NULL)
    {
        std::cout << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Create a remote thread in the target process to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibraryAddr, dllPathAddress, 0, NULL);
   

    // when I use another process such as notepad createRemoteThread returns NULL
    if (hThread == NULL)
    {
        // when I target any other porcess GetLasterror() returns 6 which mean "invalid error"
        std::cout << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }
    

    std::cout << "DLL injected successfully!" << std::endl;

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Clean up resources
    VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}