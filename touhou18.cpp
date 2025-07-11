#include "touhou18.h"

zGlobals* global_ptr = (zGlobals*)GLOBAL_PTR_ADD;
int32_t* Inputs = (int32_t*)INPUT_PTR_ADD;
int32_t* MenuInputs = (int32_t*)MENUINPUT_PTR_ADD;
int32_t* MenuInputs_prev = (int32_t*)MENUINPUT_PREV_PTR_ADD;
int32_t* Inputs_prev = (int32_t*)INPUT_PREV_PTR_ADD;
Window_struct* WINDOW = (Window_struct*)WINDOW_PTR_ADD;
char* replay_name_ptr = (char*)REPLAY_NAME_PTR;
zPlayer* player_ptr;
zBulletManager* bullet_ptr;
zMainMenu* main_menu_ptr;
zSupervisor* supervisor_ptr = (zSupervisor*)SUPERVISOR_PTR_ADD;
zTableStageData* STAGE_DATA_TABLE = (zTableStageData*)STAGE_DATA_TABLE_ADD; //(zTableStageData STAGE_DATA_TABLE[8])
zTableStageData** CUR_STAGE_DATA = (zTableStageData**)CUR_STAGE_DATA_ADD;
int32_t* abilityShop_ptr = (int32_t*)ABILITYSHOP_PTR_ADD;//maybe
zReplayManager* replay_manager_ptr;

int32_t* dword_4CF438 = (int*)0x4CF438;


typedef int(__stdcall* register__on_tick_ptr)(zUpdateFunc* thisPtr, int32_t arg1);
int register__on_tick(zUpdateFunc* function_to_register, int32_t priority) {
    return reinterpret_cast<register__on_tick_ptr>(0x401180)(function_to_register, priority);
}

void operator_delete(void* ptr) {
    return reinterpret_cast<operator_delete_ptr>(0x48DCA1)(ptr);
}

zMainMenu* get_main_menu_ptr() {
    return *(zMainMenu**)MAIN_MENU_PTR_ADD;
}

zBulletManager* get_bullet_ptr() {
    return *(zBulletManager**)BULLET_PTR_ADD;
}

zReplayManager* get_replay_manager_ptr(){
    return *(zReplayManager**)REPLAY_MANAGER_PTR_ADD;
}

zPlayer* get_player_ptr() {
    return *(zPlayer**)PLAYER_PTR_ADD;
}

//zPauseMenu* get_pausemenu_ptr() {
//    return *(zPauseMenu**)PAUSEMENU_PTR_ADD;
//}

void retrieve_game_data() {

    bullet_ptr = get_bullet_ptr();
    player_ptr = get_player_ptr();
    //pauseMenu_ptr = get_pausemenu_ptr();
    main_menu_ptr = get_main_menu_ptr();
    replay_manager_ptr = get_replay_manager_ptr();
}


