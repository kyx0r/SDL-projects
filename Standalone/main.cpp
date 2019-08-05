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

typedef struct char_ent
{
	uint16_t x;
	uint16_t y;
	unsigned char* pix_ptr; //x,y wiil hold locations for this ent
	uint16_t size;
	uint32_t color;
	float scale;
	char num;
	struct char_ent* prev;
	struct char_ent* next;
	
} char_ent;

int current_char_index = 0;
char_ent characters[2000] = {}; //should never be reached. 2000 chars max

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

void ReadPixel32(uint32_t* buf, int w, int h, int x, int y, uint32_t &pixel) 
{
	if (x < 0 || x >= w || y < 0 || y >= h) 
	{
		return;
	}

	pixel = buf[(y * w) + x]; //32 bit value.
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

void Circle(int32_t centreX, int32_t centreY, int32_t radius, uint32_t color)
{
   //printf("here \n");
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
	  SetPixel32(framebuffer, screen->w, screen->h, centreX + x, centreY - y, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX + x, centreY + y, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX - x, centreY - y, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX - x, centreY + y, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX + y, centreY - x, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX + y, centreY + x, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX - y, centreY - x, color);
	  SetPixel32(framebuffer, screen->w, screen->h, centreX - y, centreY + x, color);
 /*      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x); */

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
	
	char_ent* node = &characters[current_char_index];
	node->x = xpos;
	node->y = ypos;
	node->size = 0;
	node->color = color;
	node->scale = 1.0f;
	node->num = num;
	if(node->prev)
	{
		node->pix_ptr = node->prev->pix_ptr + (node->prev->size * sizeof(uint16_t));
	}
	else
	{
		node->pix_ptr = &atlas_chars[0]; //reuse the memory as the copy of it resides in surface anyway
	}
	
	node->next = &characters[++current_char_index];	
	node->next->prev = node;
	
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
				//this can be better if allowed to use full 32/64 bit width
				*((uint16_t*)node->pix_ptr+node->size) = xpos+(x-xwalk+1); 
				node->size += sizeof(uint16_t);
				*((uint16_t*)node->pix_ptr+node->size) = ypos+(y-ywalk+1);
				node->size += sizeof(uint16_t);
				SetPixel32(framebuffer, screen->w, screen->h, xpos+(x-xwalk+1), ypos+(y-ywalk+1), color);
			}
		}
	}
}	

void TranslateCharacter(int x, int y, int index, uint32_t clear_val)
{
	//printf("here \n");
	char_ent* node = &characters[index];
	for(unsigned char* i = &node->pix_ptr[0]; i<node->pix_ptr + (node->size * sizeof(uint16_t)); i+=8)
	{
		uint16_t xpos = *i;
		uint16_t ypos = *(i+4);
		*((uint16_t*)i) = xpos + x;
		*((uint16_t*)i+sizeof(uint16_t)) = ypos + y;
		SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, clear_val);
		SetPixel32(framebuffer, screen->w, screen->h, xpos+x, ypos+y, node->color);
	}
}

void ScaleCharacter(int index, float scale)
{
	//printf("Scalech \n");
	char_ent* node = &characters[index];
	node->scale = scale;
	int s_scale = 24 * scale;
	
	SDL_Texture* tmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 24, 24);
	ASSERT(tmp, SDL_GetError());	
	SDL_Texture* dest_tmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, s_scale, s_scale);
	ASSERT(dest_tmp, SDL_GetError());	
	
	uint32_t tile[30*30] = {}; //make sure we get enough stack space for next func calls too. Actual 24 * 24;
	uint32_t* real_pixels = (uint32_t*) texsur->pixels;
	int num_in_row = texsur->w / 25;
	int target = 0; 
	
	for(int i = node->num; i>=num_in_row; i-=num_in_row)
	{
		target++;
	}
	
	int ywalk = target * 25;
	int xwalk = (25 * node->num) - ((num_in_row * target) * 25);
	int y, x;
	
	for(y = ywalk+1; y<ywalk+24; y++)
	{
		for(x = xwalk+1; x<xwalk+24; x++)
		{
			SetPixel32(tile, 24, 24, (x-xwalk+1), (y-ywalk+1), real_pixels[(y * texsur->w) + x]);
		}
	}		
	
	ASSERT(!SDL_UpdateTexture(tmp, nullptr, tile, 24 * 4), SDL_GetError());
	
	ASSERT(!SDL_SetRenderTarget(renderer, dest_tmp), SDL_GetError());
	SDL_Rect DestR;
	DestR.x = 0;
	DestR.y = 0;
	DestR.w = s_scale;
	DestR.h = s_scale;
	ASSERT(!SDL_RenderCopy(renderer, tmp, nullptr, &DestR), SDL_GetError());
		
	uint32_t buf[s_scale*s_scale];
	ASSERT(!SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, buf, s_scale*4), SDL_GetError());
	for(y = 0; y<s_scale; y++)
	{
		for(x = 0; x<s_scale; x++)
		{
			if(buf[(y * s_scale) + x] == 0xFFFFFFFF)
			{
				SetPixel32(framebuffer, screen->w, screen->h, node->x + x, node->y + y, node->color);
			}
		}
	}
	ASSERT(!SDL_SetRenderTarget(renderer, nullptr), SDL_GetError());
	SDL_DestroyTexture(tmp);
	SDL_DestroyTexture(dest_tmp);
}
	
int main(int argc, char* argv[]) 
{

	ASSERT(!SDL_Init(SDL_INIT_EVERYTHING), SDL_GetError());
	
	window = SDL_CreateWindow("FAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	ASSERT(window, SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	ASSERT(renderer, SDL_GetError());
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	
	screen = SDL_GetWindowSurface(window); 
	ASSERT(screen, SDL_GetError());
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0,0,0));
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	
	texsur = SDL_LoadCachedBMP(atlas_chars, 32768);
	ASSERT(texsur, SDL_GetError());
	texsur = SDL_ConvertSurfaceFormat(texsur, SDL_PIXELFORMAT_RGBA8888, 0);
	ASSERT(texsur, SDL_GetError());
	ASSERT(!SDL_LockSurface(texsur), SDL_GetError());
	
	framebuffer = new uint32_t[screen->w * screen->h];
	ASSERT(framebuffer, "Malloc failed");
	
	Background_Tx = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, screen->w, screen->h);
	ASSERT(Background_Tx, SDL_GetError());
	
	
	memset(atlas_chars, 0, 32768); //the texture memory becomes free stack for future.
	

	
	DrawCharacter(50, 50, 1, 0xFF0000FF);
	
	DrawCharacter(100, 50, 5, 0x0000FFFF);	
	
	TranslateCharacter(100, 0, 1, 0x000000FF);
	
		
		
	ScaleCharacter(1, 10.0f);
		
		
/* 	clock_t t = clock();
	t = clock() - t;
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC); */	
	
	
/* 	TranslateCharacter(10, 0, 1, 0x000000FF);
	
	TranslateCharacter(10, 0, 1, 0x000000FF);
	
	TranslateCharacter(10, 0, 1, 0x000000FF);
	
	
	TranslateCharacter(10, 0, 1, 0x000000FF);
	
	
	TranslateCharacter(10, 0, 1, 0x000000FF); */
	
	//TranslateCharacter(300, 100, 1, 0x00FF00FF);
		
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
		
		//vec2_t org = {320, 200};
		//vec3_t m = {200, 20, 50}; 
		
		//ASSERT(!SDL_UpdateTexture(atlas, nullptr, atlas_buffer, screen->w * sizeof(uint32_t)), SDL_GetError());
		//SDL_RenderClear(renderer);
		
		ASSERT(!SDL_UpdateTexture(Background_Tx, nullptr, framebuffer, screen->w * 4), SDL_GetError());
		SDL_RenderCopy(renderer, Background_Tx, nullptr, nullptr);
		
		//SDL_RenderCopyEx(renderer, atlas, &SrcR, &DestR, 0, nullptr, SDL_FLIP_NONE);
	    //Triangle(org, m);
		
		Circle(100, 100, 50, 0xFFFFFFFF);
		
		SDL_RenderPresent(renderer);
		
	}	
	delete framebuffer;
	SDL_FreeSurface(texsur);
	SDL_DestroyTexture(Background_Tx);
	//SDL_DestroyTexture(atlas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
