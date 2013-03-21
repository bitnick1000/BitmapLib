
#include "stdafx.h"
#include "Bitmap.h"
#include "GrayAlphaBitmap.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

const BYTE * GrayAlphaBitmap::DataPtr(int x, int y) const
{
    return (BYTE*)data + 2 * (y * Width() + x)  ;
}
int GrayAlphaBitmap::Width() const
{
    return this->width;
}
int GrayAlphaBitmap::Height() const
{
    return this->height;
}
GrayAlphaBitmap::~GrayAlphaBitmap()
{
    if(data)
    {
        free(data);
        data = NULL;
    }
}
GrayAlphaBitmap::GrayAlphaBitmap(Bitmap& bitmap)
{
    this->width = bitmap.Width();
    this->height = bitmap.Height();
    this->data = malloc(this->height * this->width * 2);
    AGCOLOR* pData = (AGCOLOR*)this->data;
    AGCOLOR* pEnd = pData + this->height * this->width ;
    RGBQUAD* pRGBA = (RGBQUAD*)bitmap.data;
    while( pData < pEnd )
    {
        pData->Gray = (pRGBA->rgbBlue + pRGBA->rgbGreen + pRGBA->rgbRed) / 3;
        pData->Alpha = pRGBA->rgbReserved;
        pData++;
        pRGBA++;
    }
}
int GAColorArrayCompareEx(const AGCOLOR * array1, const AGCOLOR* array2WithAlpha, UINT32 length, int colorErrorRange)
{
    int errorNumber = 0;
    const AGCOLOR* array1End = array1 + length;
    while( array1 < array1End)
    {
        if( (*array2WithAlpha).Alpha != 0 )
        {
            if(	 abs((*array1).Gray - (*array2WithAlpha).Gray) > colorErrorRange )
                errorNumber++;
        }
        array1++;
        array2WithAlpha++;
    }
    return errorNumber;
}
inline BOOL GrayAlphaBitmap::IsEqualEx(COORD coord, GrayAlphaBitmap& subBitmap, int colorErrorRange, int maxErrorNum)
{
    int errorNumber = 0;
    INT32 subY;
    const void* pSub;
    const void* p;
    for(subY = 0; subY < subBitmap.Height(); subY++)
    {
        p = DataPtr(coord.X, coord.Y + subY);
        pSub = subBitmap.DataPtr(0, subY);
        errorNumber += GAColorArrayCompareEx((AGCOLOR *)p, (AGCOLOR*) pSub, subBitmap.Width(), colorErrorRange);
        if(errorNumber > maxErrorNum)
            return FALSE;
    }
    return TRUE;
}
COORD GrayAlphaBitmap::IndexOfEx(GrayAlphaBitmap& subBitmap, int colorErrorRange, int maxErrorNum)
{
    COORD coord = {0};
    for( ; coord.Y < Height() - subBitmap.Height() + 1; coord.Y++)
    {
        for(coord.X = 0 ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
        {
            if(IsEqualEx(coord, subBitmap, colorErrorRange, maxErrorNum))
            {
                coord.Y = coord.Y - (subBitmap.Height() - 1);
                return coord;
            }
        }
    }
    COORD coordNotFound = { -1, -1};
    return coordNotFound;
}
