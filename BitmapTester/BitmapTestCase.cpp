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
    color = bmp.GetPixel(0, 0);
    CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)81 );
    CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)19 );
    CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)56 );
    color = bmp.GetPixel(1, 1);
    CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)81 );
    CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)19 );
    CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)56 );
    color = bmp.GetPixel(2, 2);
    CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)92 );
    CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)28 );
    CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)62 );
    color = bmp.GetPixel(14, 4);
    CPPUNIT_ASSERT_EQUAL( color.rgbRed, (BYTE)134 );
    CPPUNIT_ASSERT_EQUAL( color.rgbGreen, (BYTE)75 );
    CPPUNIT_ASSERT_EQUAL(color.rgbBlue, (BYTE)107 );
}
void testIndexOf1()
{
    Bitmap bmp1(_T("..\\TestResources\\B.bmp"));
    Bitmap bmp2(_T("..\\TestResources\\B.bmp"));
    RECT rect;
    int result = bmp2.IndexOf(bmp1, &rect);
    CPPUNIT_ASSERT( result != -1 );
}
void testIndexOf2()
{
    Bitmap bmp1(_T("..\\TestResources\\W.bmp"));
    Bitmap bmp2(_T("..\\TestResources\\W.bmp"));
    RECT rect;
    int result = bmp2.IndexOf(bmp1, &rect);
    CPPUNIT_ASSERT( result != -1 );
}
void testIndexOf3()
{
    Bitmap bmp1(_T("..\\TestResources\\W.bmp"));
    Bitmap bmp2(_T("..\\TestResources\\BWnBW.bmp"));
    RECT rect;
    int result = bmp2.IndexOf(bmp1, &rect);
    CPPUNIT_ASSERT( result != -1 );
}
void testIndexOf4()
{
    Bitmap bmp1(_T("..\\TestResources\\W.bmp"));
    Bitmap bmp2(_T("..\\TestResources\\pic1.bmp"));
    RECT rect;
    int result = bmp2.IndexOf(bmp1, &rect);
    CPPUNIT_ASSERT( result != -1 );
}
void BitmapTestCase::testIndexOf()
{
    /*testIndexOf1();
    testIndexOf2();
    testIndexOf3();
    testIndexOf4();*/

    Bitmap bmp1(_T("..\\TestResources\\IndexOf\\sub1.bmp"));
    Bitmap bmp2(_T("..\\TestResources\\Lena.bmp"));
    //Bitmap bmp2_3(_T("..\\TestResources\\IndexOf\\pic1sub2.bmp"));
    RECT rect;
    int result = bmp2.IndexOf(bmp1, &rect);
    //int result2_2 = bmp2_1.IndexOf(bmp2_3,&rect2);
    CPPUNIT_ASSERT( result != -1 );
    //CPPUNIT_ASSERT( result2_2 != -1 );
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
