#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

struct Point
{
	double x;
	double y;
	double rgb[3];
	Point(double _x = 0, double _y = 0, double _r = 0, double _g = 0, double _b = 0)
	{
		x = _x;
		y = _y;
		rgb[0] = _r;
		rgb[1] = _g;
		rgb[2] = _b;
	}
};

int main(int argc, char const *argv[])
{
	CImg<unsigned char> img1("../src/2.bmp");
	CImg<unsigned char> img2("../src/1.bmp");
	CImg<unsigned char> img3 = CImg<unsigned char>(img2.width(), img2.height(), 1, 3, 0);

	cout << img1.height() << " " << img1.width() << endl;
	cout << img2.height() << " " << img2.width() << endl;

	double w1 = img1.width(), h1 = img1.height(), w2 = img2.width(), h2 = img2.height();
	double a = w1/w2, b = h1/h2;
	cout << a << " " << b <<endl;

	cimg_forXY(img3, x, y)
	{
		double _x = x*a, _y = y*b;
			if (y == 57 && x == 100) cout << _x << " " << _y << endl;

		Point p1(floor(_x), floor(_y)), p2(ceil(_x), floor(_y));
		Point p3(floor(_x), ceil(_y)), p4(ceil(_x), ceil(_y));
		// cout <<"y"<< p1.y <<" " <<p4.y << floor(_y) << endl;

		Point p5(_x, floor(_y)), p6(_x, ceil(_y));

		if(p1.x < 0 || p1.x >= img1.width() || p4.x < 0 || p4.x >= img1.width()
			|| p1.y < 0 || p1.y >= img1.height() || p4.y < 0 || p4.y >= img1.height())
		{
			cout << "out of range" << endl;
			continue;
		}
		if (_x == p1.x)
		{
			if (_y == p1.y)
			{
				for (int i = 0; i < 3; ++i)
					img3(x, y, i) = img1(p1.x, p1.y, i);
			} else {  //p1p3 y插值一次 
				for (int i = 0; i < 3; ++i)
					img3(x, y, i) = ((_y - p1.y)/(p3.y-p1.y))*img1(p1.x, p1.y, i) 
									 + ((p3.y-_y)/(p3.y-p1.y))*img1(p3.x, p3.y, i);
			}
		}
		else if (_x == p2.x)
		{
			if( _y == p2.y)
			{
				for (int i = 0; i < 3; ++i)
					img3(x, y, i) = img1(p2.x, p2.y, i);
			} else {  //p2p4 y插值一次
				for (int i = 0; i < 3; ++i)
					img3(x, y, i) = ((_y-p2.y)/(p4.y-p2.y))*img1(p2.x, p2.y, i) 
								  + ((p4.y-_y)/(p4.y-p2.y))*img1(p4.x, p4.y, i);
			}
		}
		else if (_y == p1.y)  // p1p2 x插值一次
		{
			for (int i = 0; i < 3; ++i)
				img3(x, y, i) = ((_x-p1.x)/(p2.x-p1.x))*img1(p1.x, p1.y, i) 
							  + ((p2.x-_x)/(p2.x-p1.x))*img1(p2.x, p2.y, i);
		}
		else if (_y == p3.y)  // p3p4 x 插值一次
 		{
			for (int i = 0; i < 3; ++i)
				img3(x, y, i) = ((_x-p3.x)/(p4.x-p3.x))*img1(p3.x, p3.y, i) 
							  + ((p4.x-_x)/(p4.x-p3.x))*img1(p4.x, p4.y, i);
		}
		else        //并不落在任何两非对角点连成的直线上
		{
			for (int i = 0; i < 3; ++i)
			{
				/* code */
			// cout << "origin" << i << " " << p4.x << " " << p4.y << " " << img1((int)p4.x, (int)p4.y, i) << " " << img1((int)p3.x, (int)p3.y, i) 
				 // << " " << img1(p2.x, p2.y, i) << " "  << img1(p1.x, p1.y, i) <<endl;
				p5.rgb[i] = ((p4.x - _x)/(p4.x - p3.x)) * img1(p4.x, p4.y, i) + ((_x - p3.x)/(p4.x - p3.x)) * img1(p3.x, p3.y, i);
				p6.rgb[i] = ((p2.x - _x)/(p2.x - p1.x)) * img1(p2.x, p2.y, i) + ((_x - p1.x)/(p2.x - p1.x)) * img1(p1.x, p1.y, i);
				// cout<< "p5p6" << p5.rgb[i] << " " <<p6.rgb[i] << endl;
				img3(x, y, i) = (int)( ((p5.y - _y)/(p5.y - p6.y)) * p5.rgb[i] + ((_y - p6.y)/(p5.y - p6.y)) * p6.rgb[i]);
			// cout << img3(x, y, i) << " ";
			}
		}
		// cout<<endl;
			// if (y == 57 && x == 100)
			// {
			// 	cout << "rgb after : " <<  img3(x, y, 0) << " " 
			// 		 <<img3(x, y, 1) << " " << img3(x, y, 2) <<endl;
			// 	cout << "p1 " << p1.x << " " << p1.y << endl << img1(p1.x, p1.y, 0) << endl;

			// 	cout << "p2 " << p2.x << " " << p2.y << endl << img1(p2.x, p2.y, 0) << " " 
			// 		 << img1(p2.x, p2.y, 1) << " " << img1(p2.x, p2.y, 2) << endl;

			// 	cout << "p3 " << p3.x << " " << p3.y << endl << img1(p3.x, p3.y, 0) << " " 
			// 		 << img1(p3.x, p3.y, 1) << " " << img1(p3.x, p3.y, 2) << endl;
			// 	cout << "p4 " << p4.x << " " << p4.y << endl << img1(p4.x, p4.y, 0) << " " 
			// 		 << img1(p4.x, p4.y, 1) << " " << img1(p4.x, p4.y, 2) << endl;
			// 	cout << "p5 " << p5.x << " " << p5.y << endl << p5.rgb[0] << " " 
			// 		 << p5.rgb[1] << " " << p5.rgb[2] << endl;
			// 	cout << "p6 " << p6.x << " " << p6.y << endl << p6.rgb[0] << " " 
			// 		 << p6.rgb[1] << " " << p6.rgb[2] << endl;
			// }
	}

	img1.display();
	// img2.display();
	img3.display();
	return 0;
}