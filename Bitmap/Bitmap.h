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
        return *(RGBQUAD*)((BYTE*)m_bitmap.bmBits + Spectrum() * (y * Width() + x) + y * m_offset) ;
    }
    const BYTE * DataPtr(int x, int y) const
    {
        return (BYTE*)m_bitmap.bmBits + Spectrum() * (y * Width() + x) + y * m_offset ;
    }
    UINT32 Width() const
    {
        return m_bitmap.bmWidth;
    }
    UINT32 Height() const
    {
        return m_bitmap.bmHeight;
    }
    UINT32 Spectrum() const
    {
        return m_bitmap.bmBitsPixel / 8;
    }

    //algorithm
public:
    INT32 IndexOf(Bitmap& subBitmap, OUT RECT* subRect);
    INT32 CutOut(const RECT* rect);
private:
    enum COLOR {ALPHA = 3, RED = 2, GREEN = 1, BLUE = 0};
    int colormemcmp(const void* src1, const void* src2, COLOR color, UINT32 size);
    BOOL IsEqual(COORD coord, Bitmap& subBitmap);
private:
    void ReadFromFile(TCHAR* filename);
    void ReadFromWnd(CWnd* pWnd, int x, int y, int width, int height);
    void FillBitmapInfo(BITMAPINFO* pBmpInfo);
    void FillBitmapFileHeader(BITMAPFILEHEADER* pBmpFileHeader);
private:
    BITMAP m_bitmap;
    BITMAPINFO m_bmpInfo;
    int m_offset;//for each row
};


#endif