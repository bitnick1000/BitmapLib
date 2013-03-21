#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "stdafx.h"

#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Bitmap
{
public:
	~Bitmap();
	Bitmap(TCHAR* filename);
	Bitmap(CWnd* pWnd, int x, int y, int width, int height);
public:
	void Save(wstring& filename);

	friend class GrayAlphaBitmap;

	//properties
public:
	RGBQUAD GetPixel(int x, int y) const;
	INT32 Width() const;
	INT32 Height() const;
	INT32 Spectrum() const;	
	void SetPixel(int x, int y,RGBQUAD color) ;
private:
	const BYTE * DataPtr(int x, int y) const;

	//algorithm
public:
	COORD IndexOf(Bitmap& subBitmap,vector<COORD>& coords);
	COORD IndexOf(Bitmap& subBitmap);
	COORD IndexOfEx(Bitmap& subBitmap);
	INT32 CutOut(const RECT* rect);
private:
	BOOL IsEqual(COORD coord, Bitmap& subBitmap);
	BOOL IsEqualEx(COORD coord, Bitmap& subBitmap);
	void CoordConvert(COORD* coord);
private:
	void ReadFromFile(TCHAR* filename);
	void ReadFromBmpFile(TCHAR* filename);
	void ReadFromPngFile(TCHAR* filename);
	void ReadFromWnd(CWnd* pWnd, int x, int y, int width, int height);
private:
	int width;
	int height;
	int spectrum;
	void* data;
};
BOOL operator == (const RGBQUAD& left, const RGBQUAD& right);
BOOL operator != (const RGBQUAD& left, const RGBQUAD& right);
#endif