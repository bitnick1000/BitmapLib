#include <cppunit/config/SourcePrefix.h>
#include "HSBImageTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HSBImageTestCase );

void HSBImageTestCase::setUp()
{
	m_value1 = 2.0;
	m_value2 = 3.0;
}
void HSBImageTestCase::testGetPixel()
{
	Bitmap bmp(_T("..\\TestResources\\Lena.bmp"));
	HSBImage<float> image;
	image.LoadFromBitmap(bmp);
	HSBColor<float> color;
	color=image.GetPixel(0,0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( color.H, 6, 0.5 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( color.S, 0.43, 0.5 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( color.B, 0.88, 0.5 );
}
void testGetAverageHue1()
{
	Bitmap bmp(_T("..\\TestResources\\Lena.bmp"));
	HSBImage<float> image;
	image.LoadFromBitmap(bmp);
	float hue= image.GetAverageHue(0,0,4,4);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( hue, 6.5, 0.5 );
}
void testGetAverageHue2()
{
	Bitmap bmp(_T("..\\TestResources\\Lena.bmp"));
	HSBImage<float> image;
	image.LoadFromBitmap(bmp);
	float hue= image.GetAverageHue(133,245,4,4);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( hue, 327, 0.5 );
}
void HSBImageTestCase::testGetAverageHue()
{
	testGetAverageHue1();
	testGetAverageHue2();
}

