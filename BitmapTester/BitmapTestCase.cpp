#include <cppunit/config/SourcePrefix.h>
#include "BitmapTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BitmapTestCase );

void BitmapTestCase::setUp()
{
	m_value1 = 2.0;
	m_value2 = 3.0;
}
void BitmapTestCase::testGetPixel()
{
	Bitmap bmp(_T("..\\TestResources\\Lena.bmp"));
	RGBQUAD color;
	color = bmp.GetPixel(0, 511);
	CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)81 );
	CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)19 );
	CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)56 );
	color = bmp.GetPixel(1, 510);
	CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)81 );
	CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)19 );
	CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)56 );
	color = bmp.GetPixel(2, 511 - 2);
	CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)92 );
	CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)28 );
	CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)62 );
	color = bmp.GetPixel(14, 511 - 4);
	CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)134 );
	CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)75 );
	CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)107 );
}
void testIndexOf1()
{
	Bitmap sub_24(_T("..\\TestResources\\IndexOf\\sub1_24.bmp"));
	Bitmap bitmap(_T("..\\TestResources\\Lena.bmp"));
	COORD result = bitmap.IndexOf(sub_24);
	CPPUNIT_ASSERT( result.X == 0 );
	CPPUNIT_ASSERT( result.Y == 502 );
	Bitmap sub_32(_T("..\\TestResources\\IndexOf\\sub1_32.bmp"));
	result = bitmap.IndexOf(sub_32);
	CPPUNIT_ASSERT( result.X == 0 );
	CPPUNIT_ASSERT( result.Y == 502 );
}
void testIndexOf2()
{
	Bitmap sub_24(_T("..\\TestResources\\IndexOf\\sub2_24.bmp"));
	Bitmap bitmap(_T("..\\TestResources\\Lena.bmp"));
	COORD result = bitmap.IndexOf(sub_24);
	CPPUNIT_ASSERT( result.X == 1 );
	CPPUNIT_ASSERT( result.Y == 500 );
}
void testIndexOf3()
{
	Bitmap sub(_T("..\\TestResources\\IndexOf\\sub3.bmp"));
	Bitmap bitmap(_T("..\\TestResources\\Lena.bmp"));
	CPPUNIT_ASSERT( sub.GetPixel(0, 0) == bitmap.GetPixel(122, 237) );
	CPPUNIT_ASSERT( sub.GetPixel(27, 0) == bitmap.GetPixel(149, 237) );
	CPPUNIT_ASSERT( sub.GetPixel(0, 1) == bitmap.GetPixel(122, 238) );
	CPPUNIT_ASSERT( sub.GetPixel(0, 23) == bitmap.GetPixel(122, 260) );
	for(int y = 0; y < sub.Height(); y++)
	{
		for(int x = 0; x < sub.Width(); x++)
		{
			CPPUNIT_ASSERT( sub.GetPixel(0 + x, 0 + y) == bitmap.GetPixel(122 + x, 237 + y) );
		}
	}
	COORD result = bitmap.IndexOf(sub);
	CPPUNIT_ASSERT( result.X == 122 );
	CPPUNIT_ASSERT( result.Y == 237 );
}
void BitmapTestCase::testIndexOf()
{
	testIndexOf1();
	testIndexOf2();
	testIndexOf3();
}
void BitmapTestCase::testIndexOfEx()
{
	Bitmap sub(_T("..\\TestResources\\IndexOfEx\\sub1.png"));
	Bitmap bitmap(_T("..\\TestResources\\Lena.bmp"));
	COORD result = bitmap.IndexOfEx(sub);
	CPPUNIT_ASSERT( result.X == 130 );
	CPPUNIT_ASSERT( result.Y == 243 );
}
void BitmapTestCase::testAdd()
{
	double result = m_value1 + m_value2;
	CPPUNIT_ASSERT( result == 5.0 );
}


void BitmapTestCase::testEquals()
{
	long* l1 = new long(12);
	long* l2 = new long(12);

	CPPUNIT_ASSERT_EQUAL( 12, 12 );
	CPPUNIT_ASSERT_EQUAL( 12L, 12L );
	CPPUNIT_ASSERT_EQUAL( *l1, *l2 );

	delete l1;
	delete l2;

	CPPUNIT_ASSERT( 12L == 12L );
	CPPUNIT_ASSERT_EQUAL( 12, 12 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.0, 11.99, 0.5 );
}
