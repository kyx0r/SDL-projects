#include "include/sprite.h"

namespace ether
{
	Sprite::Sprite( Uint32 color, int x, int y, int w, int h)
	{
		image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0); //32 = depth , other zeros are R masks
		SDL_FillRect(image, NULL, color);

		rect = image->clip_rect;
		//an SDL_Rect structure used to clip blits to the surface which can be set by SDL_SetClipRect() (read-only) 
				
		rect.x = x - origin_x;
		rect.y = y - origin_y;
	}	
	
	void Sprite::draw(SDL_Surface *dest)
	{
		SDL_BlitSurface( image, NULL, dest, &rect);
	}	
	
	
}