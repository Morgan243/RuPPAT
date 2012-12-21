#ifndef COMMON_H
#define COMMON_H
#include "Descriptors.h"

class Common
{

	public:
		
		static void SetDimensions(unsigned int width, unsigned int height);
	
		static bool TestBounds(Entity_desc &testMe, bool invert);
		
		static bool TestBounds(Pixel_desc &testMe, bool invert);

		static bool ApplyDimming(Pixel_desc &pix_t);

	private:
		static unsigned int width, height;
};

#endif
