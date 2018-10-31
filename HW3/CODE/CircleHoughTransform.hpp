#ifndef CIRCLEHOUGHTRANSFORM_HPP
#define CIRCLEHOUGHTRANSFORM_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

class CircleHoughTransform
{
private:
	CImg<unsigned char> img1;
	// vector<pair<int, int > > centers;
public:
	CircleHoughTransform(string path, int minR = 30, int maxR = 500, int threshold = 50, int deg_scale = 360);
	vector<int > findPossibleRadius(CImg<unsigned char> img, int minR = 30, int maxR = 500, int threshold = 50, int deg_scale = 360);
	CImg<float> generateCircleHoughSpace(CImg<unsigned char> &img, int r, int interval);
	vector<pair<int, int> > findCircleLocalMaximum(CImg<float> houghSpace, int threshold);
	void drawCircle(CImg<unsigned char> &img, vector<pair<int, int> > center, int r);

};

#endif