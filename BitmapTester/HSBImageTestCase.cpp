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
	HSBColorF color;
	color=image.GetPixel(0,0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( color.H, 6, 0.5 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( color.S, 0.43, 0.5 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( color.B, 0.88, 0.5 );
}
