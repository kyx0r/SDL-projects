#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include "char_tex.h"

#define MAXCHARS 2000
#define PI 3.14159265

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
	unsigned char* pix_ptr; //x,y will hold locations for this ent
	unsigned char* ex_pix_ptr; //in case we scale this will point to next memory sequence (continuation of pix_ptr).
	uint16_t size;
	uint16_t proxy_size;
	uint32_t color;
	char num;
	struct char_ent* prev;
	struct char_ent* next;
} char_ent;

int current_char_index = 0;
char_ent characters[MAXCHARS] = {}; //should never be reached.

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

void ClearCharacter(int index, uint32_t clear_val)
{
	char_ent* node = &characters[index];
	for(unsigned char* i = &node->pix_ptr[0]; i<node->pix_ptr + (node->size * 2); i+=sizeof(uintptr_t))
	{
		uint16_t xpos = *((uint16_t*)i);
		uint16_t ypos = *((uint16_t*)i+sizeof(uint8_t));
		//printf("%d \n", xpos);
		//printf("%d \n", ypos);
		SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, clear_val);
	}
	if(node->ex_pix_ptr && node->proxy_size)
	{
		for(unsigned char* i = node->ex_pix_ptr; i<node->ex_pix_ptr + (node->proxy_size * 2); i+=sizeof(uintptr_t))
		{
			uint16_t xpos = *((uint16_t*)i);
			uint16_t ypos = *((uint16_t*)i+sizeof(uint8_t));
			SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, clear_val);
		}
	}
}
	
void ScaleCharacter(int index, float scale, double angle)
{
	//printf("Scalech \n");
	ASSERT(index<MAXCHARS, "MAXCHARS > 2000");
	char_ent* node = &characters[index];
	int s_scale = 24 * scale;
	
	if(*node->pix_ptr)
	{
		ClearCharacter(index, 0x000000FF);
	}
	
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
	struct init_xy
	{
		uint16_t x;
		uint16_t y;
	}init_xy;

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
	ASSERT(!SDL_RenderCopyEx(renderer, tmp, nullptr, &DestR, angle, nullptr, SDL_FLIP_NONE), SDL_GetError());
		
	uint32_t buf[s_scale*s_scale];
	ASSERT(!SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, buf, s_scale*4), SDL_GetError());
	
	unsigned char* ptr = &node->pix_ptr[0];
	char_ent* proxy = &characters[current_char_index-1];
	uint16_t tmp_size = node->size;
	if(proxy == node)
	{
		tmp_size = 0; //short circuit if we are leading the memory
	}
	node->size = 0;
	
	//idea: find the closest pixel to x and y edge and compute the offset
	init_xy.x = s_scale; //set to max value then find minimum
	init_xy.y = s_scale;
	for(y = 0; y<s_scale; y++)
	{
		for(x = 0; x<s_scale; x++)
		{
			if(buf[(y * s_scale) + x] == 0xFFFFFFFF)
			{
				if(x < init_xy.x)
				{
					init_xy.x = x;
				}	
				if(y < init_xy.y)
				{				
					init_xy.y = y;			
				}
			}
		}
	}
	
	for(y = 0; y<s_scale; y++)
	{
		for(x = 0; x<s_scale; x++)
		{
			if(buf[(y * s_scale) + x] == 0xFFFFFFFF)
			{
				*((uint16_t*)ptr+node->size) = (node->x - init_xy.x) + x; 
				node->size += sizeof(uint8_t);
				*((uint16_t*)ptr+node->size) = (node->y - init_xy.y) + y;
				node->size += sizeof(uint8_t);	
				if(tmp_size == node->size)
				{
					node->ex_pix_ptr = proxy->pix_ptr + (proxy->size * 2);
					ptr = node->ex_pix_ptr;
					node->size = 0;
					proxy->proxy_size = tmp_size;
					tmp_size = 0;
				}
				SetPixel32(framebuffer, screen->w, screen->h, (node->x - init_xy.x) + x, (node->y - init_xy.y) + y, node->color);
			}
		}
	}
	if(scale < 1.0f || !node->ex_pix_ptr)
	{
		proxy->proxy_size = 0;
		goto no_proxy;
	}	
	tmp_size = proxy->proxy_size;
	proxy->proxy_size = node->size;
	node->proxy_size = node->size;
	node->size = tmp_size;
	
	no_proxy:
	ASSERT(!SDL_SetRenderTarget(renderer, nullptr), SDL_GetError());
	SDL_DestroyTexture(tmp);
	SDL_DestroyTexture(dest_tmp);
}	
	
void DrawCharacter(int xpos, int ypos, char num, uint32_t color, float scale = 1.0f, double angle = 0.0f)
{
	if(num == 0)
	{
		return;
	}
	
	ASSERT(current_char_index<MAXCHARS, "MAXCHARS > 2000");
	char_ent* node = &characters[current_char_index];
	node->x = xpos;
	node->y = ypos;
	node->size = 0;
	node->color = color;
	node->num = num;
	if(node->prev)
	{
		node->pix_ptr = node->prev->pix_ptr + ((node->prev->proxy_size + node->prev->size) * 2);
	}
	else
	{
		node->pix_ptr = &atlas_chars[0]; //reuse the memory as the copy of it resides in surface anyway
	}
	
	node->next = &characters[++current_char_index];	
	node->next->prev = node;
	
	if(scale != 1.0f)
	{
		ScaleCharacter(current_char_index-1, scale, angle);
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
				*((uint16_t*)node->pix_ptr+node->size) = xpos+(x-xwalk+1); 
				node->size += sizeof(uint8_t);
				*((uint16_t*)node->pix_ptr+node->size) = ypos+(y-ywalk+1);
				node->size += sizeof(uint8_t);
				SetPixel32(framebuffer, screen->w, screen->h, xpos+(x-xwalk+1), ypos+(y-ywalk+1), color);
			}
		}
	}
}	

void TranslateCharacter(int x, int y, int index, uint32_t clear_val)
{
	//printf("here \n");
	char_ent* node = &characters[index];
	for(unsigned char* i = &node->pix_ptr[0]; i<node->pix_ptr + (node->size * 2); i+=sizeof(uintptr_t))
	{
		uint16_t xpos = *((uint16_t*)i);
		uint16_t ypos = *((uint16_t*)i+sizeof(uint8_t));
		*((uint16_t*)i) = xpos + x;
		*((uint16_t*)i+sizeof(uint8_t)) = ypos + y;
		SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, clear_val);
		SetPixel32(framebuffer, screen->w, screen->h, xpos+x, ypos+y, node->color);
	}
	if(node->ex_pix_ptr && node->proxy_size)
	{
		for(unsigned char* i = node->ex_pix_ptr; i<node->ex_pix_ptr + (node->proxy_size * 2); i+=sizeof(uintptr_t))
		{
			uint16_t xpos = *((uint16_t*)i);
			uint16_t ypos = *((uint16_t*)i+sizeof(uint8_t));
			*((uint16_t*)i) = xpos + x;
			*((uint16_t*)i+sizeof(uint8_t)) = ypos + y;
			SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, clear_val);
			SetPixel32(framebuffer, screen->w, screen->h, xpos+x, ypos+y, node->color);
		}
	}
}

void DrawString(char* str, int len, int x, int y, int displ, uint32_t color, float scale = 1.0f, double angle = 0.0f)
{
	displ = displ * scale;
	int _displ = displ * cos(angle*PI/180);
	for(int i = 0; i<len; i++)
	{
		char ch = str[i] - 32;
		if(i == 0)
		{
			DrawCharacter(x, y, ch, color, scale, angle);
		}
		else if (angle == 0.0f)
		{
			DrawCharacter(x+(_displ * i), y, ch, color, scale, angle);
		}
		else
		{
			int hyp = (_displ * i) / cos(angle*PI/180);
			if(!hyp)
			{
				hyp = displ * i;
			}
			DrawCharacter(x+(_displ * i), y+sin(angle*PI/180) * hyp, ch, color, scale, angle);
		}
		
	}
}

void UpdateCharacters()
{
	for(int i = 0; i<current_char_index; i++)
	{
		char_ent* node = &characters[i];
		for(unsigned char* i = &node->pix_ptr[0]; i<node->pix_ptr + (node->size * 2); i+=sizeof(uintptr_t))
		{
			uint16_t xpos = *((uint16_t*)i);
			uint16_t ypos = *((uint16_t*)i+sizeof(uint8_t));
			SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, node->color);
		}
		if(node->ex_pix_ptr && node->proxy_size)
		{
			for(unsigned char* i = node->ex_pix_ptr; i<node->ex_pix_ptr + (node->proxy_size * 2); i+=sizeof(uintptr_t))
			{
				uint16_t xpos = *((uint16_t*)i);
				uint16_t ypos = *((uint16_t*)i+sizeof(uint8_t));
				SetPixel32(framebuffer, screen->w, screen->h, xpos, ypos, node->color);
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
	
/* 	clock_t t = clock();
	t = clock() - t;
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC); */	
	
	DrawString("Hello", 5, 250, 250, 20, 0x0000FFFF, 2.0f, 340.0f);
	
	DrawString("Welcome", 7, 50, 50, 20, 0x00FFFFFF, 1.5f, 90.0f);
		
	DrawString("Awesome", 7, 500, 250, 20, 0xFF0000FF, 3.0f, 220.0f);
		
	DrawString("Project Complete.", 17, 25, 400, 15, 0x3D1752FF, 2.5f, 0.0f);
			
	while(true)
	{
		uint32_t start_clock = SDL_GetTicks();
			
		if(!EventLoop())
		{
			break;
		}
		FrameCap(start_clock, 60);
		
		static double rot = 0.0f;
		rot += 1.0f;
		ScaleCharacter(0,4.0f, rot); //for demo rotate the char in place
		
		UpdateCharacters();	
		ASSERT(!SDL_UpdateTexture(Background_Tx, nullptr, framebuffer, screen->w * 4), SDL_GetError());
		SDL_RenderCopy(renderer, Background_Tx, nullptr, nullptr);	
		SDL_RenderPresent(renderer);
	}	
	delete framebuffer;
	SDL_FreeSurface(texsur);
	SDL_DestroyTexture(Background_Tx);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
