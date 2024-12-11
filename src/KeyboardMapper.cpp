#include "KeyboardMapper.h"
//##########################################
std::unordered_map<SDL_Keycode, Chip8Key> KeyboardMapper::m_KeyMap = {
		{SDLK_1 , KEY_1},
		{SDLK_2 , KEY_2},
		{SDLK_3 , KEY_3},
		{SDLK_q , KEY_4},
		{SDLK_w , KEY_5},
		{SDLK_e , KEY_6},
		{SDLK_a , KEY_7},
		{SDLK_s , KEY_8},
		{SDLK_d , KEY_9},
		{SDLK_z , KEY_A},
		{SDLK_x , KEY_0},
		{SDLK_c , KEY_B},
		{SDLK_4 , KEY_C},
		{SDLK_r , KEY_D},
		{SDLK_f , KEY_E},
		{SDLK_v , KEY_F},
};
//##########################################

Chip8Key KeyboardMapper::map(SDL_Keycode keyPressed)
{
    return m_KeyMap[keyPressed];
}
