#pragma once

#include <iostream>  
#include <fstream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/opencv.hpp>
#include <string>

extern "C" {
#include "vl/generic.h"
#include "vl/slic.h"
#include "vl/sift.h"
}

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::fstream;
using std::ios;
using namespace cv;

class mySift
{
public:
	mySift();
	bool SiftProcessing(string imgName, string imgPath, string spPath, string siPath);
private:
};