#include "InputHandler.h"
//##########################################
KeyState InputHandler::m_KeysPressed[CHIP_8_NUMBER_OF_KEYS] = {NOT_PRESSED};
Chip8Key InputHandler::m_LastKeyPressed = KEY_NONE;
//##########################################

void InputHandler::ProcessKeyboardInputs()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			m_KeysPressed[KEY_EXIT] = PRESSED;
			break;
		case SDL_KEYDOWN:
			m_KeysPressed[KeyboardMapper::map(evt.key.keysym.sym)] = PRESSED;
			m_LastKeyPressed = KeyboardMapper::map(evt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_KeysPressed[KeyboardMapper::map(evt.key.keysym.sym)] = NOT_PRESSED;
			m_LastKeyPressed = KEY_NONE;
			break;
		}
	}
}

KeyState InputHandler::GetKeyState(Chip8Key key)
{
	return m_KeysPressed[key];
}

Chip8Key InputHandler::GetLastKeyPressed()
{
	return m_LastKeyPressed;
}
