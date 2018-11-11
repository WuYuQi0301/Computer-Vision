#ifndef COLORPROCESSING_HPP
#define COLORPROCESSING_HPP

#include <iostream>
#include <string>
#include "CImg.h"
using namespace std;
using namespace cimg_library;


class ColorProcessing
{
private:
	//直方图均衡子函数
	CImg<unsigned char> RGB2gray(CImg<unsigned char> img1);
	float* getCDF(float* hist);
	float* getHist(CImg<unsigned char> &img1, int flag = 4);
	void showHist(float* hist);

	//颜色迁移子函数
	CImg<float> rgb2lab(CImg<float>);
	CImg<float> lab2rgb(CImg<float>);
	float* getMean(CImg<float> img);
	float* getStandardDviationRatio(CImg<float> img1, float* mean1, CImg<float> img2, float* mean2);

public:
	CImg<unsigned char> grayHisteq(string path, string root);
	CImg<unsigned char> RGBHisteq(string path);
	CImg<float> colorTransfer(string path1, string path2);
	
};

#endif