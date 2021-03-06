#include "stdafx.h"
#include "Bitmap.h"
#include <fstream>
#include <vector>
//#include "pngstruct.h"
#include "png.h"
#include "zlib.h"

#include "pngstruct.h"
#include "pnginfo.h"

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
    y = this ->height - 1 - y;
    BYTE* pos = (BYTE*)data + Spectrum() * (y * Width() + x) ;
    ret = *(RGBQUAD*)(pos);
    if (Spectrum() == 3)
        ret.rgbReserved = 0xff;
    return ret;
}
void Bitmap::SetPixel(int x, int y, RGBQUAD color)
{
    if (Spectrum() == 3)
        color.rgbReserved = 0xff;
    y = this ->height - 1 - y;
    BYTE* pos = (BYTE*)data + Spectrum() * (y * Width() + x) ;
    *(RGBQUAD*)(pos)  = color;
}
const BYTE * Bitmap::DataPtr(int x, int y) const
{
    return (BYTE*)data + Spectrum() * (y * Width() + x)  ;
}
INT32 Bitmap::Width() const
{
    return this->width;
}
INT32 Bitmap::Height() const
{
    return this->height;
}
INT32 Bitmap::Spectrum() const
{
    return this->spectrum;
}
void Bitmap::ReadFromFile(TCHAR* filename)
{
    int len   = lstrlen(filename);
    TCHAR ch  = filename[len - 1];
    if(ch == _T('g'))
        ReadFromPngFile(filename);
    else
        ReadFromBmpFile(filename);
}
void Bitmap::ReadFromBmpFile(TCHAR* filename)
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
    this->spectrum = bmpInfo.bmiHeader.biBitCount / 8;
    int readOffset = 4 - spectrum * Width() & 0x3;
    if( this->spectrum == 3)
    {
        this->spectrum = 4;
        int  dataSize = this->width * this->height * 4 ;
        data = malloc(dataSize);
        file.seekg(bmpFileHeader.bfOffBits);
        char* p = (char*)this->data;
        for(int y = 0; y < this->height; y++)
        {
            for(int x = 0; x < this->width; x++)
            {
                file.read(p, 3);
                p[3] = 0xff;
                p += 4;
            }
            file.seekg(readOffset, ios::cur);
        }
    }
    else if( this->spectrum == 4 )
    {
        int dataSize = this->width * this->height * this->spectrum ;
        data = malloc(dataSize);
        file.seekg(bmpFileHeader.bfOffBits);
        file.read((char*)data, dataSize);
    }
    else
        ASSERT(FALSE);
    file.close();
}
typedef struct
{
    BYTE* data;
    int size;
    int offset;
} ImageSource;
void PngDataReadFromBuffer(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
    ImageSource* isource = (ImageSource*)png_ptr->io_ptr;
    if(isource->offset + byteCountToRead <= isource->size)
    {
        memcpy(outBytes, isource->data + isource->offset, byteCountToRead);
        isource->offset += byteCountToRead;
    }
    else
        png_error(png_ptr, "pngReaderCallback failed");
}
void Bitmap::ReadFromPngFile(TCHAR* filename)
{
    fstream file;
    file.open(filename, ios::in | ios::binary);
    if(!file)
        return ;
    file.seekg(0, ios::end);
    int filelen = file.tellg();
    file.seekg(0, ios::beg);
    BYTE* buffer = (BYTE*)malloc(filelen);
    file.read((char*)buffer, filelen);

    byte pngSignature [8];
    memcpy(&pngSignature, buffer, 8);

    if(!png_check_sig(pngSignature, 8))
        return ;

    png_structp png_ptr = NULL;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(png_ptr == NULL)
        return ;

    png_infop info_ptr = NULL;
    info_ptr = png_create_info_struct(png_ptr);

    if(info_ptr == NULL)
    {
        // libpng must free file info struct memory before we bail
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return ;
    }
    ImageSource imgsource;
    imgsource.data = buffer + 8;
    imgsource.size = filelen - 8;
    imgsource.offset = 0;
    png_set_read_fn(png_ptr, &imgsource, PngDataReadFromBuffer);

    //tell libpng we already read the signature
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = 0;
    png_uint_32 height = 0;
    int bitDepth = 0;
    int colorType = -1;
    png_uint_32 retval = png_get_IHDR(png_ptr, info_ptr,
                                      &width,
                                      &height,
                                      &bitDepth,
                                      &colorType,
                                      NULL, NULL, NULL);

    if(retval != 1)
        return ;	// add error handling and cleanup

    if(colorType != PNG_COLOR_TYPE_RGB_ALPHA)
        ASSERT(FALSE);

    const png_uint_32 bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
    byte* rowData = new byte[bytesPerRow];


    this->width = width;
    this->height = height;
    this->spectrum = 4;
    this->data = malloc(width * height * spectrum);
    // read single row at a time
    for(UINT32 rowIdx = 0; rowIdx < height; ++rowIdx)
    {
        png_read_row(png_ptr, (png_bytep)rowData, NULL);

        const UINT32 rowOffset = rowIdx * width;

        UINT32 byteIndex = 0;
        for(UINT32 colIdx = 0; colIdx < width; ++colIdx)
        {
            const byte red   = rowData[byteIndex++];
            const byte green = rowData[byteIndex++];
            const byte blue  = rowData[byteIndex++];
            const byte alpha = rowData[byteIndex++];

            RGBQUAD color;
            color.rgbRed = red;
            color.rgbGreen = green;
            color.rgbBlue = blue;
            color.rgbReserved = alpha;
            //memcpy(this->data,&color,sizeof(RGBQUAD));
            //this->data++;
            //const UINT32 targetPixelIndex = rowOffset + colIdx;
            SetPixel(colIdx, rowIdx, color);
        }
        ASSERT(byteIndex == bytesPerRow);
    }

    delete [] rowData;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
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

    //�������
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
    for( ; array1 < array1End ; array1++, array2++)
    {
        if(*array1 != *array2)
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
int ColorArrayCompareEx(RGBQUAD* array1, const RGBQUAD* array2WithAlpha, UINT32 length)
{
    const RGBQUAD* array1End = array1 + length;
    while( array1 < array1End)
    {
        if(   (*array2WithAlpha).rgbReserved != 0 )
        {
            if(	(*array1).rgbRed != (*array2WithAlpha).rgbRed || (*array1).rgbGreen != (*array2WithAlpha).rgbGreen || (*array1).rgbBlue != (*array2WithAlpha).rgbBlue)
                return 250;
        }
        array1++;
        array2WithAlpha++;
    }
    return 0;
}
BOOL Bitmap::IsEqual(COORD coord, Bitmap& subBitmap)
{
    INT32 subY;
    const void* pSub;
    const void* p;
    int spectrum = Spectrum();
    if(subBitmap.Spectrum() == spectrum && spectrum == 4)
    {
        for(subY = 0; subY < subBitmap.Height(); subY++)
        {
            p = DataPtr(coord.X, coord.Y + subY);
            pSub = subBitmap.DataPtr(0, subY);

            if(ArrayCompare((RGBQUAD*)p, (RGBQUAD*)pSub, subBitmap.Width()))
                return FALSE;
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    return TRUE;
}
COORD Bitmap::IndexOf(Bitmap& subBitmap, vector<COORD>& coords)
{
    COORD coord = {0};
    for(vector<COORD>::iterator iter = coords.begin() ; iter != coords.end() ; iter++ )
    {
        coord = *iter;
        //origin top left to origin bottom left
        CoordConvert(&coord);
        if(IsEqual(coord, subBitmap))
        {
            return *iter;
        }
    }
    return IndexOf(subBitmap);
}
COORD Bitmap::IndexOf(Bitmap& subBitmap)
{
    COORD coord = {0};
    for( ; coord.Y < Height() - subBitmap.Height() + 1; coord.Y++)
    {
        for(coord.X = 0 ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
        {
            if(IsEqual(coord, subBitmap))
            {
                //origin bottom left to origin top left
                CoordConvert(&coord);
                coord.Y = coord.Y - subBitmap.Height() + 1;
                return coord;
            }
        }
    }
    COORD coordNotFound = { -1, -1};
    return coordNotFound;
}
BOOL Bitmap::IsEqualEx(COORD coord, Bitmap& subBitmap)
{
    INT32 subY;
    const void* pSub;
    const void* p;
    int spectrum = Spectrum();
    if(subBitmap.Spectrum() == spectrum  && spectrum == 4)
    {
        for(subY = 0; subY < subBitmap.Height(); subY++)
        {
            p = DataPtr(coord.X, coord.Y + subY);
            pSub = subBitmap.DataPtr(0, subY);

            if(ColorArrayCompareEx((RGBQUAD*)p, (RGBQUAD*)pSub, subBitmap.Width()))
                return FALSE;
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    return TRUE;
}
COORD Bitmap::IndexOfEx(Bitmap& subBitmap)
{
    COORD coord = {0};
    for( ; coord.Y < Height() - subBitmap.Height() + 1; coord.Y++)
    {
        for(coord.X = 0 ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
        {
            if(IsEqualEx(coord, subBitmap))
            {
                //origin bottom left to origin top left
                CoordConvert(&coord);
                coord.Y = coord.Y - (subBitmap.Height() - 1);
                return coord;
            }
        }
    }
    COORD coordNotFound = { -1, -1};
    return coordNotFound;
}
void Bitmap::CoordConvert(COORD* coord)
{
    coord->Y = this->height - 1 - coord->Y;
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
    tri.rgbReserved = 0Xff;
    return tri == quad;
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