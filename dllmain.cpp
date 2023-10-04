// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"
#include <stdio.h>

//Khangaroo's code
void writeMemory(void* dst, void* src, size_t len) {
    DWORD prot;
    VirtualProtect(dst, len, PAGE_READWRITE, &prot);
    memcpy(dst, src, len);
    VirtualProtect(dst, len, prot, &prot);
}
void writeMemory(DWORD dst, void* src, size_t len) {
    writeMemory((void*)dst, src, len);
}
//Khangaroo's code
void patch_call(void* target, void* func) {
    BYTE patch[5];
    patch[0] = 0xE8;
    *(DWORD*)(patch + 1) = (DWORD)func - (DWORD)target - 5;
    writeMemory(target, patch, sizeof(patch));
}

void patch_call(DWORD target, void* func) {
    patch_call((void*)target, func);
}



//My code
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        auto a = freopen("CONIN$", "r", stdin);
        a = freopen("CONOUT$", "w", stdout);
        SetConsoleTitle(L"Th18 IA");
        printf("Preparing to run........\n");
        Sleep(1000);
        printf("DLL loaded!\n");
        auto id = GetCurrentProcessId();
        auto hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
        patch_call(0x004712D9, init);
        patch_call(0x00471C2A, update);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void init() {

}

void update() {

}

