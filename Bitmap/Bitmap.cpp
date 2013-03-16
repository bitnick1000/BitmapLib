#include "stdafx.h"
#include "Bitmap.h"
#include <fstream>
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

    //CFile file;
    /*FILE* fp = _tfopen(filename,_T("r"));
    fread(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);*/
    //LONG* pWidth=&m_bmpInfo.bmiHeader.biWidth;
    //if(file.Open(filename,CFile::modeReadWrite | CFile::typeBinary))
    //{
    //	file.Read(&bmpFileHeader,sizeof(BITMAPFILEHEADER));
    //	file.Read(&m_bmpInfo,4);
    //	file.Read(pWidth,m_bmpInfo.bmiHeader.biSize-4);
    //	m_bitmap.bmBits= malloc(m_bmpInfo.bmiHeader.biSizeImage);
    //	int readed = file.Read(m_bitmap.bmBits,m_bmpInfo.bmiHeader.biSizeImage);
    //	//int readed = file.Read(&m_bitmap.bmBits,bmpFileHeader.bfSize);
    //}
    //file.Close();

    /*HANDLE hLargeFile = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,
    NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

    HANDLE hFileMap = CreateFileMapping(hLargeFile,NULL,PAGE_READONLY,0,0,NULL);

    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    DWORD dwGran = SysInfo.dwAllocationGranularity;
    DWORD dwFileSizeHigh;
    __int64 qwFileSize = GetFileSize(hLargeFile, &dwFileSizeHigh);
    qwFileSize |= (((_int64)dwFileSizeHigh) << 32);

    CloseHandle(hLargeFile);

    DWORD dwBlockBytes = dwGran * 1000;
    TCHAR *lpbMapAddress = (TCHAR *)MapViewOfFile(hFileMap,FILE_MAP_READ, 0, 0, dwBlockBytes);

    CloseHandle(hLargeFile);*/
    //file.Write(&bmpFileHeader,sizeof(BITMAPFILEHEADER));
    //file.Write(&m_bmpInfo,sizeof(BITMAPINFOHEADER));
    //file.Write(m_bitmap.bmBits,bmpFileHeader.bfSize);
    //CBitmap bmp;
    //HBITMAP hBitmap;
    //CDC memDC;
    ////装载资源 MM.bmp是我的一个文件名，用你的替换
    //hBitmap=(HBITMAP)::LoadImage (::AfxGetInstanceHandle(), filename, IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    //bmp.Attach(hBitmap);

    //bmp.GetBitmap(&m_bitmap);
    ////memDC.BitBlt(0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,pDeskDC,0,0,SRCCOPY);

    //memDC.SelectObject(bmp);
    //DWORD size = m_bitmap.bmWidthBytes * m_bitmap.bmHeight;
    //m_bitmap.bmBits=(LPVOID*)malloc(size);

    //FillBitmapInfo(&m_bmpInfo);

    //GetDIBits(memDC.m_hDC,bmp,0,m_bitmap.bmHeight,m_bitmap.bmBits,&m_bmpInfo,DIB_RGB_COLORS);
    //dcMem.CreateCompatibleDC(GetDC());
    //CBitmap *poldBitmap=(CBitmap*)dcMem.SelectObject(hbmp);


}
void Bitmap::ReadFromWnd(CWnd* pWnd, int x, int y, int width, int height)
{
    CDC* pDeskDC =  pWnd->GetDC();
    CRect rc;
    pWnd->GetClientRect(rc);
    //rc.right=10;
    CDC  memDC;
    memDC.CreateCompatibleDC(pDeskDC);
    CBitmap bmp;
    BITMAP bitmap;
    bmp.CreateCompatibleBitmap(pDeskDC, width, height);
    //bmp.CreateCompatibleBitmap(pDeskDC,rc.Width(),rc.Height());
    memDC.SelectObject(&bmp);

    bmp.GetBitmap(&bitmap);
    memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, pDeskDC, x, y, SRCCOPY);
    //memDC.BitBlt(0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,pDeskDC,0,0,SRCCOPY);

    //添加鼠标
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
    //file.write
    //CFile file;
    //if(file.Open(filename,CFile::modeCreate | CFile::modeReadWrite ))
    //{
    //	file.Write(&bmpFileHeader,sizeof(BITMAPFILEHEADER));
    //	file.Write(&m_bmpInfo,sizeof(BITMAPINFOHEADER));
    //	file.Write(m_bitmap.bmBits,bmpFileHeader.bfSize);
    //	file.Close();
    //}
}


//int Bitmap::ColorChanelCompare(const void* src1,const int spectrum1, const void* src2,const int spectrum2, COLOR color, UINT32 size)
//{
//
//	const BYTE* p1 = (BYTE*)src1 + color;
//	const BYTE* p2 = (BYTE*)src2 + color;
//	const BYTE* p1End = p1 + size;
//	while(p1 < p1End)
//	{
//		if(*p1 != *p2)
//			return 250;
//		p1 += spectrum1;
//		p2 += spectrum2;
//	}
//	return 0;
//}
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

        //for ignore alpha
        //so compare each color
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
            /*if(colormemcmp(p, pSub, RED, spectrum * subBitmap.Width()) )
            return FALSE;
            if(colormemcmp(p, pSub, GREEN, spectrum * subBitmap.Width()) )
            return FALSE;
            if(colormemcmp(p, pSub, BLUE, spectrum * subBitmap.Width()) )
            return FALSE;*/
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
INT32 Bitmap::IndexOf(Bitmap& subBitmap, OUT RECT* subRect)
{
    COORD coord = {0};
    for( ; coord.Y < Height() - subBitmap.Height() + 1; coord.Y++)
    {
        for(coord.X = 0 ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
        {
            if(IsEqual(coord, subBitmap))
                return 1;
        }
    }
    return -1;
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
    return *(int*)&left != *(int*)&right;
}
BOOL operator == (const RGBTRIPLE triple, const RGBQUAD quad)
{
    return *(short*)&triple == *(short*)&quad && triple.rgbtRed == quad.rgbRed && quad.rgbReserved == 0xff;
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