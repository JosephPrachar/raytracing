#include "stdafx.h"

Intersection::Intersection(Triangle sphere, Point point):
	mShape(sphere),
	mPoint(point)
{
}
Intersection::Intersection():
	mPoint(0,0,0),
	mShape()
{
}

Intersection Intersection::copy(){
	return *this;//new Intersection(this->mShape, this->mPoint);
}

std::wstringstream& operator<<(std::wstringstream& os, const Intersection& obj){
	os << '{' << &obj.mShape << ', ' << &obj.mPoint << '}';
	return os;
}