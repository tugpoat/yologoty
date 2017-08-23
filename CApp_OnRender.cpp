//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnRender() {
	SDL_Rect Rect;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = WWIDTH;
	Rect.h = WHEIGHT;

	SDL_FillRect(Surf_Display, &Rect, 0);

	CMap::MapControl.OnRender(Surf_Display, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

    //--------------------------------------------------------------------------
    // Entities
    //--------------------------------------------------------------------------
    for(unsigned int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnRender(Surf_Display);
    }

	SDL_Flip(Surf_Display);
}

//==============================================================================
