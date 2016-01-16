#pragma once

#include "Point.h"
#include "Vector.h"

class Camera {
public:
	Camera();
	Camera(Point start, Vector rot);

	void rotate(Vector rot);
	void pointAt(Point pt);
	void moveBy(Vector translation);
	void moveTo(Point pt);

	void advanceCastPoint(void);
	void resetCastPoint(void);
	Ray getCastRay(void);
private:
	Point loc;
	Vector rot;
	Point rect[4];
	Point castPoint;

	Vector advanceX;
	Vector advanceY;
	int countX;
	int countY;

	float degToRad(float deg);
	float radToDeg(float rad);

	void reCalculate(void);
};