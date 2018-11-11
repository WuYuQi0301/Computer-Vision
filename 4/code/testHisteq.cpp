#include "ColorProcessing.hpp"

int main(int argc, char const *argv[])
{
	string root = "../result/", rootSrc = "../testdata/";
	ColorProcessing test = ColorProcessing();

	// CImg<unsigned char> img1 = test.grayHisteq("../testdata/river.bmp", (root+"before1.bmp"));
	// img1.display("grayhisteqed");
	// img1.save((root+"gray1.bmp").c_str());

	// CImg<unsigned char> img2 = test.grayHisteq("../testdata/9.bmp", (root+"before2.bmp"));
	// img2.display("grayhisteqed");
	// img2.save((root+"gray2.bmp").c_str());

	// CImg<unsigned char> img3 = test.grayHisteq("../testdata/10.bmp", (root+"before3.bmp"));
	// img3.display("grayhisteqed");
	// img3.save((root+"gray3.bmp").c_str());

	// CImg<unsigned char> img4 = test.grayHisteq("../testdata/sunset1.bmp", (root+"before4.bmp"));
	// img4.display("grayhisteqed");
	// img4.save((root+"gray4.bmp").c_str());

	// CImg<unsigned char> img5 = test.grayHisteq("../testdata/6.bmp", (root+"before5.bmp"));
	// img5.display("grayhisteqed");
	// img5.save((root+"gray5.bmp").c_str());
	
	CImg<unsigned char> img6 = test.RGBHisteq("../testdata/lena.bmp");
	img6.display("RGBhisteqed");
	img6.save((root+"RGB1.bmp").c_str());

	CImg<unsigned char> img7 = test.RGBHisteq("../testdata/3.bmp");
	img7.display("RGBhisteqed");
	img7.save((root+"RGB2.bmp").c_str());

	CImg<unsigned char> img8 = test.RGBHisteq("../testdata/4.bmp");
	img8.display("RGBhisteqed");
	img8.save((root+"RGB3.bmp").c_str());

	CImg<unsigned char> img9 = test.RGBHisteq("../testdata/1.bmp");
	img9.display("RGBhisteqed");
	img9.save((root+"RGB4.bmp").c_str());

	CImg<unsigned char> img10 = test.RGBHisteq("../testdata/7.bmp");
	img10.display("RGBhisteqed");
	img10.save((root+"RGB5.bmp").c_str());
	
	

	
	// CImg<unsigned char> img1 = test.colorTransfer((rootSrc+"sunset1.bmp"), (rootSrc+"sunset2.bmp"));
	// img1.display();
	// img1.save("../result/ct1.bmp");

	// CImg<unsigned char> img2 = test.colorTransfer((rootSrc+"2.bmp"), (rootSrc+"sunset2.bmp"));
	// img2.display();
	// img2.save("../result/ct2.bmp");

	// CImg<unsigned char> img3 = test.colorTransfer((rootSrc+"2.bmp"), (rootSrc+"1.bmp"));
	// img3.display();
	// img3.save("../result/ct3.bmp");

	// CImg<unsigned char> img4 = test.colorTransfer((rootSrc+"1.bmp"), (rootSrc+"2.bmp"));
	// img4.display();
	// img4.save("../result/ct4.bmp");

	// CImg<unsigned char> img5 = test.colorTransfer((rootSrc+"4.bmp"), (rootSrc+"2.bmp"));
	// img5.display();
	// img5.save("../result/ct5.bmp");

	// CImg<unsigned char> img6 = test.colorTransfer((rootSrc+"4.bmp"), (rootSrc+"sunset2.bmp"));
	// img6.display();
	// img6.save("../result/ct6.bmp");
}