#pragma once
#include "stdafx.h"

class Intersection{
public:
	Intersection(Triangle shape, Point point);
	Intersection();

	Triangle mShape;
	Point mPoint;

	Intersection copy();
};

inline bool operator==(const Intersection& lhs, const Intersection& rhs){
	return lhs.mPoint == rhs.mPoint &&
		lhs.mShape == rhs.mShape;
}
inline bool operator!=(const Intersection& lhs, const Intersection& rhs){
	return !(lhs == rhs);
}

std::wstringstream& operator<<(std::wstringstream& os, const Intersection& obj);