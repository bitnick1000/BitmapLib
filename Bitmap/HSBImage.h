#ifndef _HSB_IMAGE_H_
#define _HSB_IMAGE_H_

#include "stdafx.h"
#include "Bitmap.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct HSBColor
{
	typedef T value_type;
	value_type H;
	value_type S;
	value_type B;
};

//T is color
template <typename T>
class HSBImage
{
	typedef T value_type;
public:
	HSBImage(Bitmap& bitmap);
public :
	UINT32 Width() const;
	UINT32 Height() const;
private:
	unsigned int width;
	unsigned int height;
};

#endif