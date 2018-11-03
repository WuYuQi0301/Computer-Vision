#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

int main(int argc, char const *argv[])
{
	CImg<float> img("../testdata/2.bmp");

	// int x = 123, y = 400;

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
			}
			LMS[i]  = log10(LMS[i]);
		}

		// cout << LMS[0] << " " << LMS[1] << " " << LMS[2] << endl;
			//lms 2 lab
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				temp[i] += matrix2[i][j] * LMS[j]; 
			}
		}
// 
		// for (int i = 0; i < 3; ++i)
		 // {
		 	// cout << temp[i] << " ";
		 // } 
		 // cout << endl;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				lab[i] += matrix3[i][j] * temp[j]; 
			}
			img(x, y, i) = lab[i];
		}
	}
    // cout << img(x, y, 0) << " " << img(x, y, 1) << " " << img(x, y, 2) << endl;


    // cout << img(x, y, 0) << " " << img(x, y, 1) << " " << img(x, y, 2) << endl;
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
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				LMS[i] += matrix5[i][j] * temp[j]; 
			}
			LMS[i] = pow(LMS[i], 10);
		}
		//lms 2 rgb
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				rgb[i] += matrix4[i][j] * LMS[j]; 
			}
			img(x, y, i) = rgb[i];
		}
	}
	CImg<float> img2("../testdata/2.bmp");
	
	img = img - img2;
	img.display();
	img2.display();
	return 0;
}