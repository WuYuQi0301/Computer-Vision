#include "canny_cimg.hpp"

int main(int argc, char const *argv[])
{
	string path = "./test_Data/lena.bmp";
// canny_cimg::canny_cimg(string filename, int filter_size,int sigmaIn, int low, int high)
	canny_cimg myCanny(path);

	return 0;
}