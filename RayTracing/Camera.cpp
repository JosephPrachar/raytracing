#include "stdafx.h"
#define PIXELS_X 800
#define PIXELS_Y 600

Camera::Camera() :
	loc(0,0,50),
	rot(0, 0, 0)
{
	rect[0] = Point(-20, 15, 0);    // 0------1
	rect[1] = Point(20, 15, 0);     // |      |
	rect[2] = Point(20, -15, 0);	// |      |
	rect[3] = Point(-20, -15, 0);	// 3------2

	reCalculate();
}

void Camera::reCalculate(void) {
	castPoint = rect[0];

	advanceX = Point::vectorFromTo(rect[0], rect[1]);
	advanceX.scale(1.0/PIXELS_X);

	advanceY = Point::vectorFromTo(rect[0], rect[3]);
	advanceY.scale(1.0/PIXELS_Y);

	countX = 0;
	countY = 0;
}

void Camera::rotate(Vector rot) {
	Vector eyeVect = loc.toVector();
	Vector eyeVectNeg = eyeVect;
	eyeVectNeg.scale(-1);

	float x = rot.getX();
	float y = rot.getY();
	float z = rot.getZ();

	// Calculate Rotation matrix
	// Formula from https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions#Euler_angles_.28x-y-z_extrinsic.29_.E2.86.92_Rotation_matrix
	Vector xTrans = Vector(cos(y) * cos(z),
						   cos(x) * sin(z) + sin(x) * sin(y) * cos(z),
						   sin(x) * sin(z) - sin(x) * sin(y) * cos(z));

	Vector yTrans = Vector(-cos(y) * sin(z),
						   cos(x) * cos(z) - sin(x) * sin(y) * sin(z),
						   sin(x) * cos(z) + cos(x) * sin(y) * sin(z));

	Vector zTrans = Vector(sin(y),
						   -sin(x) * cos(y),
						   cos(x) * cos(y));

	for (unsigned i = 0; i < 4; i++)
	{
		// translate rect point as if eye was at origin so it is unaffected by the translation
		rect[i].translate(eyeVectNeg);
		Vector curVect = rect[i].toVector();
		rect[i] = Point(xTrans.dotWith(curVect),
						yTrans.dotWith(curVect),
						zTrans.dotWith(curVect));
		rect[i].translate(eyeVect);
	}

	reCalculate();
}

void Camera::moveTo(Point pt) {
	Vector toMove = Point::vectorFromTo(this->loc, pt);

	for (int i = 0; i < 4; i++) {
		rect[i].translate(toMove);
	}

	reCalculate();
}

void Camera::moveBy(Vector translation) {
	for (int i = 0; i < 4; i++) {
		rect[i].translate(translation);
	}

	reCalculate();
}

void Camera::advanceCastPoint(void) {
	castPoint.translate(this->advanceX);
	countX++;
	if (countX >= PIXELS_X) {
		countX = 0;
		countY++;
		castPoint = rect[0];
		for (int i = 0; i < countY; i++) 
			castPoint.translate(advanceY);
	}
}

void Camera::resetCastPoint(void) {
	countX = 0;
	countY = 0;

	castPoint = rect[0];
}

Ray Camera::getCastRay(void) {
	return Ray(this->loc, Point::vectorFromTo(this->loc, this->castPoint));
}