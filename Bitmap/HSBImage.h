#ifndef _HSB_IMAGE_H_
#define _HSB_IMAGE_H_

#include "stdafx.h"
#include "Bitmap.h"
#include <string>
#include <iostream>
#include <vector>
#include "ColorModel.h"
//#include "Header.h"
//#include "ColorModelConvert.cpp"
using namespace std;

template <typename T>
struct HSBColor
{
	typedef T value_type;
	value_type H;
	value_type S;
	value_type B;
};
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
	HSBImage(){}
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
					RGB2HSB<BYTE>(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue,	(T*)p);
					p++;
				}
			}
		}
	}
public :
	HSBColor<T> GetPixel(UINT32 x, UINT32 y) const
	{
		BYTE* p = (BYTE*)data + (y * height + x) * sizeof(T) * 3;
		return *(HSBColor<T>*)p;
	}
	UINT32 Width() const
	{
		return this->width;
	}
	UINT32 Height() const
	{
		return this->height;
	}
private:
	inline void RGB2HSB(T r, T g, T b,HSBColor<T>* pHSB)
	{
		int max = max(max(r, g), b);
		int min = min(min(r, g), b);

		pHSB->B = max / 255.0f;
		pHSB->S = max == 0 ? 0 : (max - min) / (float) max;

		pHSB->H = 0;
		/*if (max == r && g >= b)
		pHSB->H =(T)( (g - b) * 60f / (max - min) + 0);
		else if (max == r && g < b)
		pHSB->H = (T)( (g - b) * 60f / (max - min) + 360);
		else if (max == g)
		pHSB->H =(T)(  (b - r) * 60f / (max - min) + 120);
		else if (max == b)
		pHSB->H = (T)( (r - g) * 60f / (max - min) + 240);*/
	}
private:
	UINT32 width;
	UINT32 height;
	void* data;
};

#endif