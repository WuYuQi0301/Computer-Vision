#ifndef COPY_HPP
#define COPY_HPP

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

class canny
{
private:
    CImg<unsigned char> img;           //原始图像
    CImg<unsigned char> grayscaled;    //灰度图
    CImg<unsigned char> gFiltered;     //梯度
    CImg<unsigned char> sFiltered;     //sobel滤波
    CImg<unsigned char> angles;        //angle map
    CImg<unsigned char> non;           //非极大值抑制
    CImg<unsigned char> thres;         //双阈值处理结果
    CImg<unsigned char> final;         //删除短边缘结果

public:
    canny(string filename, string root,int num , int filter_size = 3,int sigmaIn = 2, int low = 40, int high = 60, int note = 0);
    ~canny() { }
    CImg<unsigned char> toGrayScale();
    vector<vector<double> > createFilter(int, int, double);                       //用于平滑的高斯滤波器
    CImg<unsigned char> useFilter(CImg<unsigned char>, vector<vector<double> >);
    CImg<unsigned char> sobel();
    CImg<unsigned char> nonMaxSupp();
    CImg<unsigned char> threshold(CImg<unsigned char>, int, int);

    CImg<unsigned char> shortEdgeSupp(CImg<unsigned char> in, int minLength = 20);
};

#endif