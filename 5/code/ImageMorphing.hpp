#ifndef IMAGEMORPHING_HPP
#define IMAGEMORPHING_HPP

#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

class ImageMorphing
{
public:
	ImageMorphing();
	~ImageMorphing();
	CImg<unsigned char> scale(CImg<unsigned char>img, int w2, int h2);
	void loadControlPoint(CImg<unsigned char>img, string path);
	
};


#endif