#include "Histeq.hpp"

Histeq::Histeq(string path, string root, int seq)
{
	img.load_bmp(path.c_str());
	if (img.spectrum() != 1)
		img = RGB2gray(img);
	float* hist = getHist(img);
	showHist(hist);
	float* cdf = getCDF(hist);
	img = mapping(img, cdf);
	img.display("histeqed");
	showHist(getHist(img));
	img.save((root+to_string(seq)+".bmp").c_str());
	delete[] hist;
	delete[] cdf;
 cout << img.spectrum() << endl;
}

float* Histeq::getHist(CImg<unsigned char> &img1)
{
	float* hist = new float[256];
	for (int i = 0; i < 256; ++i)
		hist[i] = 0;
	cimg_forXY(img1, x, y)
	{
		// cout << img1(x, y) << endl;
		hist[img1(x, y)]++;
	}
	int size = img1.width() * img1.height();
	for (int i = 0; i < 256; ++i)
	{
		hist[i] /= size;
	}
	return hist;
}	

float* Histeq::getCDF(float* hist) 
{
	float* cdf = new float[256];
	for (int i = 0; i < 256; ++i)
		cdf[i] = 0;
	for (int i = 1; i < 256; ++i)
	{
		cdf[i] = hist[i] + cdf[i-1];
	}
	cout << "cdf" << cdf[100] << endl;
	return cdf;
}

CImg<unsigned char> Histeq::mapping(CImg<unsigned char> img1, float* cdf)
{
	CImg<unsigned char> result = CImg<unsigned char>(img1.width(), img1.height(), 1,1,0);
	cimg_forXY(img, x, y)
	{
		result(x, y) = (int)255*cdf[img1(x, y)];
	}
	return result;
}

void Histeq::showHist(float* hist)
{
	CImg<unsigned char> fig = CImg<unsigned char>(256, img.height(), 1, 1, 0);
	cimg_forXY(fig, x, y)
	{	
		if (y>(img.height() - 8000*hist[x]))
			fig(x, y) = 255;
	}
	fig.display();
}

CImg<unsigned char> Histeq::RGB2gray(CImg<unsigned char> img1)
{
	CImg<unsigned char> gray = CImg<unsigned char>(img1.width(), img1.height(), 1, 1, 0);
	cimg_forXY(img1, x, y)
	{
		gray(x, y) = (unsigned char)((img1(x, y, 0) * 0.2126 + img1(x, y, 1) * 0.7152 + img1(x, y, 2) * 0.0722));
	}
	return gray;
}