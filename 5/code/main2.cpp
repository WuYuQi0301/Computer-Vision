#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

int main(int argc, char const *argv[])
{
	CImg<unsigned char> img2("../src/4.bmp");
	img2.display();
	return 0;
}