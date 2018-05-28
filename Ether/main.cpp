
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
//#include <string.h>

#include "include/window.h"


int main(int argc, char* argv[]) {
	
	ether::Screen app;
	
	if (app.init() == false) {
		std::cout << "Failed to init application" << std::endl;
	}
	
/* 	SDL_Surface *screen = SDL_GetWindowSurface(window); 
	Uint32 blue = SDL_MapRGB(screen->format, 0,0,255);   
	SDL_FillRect(screen,NULL,blue);
	
	SDL_UpdateWindowSurface(window); */
	
	while (true)
	{
		Uint32 start_clock = SDL_GetTicks();
		
		if (app.Event_Loop() == false)
		{
			break;
		}	
		
		app.Frame_Cap(start_clock, 60);
	}
	
	app.close();

    return 0;
}
