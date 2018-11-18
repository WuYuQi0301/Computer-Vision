#include <iostream>
#include <stringstream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

int main(int argc, char const *argv[])
{
	CImg<unsigned char> img1("../src/2.bmp");
	img1.display();

	vector<Point> pset;

	return 0;
}