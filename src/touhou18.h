#pragma once

#include "th18structdefs.h"

namespace th18 {
	using namespace th18;
	int register__on_tick(zUpdateFunc* function_to_register, int32_t priority);
	void operator_delete(void* ptr);

	//Game's data related
	static constexpr int32_t NINPUTSPBULLET = 5;
	static constexpr int32_t NINPUTSENEMY = 3;
	static constexpr int32_t N_BULLETS = 2000;
	static constexpr int32_t INPUTS_MAX = NINPUTSENEMY + NINPUTSPBULLET + 2;
	static constexpr int32_t OUTPUTS_MAX = 5;

	//Addresses
	static constexpr int32_t PLAYER_PTR_ADD = 0x4CF410;
	static constexpr int32_t WINDOW_PTR_ADD = 0x568c30;
	static constexpr int32_t GLOBAL_PTR_ADD = 0x4cccc0;
	static constexpr int32_t BULLET_PTR_ADD = 0x4CF2BC;
	static constexpr int32_t INPUT_PTR_ADD = 0x4CA210;
	static constexpr int32_t MENUINPUT_PTR_ADD = 0x4ca21C;
	static constexpr int32_t MENUINPUT_PREV_PTR_ADD = 0x4ca218;
	static constexpr int32_t INPUT_PREV_PTR_ADD = 0x4CA214;
	static constexpr int32_t PAUSEMENU_PTR_ADD = 0x4CF40C;
	static constexpr int32_t ABILITYSHOP_PTR_ADD = 0x4CF2A4;
	static constexpr int32_t REPLAY_NAME_PTR = 0x4CF2F0;
	static constexpr int32_t MAIN_MENU_PTR_ADD = 0x4cf43c;
	static constexpr int32_t SUPERVISOR_PTR_ADD = 0x4CCDF0;
	static constexpr int32_t REPLAY_MANAGER_PTR_ADD = 0x4CF418;
	static constexpr int32_t CUR_STAGE_DATA_ADD = 0x4CF428;
	static constexpr int32_t STAGE_DATA_TABLE_ADD = 0x4C9410;

	extern zGlobals* global_ptr;
	extern zPlayer* player_ptr;
	extern zBulletManager* bullet_ptr;
	extern int32_t* Inputs;
	extern int32_t* MenuInputs;
	extern int32_t* Inputs_prev;
	extern int32_t* MenuInputs_prev;
	extern zPauseMenu* pauseMenu_ptr;
	extern zSupervisor* supervisor_ptr;
	extern zMainMenu* main_menu_ptr;
	extern Window_struct* WINDOW;
	extern char* replay_name_ptr;
	extern int32_t* dword_4CF438;
	extern zTableStageData* STAGE_DATA_TABLE;
	extern zTableStageData** CUR_STAGE_DATA;
	extern zReplayManager* replay_manager_ptr;

	void retrieve_game_data();
	zPlayer* get_player_ptr();
}