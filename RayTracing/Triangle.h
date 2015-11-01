#pragma once

class Triangle {
public:
	Triangle(Color color, Finish finish, Point one, Point two, Point three);
	Triangle();

	Point* getPoints() const;
	Color getColor() const;
	Finish getFinish() const;

	Point* rayIntersection(Ray toIntersect);
	bool triangleIntersection(Ray toIntersect, float* out);
	Vector normalAtPoint(Point pt);
	void alignNormalWithEyePt(Point eye);
	Triangle* copy();

private:
	Point mOne;
	Point mTwo;
	Point mThree;
	Vector mNormal;
	Color mColor;
	Finish mFinish;

	void computeNormal();
};

inline bool operator==(const Triangle& lhs, const Triangle& rhs){
	Point *lhsPt = lhs.getPoints();
	Point *rhsPt = rhs.getPoints();
	return lhsPt[0] == rhsPt[0] &&
		lhsPt[1] == rhsPt[1] && 
		lhsPt[2] == rhsPt[2];
}
inline bool operator!=(const Triangle& lhs, const Triangle& rhs){
	return !(lhs == rhs);
}

