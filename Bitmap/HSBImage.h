#ifndef _HSB_IMAGE_H_
#define _HSB_IMAGE_H_

#include "stdafx.h"
#include "Bitmap.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;


template <typename T>
struct HSBColor
{
	typedef T value_type;
	value_type H;
	value_type S;
	value_type B;
};
typedef HSBColor<float> HSBColorF;

template <typename T>
class HSBImage
{
	typedef T value_type;
	typedef T* iterator;
	typedef const T* const_iterator;
public:
	~HSBImage()
	{
		{
			if(data)
			{
				free(data);
				data = NULL;
			}
		}
	}
	HSBImage() {}
	HSBImage(Bitmap& bitmap)
	{
		LoadFromBitmap(bitmap);
	}
	void LoadFromBitmap(Bitmap& bitmap)
	{
		{
			this->width = bitmap.Width();
			this->height = bitmap.Height();
			data = malloc(this->width * this->height * sizeof(T) * 3);
			HSBColor<T>* p = (HSBColor<T>*)data;
			for(UINT32 y = 0; y < height; y++)
			{
				for(UINT32 x = 0; x < width; x++)
				{
					RGBQUAD rgb = bitmap.GetPixel(x, y);
					RGB2HSB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue,	p);
					p++;
				}
			}
		}
	}
public :
	HSBColor<T> GetPixel(UINT32 x, UINT32 y) const
	{
		BYTE* p = (BYTE*)data + (y * height + x) * sizeof(T) * 3;
		HSBColor<T>* pHSB =	(HSBColor<T>*)p;
		return *pHSB;
	}
	UINT32 Width() const
	{
		return this->width;
	}
	UINT32 Height() const
	{
		return this->height;
	}
	T GetAverageHue(int x, int y, int width, int height) const
	{
		HSBColor<T>* p =	(HSBColor<T>*)DataPtr(x, y);
		int left=this->width-width;
		double sum=0.0;
		for(int loopY=0; loopY <  height; loopY++)
		{
			for(int loopX=0; loopX <  width; loopX++)
			{
				sum+=p->H;
				p++;
			}
			p+=left;
		}
		return sum/(width*height);
	}
private:
	const BYTE * DataPtr(int x, int y) const
	{
		return (BYTE*)data + 3 * sizeof(T) * (y * Width() + x)  ;
	}
	inline void RGB2HSB(BYTE r, BYTE g, BYTE b, HSBColor<T>* pHSB)
	{
		int max = max(max(r, g), b);
		int min = min(min(r, g), b);

		pHSB->B = max / 255.0f;
		pHSB->S = max == 0 ? 0 :  (max - min) / (float) max;

		float h;

		if (max == r && g >= b)
			h =  (g - b) * 60.0 / (max - min) + 0;
		else if (max == r && g < b)
			h =  (g - b) * 60.0 / (max - min) + 360;
		else if (max == g)
			h =   (b - r) * 60.0 / (max - min) + 120;
		else if (max == b)
			h =  (r - g) * 60.0 / (max - min) + 240;
		pHSB->H = h;
	}
private:
	UINT32 width;
	UINT32 height;
	void* data;
};

#endif