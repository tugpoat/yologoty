//=============================================================================
#include "CMap.h"
#include <iostream>
#include <sstream>

//=============================================================================
CMap CMap::MapControl;

//=============================================================================
CMap::CMap() {
	Surf_Tileset = NULL;
}

//=============================================================================
bool CMap::OnLoad(char* File) {
    TileList.clear();

    pugi::xml_document doc;
    std::cout << File << std::endl;
    if (!doc.load_file(File)) return false;

    pugi::xml_node map = doc.child("map");

    mWidth = map.attribute("width").as_int();
    mHeight = map.attribute("height").as_int();
    mTile_W = map.attribute("tilewidth").as_int();
    mTile_H = map.attribute("tileheight").as_int();

    std::cout << "Map Dimensions: " << mWidth << "x" << mHeight << std::endl;

    pugi::xml_node tileset = map.child("tileset");

    std::string TilesetFile = tileset.child("image").attribute("source").as_string();
    std::cout << TilesetFile << std::endl;

    if((Surf_Tileset = CSurface::OnLoad(TilesetFile.c_str())) == false) {
        printf("%s\n", "CArea::OnLoad() Failed to load tileset");
        return false;
    }

    pugi::xpath_node_set tiles = tileset.select_nodes("tile");
   // std::cout<< tileset.find_child_by_attribute("tile", "id", "391").child("property").name() << std::endl;
    for (pugi::xpath_node_set::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
    {
        pugi::xpath_node node = *it;
        std::cout << node.node().attribute("id").as_string() << "\n";
        std::cout << node.node().find_child_by_attribute("property", "name", "type").name() << std::endl;
    }

    std::string foreground = map.find_child_by_attribute("layer", "name", "foreground").child_value("data");

    //std::cout << foreground << std::endl;
    foreground.erase(0,1); //drop first newline

    std::size_t newlinepos(foreground.find_first_of("\n"));
    
    while (newlinepos != std::string::npos) {
        std::string line = foreground.substr(0, newlinepos);
        std::size_t cpos(line.find_first_of(","));
        while (cpos != std::string::npos) {
            CTile tempTile;
            tempTile.TileID = atoi(line.substr(0, cpos).c_str());

            pugi::xml_node tile_properties = tileset.find_child_by_attribute("tile", "id", line.substr(0, cpos).c_str()).child("properties");
            //std::cout << tile_properties.find_child_by_attribute("property", "name", "type").attribute("value").as_int() << std::endl;
            tempTile.TypeID = tile_properties.find_child_by_attribute("property", "name", "type").attribute("value").as_int();
            std::cout << tempTile.TileID << ":" << tempTile.TypeID << " ";

            TileList.push_back(tempTile);
            line.erase(0, cpos+1);
            cpos = line.find_first_of(",");

        }
        std::cout <<std::endl;
        foreground.erase(0, newlinepos+1);
        newlinepos = foreground.find_first_of("\n");
    }
    return true;
}

//-----------------------------------------------------------------------------
void CMap::OnRender(SDL_Surface* Surf_Display, int MapX, int MapY) {
	if(Surf_Tileset == NULL) return;

	int TilesetWidth  = Surf_Tileset->w / TILE_SIZE;
	int TilesetHeight = Surf_Tileset->h / TILE_SIZE;

	int ID = 0;

	for(int Y = 0;Y < MAP_HEIGHT;Y++) {
		for(int X = 0;X < MAP_WIDTH;X++) {
			if(TileList[ID].TypeID == TILE_TYPE_NONE) {
				ID++;
				continue;
			}

			int tX = MapX + (X * TILE_SIZE);
			int tY = MapY + (Y * TILE_SIZE);

			int TilesetX = (TileList[ID].TileID % TilesetWidth) * TILE_SIZE;
			int TilesetY = (TileList[ID].TileID / TilesetWidth) * TILE_SIZE;

			CSurface::OnDraw(Surf_Display, Surf_Tileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);

			ID++;
		}
	}
}

//=============================================================================
CTile* CMap::GetTile(int X, int Y) {
    unsigned int ID = 0;

    ID = X / TILE_SIZE;
    ID = ID + (MAP_WIDTH * (Y / TILE_SIZE));

    if(ID < 0 || ID >= TileList.size()) {
    	return NULL;
    }

    return &TileList[ID];
}

//=============================================================================
