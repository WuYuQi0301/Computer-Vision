#include "Histeq.hpp"

CImg<unsigned char> Histeq::grayHisteq(string path, string root)
{
	cout << "gray histgram" << endl;
	CImg<unsigned char> img1;
	img1.load_bmp(path.c_str());

	if (img1.spectrum() != 1)
		img1 = RGB2gray(img1);
	cout << "gray histgram" << endl;

	float* hist = getHist(img1, 4);
	cout << "gray histgram" << endl;
	showHist(hist);
	cout << "gray histgram" << endl;

	float* cdf = getCDF(hist);
	cimg_forXY(img1, x, y)
	{
		img1(x, y) = (int)255*cdf[img1(x, y)];
	}
	cout << "gray histgram" << endl;
	float* rhist = getHist(img1, 4);
	showHist(rhist);

	delete[] rhist;
	delete[] hist;
	delete[] cdf;
	return img1;
}
CImg<unsigned char> Histeq::RGBHisteq(string path, string root)
{
	cout << "rgb histgram" << endl;
	CImg<unsigned char> img1;
	img1.load_bmp(path.c_str());

	float* hists[3], *cdfs[3];
	for (int i = 0; i < 3; ++i)
	{
		hists[i] = getHist(img1, i);
		cdfs[i] = getCDF(hists[i]);
	}
	cimg_forXY(img1, x, y)
	{
		img1(x, y, 0) = (unsigned char)255*cdfs[0][img1(x, y, 0)];
		img1(x, y, 1) = (unsigned char)255*cdfs[1][img1(x, y, 1)];
		img1(x, y, 2) = (unsigned char)255*cdfs[2][img1(x, y, 2)];
	}

	for (int i = 0; i < 3; ++i)
	{
		float* temp = hists[i];
		delete[] temp;
		temp = cdfs[i];
		delete[] temp;
	}
	return img1;
}


float* Histeq::getHist(CImg<unsigned char> &img1, int flag)
{
	float* hist = new float[256];
	for (int i = 0; i < 256; ++i)
		hist[i] = 0;
	switch (flag)
	{
		case 4:
			cimg_forXY(img1, x, y)
			{
				hist[img1(x, y)]++;
			}
			break;
		default:
			cimg_forXY(img1, x, y)
			{
				hist[img1(x, y, flag)]++;
			}			
	}
	int size = img1.width() * img1.height();
	for (int i = 0; i < 256; ++i)
		hist[i] /= size;
	return hist;
}	

float* Histeq::getCDF(float* hist) 
{
	float* cdf = new float[256];
	for (int i = 1; i < 256; ++i)
		cdf[i] = hist[i] + cdf[i-1];
	cout << "cdf" << cdf[100] << endl;
	return cdf;
}

CImg<unsigned char> Histeq::mapping(CImg<unsigned char> img1, float* cdf)
{
	CImg<unsigned char> result = CImg<unsigned char>(img1.width(), img1.height(), 1,1,0);

	return result;
}

void Histeq::showHist(float* hist)
{
	CImg<unsigned char> fig = CImg<unsigned char>(256, 300, 1, 1, 0);
	cimg_forXY(fig, x, y)
	{	
		if (y>(300 - 1500*hist[x]))
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