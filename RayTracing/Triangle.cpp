#include "stdafx.h"
#include "Triangle.h"

#define EPSILON .000001

Triangle::Triangle(Color color, Finish finish, Point one, Point two, Point three):
	mColor(color),
	mFinish(finish),
	mOne(one),
	mTwo(two),
	mThree(three)
{
	computeNormal();
}
Triangle::Triangle():
	mColor(),
	mFinish(),
	mOne(),
	mTwo(),
	mThree()
{
}

Point* Triangle::getPoints() const{
	return NULL;
}
Color Triangle::getColor() const{
	return this->mColor;
}
Finish Triangle::getFinish() const{
	return this->mFinish;
}

Point Triangle::rayIntersection(Ray toIntersect, bool* hit){
	float scalar = 0;
	*hit = triangleIntersection(toIntersect, &scalar);

	if (*hit){
		Vector scaled = toIntersect.getDirection().copy();
		scaled.scale(scalar);

		Point intersection = toIntersect.getPoint().copy();
		intersection.translate(scaled);

		return intersection;
	}

	return Point();
}
bool Triangle::triangleIntersection(Ray toIntersect, float* out){
	// M�ller�Trumbore intersection algorithm
	// Translated from the C program posted on
	// the Wikipedia page of the same name at
	// http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	Vector edge1, edge2;
	Vector P, Q, T;
	float det, inv_det, u, v;
	float t;

	edge1 = Point::differenceVector(this->mTwo, this->mOne);
	edge2 = Point::differenceVector(this->mThree, this->mOne);

	P = toIntersect.getDirection().crossWith(edge2);
	det = edge1.dotWith(P);
	if (det >-EPSILON && det < EPSILON)
		return false;

	inv_det = 1.f / det;
	T =  Point::differenceVector(toIntersect.getPoint(), this->mOne);
	u = T.dotWith(P) * inv_det;
	if (u < 0.f || u > 1.f)
		return false;

	Q = T.crossWith(edge1);
	v = toIntersect.getDirection().dotWith(Q) * inv_det;
	if (v < 0.f || u + v > 1.f)
		return false;

	t = edge2.dotWith(Q) * inv_det;
	if (t > EPSILON){
		*out = t;
		return true;
	}

	return false;
}

Vector Triangle::normalAtPoint(Point pt){
	return this->mNormal;
}
void Triangle::alignNormalWithEyePt(Point eye){
	Point avg = mOne.copy();	
	avg.translate(Point::vectorFromTo(Point(0,0,0), mTwo));
	avg.translate(Point::vectorFromTo(Point(0,0,0), mThree));
	Vector v = Point::vectorFromTo(Point(0,0,0), avg);
	v.scale(.3);

	Point origin = Point(0,0,0);
	origin.translate(v);

	Vector eyeToCenter = Point::vectorFromTo(origin, eye);
	float dot = eyeToCenter.dotWith(this->mNormal);

	if (dot < 0)
	{
		this->mNormal.scale(-1);
		cout<< this->mNormal.getX() << " " << this->mNormal.getY() << " " << this->mNormal.getZ() << endl;
	}
}

Triangle Triangle::copy(){
	return *this;// new Triangle(mColor, mFinish, mOne, mTwo, mThree);
}

void Triangle::computeNormal(){
	Vector edge1 = Point::differenceVector(this->mTwo, this->mOne);
	Vector edge2 = Point::differenceVector(this->mThree, this->mOne);

	this->mNormal = edge1.crossWith(edge2);
}