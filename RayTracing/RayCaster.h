#pragma once
#include "stdafx.h"
using namespace std;

class RayCaster {
public:
	RayCaster(Camera view, Point eyePoint, std::vector<Triangle*> *shapeList, Color ambientColor, Light pointLight);
	~RayCaster();

	Color castRay(Intersection* hitPointMem);
	void castAllRays();
	void printPicture(HDC hdc);

	Camera* getCamera();

	double computeTime;
private:
	byte* bitMap;
	Camera mView;
	Point mEye;
	std::vector<Triangle*> mShapeList;
	Color mAmbient;
	Light mPointLight;
	
	int findIntersectionPoints(Ray ray, Intersection* hitPointMem);

	Color computeAmbientLight(Triangle* shape);
	Color computePointAndSpecular(Intersection* intersect, Intersection* hitPointMem);

	int shortestDistFromPoint(Intersection* hitPoints, int length);
};
