#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

int main(int argc, char const *argv[])
{
	CImg<float> img("../testdata/3.bmp");

	cimg_forXY(img, x, y)
	{
	    float matrix1[3][3] = {{0.3811, 0.5783, 0.0402},{0.1967, 0.7244, 0.0782},{0.0241, 0.1288, 0.8444}};
		float matrix2[3][3] = {{1, 1, 1},{1, 1,-2},{1,-1, 0}};
		float matrix3[3][3] = {{1/sqrt(3), 0, 0},{0, 1/sqrt(6), 0},{0, 0, 1/sqrt(2)}};
		
		float LMS[3] = {0}, temp[3] = {0}, lab[3] = {0};
		//rgb 2 lms
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				LMS[i] += matrix1[i][j] * img(x, y, j);
				if (x == 255 && y == 255)
				{
					cout << matrix1[i][j] << " " ;
				}
			}
			if (x == 255 && y == 255)
				cout << endl;
			LMS[i]  = log10(LMS[i]);
		}
		if (x == 255 && y == 255)
		{
			for (int i = 0; i < 3; i++)
			{
				cout << LMS[i] << " " << endl;
			}
			cout << endl;
		}
			//lms 2 lab
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				temp[i] += matrix2[i][j] * LMS[j];
				if (x == 255 && y == 255)
					cout << matrix2[i][j] << " " ;
			}
			if (x == 255 && y == 255)
				cout << endl ;
		}

		if (x == 255 && y == 255)
		{
			for (int i = 0; i < 3; i++)
			{
				cout << temp[i] << " " << endl;
			}
			cout << endl;
		}
		
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				lab[i] += matrix3[i][j] * temp[j]; 
			}
			img(x, y, i) = lab[i];
		}

		if (x == 255 && y == 255)
		{
			cout << "in lab" << endl;
			for (int i = 0; i < 3; i++)
			{
				cout << img(x, y, i) << " " << endl;
			}
			cout << endl;
		}
	}
	float matrix4[3][3] = {{4.4679, -3.5873, 0.1193},{-1.2186, 2.3809, -0.1624},{0.0497, -0.2439,  1.2045}};
	float matrix5[3][3] = {{1, 1, 1},{1, 1,-1},{1,-2, 0}};
	float matrix6[3][3] = {{sqrt(3)/3, 0, 0},{0, sqrt(6)/6, 0},{0, 0, sqrt(2)/2}};
	cimg_forXY(img, x, y)
	{
		float LMS[3] = {0}, temp[3] = {0}, rgb[3] = {0};
		//lab 2 lms
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				temp[i] += matrix6[i][j] * img(x, y, j); 
				if (x == 255 && y == 255)
					cout << matrix6[i][j] << " " ;
			}
			if (x == 255 && y == 255)
				cout << endl ;
		}
		if (x == 255 && y == 255)
		{
			for (int i = 0; i < 3; i++)
			{
				cout << temp[i] << " " << endl;
			}
			cout << endl;
		}

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				LMS[i] += matrix5[i][j] * temp[j]; 
				if (x == 255 && y == 255)
					cout << matrix5[i][j] << " " ;
			}
			if (x == 255 && y == 255)
				cout << endl ;

		}
		if (x == 255 && y == 255)
		{
			cout << "LMS " << endl;
			for (int i = 0; i < 3; i++)
				cout << LMS[i] << " " << endl;
			cout << endl;
		}

		for (int i = 0; i < 3; i++)
			LMS[i] = pow(10, LMS[i]);
		if (x == 255 && y == 255)
		{
			cout << "LMS '" << endl;
			for (int i = 0; i < 3; i++)
				cout << LMS[i] << " " << endl;
			cout << endl;
		}
		if (x == 255 && y == 255)
		{
			cout << "LMS ' " << endl;
			for (int i = 0; i < 3; i++)
				cout << LMS[i] << " " << endl;
			cout << endl;
		}
		//lms 2 rgb
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				rgb[i] += matrix4[i][j] * LMS[j]; 
				if (x == 255 && y == 255)
					cout << matrix4[i][j] << " " ;
			}
			if (x == 255 && y == 255)
				cout << endl ;
			img(x, y, i) = (int)rgb[i];
		}
	}



	CImg<float> img2("../testdata/3.bmp");
	cout << img(255, 255, 0) << " " << img(255, 255, 1) << " " << img(255, 255, 2) << endl;
	cout << img2(255, 255, 0) << " " << img2(255, 255, 1) << " " << img2(255, 255, 2) << endl << endl << endl;

	img2.display();
	img.display();
	img = img - img2;
	img.display();
	return 0;
}