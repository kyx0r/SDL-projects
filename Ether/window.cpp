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
		
		// create a pixel buffer
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
		}
		
		SDL_Surface *screen = SDL_GetWindowSurface(window); 
		blue = SDL_MapRGB(screen->format, 0,0,255);   
		green = SDL_MapRGB(screen->format, 0,255,0);  
		red = SDL_MapRGB(screen->format, 255,0,0);
		black = SDL_MapRGB(screen->format, 0,0,0);
		
		return true;
	}
	
	void Screen::Update(Uint32 start_clock)
	{
		SDL_Surface *screen = SDL_GetWindowSurface(window); 

		if (!cap)
		{
			SCREEN_WIDTH = SDL_GetWindowSurface(window)->w;
		    SCREEN_HEIGHT = SDL_GetWindowSurface(window)->h;
			std::cout << SCREEN_WIDTH << std::endl;
			std::cout << SCREEN_HEIGHT << std::endl;
			// 640 * 480 = 480h
			
			for(int i=prev; i<=SCREEN_WIDTH * SCREEN_HEIGHT; ++i)
			{
				if(i==0)
				{
					SDL_FillRect(screen,NULL,black);
				}
				Sprite sprite(red, startX=startX+tick, startY=startY+tick_y, 1, 1);
				//Sprite sprite2(green, startX-tick, startY-tick_y, 10, 10);
				//Sprite sprite3(blue, startX++, startY++, 1, 1);
				
				Sprite_Group All_Sprites;
				
				All_Sprites.Add(&sprite); 
				//All_Sprites.Add(&sprite2);
				//All_Sprites.Add(&sprite3);
				All_Sprites.draw(screen);
				
				prev=i;
				
				if(startX>=SCREEN_WIDTH)
				{
					tick = -1;
				}
				if(startY>=SCREEN_HEIGHT)
				{
					tick_y = -1;
				}
				if(startY<=0)
				{
					tick_y = 1;
				}
				if(startX<=0)
				{
					tick = 1;
				}
				
				if(startX==(SCREEN_WIDTH/2)-1)
				{
					hitCount++;
					std::cout << "Hitcount: ";
				    std::cout << hitCount << std::endl;
					if(hitCount>=(SCREEN_HEIGHT)/10)
					{
						break;
					}
				}
				
				SDL_UpdateWindowSurface(window);
				
/* 				std::cout << "startX: ";
				std::cout << startX << std::endl;
				std::cout << "startY: ";
				std::cout << startY << std::endl;
				
				std::cout << "tick: ";
				std::cout << tick << std::endl;
				std::cout << "tick_y: ";
				std::cout << tick_y << std::endl; */
			}
			hitCount = 0;
			cap = true; 
			if(resize)
			{
				startX = SCREEN_WIDTH/2;
				startY = SCREEN_HEIGHT/2;
				resize = false;
			}
		}
		
			
/* 		SDL_UpdateTexture(texture, nullptr, buffer, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer); */
		
		SDL_UpdateWindowSurface(window);
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
				if (event.type == SDL_WINDOWEVENT) {
					
				if (event.window.event == SDL_WINDOWEVENT_RESIZED||event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{cap = false; resize = true;std::cout << "Resized." << std::endl;}	
			
			
				}
			}
			return true; 
	}
	
}
