//==============================================================================
// FPS Class
//==============================================================================
#ifndef _CFPS_H_
    #define _CFPS_H_

#include <SDL.h>

//==============================================================================
class CFPS {
	public:
		static CFPS FPSControl;

	private:
		unsigned int     OldTime;
		unsigned int     LastTime;

		float 	SpeedFactor;

		unsigned int		NumFrames;
		unsigned int     Frames;

	public:
		CFPS();

		void    OnLoop();

	public:
		int     GetFPS();

		float   GetSpeedFactor();
};

//==============================================================================
#endif
