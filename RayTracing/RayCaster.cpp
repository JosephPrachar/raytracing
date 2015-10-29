#include "stdafx.h"
using namespace std;
#include <vector>

RayCaster::RayCaster(Window view, Point eyePoint, vector<Shape*> *shapeList, Color ambientColor, Light pointLight):
	mView(view),
	mEye(eyePoint),
	mShapeList(*shapeList),
	mAmbient(ambientColor),
	mPointLight(pointLight)
{
	this->curX = mView.x_min;
	this->curY = mView.y_max;

	this->computeTime = -1;

	bitMap = NULL;
}

void RayCaster::castAllRays(){
	std::clock_t start;
	start = std::clock();

	// calculate total size of buffer
	long size = mView.width * mView.height * 3;

	// allocate memory for findIntersectionPoints
	Intersection* hitPointMem = new Intersection[this->mShapeList.size()];

	if (bitMap == NULL) {
		bitMap = new byte[mView.width * mView.height * 3];
		if (bitMap == NULL) exit(1);
	} else {
		return;
	}

	// check for valid memory allocation
	if (hitPointMem == NULL){
		exit(1);
	}

	// for the console 'progress bar'
	long count = 0;
	float percent = .02f;

	while (count < size){
		Color result = this->castRay(hitPointMem);
		result.scaleForPrinting();

		//if (this->curX == 0 || this->curY == 0)

			//result =  Color(1,1,1);

		// fill buffer with current pixel info
#pragma warning(suppress: 6386)
		bitMap[count] = (byte)result.getRed();
		count++;
		bitMap[count] = (byte)result.getGreen();
		count++;
		bitMap[count] = (byte)result.getBlue();
		count++;

		this->advanceCastPoint();

		// for the console 'progress bar'
		if (count % 1000 == 0 || count % 1000 == 1 || count % 1000 == 2){
			if ((float)count / (float)size > percent){
				std::cout << '=';
				percent += .02;
			}
		}
	}

	this->computeTime = (std::clock() - start)/(double)CLOCKS_PER_SEC;

	std::cout << "Done Computing";

	// output buffer to file
	//this->printPicture(hdc, bitMap);

	// free memory
	delete[] hitPointMem;
}

void RayCaster::printPicture(HDC hdc){
	long size = mView.width * mView.height * 3;

	// keep track of current writing position
	long pos = 0;
	for (int y = 0; y < mView.height; y++) {
		for (int x = 0; x < mView.width; x++) {
			SetPixel(hdc, x, y, RGB(bitMap[pos], bitMap[pos+1], bitMap[pos+2]));
			pos += 3;
		}
	}
}

Color RayCaster::castRay(Intersection* hitPointMem){

	Color toReturn = Color(1.0, 1.0, 1.0);

	Point pt = Point(curX, curY, 0);
	Vector v = Point::vectorFromTo(this->mEye, pt);
	Ray ray = Ray(this->mEye, v);
	
	int length = this->findIntersectionPoints(ray, hitPointMem);

	if (length != 0){ // hits a sphere
		int iSmall = 0;
		if (length > 1){ // if length is not greater than 1 no need to find shortest
			iSmall = this->shortestDistFromPoint(hitPointMem, length);
		}
		// extract closest intersection point so that hitPointsMem can be reused in findIntersectionPoints call by the specular color computation
		Intersection intersect = hitPointMem[iSmall].copy();		

		Color ambientColorAddition = this->computeAmbientLight(intersect.mShape);
		Color pointLighting = computePointAndSpecular(intersect, hitPointMem);
		ambientColorAddition.add(pointLighting);

		toReturn = ambientColorAddition;
		//toReturn = intersect.mShape->getColor();
	}

	return toReturn;
}

int RayCaster::shortestDistFromPoint(Intersection* hitPoints, int length){
	int iSmall = 0;
	float distSmall = this->mEye.distance(hitPoints[0].mPoint);

	for (int i = 1; i < length; ++i){
		float cur = this->mEye.distance(hitPoints[i].mPoint);
		if (cur < distSmall){
			iSmall = i;
			distSmall = cur;
		}
	}
	
	return iSmall;
}

int RayCaster::findIntersectionPoints(Ray ray, Intersection* hitPointMem){	
	int count = 0;
	for (int i = 0; i < this->mShapeList.size(); ++i){
		bool hit = false;
		Point pt = this->mShapeList[i]->rayIntersection(ray, &hit);
		if (hit == true){
			//cout<<i << " ";
			hitPointMem[count] = Intersection(this->mShapeList[i], pt);
			count++;
		}		
	}
	//if (count !=0)
	//	cout<<count;
	return count;
}

Color RayCaster::computeAmbientLight(Shape* shape){
	Color toReturn = shape->getColor().copy();
	toReturn.multiply(this->mAmbient);
	toReturn.scale(shape->getFinish().getAmbient());
	return toReturn;
}

Color RayCaster::computePointAndSpecular(Intersection intersect, Intersection* hitPointMem){
	//// do math...

	//Vector normal = intersect.mShape->normalAtPoint(intersect.mPoint);
	//Vector scaledNormal = normal.copy();
	//scaledNormal.scale(.01);

	//Point pSubE = intersect.mPoint.copy();
	//pSubE.translate(scaledNormal);

	//Vector lSubDir = Point::vectorFromTo(pSubE, this->mPointLight.getPoint());
	//lSubDir.normalize();

	//float lDotN = normal.dotWith(lSubDir);

	//if (lDotN <= 0){
	//	return Color(0,0,0);
	//}

	//Ray rayToLight = Ray(pSubE, lSubDir);

	//int length = this->findIntersectionPoints(rayToLight, hitPointMem);

	//if (length > 0){
	//	float distToLight = pSubE.distance(this->mPointLight.getPoint());
	//	for (int i = 0; i < length; ++i){
	//		if (pSubE.distance(hitPointMem[i].mPoint) < distToLight){
	//			return Color(0,0,0);
	//		}
	//	}
	//}

	//float dotTimesDiffuse = lDotN * intersect.mShape->getFinish().getDiffuse();
	//Color pointColor = intersect.mShape->getColor().copy();
	//pointColor.multiply(this->mPointLight.getColor());
	//pointColor.scale(dotTimesDiffuse);

	//Vector temp = normal.copy();
	//temp.scale(2*lDotN);
	//Vector reflect = lSubDir.copy();
	//reflect.subtract(temp);

	//Vector vSubDir = Point::vectorFromTo(this->mEye, pSubE);
	//vSubDir.normalize();

	//float specIntense = reflect.dotWith(vSubDir);
	//Color specColor = Color(0,0,0);
	//if (specIntense > 0){
	//	float scale = intersect.mShape->getFinish().getSpecular() * pow(specIntense, (1.0 / intersect.mShape->getFinish().getRoughness()));
	//	specColor = this->mPointLight.getColor().copy();
	//	specColor.scale(scale);
	//}
	////return specColor;
	//pointColor.add(specColor);
	//return pointColor;

	Vector normal = intersect.mShape->normalAtPoint(intersect.mPoint);
	Vector lightToPoint = Point::vectorFromTo(this->mPointLight.getPoint(), intersect.mPoint);
	normal.normalize();
	lightToPoint.normalize();
	float cosTheta = normal.dotWith(lightToPoint);

	if (cosTheta <= 0)
		return Color(0,0,0);

	float dotTimesDiffuse = cosTheta * intersect.mShape->getFinish().getDiffuse();
	Color pointColor = intersect.mShape->getColor().copy();
	pointColor.multiply(this->mPointLight.getColor());
	pointColor.scale(dotTimesDiffuse);
	return pointColor;	
}

void RayCaster::advanceCastPoint(){
	// move current point to next pixel
	this->curX += this->mView.delta_x;
	if (this->curX >= this->mView.x_max){
		this->curX = this->mView.x_min;
		this->curY -= this->mView.delta_y;
	}
}

