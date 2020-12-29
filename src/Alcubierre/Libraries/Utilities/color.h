#pragma onces

class color
{
	
};

struct RGBA
{
public:
	double R, G, B, A;
};

namespace Colors
{
	
	RGBA To_Normalized(RGBA In_COLOR)
	{
		RGBA NORMALIZED_;
		NORMALIZED_.R = (((double)1 / (double)255) * In_COLOR.R);
		NORMALIZED_.G = (((double)1 / (double)255) * In_COLOR.G);
		NORMALIZED_.B = (((double)1 / (double)255) * In_COLOR.B);
		NORMALIZED_.A = (((double)1 / (double)255) * In_COLOR.A);
		return NORMALIZED_;
	}

	const RGBA RED = { 255,0,0,255 };
	const RGBA GREEN = { 0,25,0,255 };
	const RGBA BLUE = { 0,0,255,255 };
	const RGBA JAPANESE_INDIGO = { 39, 65, 86, 255 };

};