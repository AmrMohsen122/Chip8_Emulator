#pragma once
#include <stdint.h>
#include <SDL.h>
#include <iostream>
//##########################################

#define SDL_WINDOW_NAME  "CHIP_8 Emulator"
#define SDL_DISPLAY_FLAGS 0

#define CHIP_8_DISPLAY_WIDTH_IN_PIXELS	64
#define CHIP_8_DISPLAY_LENGTH_IN_PIXELS 32
#define PIXEL_TO_RECTANGLE_SIZE 8
#define CHIP_8_DISPLAY_WIDTH_IN_RECTANGLES  (CHIP_8_DISPLAY_WIDTH_IN_PIXELS  * PIXEL_TO_RECTANGLE_SIZE)
#define CHIP_8_DISPLAY_LENGTH_IN_RECTANGLES (CHIP_8_DISPLAY_LENGTH_IN_PIXELS * PIXEL_TO_RECTANGLE_SIZE)

#define DISPLAY_TOP_CORNER_COORDINATE 0
#define DISPLAY_LEFT_CORNER_COORDINATE 0

#define WHITE_RED_COMPONENT 0xFF
#define WHITE_GREEN_COMPONENT 0xFF
#define WHITE_BLUE_COMPONENT 0xFF
#define BLACK_RED_COMPONENT 0x00
#define BLACK_GREEN_COMPONENT 0x00
#define BLACK_BLUE_COMPONENT 0x00
//##########################################


class Display
{
public:
	const SDL_Color m_WHITE_COLOR{ WHITE_RED_COMPONENT  , WHITE_GREEN_COMPONENT , WHITE_BLUE_COMPONENT };
	const SDL_Color m_BLACK_COLOR{ BLACK_RED_COMPONENT  , BLACK_GREEN_COMPONENT , BLACK_BLUE_COMPONENT };
	enum Chip8PixelState : bool
	{
		OFF,
		ON
	};
private:
	SDL_Window* m_SDLWindow;
	Chip8PixelState m_PixelStates[CHIP_8_DISPLAY_WIDTH_IN_PIXELS][CHIP_8_DISPLAY_LENGTH_IN_PIXELS] = {OFF};
	SDL_Event m_evt;
private:
	bool IsInsideDisplay(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate);
	void ColorPixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate, SDL_Color color);
	void ColorPixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate, uint8_t redComponent, uint8_t greenComponent, uint8_t blueComponent);
public:
	Display();
	void DrawPixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate);
	void ErasePixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate);
	void TogglePixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate);
	void Clear();
	Chip8PixelState GetPixelValue(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate);
	~Display();

};

