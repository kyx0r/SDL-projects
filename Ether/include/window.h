#pragma once
#include <iostream>
#include <SDL2/SDL.h>

namespace ether
{

class Screen
{
	public:
		int SCREEN_WIDTH = 640;
		int SCREEN_HEIGHT = 480;
		SDL_Event event;
		SDL_Window *window = NULL;      
		Uint32 Window_Flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		Uint32 *buffer;
		Uint32 *bufferBlur;	
		Uint32 test = 20;
	
	public:
		bool init();
		void close();
		bool Event_Loop();
		void Frame_Cap (Uint32 start_clock, int fps);
		void Update();
		
};
}