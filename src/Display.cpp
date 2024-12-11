#include "Display.h"

Display::Display()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0 )
		std::cout << "[Display] Failed to initialize SDL: " << SDL_GetError() << std::endl;
	m_SDLWindow = SDL_CreateWindow(SDL_WINDOW_NAME , SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,  CHIP_8_DISPLAY_WIDTH_IN_RECTANGLES,
		CHIP_8_DISPLAY_LENGTH_IN_RECTANGLES, SDL_DISPLAY_FLAGS);
	if (!m_SDLWindow)
		std::cout << "[Display] Failed to create SDL Window: " << SDL_GetError() << std::endl;
}

//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis). 
bool Display::IsInsideDisplay(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate)
{
	return (pixelXCoordinate >= 0 && pixelXCoordinate <= CHIP_8_DISPLAY_WIDTH_IN_PIXELS) && (pixelYCoordinate >= 0 && pixelYCoordinate <= CHIP_8_DISPLAY_LENGTH_IN_PIXELS);
}

//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis). 
void Display::ColorPixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate, uint8_t redComponent, uint8_t greenComponent, uint8_t blueComponent)
{
	SDL_Surface* winSurface = SDL_GetWindowSurface(m_SDLWindow);
	SDL_Rect pixelRectangle{
		pixelXCoordinate * PIXEL_TO_RECTANGLE_SIZE,
		pixelYCoordinate * PIXEL_TO_RECTANGLE_SIZE,
		PIXEL_TO_RECTANGLE_SIZE,
		PIXEL_TO_RECTANGLE_SIZE
	};
	SDL_FillRect(winSurface, &pixelRectangle, SDL_MapRGB(winSurface->format, redComponent, greenComponent, blueComponent));
	SDL_UpdateWindowSurface(m_SDLWindow);
}

//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis). 
void Display::ColorPixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate, SDL_Color color)
{
	ColorPixel(pixelXCoordinate, pixelYCoordinate, color.r, color.g, color.b);
}


//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis). 
void Display::DrawPixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate)
{
	if (IsInsideDisplay(pixelXCoordinate, pixelYCoordinate))
	{
		ColorPixel(pixelXCoordinate, pixelYCoordinate, m_WHITE_COLOR);
		m_PixelStates[pixelXCoordinate][pixelYCoordinate] = ON;
	}
}

//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis).
void Display::ErasePixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate)
{
	if (IsInsideDisplay(pixelXCoordinate, pixelYCoordinate))
	{
		ColorPixel(pixelXCoordinate, pixelYCoordinate, m_BLACK_COLOR);
		m_PixelStates[pixelXCoordinate][pixelYCoordinate] = OFF;
	}
}

//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis).
void Display::TogglePixel(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate)
{
	if (!IsInsideDisplay(pixelXCoordinate, pixelYCoordinate))
		return;
	if (m_PixelStates[pixelXCoordinate][pixelYCoordinate] == OFF)
	{
		DrawPixel(pixelXCoordinate, pixelYCoordinate);
		m_PixelStates[pixelXCoordinate][pixelYCoordinate] = ON;
	}
	else 
	{
		ErasePixel(pixelXCoordinate, pixelYCoordinate);
		m_PixelStates[pixelXCoordinate][pixelYCoordinate] = OFF;
	}
}	

void Display::Clear()
{
	for (uint8_t xCoordinate = 0; xCoordinate < CHIP_8_DISPLAY_WIDTH_IN_PIXELS; ++xCoordinate)
	{
		for (uint8_t yCoordinate = 0; yCoordinate < CHIP_8_DISPLAY_LENGTH_IN_PIXELS; ++yCoordinate)
		{
			ErasePixel(xCoordinate, yCoordinate);
		}
	}
}

//@param pixelXCoordinate "The X coordinate of pixel(Horizontal Axis).
//@param pixelXCoordinate "The Y coordinate of pixel(Vertical Axis).
//@return 1 if pixel is white, 0 otherwise.
Display::Chip8PixelState Display::GetPixelValue(uint8_t pixelXCoordinate, uint8_t pixelYCoordinate)
{
	return m_PixelStates[pixelXCoordinate][pixelYCoordinate];
}

Display::~Display()
{
	SDL_DestroyWindow(m_SDLWindow);
	SDL_Quit();
}

