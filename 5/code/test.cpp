#include "ImageMorphing.hpp"
int main(int argc, char const *argv[])
{
	string p1 = "../src/2.bmp", p2 = "../src/4.bmp", rp = "../result/";
	ImageMorphing im(p1, p2, rp);
	return 0;
}