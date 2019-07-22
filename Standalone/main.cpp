#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <assert.h>

int main(int argc, char* argv[]) 
{

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init error %s\n", SDL_GetError());
		assert(false);
	}
	
	SDL_Window* win = SDL_CreateWindow("FAN", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		printf("SDL_CreateWindow error %s\n", SDL_GetError());
		SDL_Quit();
		assert(false);
	}	
	
	assert(false);
	return 0;
}
