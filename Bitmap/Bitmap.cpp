#include "stdafx.h"
#include "Bitmap.h"
#include <fstream>
#include <vector>
using namespace std;
Bitmap::~Bitmap()
{
	if(data)
	{
		free(data);
		data = NULL;
	}
}
Bitmap::Bitmap(TCHAR* filename)
{
	ReadFromFile(filename);
}
Bitmap::Bitmap(CWnd* pWnd, int x, int y, int width, int height)
{
	ReadFromWnd(pWnd, x, y, width, height);
}

//properties
RGBQUAD Bitmap::GetPixel(int x, int y) const
{
	RGBQUAD ret;
	BYTE* pos = (BYTE*)data + Spectrum() * (y * Width() + x) + y * this->offset;
	ret = *(RGBQUAD*)(pos);
	if (Spectrum() == 3)
		ret.rgbReserved = 0xff;
	return ret;
}
const BYTE * Bitmap::DataPtr(int x, int y) const
{
	return (BYTE*)data + Spectrum() * (y * Width() + x) + y * this->offset ;
}
UINT32 Bitmap::Width() const
{
	return this->width;
}
UINT32 Bitmap::Height() const
{
	return this->height;
}
UINT32 Bitmap::Spectrum() const
{
	return this->spectrum;
}

void Bitmap::ReadFromFile(TCHAR* filename)
{
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFO bmpInfo;
	fstream file;
	file.open(filename, ios::in | ios::binary);
	if(!file)
		return ;
	file.read((char*)&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	file.read((char*)&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER));

	this->width = bmpInfo.bmiHeader.biWidth;
	this->height = bmpInfo.bmiHeader.biHeight;
	this->spectrum =      bmpInfo.bmiHeader.biBitCount / 8;
	this->offset = 4 - Spectrum() * Width() & 0x3;

	int dataSize = this->width * this->height * this->spectrum + offset * this->height;
	data = malloc(dataSize);
	file.seekg(bmpFileHeader.bfOffBits);
	file.read((char*)data, dataSize);

	file.close();
}
void Bitmap::ReadFromWnd(CWnd* pWnd, int x, int y, int width, int height)
{
	CDC* pDeskDC =  pWnd->GetDC();
	CDC  memDC;
	memDC.CreateCompatibleDC(pDeskDC);
	CBitmap bmp;
	BITMAP bitmap;
	bmp.CreateCompatibleBitmap(pDeskDC, width, height);
	memDC.SelectObject(&bmp);

	bmp.GetBitmap(&bitmap);
	memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, pDeskDC, x, y, SRCCOPY);

	//Ìí¼ÓÊó±ê
	/*CPoint point;
	GetCursorPos(&point);
	HICON hicon = (HICON)GetCursor();
	memDC.DrawIcon(point,hicon);  */

	DWORD size = bitmap.bmWidthBytes * bitmap.bmHeight;
	bitmap.bmBits = (LPVOID*)malloc(size);
	if(bitmap.bmBits == NULL)
		::MessageBox(NULL, _T("GlobalAllocPtr Failed\n"), _T("SDF"), NULL);

	BITMAPINFO bitmapInfo;
	BITMAPINFO* pBitmapInfo = &bitmapInfo;
	pBitmapInfo->bmiHeader.biSize						= 40;
	pBitmapInfo->bmiHeader.biWidth					= bitmap.bmWidth;
	pBitmapInfo->bmiHeader.biHeight					= bitmap.bmHeight;
	pBitmapInfo->bmiHeader.biPlanes					= bitmap.bmPlanes;
	pBitmapInfo->bmiHeader.biBitCount				= bitmap.bmBitsPixel;
	pBitmapInfo->bmiHeader.biCompression			= 0;
	pBitmapInfo->bmiHeader.biSizeImage				= bitmap.bmWidthBytes * bitmap.bmHeight;
	pBitmapInfo->bmiHeader.biXPelsPerMeter		= 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter		= 0;
	pBitmapInfo->bmiHeader.biClrUsed					= 0;
	pBitmapInfo->bmiHeader.biClrImportant			= 0;

	GetDIBits(memDC.m_hDC, bmp, 0, bitmap.bmHeight, bitmap.bmBits, pBitmapInfo, DIB_RGB_COLORS);

	this->width = bitmap.bmWidth;
	this->height = bitmap.bmHeight;
	this->spectrum = bitmap.bmBitsPixel / 8;
	this->offset = 4 - Spectrum() * Width() & 0x3;
	data = bitmap.bmBits;

}
void Bitmap::Save(wstring& filename)
{
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPFILEHEADER* pBitmapFileHeader = &bitmapFileHeader;
	pBitmapFileHeader->bfType				= MAKEWORD('B', 'M');
	pBitmapFileHeader->bfSize				= 54 +  this->height * this->width * this->spectrum;
	pBitmapFileHeader->bfReserved1		= 0;
	pBitmapFileHeader->bfReserved2		= 0;
	pBitmapFileHeader->bfOffBits			= 54;
	BITMAPINFO bitmapInfo;
	BITMAPINFO* pBitmapInfo = &bitmapInfo;
	pBitmapInfo->bmiHeader.biSize						= 40;
	pBitmapInfo->bmiHeader.biWidth					= this->width;
	pBitmapInfo->bmiHeader.biHeight					= this->height;
	pBitmapInfo->bmiHeader.biPlanes					= 0;//bitmap.bmPlanes;
	pBitmapInfo->bmiHeader.biBitCount				= this->spectrum * 8; //bitmap.bmBitsPixel;
	pBitmapInfo->bmiHeader.biCompression			= 0;
	pBitmapInfo->bmiHeader.biSizeImage				= this->height * this->width * this->spectrum; // bitmap.bmWidthBytes * bitmap.bmHeight;
	pBitmapInfo->bmiHeader.biXPelsPerMeter		= 0;
	pBitmapInfo->bmiHeader.biYPelsPerMeter		= 0;
	pBitmapInfo->bmiHeader.biClrUsed					= 0;
	pBitmapInfo->bmiHeader.biClrImportant			= 0;

	fstream file(filename, ios::out | ios::binary);
	file.write((char*)pBitmapFileHeader, sizeof(BITMAPFILEHEADER));
	file.write((char*)pBitmapInfo, pBitmapInfo->bmiHeader.biSize);
	file.write((char*)data, this->height * this->width * this->spectrum);
	file.close();
}
template<typename T>
int ArrayCompare(const T* array1, const T* array2, UINT32 length)
{
	const T* array1End = array1 + length;
	for( ; array1 < array1End ; array1++)
	{
		if(*array1++ != *array2++)
			return 250;
	}
	return 0;
}
template<typename T1, typename T2>
int ColorArrayCompare(const T1* array1, const T2* array2, UINT32 length)
{
	const T1* array1End = array1 + length;
	while( array1 < array1End)
	{
		if(*array1++ != *array2++)
			return 250;
	}
	return 0;
}
BOOL Bitmap::IsEqual(COORD coord, Bitmap& subBitmap)
{
	UINT32 subY;
	const void* pSub;
	const void* p;
	int spectrum = Spectrum();
	for(subY = 0; subY < subBitmap.Height(); subY++)
	{
		p = DataPtr(coord.X, coord.Y + subY);
		pSub = subBitmap.DataPtr(0, subY);

		if(subBitmap.Spectrum() == spectrum)
		{
			if(spectrum == 4)
			{
				if(ArrayCompare((RGBQUAD*)p, (RGBQUAD*)pSub, subBitmap.Width()))
					return FALSE;
			}
			else if(spectrum == 3)
			{
				if(memcmp(p, pSub, subBitmap.Spectrum()* subBitmap.Width()))
					return FALSE;
			}
			else
				ASSERT(FALSE);
		}
		else
		{
			if(spectrum == 3 && subBitmap.Spectrum() == 4)
			{
				if(ColorArrayCompare((RGBTRIPLE*)p, (RGBQUAD*)pSub, subBitmap.Width()))
					return FALSE;
			}
			else if(spectrum == 4 && subBitmap.Spectrum() == 3)
			{
				if(ColorArrayCompare((RGBQUAD*)p, (RGBTRIPLE*)pSub, subBitmap.Width()))
					return FALSE;
			}
			else
				ASSERT(FALSE);
		}
	}
	return TRUE;
}
COORD Bitmap::IndexOf(Bitmap& subBitmap,ORIGIN origin,vector<COORD>& coords)
{
	COORD coord = {0};
	for(vector<COORD>::iterator iter=coords.begin() ; iter!=coords.end() ;iter++ )
	{
		if(IsEqual(*iter, subBitmap))
		{
			if (origin==BOTTOM_LEFT)
				return *iter;
			else if(origin==TOP_LEFT)
			{
				coord=*iter;
				coord.Y=Height()-coord.Y;
				return coord;
			}
		}
	}
	for( ; coord.Y < Height() - subBitmap.Height() + 1; coord.Y++)
	{
		for(coord.X = 0 ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
		{
			if(IsEqual(coord, subBitmap))
			{
				if (origin==BOTTOM_LEFT)
					return coord;
				else if(origin==TOP_LEFT)
				{
					coord.Y=Height()-coord.Y;
					return coord;
				}
			}
		}
	}
	COORD coordNotFound = { -1, -1};
	return coordNotFound;
}
COORD Bitmap::IndexOf(Bitmap& subBitmap,ORIGIN origin)
{
	COORD coord = {0};
	for( ; coord.Y < Height() - subBitmap.Height() + 1; coord.Y++)
	{
		for(coord.X = 0 ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
		{
			if(IsEqual(coord, subBitmap))
			{
				if (origin==BOTTOM_LEFT)
					return coord;
				else if(origin==TOP_LEFT)
				{
					coord.Y=Height()-coord.Y;
					return coord;
				}
			}
		}
	}
	COORD coordNotFound = { -1, -1};
	return coordNotFound;
}
INT32 Bitmap::CutOut(const RECT* rect)
{
	int height = rect->right - rect->left;
	int width = rect->bottom - rect->top;
	ASSERT(height < this->height);
	ASSERT(width < this->width);
	this->width = width;
	this->height = height;
	LPVOID  newBmBits = malloc(this->width * this->height * this->spectrum);
	BYTE* writer = (BYTE*)newBmBits;
	int y;
	for(y = 0; y < height; y++)
	{
		const BYTE* p = DataPtr(rect->left, rect->bottom + y);
		memcpy(writer, p, width * Spectrum());
	}
	return 1;
}
BOOL operator == (const RGBQUAD& left, const RGBQUAD& right)
{
	return *(int*)&left == *(int*)&right;
}
BOOL operator != (const RGBQUAD& left, const RGBQUAD& right)
{
	return !( left == right ) ;
}
BOOL operator == (const RGBTRIPLE triple, const RGBQUAD quad)
{
	RGBQUAD tri =   *(RGBQUAD*) &triple;
	tri.rgbReserved=0Xff;
	return tri==quad;
}
BOOL operator != (const RGBTRIPLE triple, const RGBQUAD quad)
{
	return !(triple == quad);
}
BOOL operator == (const RGBQUAD& quad, const RGBTRIPLE& triple)
{
	return triple == quad;
}
BOOL operator != (const RGBQUAD& quad, const RGBTRIPLE& triple)
{
	return !(quad == triple);
}