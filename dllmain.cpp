// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#pragma once
#include "pch.h"
#include <stdio.h>
#include "touhou18.h"
#include "shellapi.h"
#include "analyser.h"

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
char user_replay_name[256] = {};
int32_t analyze_state = 0;
Analyzer main_analyzer;


void init();
int update();
void load_replay();
void register_update();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {    
        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        AllocConsole();
        auto a = freopen("CONIN$", "r", stdin);
        a = freopen("CONOUT$", "w", stdout);
        SetConsoleTitle(L"ReplayAnalyzer Debug");
        printf("Preparing to run........\n");
        printf("DLL loaded!\n");
        auto id = GetCurrentProcessId();
        auto hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
        patch_call(0x4719B8, init);
        BYTE fix_missing_bytes2[] = { 0x90, 0x90, 0x90 };
        BYTE fix_missing_bytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
        BYTE force_speedup_replay[] = { 0x06 };
        /*BYTE delete_chdir_appdata[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
        writeMemory(0x004626C0, delete_chdir_appdata, sizeof(delete_chdir_appdata));*/
        writeMemory(0x4719B8+0x5, fix_missing_bytes2, sizeof(fix_missing_bytes2));
        writeMemory(0x462A67, fix_missing_bytes, sizeof(fix_missing_bytes));//je sais plus...
        writeMemory(0x461E06, force_speedup_replay, sizeof(force_speedup_replay));
        wcstombs(user_replay_name, argv[0], 256);
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
    retrieve_game_data();
    WINDOW->actual_time_second__ = 0.0; //do what i replaced in the WinMain's game function
    register_update();
    printf("End of init func\n");
}

void register_update() {
    zUpdateFunc* update_function = zUpdateFunc::operator_new(update);
    update_function->flags |= 3u;//magic number from decomp (don't know what it's doing)
    register__on_tick(update_function, 1);
}

int update() {
    //global_ptr->current_lives = 4;
    //printf("update\n");
    retrieve_game_data();
   // printf("%x, %d, %d \n", main_menu_ptr, supervisor_ptr->gamemode_current, global_ptr->time);
    if (main_menu_ptr && supervisor_ptr->gamemode_current == 4 && global_ptr->time > 1) {
        if (analyze_state == 0) {
            load_replay();
            analyze_state = 1;
        }
    }
    if (analyze_state == 1) {
        main_analyzer.Update();
    }
    return 1;
}

void load_replay() {
    //find a way to get the replay name from the .exe command (??)
    memcpy(replay_name_ptr, user_replay_name, 12);
    zReplayManager* replay_man = zReplayManager::zReplayManager_new(replay_name_ptr);
    auto v9 = &replay_man->__stage_array[0].gamestate_at_stage_begin;
    int j = 0;
    for (j= 0; j < 8; ++j)
    {
        if (*v9)
            break;
        v9 += 10;
    }
    global_ptr->inner.stage_num = j;
    global_ptr->inner.__stage_num = j;
    supervisor_ptr->gamemode_to_switch_to = 13;
    auto replay_info = replay_man->replay_info;
    *CUR_STAGE_DATA = &STAGE_DATA_TABLE[j];
  
    global_ptr->inner.shottype = replay_info->shottype;
    global_ptr->inner.subshot = replay_info->subshot;
    global_ptr->inner.field_FC = global_ptr->inner.difficulty;
    global_ptr->inner.difficulty = replay_info->difficulty;
    replay_man->destructor();
    operator_delete(replay_man);
    *dword_4CF438 = 1;
}

