#include "stdafx.h"
#include "Bitmap.h"
#include <fstream>
using namespace std;
Bitmap::~Bitmap()
{
	if(m_bitmap.bmBits)
	{
		free(m_bitmap.bmBits);
		m_bitmap.bmBits = NULL;
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
	fstream file;
	file.open(filename, ios::in | ios::binary);
	if(!file)
		return ;
	file.read((char*)&bmpFileHeader, sizeof(BITMAPFILEHEADER));

	file.read((char*)&m_bmpInfo, 4);
	LONG* pWidth = &m_bmpInfo.bmiHeader.biWidth;
	file.read((char*)pWidth, m_bmpInfo.bmiHeader.biSize - 4);

	m_bitmap.bmWidth = m_bmpInfo.bmiHeader.biWidth;
	m_bitmap.bmHeight =  m_bmpInfo.bmiHeader.biHeight;
	m_bitmap.bmPlanes =   m_bmpInfo.bmiHeader.biPlanes;
	m_bitmap.bmBitsPixel =  m_bmpInfo.bmiHeader.biBitCount;

	m_offset = Spectrum() * Width() & 0x3;

	m_bitmap.bmBits = malloc(m_bmpInfo.bmiHeader.biSizeImage);
	file.read((char*)m_bitmap.bmBits, m_bmpInfo.bmiHeader.biSizeImage);

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
	bmp.CreateCompatibleBitmap(pDeskDC, width, height);
	//bmp.CreateCompatibleBitmap(pDeskDC,rc.Width(),rc.Height());
	memDC.SelectObject(&bmp);

	bmp.GetBitmap(&m_bitmap);
	memDC.BitBlt(0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, pDeskDC, x, y, SRCCOPY);
	//memDC.BitBlt(0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,pDeskDC,0,0,SRCCOPY);

	//添加鼠标
	/*CPoint point;
	GetCursorPos(&point);
	HICON hicon = (HICON)GetCursor();
	memDC.DrawIcon(point,hicon);  */

	DWORD size = m_bitmap.bmWidthBytes * m_bitmap.bmHeight;
	m_bitmap.bmBits = (LPVOID*)malloc(size);
	if(m_bitmap.bmBits == NULL)
		::MessageBox(NULL, _T("GlobalAllocPtr Failed\n"), _T("SDF"), NULL);

	FillBitmapInfo(&m_bmpInfo);

	GetDIBits(memDC.m_hDC, bmp, 0, m_bitmap.bmHeight, m_bitmap.bmBits, &m_bmpInfo, DIB_RGB_COLORS);
}
void Bitmap::Save(wstring& filename)
{
	BITMAPFILEHEADER bmpFileHeader;
	FillBitmapFileHeader(&bmpFileHeader);
	fstream file(filename, ios::out | ios::binary);
	file.write((char*)&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	file.write((char*)&m_bmpInfo, m_bmpInfo.bmiHeader.biSize);
	file.write((char*)m_bitmap.bmBits, m_bmpInfo.bmiHeader.biSizeImage);
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
void Bitmap::FillBitmapInfo(BITMAPINFO* pBmpInfo)
{
	pBmpInfo->bmiHeader.biSize						= 40;
	pBmpInfo->bmiHeader.biWidth					= m_bitmap.bmWidth;
	pBmpInfo->bmiHeader.biHeight					= m_bitmap.bmHeight;
	pBmpInfo->bmiHeader.biPlanes					= m_bitmap.bmPlanes;
	pBmpInfo->bmiHeader.biBitCount				= m_bitmap.bmBitsPixel;
	pBmpInfo->bmiHeader.biCompression			= 0;
	pBmpInfo->bmiHeader.biSizeImage				= m_bitmap.bmWidthBytes * m_bitmap.bmHeight;
	pBmpInfo->bmiHeader.biXPelsPerMeter		= 0;
	pBmpInfo->bmiHeader.biYPelsPerMeter		= 0;
	pBmpInfo->bmiHeader.biClrUsed					= 0;
	pBmpInfo->bmiHeader.biClrImportant			= 0;
}
void Bitmap::FillBitmapFileHeader(BITMAPFILEHEADER* pBmpFileHeader)
{
	pBmpFileHeader->bfType				= MAKEWORD('B', 'M');
	pBmpFileHeader->bfSize				= 54 + m_bitmap.bmWidthBytes * m_bitmap.bmHeight;
	pBmpFileHeader->bfReserved1		= 0;
	pBmpFileHeader->bfReserved2		= 0;
	pBmpFileHeader->bfOffBits			= 54;
}
int Bitmap::colormemcmp(const void* src1, const void* src2, COLOR color, UINT32 size)
{
	const BYTE* p1 = (BYTE*)src1 + color;
	const BYTE* p2 = (BYTE*)src2 + color;
	const BYTE* p1End = p1 + size;
	while(p1 < p1End)
	{
		if(*p1 != *p2)
			return 250;
		p1 += Spectrum();
		p2 += Spectrum();
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
			switch (spectrum)
			{
			case 3:
				if(memcmp(p, pSub, spectrum * subBitmap.Width()) )
					return FALSE;
				break;
			case 4:
				if(colormemcmp(p, pSub, RED, spectrum * subBitmap.Width()) )
					return FALSE;
				if(colormemcmp(p, pSub, GREEN, spectrum * subBitmap.Width()) )
					return FALSE;
				if(colormemcmp(p, pSub, BLUE, spectrum * subBitmap.Width()) )
					return FALSE;
				break;
			default:
				break;
			}

		}
		else
		{
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
		for( ; coord.X < Width() - subBitmap.Width() + 1; coord.X++)
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
	ASSERT(height < m_bitmap.bmHeight);
	ASSERT(width < m_bitmap.bmWidth);
	m_bitmap.bmHeight = height;
	m_bitmap.bmWidth = width;
	LPVOID  newBmBits = malloc(m_bitmap.bmHeight * m_bitmap.bmWidth);
	BYTE* writer = (BYTE*)newBmBits;
	int y;
	for(y = 0; y < m_bitmap.bmHeight; y++)
	{
		const BYTE* p = DataPtr(rect->left, rect->bottom + y);
		memcpy(writer, p, width * Spectrum());
	}
	return 1;
}