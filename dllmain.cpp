// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#pragma once
#include "pch.h"
#include <stdio.h>
#include "touhou18.h"

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




int32_t analyze_state = 0;
void init();
void update();
void start_replay();
void register_update();


//My code
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {    
        AllocConsole();
        auto a = freopen("CONIN$", "r", stdin);
        a = freopen("CONOUT$", "w", stdout);
        SetConsoleTitle(L"ReplayAnalyzer Debug");
        printf("Preparing to run........\n");
        Sleep(1000);
        printf("DLL loaded!\n");
        auto id = GetCurrentProcessId();
        auto hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
        //patch_call(0x004712D9, init); this place crashes the game sometimes
        patch_call(0x4719B8, init);
        BYTE fix_missing_bytes2[] = { 0x90, 0x90, 0x90 };
        BYTE fix_missing_bytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
        writeMemory(0x4719B8+0x5, fix_missing_bytes2, sizeof(fix_missing_bytes2));
        writeMemory(0x462A67, fix_missing_bytes, sizeof(fix_missing_bytes));
        printf("End of dll init");
        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void init() {
    WINDOW->actual_time_second__ = 0.0; //do what i replaced in the WinMain's game function
    register_update();
    printf("End of init func");
}

void register_update() {
    zUpdateFunc* update_function = new zUpdateFunc();
    update_function->flags &= 0xFFFFFFFE;
    update_function->function_pointer = 0;
    update_function->on_registration = 0;
    update_function->on_cleanup = 0;
    update_function->priority__ = 0;
    update_function->list_node.entry = update_function;
    update_function->list_node.next = 0;
    update_function->list_node.prev = 0;
    update_function->list_node.__seldom_used = 0;
    update_function->flags |= 3u;
    update_function->function_pointer = (int)update;
    update_function->on_registration = 0;
    update_function->on_cleanup = 0;
    update_function->ecx_arg_to_function = 0;
    register__on_tick(update_function, 0);
}

void update() {
    global_ptr->current_lives = 4;
    if ((*main_menu_ptr)) {
        if (analyze_state == 0) {
            memcpy(replay_name_ptr, "th18_01.rpy", 11);
            //(*main_menu_ptr)->menu_state = 4;
            char* string = replay_name_ptr;
            (*main_menu_ptr)->current_menu = 12;
            (*main_menu_ptr)->menu_state = 0;
            //supervisor_ptr->gamemode_current = 0;
            //supervisor_ptr->gamemode_to_switch_to = 13;
            //supervisor_ptr->change_gamemode();
            analyze_state = 2;
        }
        else if (analyze_state == 2) {
            (*main_menu_ptr)->menu_state = 3;
            analyze_state = 3;
        }
    }
}

void start_replay() {
    //find a way to get the replay name from the .exe command (??)
    memcpy(replay_name_ptr, "th18_01.rpy", 11);
}

