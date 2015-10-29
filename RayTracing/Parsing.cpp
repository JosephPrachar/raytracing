#include "stdafx.h"

void readSetupFile(string setupFile, Window* view, Point* eye, Color* ambient, Light* light) {
	// todo: check if successful input on each line
	
	string line;
	bool good = true;

	std::ifstream f(setupFile);

	if (f.is_open()){
		getline(f, line);
		view = parseWindow(line);

		getline(f, line);
		eye = parsePoint(line);

		getline(f, line);
		ambient = parseColor(line);

		getline(f, line);
		light = parseLight(line);
	}

	f.close();
}
void readTriangles(string pointsFile, string verticiesFile, vector<Shape*> &list) {
	Color color = Color(1, 0, 0);
	Finish finish = Finish(.2, .4, .5, .05);

	vector<vector<int>> verticies = vector<vector<int>>();
	vector<Point> points = vector<Point>();

	readVerticies(verticiesFile, verticies);
	readPoints(pointsFile, points);

	for (int i = 0; i < verticies.size(); i++) {
		// TODO: bounds check
		list.push_back(new Triangle(color, finish, 
			points[verticies[i][0]], points[verticies[i][1]], points[verticies[i][2]]));
	}

	//list->push_back(new Triangle(Color(0,0,1), Finish(.2, .4, .5, .05), Point(0,0,0), Point(1,0,-2), Point(0,1,-1)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(4,4,0), Point(4,0,-10), Point(0,4,-10)));
	//list->push_back(new Triangle(Color(0,1,0), Finish(.2, .4, .5, .05), Point(1,2,0), Point(-1,0,0), Point(0,-1,0)));
	//list->push_back(new Sphere(Point(-1, 1, 5), 2, Color(1, 0, 1), Finish(.2, .4, .5, .05)));
	//list->clear();

	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(0,1,0), Point(1,0,0)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(-1,0,0), Point(0,1,0)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(0,-1,0), Point(-1,0,0)));
	//list->push_back(new Triangle(Color(1,0,0), Finish(.2, .4, .5, .05), Point(0,0,-1), Point(1,0,0), Point(0,-1,0)));

	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(0,1,0), Point(1,0,0)));
	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(-1,0,0), Point(0,1,0)));
	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(0,-1,0), Point(-1,0,0)));
	//list->push_back(new Triangle(Color(0,0,0), Finish(.2, .4, .5, .05), Point(0,0,0), Point(1,0,0), Point(0,-1,0)));

	//list->push_back(new Sphere(Point(8, -10, 110), 100, Color(.2, .2, .6), Finish(.4, .8, 0, .05)));
}

void readVerticies(string verticiesFile, vector<vector<int>> &triangles) {
	string line;
	vector<int> nums = vector<int>();
	for (int i = 0; i < 3; i++)
		nums.push_back(0);

	ifstream file(verticiesFile);
	if (file.is_open()) {
		while (getline(file, line)) {
			Point* temp = parsePoint(line);

			if (temp != NULL) {
				nums[0] = (int)temp->getX();
				nums[1] = (int)temp->getY();
				nums[2] = (int)temp->getZ();
				triangles.push_back(nums);
			}

			delete temp;
		}
	}
}

void readPoints(string pointsFile, vector<Point> &points) {
	string line;

	ifstream file(pointsFile);
	if (file.is_open()) {
		while (getline(file, line)) {
			Point* temp = parsePoint(line);

			if (temp != NULL)
				points.push_back(*temp);

			delete temp;
		}
	}
}

Sphere* parseSphere(string line) {
	vector<float> vals = vector<float>();
	parseFloats(line, 11, vals);
	if (vals.size() != 0){
		return new Sphere(Point(vals[0], vals[1], vals[2]), vals[3], Color(vals[4], vals[5], vals[6]), Finish(vals[7], vals[8], vals[9], vals[10]));
	} else {
		return NULL;
	}
}
Window* parseWindow(std::string line) {
	vector<float> vals = vector<float>();
	parseFloats(line, 6, vals);
	if (vals.size() != 0){
		return new Window(vals[0], vals[1], vals[2], vals[3], (int)vals[4], (int)vals[5]);
	} else {
		return NULL;
	}
}
Point* parsePoint(std::string line) {
	vector<float> vals = vector<float>();
	parseFloats(line, 3, vals);
	if (vals.size() != 0){
		return new Point(vals[0], vals[1], vals[2]);
		
	} else {
		return NULL;
	}
}
Light* parseLight(std::string line) {
	vector<float> vals = vector<float>();
	parseFloats(line, 6, vals);
	if (vals.size() != 0){
		return new Light(Point(vals[0], vals[1], vals[2]), Color(vals[3], vals[4], vals[5]));
	} else {
		return NULL;
	}
}
Color* parseColor(std::string line) {
	vector<float> vals = vector<float>();
	parseFloats(line, 3, vals);
	if (vals.size() != 0){
		return new Color(vals[0], vals[1], vals[2]);
	} else {
		return NULL;
	}
}
void parseFloats(std::string line, int expected, vector<float> &floats) {
	vector<string> chars = vector<string>();
	split(line, ' ', chars);

	if (chars.size() == expected){
		try{
			for (int i = 0; i < expected; ++i){
				float f = stof(chars[i]);
				floats.push_back(f);
			}
		}
		catch (invalid_argument e){
			floats.clear();
			return;
		}
	}
}

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}