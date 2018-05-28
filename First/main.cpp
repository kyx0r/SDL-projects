
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>

SDL_Event event;
bool running = true;
SDL_Window *window = NULL;      
Uint32 start_clock;
Uint32 Window_Flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;

void Frame_Cap (Uint32 start_clock, int fps)
{
	if ((1000/fps)> SDL_GetTicks() - start_clock)
	{
		SDL_Delay(1000/fps - (SDL_GetTicks() - start_clock));
	}
}	

int main(int argc, char* argv[]) {
	
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	
    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "SDL2 Magic",                      // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        Window_Flags
    );

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
	
	SDL_Surface *screen = SDL_GetWindowSurface(window); 
	Uint32 blue = SDL_MapRGB(screen->format, 0,0,255);   
	SDL_FillRect(screen,NULL,blue);
	setPixel(320, 280, 255, 0, 0); 
	SDL_UpdateWindowSurface(window);
	
	while (running)
	{
		start_clock = SDL_GetTicks();
		
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}
		}
		
	}
	
	Frame_Cap(start_clock, 60);
	
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
