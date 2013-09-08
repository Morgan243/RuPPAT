// 			[Class] Render
//--------------------------------------------------------------------
//| 	Methods and surfaces of the primary screens/buffers. Everything
//|that ends up on the screen eventually must go through one of these
//|methods.
//--------------------------------------------------------------------

#ifndef RENDER_H
#define RENDER_H
#include "SDL2/SDL.h"
//#include "SDL2/SDL_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include "Descriptors.h"

class Render
{
	public:
		Render(int width,
			   	int height,
			   	int BPP,
			   	Uint32 flags);

		Render(int width,
			   	int height,
			   	int BPP,
			   	Uint32 flags,
				int game_width,
			   	int game_height);

		~Render();

		void setGameArea(const int w, const int h);

		void setMainScreen(const int color);

		void OnRender();

		void OnRender(const int x, const int y);
		
		Uint32 getPixel(const int x,
                        const int y,
                        const int screenID) const;

		void putPixel(const vector<Pixel_desc> pixels);

		void putPixel(const int x,
			   			const int y,
					   	Uint32 color,
					   	const int screenID);

		void putSprite(const int x,
			   			const int y,
					   	SDL_Surface* sprite);

		void putSprite(const vector<Surface_Container> sprites);

		void putSprite(const int x,
			   			const int y,
					   	SDL_Texture* sprite);

		void putSprite(const vector<Texture_Container> sprites);


		void applySurface(const int x,
			   				const int y,
						   	SDL_Surface* source);	
		
        void applyTextureToMain(const int x,
                                const int y,
                                SDL_Texture* source);

        void clearMainText();

        SDL_Window *mainWindow;
        SDL_Renderer *renderer; //sdl uses its magic to make the pretties come on screen
        SDL_Texture *mainTexture; //streaming texture that we give to renderer when bliting
        SDL_Texture *test_text;

		SDL_Surface *mainScreen,
					*pre_surface,
				   	*sprite_surface;
        std::vector<SDL_Texture*> low_composite;
        std::vector<SDL_Texture*> high_composite;

        std::vector<SDL_Surface*> bot_composite;
        std::vector<SDL_Surface*> top_composite;

        Uint32 main_format;

        int xOrigin = 0, yOrigin = 0;
	private:
        int comp_i = 0;
		int mainWidth, game_width,
			 mainHeight, game_height,
			 mainBPP;

		Uint32 mainFlags;
};
#endif
