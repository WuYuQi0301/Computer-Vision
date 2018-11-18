#ifndef POINT_HPP
#define POINT_HPP

class Point
{
	double x;
	double y;
	int rgb[3];
	Point(double _x = 0, double _y = 0)
	{
		x = _x; 
		y = _y;
		for (int i = 0; i < 3; ++i)
			rgb[i] = 0;
	}

	// Subtract
	Point operator - (const Point& v) const {
		return Point(x - v.x, y - v.y);
	}
	// Cross product
	double Cross(const Point& v) const {
		return (x * v.y - y * v.x);
	}
	// Dot product
	double Dot(const Point& v) const {
		return (x * v.x + y * v.y);
	}


};

// Determine whether two vectors v1 and v2 point to the same direction
// v1 = Cross(AB, AC)
// v2 = Cross(AB, AP)
bool SameSide(const Point &A, const Point &B, const Point &C, const Point &P) {
	Point AB = B - A;
	Point AC = C - A;
	Point AP = P - A;


	double u = (AB.Dot(AB)*AP.Dot(AC) - AC.Dot(AB)*AP.Dot(AB)) / (AC.Dot(AC)*AB.Dot(AB) - AC.Dot(AB)*AB.Dot(AC));
	double v = (AC.Dot(AC)*AP.Dot(AB) - AC.Dot(AB)*AP.Dot(AC)) / (AC.Dot(AC)*AB.Dot(AB) - AC.Dot(AB)*AB.Dot(AC));

	if(u >= 0 && u <= 1 && v >= 0 && v <= 1 && (u+v) <= 1)
		return true;
	else return false;
}

// Same side method
// Determine whether point P in triangle ABC
// 当选择某一条边时，需验证点P与该边所对的点在同一侧
bool PointinTriangle1(const Point &A, const Point &B, const Point &C, const Point &P) {
	return SameSide(A, B, C, P) &&
		SameSide(B, C, A, P) &&
		SameSide(C, A, B, P);
}

#endif