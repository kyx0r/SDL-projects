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
		SDL_Surface *screen = NULL;
		Uint32 Window_Flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		Uint32 *buffer;
		Uint32 *bufferBlur;	
		int startX = SCREEN_WIDTH/2;
		int startY = SCREEN_HEIGHT/2;
		Uint32 blue;
		Uint32 green; 
		Uint32 red;
		Uint32 black;
		bool cap=false; 
		bool resize = false;
		int prev =0;
		int tick = 1;
		int tick_y = 1;
		int hitCount=0;
	
	public:
		bool init();
		void close();
		bool Event_Loop();
		void Frame_Cap (Uint32 start_clock, int fps);
		void Update(Uint32 start_clock);
		
};
}