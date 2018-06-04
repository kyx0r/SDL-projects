#include <iostream>
#include <vector>
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
			void update(){};
			SDL_Surface *get_image() const {return image;}
			
			bool operator == (const Sprite &other) const ; //unfortunately we cannot compare 
		
	};
	
	class Sprite_Group
	{
		private: 
		std::vector <Sprite*> Arr_Spr; //Arr_Spr will be array to hold
		int Spr_Size;
		
		public:
		std::vector <Sprite*> get_Sprites();
		Sprite_Group Copy();
		void Add(Sprite *sprite);
		void Remove(Sprite S_obj);
		void update();
		void empty() {Arr_Spr.clear(); Spr_Size = sizeof(Arr_Spr); }
		void draw(SDL_Surface *dest);
		int size(){return Spr_Size;}
	};
	
	
	
	
	
}