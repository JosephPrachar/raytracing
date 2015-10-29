#pragma once
#include "stdafx.h"

void parseSphere(string line, bool* good, Sphere &sphere);
void parseWindow(string line, bool* good, Window &window);
void parsePoint(string line, bool* good, Point &point);
void parseLight(string line, bool* good, Light &light);
void parseColor(string line, bool* good, Color &color);
void parseFloats(string line, int expected, vector<float> &floats);
void split(const string &s, char delim, vector<string> &elems);