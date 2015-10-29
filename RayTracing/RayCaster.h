#pragma once
#include "stdafx.h"
using namespace std;

class RayCaster {
public:
	RayCaster(Window view, Point eyePoint, std::vector<Shape*> *shapeList, Color ambientColor, Light pointLight);

	Color castRay(Intersection* hitPointMem);
	void castAllRays();
	void printPicture(HDC hdc);

	double computeTime;
private:
	float curX;
	float curY;

	byte* bitMap;
	Window mView;
	Point mEye;
	std::vector<Shape*> mShapeList;
	Color mAmbient;
	Light mPointLight;
	
	int findIntersectionPoints(Ray ray, Intersection* hitPointMem);

	Color computeAmbientLight(Shape* shape);
	Color computePointAndSpecular(Intersection intersect, Intersection* hitPointMem);

	void advanceCastPoint();

	int shortestDistFromPoint(Intersection* hitPoints, int length);
};
