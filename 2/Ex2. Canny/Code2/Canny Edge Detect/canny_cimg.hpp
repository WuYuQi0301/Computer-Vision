#ifndef CANNY_CIMG_HPP
#define CANNY_CIMG_HPP

#include "stdafx.hpp"
using namespace std;
using namespace cimg_library;

class canny_cimg
{
private:
    CImg<unsigned char> img;           //原始图像
    CImg<unsigned char> grayscaled;    //灰度图
    CImg<unsigned char> gFiltered;     //梯度
    CImg<unsigned char> sFiltered;     //sobel滤波
    CImg<unsigned char> angles;        //angle map
    CImg<unsigned char> non;           //非极大值抑制
    CImg<unsigned char> thres;         //双阈值处理结果

public:
    canny_cimg(string filename, int filter_size = 3,int sigmaIn = 1, int low = 20, int high = 40);
    CImg<unsigned char> toGrayScale();
    void createFilter(vector<vector<double> > &, int, int, double);                       //用于平滑的高斯滤波器
    CImg<unsigned char> useFilter(CImg<unsigned char>, vector<vector<double> >);
    CImg<unsigned char> sobel();
    CImg<unsigned char> nonMaxSupp();
    CImg<unsigned char> threshold(CImg<unsigned char>, int, int);
};

#endif