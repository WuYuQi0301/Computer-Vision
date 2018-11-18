#ifndef POINT_HPP
#define POINT_HPP

struct Point
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
};

#endif