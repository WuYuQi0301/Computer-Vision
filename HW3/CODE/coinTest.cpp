#include "CircleHoughTransform.hpp"

int main(int argc, char const *argv[])
{
	if (argc == 5)
		CircleHoughTransform circleHT1("../coins/cannyed.bmp", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
// generateHoughSpace(CImg<unsigned char> img, int minR, int maxR,int threshold, int deg_scale)
// CircleHoughTransform(string path, int minR, int maxR, int threshold, int deg_scale)
	
	// CircleHoughTransform circleHT2("../coins/2Thresholded.bmp");
	// CircleHoughTransform circleHT3("../coins/5Thresholded.bmp");
	// circleHT1.generateHoughSpace();
	return 0;
}