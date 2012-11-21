#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"

using namespace std;

class Primitives
{
	public:
		//Constructor
		Primitives();
		Primitives(int def_lnWidth, Uint32 def_lnColor, SDL_Surface *surface);

		//Deconstruct
		~Primitives();

		float calcSlope(CoOrd pointA, CoOrd pointB);
	
		//allow caching! Returned int is the ID, or index of cache primitive
		int drawCircle(CoOrd center, int radius, bool cache);

		void drawCircle(CoOrd center, int radius, SDL_Surface *surface);

		void drawLine(CoOrd pointA, CoOrd pointB);

		void drawLine(CoOrd pointA, CoOrd pointB, SDL_Surface *surface);
			
		void drawLine(CoOrd pointA, CoOrd pointB, int width);

		void drawLine(CoOrd pointA, CoOrd pointB, int width, SDL_Surface *surface);

		void putPixel(int x, int y, Uint32 color, SDL_Surface *surface);


		SDL_Surface* Get_Cached(int id);
	private:
		SDL_Surface *default_surface;
		
		vector <Surface_Container *> surface_cache;

		int default_lineWidth;

		Uint32 default_lineColor;

		unsigned int default_fillColor;
};

