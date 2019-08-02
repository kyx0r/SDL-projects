#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include "char_tex.h"

SDL_Window* window = nullptr;
SDL_Surface* screen = nullptr;
SDL_Renderer* renderer = nullptr;

typedef int32_t vec2_t[2];
typedef int32_t vec3_t[3];

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) \
		{ \
			printf("%s \n", message);\
			printf("Assertion %s failed in, %s line: %d \n", #condition, __FILE__, __LINE__);\
            char buf[10]; \
			fgets(buf, 10, stdin); \
			exit(1); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

void FrameCap(uint32_t start_clock, uint8_t fps)
{
	if ((1000/fps) > SDL_GetTicks() - start_clock)
	{
		SDL_Delay(1000/fps - (SDL_GetTicks() - start_clock));
	}	
}

bool EventLoop()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
				return false;
		}
		if (event.type == SDL_KEYDOWN)
		{

		} 		
		
		if (event.type == SDL_WINDOWEVENT)
		{
					
			if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				screen = SDL_GetWindowSurface(window); 
			}	
		}
	}	
	return true;
}

void Triangle(const vec2_t org, const vec3_t magnitude)
{
	SDL_Point points[4] = 
	{
		{org[0], org[1]},
		{org[0] + magnitude[1], org[1] + magnitude[2] / 2},
		{org[0] + magnitude[2], org[1] - magnitude[2] / 2},
		{org[0], org[1]}
	};
	SDL_RenderDrawLines(renderer, points, 4);
}

void Circle(int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}


int main(int argc, char* argv[]) 
{

	ASSERT(!SDL_Init(SDL_INIT_EVERYTHING), SDL_GetError());
	
	window = SDL_CreateWindow("FAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	ASSERT(window, SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	ASSERT(renderer, SDL_GetError());
	
	screen = SDL_GetWindowSurface(window); 
	ASSERT(screen, SDL_GetError());
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0,0,0));
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	
	SDL_Surface* texsur = SDL_LoadCachedBMP(alpa_chars, 364544);
	ASSERT(texsur, SDL_GetError());
    SDL_Texture* atlas = SDL_CreateTextureFromSurface(renderer, texsur);
	ASSERT(atlas, SDL_GetError());
	
	
/*     SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, 100, 100, 32, SDL_PIXELFORMAT_RGBA8888);
    ASSERT(surf, SDL_GetError());
	
	ASSERT(SDL_BlitSurface(texsur, nullptr, surf, &texsur->clip_rect),SDL_GetError()); */
    SDL_FreeSurface(texsur);
	
	uint32_t back_format = SDL_PIXELFORMAT_RGBA8888;
	SDL_Texture* Background_Tx = SDL_CreateTexture(renderer, back_format, SDL_TEXTUREACCESS_TARGET, screen->w, screen->h);
	ASSERT(Background_Tx, SDL_GetError());

	int atlw;
	int atlh;
	ASSERT(!SDL_QueryTexture(atlas, nullptr, nullptr, &atlw, &atlh), SDL_GetError());	
	
	SDL_SetRenderTarget(renderer, Background_Tx);
	
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = atlw;
	SrcR.h = atlh;

	DestR.x = 50;
	DestR.y = 50;
	DestR.w = screen->w/2;
	DestR.h = screen->h/2;	
		
	SDL_RenderCopy(renderer, atlas, &SrcR, &DestR);
	SDL_SetRenderTarget(renderer, nullptr);
	
	while(true)
	{
		uint32_t start_clock = SDL_GetTicks();
			
		if(!EventLoop())
		{
			break;
		}
		
		FrameCap(start_clock, 60);
		
		vec2_t org = {320, 200};
		vec3_t m = {200, 20, 50}; 
		
		//ASSERT(!SDL_UpdateTexture(Background_Tx, nullptr, &back_format, screen->w*4), SDL_GetError());
		//ASSERT(!SDL_RenderCopyEx(renderer, Background_Tx, nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE), SDL_GetError());
		
		SDL_RenderCopyEx(renderer, atlas, &SrcR, &DestR, 0, nullptr, SDL_FLIP_NONE);
	    //Triangle(org, m);
		//Circle(100, 100, 50);
		
		SDL_RenderPresent(renderer);
		
	}	
	SDL_DestroyTexture(Background_Tx);
	SDL_DestroyTexture(atlas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
