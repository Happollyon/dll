#include <windows.h>

// compile command => x86_64-w64-mingw32-g++ -shared -o dlli.dll dlli.c
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        MessageBox(NULL, "Hello, World!", "DLL Message", MB_OK);
    }
    return TRUE;
}