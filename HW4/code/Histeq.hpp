

#ifndef HISTEQ_HPP
#define HISTEQ_HPP

#include <iostream>
#include <string>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

class Histeq
{
private:
	CImg<unsigned char> img;
public:
Histeq(string path, string root, int seq);

void showHist(float* hist);
CImg<unsigned char> mapping(CImg<unsigned char> img1, float* cdf);
float* getCDF(float* hist);
float* getHist(CImg<unsigned char> &img1);

CImg<unsigned char> RGB2gray(CImg<unsigned char> img1);

};

#endif