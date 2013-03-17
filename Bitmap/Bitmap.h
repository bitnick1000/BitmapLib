#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

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
	RGBQUAD GetPixel(int x, int y) const;
	const BYTE * DataPtr(int x, int y) const;
	UINT32 Width() const;
	UINT32 Height() const;
	UINT32 Spectrum() const;

	//algorithm
public:
	enum ORIGIN{TOP_LEFT,BOTTOM_LEFT};
	COORD IndexOf(Bitmap& subBitmap,ORIGIN origin,vector<COORD>& coords);
	COORD IndexOf(Bitmap& subBitmap,ORIGIN origin);
	INT32 CutOut(const RECT* rect);
private:
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