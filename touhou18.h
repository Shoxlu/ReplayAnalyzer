#pragma once

#include "th18structdefs.h"

typedef int(__stdcall* register__on_tick_ptr)(zUpdateFunc* thisPtr, int32_t arg1);
int register__on_tick(zUpdateFunc* function_to_register, int32_t priority) {
	return reinterpret_cast<register__on_tick_ptr>(0x401180)(function_to_register, priority);
}


Window_struct* WINDOW = (Window_struct*)0x568c30;
zGlobals* global_ptr = (zGlobals*)0x4cccc0;
zPlayer** player_ptr = (zPlayer**)0x4CF410;
zBulletManager** Bullet_PTR = (zBulletManager**)0x4CF2BC;
int32_t* Inputs = (int32_t*)0x4CA210;
int32_t* MenuInputs = (int32_t*)0x4ca21C;
int32_t* MenuInputs_prev = (int32_t*)0x4ca218;
int32_t* Inputs_prev = (int32_t*)0x4CA214;
zPauseMenu* pauseMenu_ptr = *(zPauseMenu**)0x4CF40C;
zMainMenu** main_menu_ptr = (zMainMenu**)0x4cf43c;
zReplayManager** replay_manager_ptr = (zReplayManager**)0x4CF418;
zSupervisor* supervisor_ptr = (zSupervisor*)0x4CCDF0;
zTableStageData** CUR_STAGE_DATA_ptr = (zTableStageData**)0x4CF428;
zTableStageData* STAGE_DATA_TABLE = (zTableStageData*)0x4C9410; //(zTableStageData STAGE_DATA_TABLE[8])
int* abilityShop_ptr = (int*)0x4CF2A4;//maybe
char* replay_name_ptr = (char*)0x4CF2F0;
int* dword_4CF438 = (int*)0x4CF438;