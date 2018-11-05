#include "HoughTransform.hpp"

int main(int argc, char const *argv[])
{
	if (argc == 3)  //choose paper and threshold
	{
		string srcpath =  argv[1];
		srcpath = ("../papers/" + srcpath + "Thresholded.bmp");
		HoughTransform test1(srcpath, atoi(argv[2]), "../result/line/");
	}
	return 0;
}