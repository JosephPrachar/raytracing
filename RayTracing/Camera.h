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
private:
	Point loc;
	Vector rot;
	Point rect[4];

	float degToRad(float deg);
	float radToDeg(float rad);
};