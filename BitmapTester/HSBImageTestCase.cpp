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
	HSBImage<BYTE> image;
	image.LoadFromBitmap(bmp);
	HSBColor<BYTE> color;
	color=image.GetPixel(0,0);
	CPPUNIT_ASSERT_EQUAL( color.H, (BYTE)81 );
	CPPUNIT_ASSERT_EQUAL( color.S, (BYTE)19 );
	CPPUNIT_ASSERT_EQUAL(color.B, (BYTE)56 );
}
