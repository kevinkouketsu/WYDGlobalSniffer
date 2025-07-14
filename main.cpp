#define NOMINMAX

#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <TlHelp32.h>
#include "Hooks.hpp"

void CreateConsole()
{
    AllocConsole();

    // Output fix 
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateConsole();
        DoPatchs((uintptr_t)GetModuleHandle(NULL) + 0x1000);
        srand(time(nullptr));
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}