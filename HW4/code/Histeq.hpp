

#ifndef HISTEQ_HPP
#define HISTEQ_HPP

#include <iostream>
#include <string>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

class Histeq
{
public:
CImg<unsigned char> grayHisteq(string path, string root);
CImg<unsigned char> RGBHisteq(string path, string root);

void showHist(float* hist);
CImg<unsigned char> mapping(CImg<unsigned char> img1, float* cdf);
float* getCDF(float* hist);
float* getHist(CImg<unsigned char> &img1, int flag = 4);

CImg<unsigned char> RGB2gray(CImg<unsigned char> img1);

};

#endif