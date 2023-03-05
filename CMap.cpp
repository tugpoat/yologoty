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

    this->mWidth = map.attribute("width").as_int();
    this->mHeight = map.attribute("height").as_int();
    this->mTile_W = map.attribute("tilewidth").as_int();
    this->mTile_H = map.attribute("tileheight").as_int();

    std::cout << "Map Dimensions: " << mWidth << "x" << mHeight << std::endl;

    pugi::xml_node tileset = map.child("tileset");

    std::string TilesetFile = tileset.child("image").attribute("source").as_string();
    std::cout << "tileset: " << TilesetFile << std::endl;

    if((Surf_Tileset = CSurface::OnLoad(TilesetFile.c_str())) == nullptr) {
        printf("%s\n", "CArea::OnLoad() Failed to load tileset");
        return false;
    }

    pugi::xpath_node_set tiles = tileset.select_nodes("tile");
   // std::cout<< tileset.find_child_by_attribute("tile", "id", "391").child("property").name() << std::endl;
    for (pugi::xpath_node_set::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
    {
        pugi::xpath_node node = *it;
        std::cout << node.node().attribute("id").as_string();
        std::cout << node.node().find_child_by_attribute("property", "name", "type").name() << std::endl;
    }

    for (pugi::xml_node layer: map.children("layer")) {
        std::cout << "Processing layer " << layer.attribute("name").as_string() << std::endl;
        std::string layerData = layer.child_value("data");
        std::vector<CTile> layerTiles;

        std::erase(layerData, '\n');
        std::cout << layerData << std::endl;
        
        //std::cout << "dicks " << layerData << std::endl;

        std::size_t cpos(layerData.find_first_of(","));
        std::size_t npos(layerData.size());
        

        while (cpos <= npos) {
            std::cout << cpos << std::endl;
            CTile tempTile;
            std::string tileidstr = layerData.substr(0, cpos);
            std::cout << "tileidstr: " << tileidstr << std::endl;
            tempTile.TileID = stoi(tileidstr);

            pugi::xml_node tile_properties = tileset.find_child_by_attribute("tile", "id", tileidstr.c_str()).child("properties");
            //std::cout << "tiletype: " << tile_properties.find_child_by_attribute("property", "name", "type").attribute("value").as_int() << std::endl;
            tempTile.TypeID = tile_properties.find_child_by_attribute("property", "name", "type").attribute("value").as_int();
            std::cout << tempTile.TileID << ":" << tempTile.TypeID << " ";

            layerTiles.push_back(tempTile);
            layerData.erase(0, cpos+1);
            cpos = layerData.find_first_of(",");

        }
        mLayerData.push_back(layerTiles);
        std::cout <<std::endl;
    }

    return true;
}

//-----------------------------------------------------------------------------
void CMap::OnRender(SDL_Surface* Surf_Display, int MapX, int MapY) {
	if(Surf_Tileset == NULL) return;

	int TilesetWidth  = Surf_Tileset->w / TILE_SIZE;
	int TilesetHeight = Surf_Tileset->h / TILE_SIZE;

	
    for (auto&& layer: mLayerData){
        int ID = 0;
    	for(int Y = 0;Y < MAP_HEIGHT;Y++) {
    		for(int X = 0;X < MAP_WIDTH;X++) {
    			if(layer[ID].TypeID == TILE_TYPE_NONE) {
    				ID++;
    				continue;
    			}

    			int tX = MapX + (X * TILE_SIZE);
    			int tY = MapY + (Y * TILE_SIZE);

    			int TilesetX = (layer[ID].TileID % TilesetWidth) * TILE_SIZE;
    			int TilesetY = (layer[ID].TileID / TilesetWidth) * TILE_SIZE;

    			CSurface::OnDraw(Surf_Display, Surf_Tileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);

    			ID++;
    		}
    	}
    }
}

//=============================================================================
CTile* CMap::GetTile(int X, int Y) {
    unsigned int ID = 0;
    for (auto&& layer: mLayerData){
       
    }
    ID = X / TILE_SIZE;
    ID = ID + (MAP_WIDTH * (Y / TILE_SIZE));

    if(ID < 0 || ID >= mLayerData[0].size()) {
        std::cout<< "invalid tile" << std::endl;
    	return NULL;
    }

    return &mLayerData[0][ID];
}

//=============================================================================
