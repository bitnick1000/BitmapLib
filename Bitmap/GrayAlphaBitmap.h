#ifndef _GRAY_ALPHA_BITMAP_H_
#define _GRAY_ALPHA_BITMAP_H_

#include "stdafx.h"
#include "Bitmap.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
typedef struct
{
    BYTE Alpha;
    BYTE Gray;
} AGCOLOR;
class GrayAlphaBitmap
{
public:
	~GrayAlphaBitmap();
    GrayAlphaBitmap(Bitmap& bitmap);
    BOOL IsEqualEx(COORD coord, GrayAlphaBitmap& subBitmap, int colorErrorRange, int maxErrorNum);
    COORD IndexOfEx(GrayAlphaBitmap& subBitmap, int colorErrorRange, int maxErrorNum);

    //properties
public:

    int Width() const;
    int Height() const;
private:
    BOOL IsColorLike(AGCOLOR color1, AGCOLOR colorWithAlpha, int colorErrorRange);
    const BYTE * DataPtr(int x, int y) const;
private:
    void* data;
    int width;
    int height;
};

#endif