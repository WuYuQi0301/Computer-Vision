#include "MySift.h"

int main(int argc, char const *argv[])
{
 	
	char* inpath1 = "../testData/TEST-ImageData(1)/pano1_0008";
	MySift mysift(inpath1, 1);
	mysift.SiftMainProcess();
	mysift.saveImgWithKeypoint("test.bmp");
	return 0;
}