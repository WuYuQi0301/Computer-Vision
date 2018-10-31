#ifndef HOUGHTRANSFORM_HPP
#define HOUGHTRANSFORM_HPP

#include "CImg.h"
#include <iostream>
#include <vector>
#include <string>
using namespace cimg_library;
using namespace std;

class HoughTransform
{
private:
	CImg<unsigned char> img1;

	CImg<float> houghSpace;

	vector<pair<int, int> > lines;
	vector<pair<double, double> > lines_img_space;

	string root;
public:
	HoughTransform(string path, int threshold, string _root);
	CImg<float> generateHoughSpace(CImg<unsigned char> img, int &degreeInterval, int &maxLength, double coef);
	vector<pair<int, int> > findLocalMaximum(CImg<float> houghSpace, int threshold);
	vector<pair<double, double> > drawLines(CImg<unsigned char> &img, vector<pair<int, int> > line_in_hough, int maxLen, float coef);
	void drawRelativeEdgePoint(CImg<unsigned char> &img, vector<pair<double, double> > line);
	void drawPoints(CImg<unsigned char> &img, vector<pair<double, double> > lines);
};


#endif