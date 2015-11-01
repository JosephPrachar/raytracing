#pragma once
#include "stdafx.h"
using namespace std;

class RayCaster {
public:
	RayCaster(Window view, Point eyePoint, std::vector<Triangle*> *shapeList, Color ambientColor, Light pointLight);
	~RayCaster();

	Color* castRay(Intersection* hitPointMem);
	void castAllRays();
	void printPicture(HDC hdc);

	double computeTime;
private:
	float curX;
	float curY;

	byte* bitMap;
	Window mView;
	Point mEye;
	std::vector<Triangle*> mShapeList;
	Color mAmbient;
	Light mPointLight;
	
	int findIntersectionPoints(Ray ray, Intersection* hitPointMem);

	Color* computeAmbientLight(Triangle* shape);
	Color* computePointAndSpecular(Intersection* intersect, Intersection* hitPointMem);

	void advanceCastPoint();

	int shortestDistFromPoint(Intersection* hitPoints, int length);
};
