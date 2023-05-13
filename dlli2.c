#include <Windows.h>
#include <fstream>

// compile command => x86_64-w64-mingw32-g++ -shared -o dlli2.dll dlli2.c
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Create a file called "injected.txt" in the current directory
        std::ofstream file("injected.txt");
        if (file.is_open())
        {
            file << "DLL Injected Successfully!" << std::endl;
            file.close();
        }
        break;
    }
    return TRUE;
}