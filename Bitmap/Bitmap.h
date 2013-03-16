#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <string>
#include "stdafx.h"
using namespace std;
class Bitmap
{
public:
	~Bitmap();
	Bitmap(TCHAR* filename);
	Bitmap(CWnd* pWnd, int x, int y, int width, int height);
public:
	void Save(wstring& filename);

	//properties
public:
	RGBQUAD GetPixel(int x, int y) const
	{
		RGBQUAD ret;
		BYTE* pos = (BYTE*)data + Spectrum() * (y * Width() + x) + y * this->offset;
		ret=*(RGBQUAD*)(pos);
		if (Spectrum()==3)
			ret.rgbReserved=0xff;
		return ret;
	}
	const BYTE * DataPtr(int x, int y) const
	{
		return (BYTE*)data + Spectrum() * (y * Width() + x) + y * this->offset ;
	}
	UINT32 Width() const
	{
		return this->width;
	}
	UINT32 Height() const
	{
		return this->height;
	}
	UINT32 Spectrum() const
	{
		return this->spectrum;
	}

	//algorithm
public:
	INT32 IndexOf(Bitmap& subBitmap, OUT RECT* subRect);
	INT32 CutOut(const RECT* rect);
private:
	//enum COLOR {ALPHA /*= 3, RED = 2, GREEN = 1, BLUE = 0};
	//int colormemcmp(const voi*/d* src1, const void* src2, COLOR color, UINT32 size);
	BOOL IsEqual(COORD coord, Bitmap& subBitmap);
private:
	void ReadFromFile(TCHAR* filename);
	void ReadFromWnd(CWnd* pWnd, int x, int y, int width, int height);
private:
	int width;
	int height;
	int spectrum;
	void* data;
	int offset;
};
BOOL operator == (const RGBQUAD& left, const RGBQUAD& right);
#endif