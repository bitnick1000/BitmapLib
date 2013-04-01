#include "HSBImage.h"

template <typename T>
HSBImage<T>::HSBImage(Bitmap& bitmap)
{
	//return NULL;
}
template <typename T>
UINT32 HSBImage<T>::Width() const
{
	return this->width;
}
template <typename T>
UINT32 HSBImage<T>::Height() const
{
	return this->height;
}