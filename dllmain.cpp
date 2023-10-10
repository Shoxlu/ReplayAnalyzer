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
void load_replay();
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
        BYTE change_timer_replay_title[] = { 0x83, 0xF9, 0x00, 0x90, 0x90, 0x90 };
        writeMemory(0x4719B8+0x5, fix_missing_bytes2, sizeof(fix_missing_bytes2));
        writeMemory(0x462A67, fix_missing_bytes, sizeof(fix_missing_bytes));
        //writeMemory(0x464F7C, change_timer_replay_title, sizeof(change_timer_replay_title));
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
    zUpdateFunc* update_function = zUpdateFunc::operator_new(update);
    update_function->flags |= 3u;
    register__on_tick(update_function, 20);
}

void update() {
    //global_ptr->current_lives = 4;
    if ((*main_menu_ptr) && supervisor_ptr->gamemode_current == 4 && global_ptr->time > 1) {
        if (analyze_state == 0) {
            load_replay();
            analyze_state = 2;
        }
        else if (analyze_state == 2) {
            //(*main_menu_ptr)->menu_state = 3;
            analyze_state = 3;
        }
    }
}

void load_replay() {
    //find a way to get the replay name from the .exe command (??)
    memcpy(replay_name_ptr, "th18_01.rpy", 12);
    zReplayManager* replay_man = 0;
    replay_man = zReplayManager::zReplayManager_new(replay_name_ptr);
    auto v9 = &replay_man->__stage_array[0].gamestate_at_stage_begin;
    int j = 0;
    for (int i = 0; i < 8; ++i)
    {
        j = i;
        if (*v9)
            break;
        v9 += 10;
    }
    global_ptr->inner.player_stage_num = j;
    global_ptr->inner.__stage_num = j;
    supervisor_ptr->gamemode_to_switch_to = 13;
    auto replay_info = replay_man->replay_info;
    *CUR_STAGE_DATA_ptr = &STAGE_DATA_TABLE[j];
    global_ptr->inner.shottype = replay_info->shottype;
    global_ptr->inner.subshot = replay_info->subshot;
    global_ptr->inner.field_FC = global_ptr->inner.difficulty;
    global_ptr->inner.difficulty = replay_info->difficulty;
    replay_man->destructor();
    //printf("%d",sizeof(zReplayManager));
    reinterpret_cast<operator_delete_ptr>(0x48DCA1)(replay_man);
    *dword_4CF438 = 1;
}

