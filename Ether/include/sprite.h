#include <iostream>
//#include <SDL2/SDL.h>
#include "window.h"

namespace ether 
{
	class Sprite : public Screen
	{
		private:
			SDL_Surface *image;
			SDL_Rect rect;
			
			int origin_x=0, origin_y=0;
		public: 
			int w=48, h=64;
			Sprite( Uint32 color, int x, int y, int w, int h );
			void draw(SDL_Surface *dest);
		
		
		
	};
	
	class Sprite_Group
	{
		
	};
	
	
	
	
	
}