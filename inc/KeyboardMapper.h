#pragma once
#include "SDL.h"
#include <unordered_map>
#include <stdint.h>
//##########################################
enum Chip8Key : uint8_t
{
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F,
	KEY_EXIT, KEY_NONE
};
//##########################################
class KeyboardMapper
{
private:
	static std::unordered_map<SDL_Keycode, Chip8Key> m_KeyMap;
public:
	static Chip8Key map(SDL_Keycode keyPressed);
};

