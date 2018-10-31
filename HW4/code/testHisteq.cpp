#include "Histeq.hpp"

int main(int argc, char const *argv[])
{
	string root = "../result/" ;
	Histeq test = Histeq();
	
	CImg<unsigned char> img1 = test.grayHisteq("../testdata/river.bmp", root);
	
	img1.display("grayhisteqed");
	img1.save((root+"gray1.bmp").c_str());
	
	CImg<unsigned char> img2 = test.RGBHisteq("../testdata/lena.bmp", root);
	img2.display("RGBhisteqed");
	img2.save((root+"RGB1.bmp").c_str());
	return 0;
}