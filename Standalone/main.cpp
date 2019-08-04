#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include "char_tex.h"

SDL_Window* window = nullptr;
SDL_Surface* screen = nullptr;
SDL_Surface* texsur = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* Background_Tx = nullptr;
uint32_t* framebuffer = nullptr;

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
				delete framebuffer;
				SDL_DestroyTexture(Background_Tx);
				framebuffer = new uint32_t[screen->w * screen->h];
				ASSERT(framebuffer, "Malloc failed");
				Background_Tx = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, screen->w, screen->h);
				ASSERT(Background_Tx, SDL_GetError());
				
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

void SetPixel(uint32_t* buf, int w, int h, int x, int y, uint8_t red, uint8_t green, uint8_t blue) 
{
	uint32_t color = 0;

	if (x < 0 || x >= w || y < 0 || y >= h) 
	{
		return;
	}
		
	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF; //a mask 

	buf[(y * w) + x] = color; //32 bit value.
}

void SetPixel32(uint32_t* buf, int w, int h, int x, int y, uint32_t pixel) 
{
	if (x < 0 || x >= w || y < 0 || y >= h) 
	{
		return;
	}

	buf[(y * w) + x] = pixel; //32 bit value.
}

void DebugSurface(SDL_Surface* sur)
{
	printf("width: %d \n", sur->w);
	printf("height: %d \n", sur->h);
	printf("pitch: %d \n", sur->pitch);
	printf("Destiny: %p \n", sur->pixels);	
	
	//printf("format: %d \n", texsur->format);
	
	ASSERT(false, "End of surface dump.");
}
	//uint32_t* atlas_buffer = new uint32_t[screen->w * screen->h];
	//memcpy(atlas_buffer, texsur->pixels, screen->w * screen->h);
	//memset(atlas_buffer, 0, screen->w * screen->h);
	
	//ASSERT(!SDL_LockSurface(texsur), SDL_GetError());

	
    //SDL_Texture* atlas = SDL_CreateTextureFromSurface(renderer, texsur);
	//ASSERT(atlas, SDL_GetError());
    //SDL_FreeSurface(texsur);
	
	//ASSERT(!SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, ), SDL_GetError());
	
void DrawCharacter(int xpos, int ypos, char num, uint32_t color)
{
	if(num == 0)
	{
		return;
	}
	
	uint32_t* real_pixels = (uint32_t*) texsur->pixels;
	int num_in_row = texsur->w / 25;
	int target = 0; 
	
	for(int i = num; i>=num_in_row; i-=num_in_row)
	{
		target++;
	}
	
	int ywalk = target * 25;
	int xwalk = (25 * num) - ((num_in_row * target) * 25);
		
	for(int y = ywalk+1; y<ywalk+24; y++)
	{
		for(int x = xwalk+1; x<xwalk+24; x++)
		{
			if(real_pixels[(y * texsur->w) + x] == 0xFFFFFFFF)
			{
				SetPixel32(framebuffer, screen->w, screen->h, xpos+(x-xwalk+1), ypos+(y-ywalk+1), color);
			}
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
	
	texsur = SDL_LoadCachedBMP(atlas_chars, 32768);
	ASSERT(texsur, SDL_GetError());
	texsur = SDL_ConvertSurfaceFormat(texsur, SDL_PIXELFORMAT_RGBA8888, 0);
	ASSERT(texsur, SDL_GetError());
	ASSERT(!SDL_LockSurface(texsur), SDL_GetError());
	
	//SDL_Texture* atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, texsur->w, texsur->h);
	//ASSERT(atlas, SDL_GetError());
	
	framebuffer = new uint32_t[screen->w * screen->h];
	ASSERT(framebuffer, "Malloc failed");
	
	Background_Tx = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, screen->w, screen->h);
	ASSERT(Background_Tx, SDL_GetError());
	
	DrawCharacter(50, 50, 1, 0xFF0000FF);
	
	DrawCharacter(100, 50, 5, 0x0000FFFF);	
		
/*     for (int i = 0; i < texsur->w * texsur->h; i++)
    {
        if (real_pixels[i] == 0xFFFFFFFF)
		{
			printf("pixel: %d \n", i);
			break;
			//real_pixels[i] = 0x00000000;
		}
        
    } */
	
	//printf("pixel: %d \n", texsur->w+87);
	
	//ASSERT(!SDL_UpdateTexture(atlas, nullptr, real_pixels, texsur->pitch), SDL_GetError());
	//SDL_FreeSurface(texsur);

/* 	int atlw;
	int atlh;
	ASSERT(!SDL_QueryTexture(atlas, nullptr, nullptr, &atlw, &atlh), SDL_GetError());	 */
	
/* 	void* pixels;
	int pitch;
	ASSERT(!SDL_LockTexture(atlas, nullptr, &pixels, &pitch), SDL_GetError());
	uint32_t* real_pixels = (uint32_t*) pixels;
	
    // manipulate pixels
    for (int i = 0; i < atlw * atlh; i++)
    {
        if (real_pixels[i] == 0)
		{
			printf("pixels \n");
		}
        
    }	 */
	
	//SDL_SetRenderTarget(renderer, Background_Tx);
	
/* 	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = atlw;
	SrcR.h = atlh;

	DestR.x = 0;
	DestR.y = 0;
	DestR.w = screen->w;
	DestR.h = screen->h;	
		
	SDL_RenderCopy(renderer, atlas, &SrcR, &DestR); */
	//SDL_SetRenderTarget(renderer, nullptr);
	
	//SDL_Texture* _texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, screen->w, screen->h);
	//uint32_t* atlas_buffer = new uint32_t[screen->w * screen->h];
	//memset(atlas_buffer, 0, screen->w * screen->h);
	
	//setPixel(atlas_buffer, 100, 100, 255,255,255);
	
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
		
		//ASSERT(!SDL_UpdateTexture(atlas, nullptr, atlas_buffer, screen->w * sizeof(uint32_t)), SDL_GetError());
		//SDL_RenderClear(renderer);
		

		
		ASSERT(!SDL_UpdateTexture(Background_Tx, nullptr, framebuffer, screen->w * 4), SDL_GetError());
		SDL_RenderCopy(renderer, Background_Tx, nullptr, nullptr);
		
		//SDL_RenderCopyEx(renderer, atlas, &SrcR, &DestR, 0, nullptr, SDL_FLIP_NONE);
	    //Triangle(org, m);
		//Circle(100, 100, 50);
		
		SDL_RenderPresent(renderer);
		
	}	
	SDL_DestroyTexture(Background_Tx);
	//SDL_DestroyTexture(atlas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
