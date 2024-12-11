#pragma once
//##########################################
#include <SDL.h>
#include "KeyboardMapper.h"

//##########################################
#define CHIP_8_NUMBER_OF_KEYS  17U

//##########################################
enum KeyState : uint8_t
{
	NOT_PRESSED, PRESSED
};
//##########################################

class InputHandler
{
private:
	static KeyState m_KeysPressed[CHIP_8_NUMBER_OF_KEYS];
	static Chip8Key m_LastKeyPressed;
	InputHandler();
public:
	static void ProcessKeyboardInputs();
	static KeyState GetKeyState(Chip8Key key);
	static Chip8Key GetLastKeyPressed();
};

