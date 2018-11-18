#ifndef IMAGEMORPHING_HPP
#define IMAGEMORPHING_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
// #include "CImg.h"
#include "Delaunay.hpp"
#include "Matrix.h"
using namespace std;
using namespace cimg_library;



class ImageMorphing
{
private:
	vector<Point> pset1;
	vector<Point> pset2;

public:
	ImageMorphing(string spath1, string spath2, string rpath);
	CImg<unsigned char> scale(CImg<unsigned char>img, int w2, int h2);
	vector<Point> loadControlPoint(CImg<unsigned char>img, string path);
	Delaunay triangleMesh(CImg<unsigned char> img, vector<Point> pset);
	unsigned char* BilinearFilter(CImg<unsigned char> &src, double _x, double _y);

};


#endif