#pragma once
#include "stdafx.h"

void readSetupFile(string setupFile, Window* view, Point* eye, Color* ambient, Light* light);
void readTriangles(string pointsFile, string verticiesFile, vector<Triangle*> &list);

void readVerticies(string verticiesFile, vector<vector<int>> &triangles);
void readPoints(string pointsFile, vector<Point> &points);

Window* parseWindow(string line);
Point* parsePoint(string line);
Light* parseLight(string line);
Color* parseColor(string line);
void parseFloats(string line, int expected, vector<float> &floats);

void split(const string &s, char delim, vector<string> &elems);