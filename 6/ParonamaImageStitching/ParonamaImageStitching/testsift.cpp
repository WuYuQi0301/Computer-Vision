
#include "mySift.h"
//#include "CImg.h"

int main()
{
	//CImg<float> test("testData/pano1_0008.bmp");
	//test.display();
	cout << "sift demo" << endl;

	string name1 = "pano1_0008", name2 = "pano1_0009", name3 = "pano1_0010";
	string imgPath = "testData/", si = "result/", sp = "keyPointSet/";

	mySift sift1;
	if (!sift1.SiftProcessing(name1, imgPath, sp, si))
		cout << "error1" << endl;
	if (!sift1.SiftProcessing(name2, imgPath, sp, si))
		cout << "error2" << endl;
	if (!sift1.SiftProcessing(name3, imgPath, sp, si))
		cout << "error3" << endl;


	return 0;
}