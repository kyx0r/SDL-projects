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
	
	bool Sprite::operator == (const Sprite &other) const
	{return (image == other.get_image());}       //array or vector to the object so create our own operator. 
	
	void Sprite_Group::update()  //overwrite/overload this func in Sprite class. 
	{
		if (!Arr_Spr.empty())
		{
			for (int i=0; i<Spr_Size; i++)
			{
				Arr_Spr[i]->update();
			}
		}
	}	
	
	void Sprite_Group::draw(SDL_Surface *dest)
	{
		if (!Arr_Spr.empty())
		{
			for (int i=0; i<Spr_Size; i++)
			{
				Arr_Spr[i]->draw(dest);
			}
		}
	}
	
	void Sprite_Group::Add(Sprite *sprite)
	{
		Arr_Spr.push_back(sprite); //add to the end of vector 
		Spr_Size = Arr_Spr.size();
		std::cout << "Size of Sprites: ";
		std::cout << Spr_Size << std::endl;
	}
	
	void Sprite_Group::Remove(Sprite S_obj)
	{
		for (int i=0; i<Spr_Size; i++)
		{
			if (*Arr_Spr[i] == S_obj)
			{
				Arr_Spr.erase(Arr_Spr.begin()+i); //Access the currect element and delete. 
				Spr_Size = sizeof(Arr_Spr);
			}
		}
	}
	
	std::vector<ether::Sprite*> Sprite_Group::get_Sprites()
	{
		std::cout << "Sprites:" << std::endl;
		return Arr_Spr;
	}
	
	Sprite_Group Sprite_Group::Copy()
	{
		Sprite_Group new_group;
		for (int i=0; i<Spr_Size; i++)
		{
			new_group.Add(Arr_Spr[i]);
		}
			
		return new_group; 
	}	
	
}