//==============================================================================
#ifndef _CMAP_H_
    #define _CMAP_H_

#include <SDL.h>
#include <vector>
#include <pugixml.hpp>

#include "CTile.h"
#include "CSurface.h"

//==============================================================================
class CMap {
	public:
		static CMap 			MapControl;
		SDL_Surface*			Surf_Tileset;

	private:
		unsigned int			mWidth, mHeight;
		unsigned int			mTile_W, mTile_H;
		std::vector<CTile>      TileList;

	public:
		CMap();

	public:
		bool OnLoad(char* File);

		void OnRender(SDL_Surface* Surf_Display, int MapX, int MapY);

    public:
        CTile*	GetTile(int X, int Y);
        int getWidth() { return mWidth;};
        int getHeight() { return mHeight;};
};

//==============================================================================

#endif
