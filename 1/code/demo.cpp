#include "CImg.h"
#include <cmath>
#include <iostream>
using namespace cimg_library;
using namespace std;

void test_cimg()
{
	//read in bmp image
	CImg<unsigned char> SrcImg;
	SrcImg.load_bmp("1.bmp");

	//show image
	SrcImg.display();





	//show
	SrcImg.display();

	//save as a new bmp
	SrcImg.save("T1.bmp");

}

int main(int argc, char const *argv[])
{
	test_cimg();
	return 0;
}