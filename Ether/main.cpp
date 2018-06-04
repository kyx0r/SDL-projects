#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "include/window.h"
#include "include/sprite.h"

int main(int argc, char* argv[]) {
	
	ether::Screen app;
	
	if (app.init() == false) {
		std::cout << "Failed to init application" << std::endl;
	}
			
	while (true)
	{
		Uint32 start_clock = SDL_GetTicks();
		
		if (app.Event_Loop() == false)
		{
			break;
		}	
		
		app.Frame_Cap(start_clock, 60);
		app.Update(start_clock);
	}
	
	app.close();

    return 0;
}
