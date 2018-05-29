#include "include/window.h"
#include "include/sprite.h"
#include <string.h>

namespace ether
{
	
	bool Screen::init()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
		{
			std::cout << "Failed to initialize SDL. Error = " << SDL_GetError() << std::endl;
			return false;
		}
		std::cout << "SDL initiated Successfully" << std::endl;

		window = SDL_CreateWindow("SDL2 Magic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, Window_Flags);

		if (window == nullptr) 
		{
			std::cout << "Failed to create window" << std::endl;
			close();
			return false;
		}
		std::cout << "Window created successfully" << std::endl;

		//create a renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

		if (renderer == nullptr) 
		{
			std::cout << "Failed to create renderer. Error = " << SDL_GetError() << std::endl;
			close();
			return false;
		}

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

		if (texture == nullptr) 
		{
			std::cout << "Failed to create texture. Error = " << SDL_GetError() << std::endl;
			close();
			return false;
		}
		std::cout << "Texture created successfully." << std::endl;

		
		SDL_Surface *screen = SDL_GetWindowSurface(window); 
		Uint32 blue = SDL_MapRGB(screen->format, 0,0,255);   
		Uint32 purple = SDL_MapRGB(screen->format, 100,100,255);  
		Uint32 green = SDL_MapRGB(screen->format, 0,255,0);  
		Uint32 red = SDL_MapRGB(screen->format, 255,0,0);
		SDL_FillRect(screen,NULL,blue);
		Sprite sprite(red, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 48, 64);
		sprite.draw(screen);
		SDL_UpdateWindowSurface(window);
		
/* 		// create a pixel buffer
		buffer = nullptr;

		try {
			buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
			bufferBlur = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

			// setting value to the buffer
			memset(buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
			memset(bufferBlur, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

		}
		catch (std::bad_alloc& aerr) 
		{
			std::cerr << "Failed to allocate memory. Error = " << aerr.what() << std::endl;
			close();
			return false;
		} */

		return true;
	}
	
	void Screen::Frame_Cap (Uint32 start_clock, int fps)
	{
		if ((1000/fps)> SDL_GetTicks() - start_clock)
		{
			SDL_Delay(1000/fps - (SDL_GetTicks() - start_clock));
		}	
	}
	
	void Screen::close()
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	bool Screen::Event_Loop()
	{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					return false;
				}
			}
			return true; 
	}
	
}
