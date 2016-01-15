#include "stdafx.h"

Camera::Camera() :
	loc(0,0,50),
	rot(0, 0, 0)
{
	rect[0] = Point(-20, 15, 0);    // 0------1
	rect[1] = Point(20, 15, 0);     // |      |
	rect[2] = Point(20, -15, 0);	// |      |
	rect[3] = Point(-20, -15, 0);	// 3------2
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
}

void Camera::moveTo(Point pt) {
	Vector toMove = Point::vectorFromTo(this->loc, pt);

	for (int i = 0; i < 4; i++) {
		rect[i].translate(toMove);
	}
}

