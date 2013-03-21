
#include "stdafx.h"
#include "Bitmap.h"
#include "GrayAlphaBitmap.h"
#include <string>
#include <iostream>
#include <vector>


GrayAlphaBitmap::GrayAlphaBitmap(Bitmap& bitmap)
{
    this->width = bitmap.Width();
    this->height = bitmap.Height();
    this->data = malloc(this->height * this->width * 2);
	for(int y=0;y<this->height;y++)
	{
		for(int x=0;x<this->width;y++)
		{

		}
	}
}
COORD GrayAlphaBitmap::IndexOfEx(GrayAlphaBitmap& subBitmap)
{
}
