
#ifndef CPP_UNIT_EXAMPLETESTCASE_H
#define CPP_UNIT_EXAMPLETESTCASE_H

#include <cppunit/extensions/HelperMacros.h>
#include <Bitmap.h>
#include <HSBImage.h>
#include <GrayAlphaBitmap.h>

/* 
* A test case that is designed to produce
* example errors and failures
*
*/

class HSBImageTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HSBImageTestCase );
	CPPUNIT_TEST( testGetPixel );
	CPPUNIT_TEST( testGetAverageHue );

	CPPUNIT_TEST_SUITE_END();

protected:
	double m_value1;
	double m_value2;

public:
	void setUp();

protected:
	void testGetPixel();
	void testGetAverageHue();
};

#endif
