#ifndef PAINT_HPP
#define PAINT_HPP value
#include "CImg.h"
#include <cmath>
#include <string>
using namespace cimg_library;
using namespace std;

class Paint
{
private:
	CImg<unsigned char> SrcImg;

public:
	Paint();
	void Display();
	void Save(string fname);
	void ChangeColor();

	void BlueCircle();
	void MyBlueCircle();

	void YellowCircle();
	void MyellowCircle();

	void BlueLine();
	void MyBlueLine();
};

#endif