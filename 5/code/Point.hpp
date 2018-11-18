#ifndef POINT_HPP
#define POINT_HPP

class Point
{
public:
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
	// double Cross(const Point& v) const {
	// 	return (x * v.y - y * v.x);
	// }
	// Dot product
	double Dot(const Point& v) const {
		return (x * v.x + y * v.y);
	}
};

// Determine whether two vectors v1 and v2 point to the same direction
// v1 = Cross(AB, AC)
// v2 = Cross(AB, AP)

#endif