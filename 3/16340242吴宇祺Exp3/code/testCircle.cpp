#include "CircleHoughTransform.hpp"

int main(int argc, char const *argv[])
{
	if (argc == 5)
		CircleHoughTransform circleHT1("../coins/cannyed.bmp", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	return 0;
}