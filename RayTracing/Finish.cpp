#include "stdafx.h"

Finish::Finish(float ambient, float diffuse, float specular, float roughness){
	this->mAmbient = ambient;
	this->mDiffuse = diffuse;
	this->mSpecular = specular;
	this->mRough = roughness;	
}
Finish::Finish(){
	this->mAmbient = 0;
	this->mDiffuse = 0;
	this->mSpecular = 0;
	this->mRough = 0;
}

float Finish::getAmbient() const{
	return this->mAmbient;
}
float Finish::getDiffuse() const{
	return this->mDiffuse;
}
float Finish::getSpecular() const{
	return this->mSpecular;
}
float Finish::getRoughness() const{
	return this->mRough;
}

Finish* Finish::copy(){
	return new Finish(mAmbient, mDiffuse, mSpecular, mRough);
}

std::wstringstream& operator<<(std::wstringstream& os, const Finish& obj){
	os << '<' << obj.getAmbient() << ", " << obj.getDiffuse() << ", " << obj.getSpecular() << ', ' << obj.getRoughness() << '>';
	return os;
}