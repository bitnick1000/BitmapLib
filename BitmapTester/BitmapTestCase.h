
#ifndef CPP_UNIT_EXAMPLETESTCASE_H
#define CPP_UNIT_EXAMPLETESTCASE_H

#include <cppunit/extensions/HelperMacros.h>
#include <Bitmap.h>

/* 
* A test case that is designed to produce
* example errors and failures
*
*/

class BitmapTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( BitmapTestCase );
	CPPUNIT_TEST( testGetPixel );
	CPPUNIT_TEST( testIndexOf );

	CPPUNIT_TEST( testAdd );
	CPPUNIT_TEST( testEquals );
	CPPUNIT_TEST_SUITE_END();

protected:
	Bitmap* m_pBitmap;
	double m_value1;
	double m_value2;

public:
	void setUp();

protected:
	void testGetPixel();
	void testIndexOf();
	
	void testAdd();
	void testEquals();
};


#endif
