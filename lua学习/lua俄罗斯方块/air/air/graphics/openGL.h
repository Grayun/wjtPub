#ifndef openGL_h_
#define openGL_h_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl\glu.h>

namespace air
{
	struct color
	{
		unsigned char r, g, b, a;
		color()
		{
			r = g = b = a = 0;
		}
		color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}
	};
}

#endif
