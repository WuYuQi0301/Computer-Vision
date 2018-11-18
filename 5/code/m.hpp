#ifndef MORPHING_H
#define MORPHING_H

#include <string>
#include <vector>
#include "Point.h"
#include "Delaunay.h"
#include "CImg.h"

using namespace cimg_library;
using std::string;

class Morphing {
private:
	string path1;
	string path2;
	CImg<unsigned char> img1;
	CImg<unsigned char> img2;
	std::vector<Point> points1;
	std::vector<Point> points2;

	const string pfile1 = "1.txt";
	const string pfile2 = "2.txt";

public:
	// constructor
	Morphing();
	Morphing(string, string);

	void getFeatrues();

	void doMorphing(float, string);

	unsigned char* BilinearFilter(CImg<unsigned char> &src, Point * p);

	Delaunay doTriangle(CImg<unsigned char> &img, std::vector<Point> points);
};

void wtFeaturesToFile(string, string);

std::vector<Point> getFeaturePointsFromTxt(CImg<unsigned char> &img, string filename);

#endif