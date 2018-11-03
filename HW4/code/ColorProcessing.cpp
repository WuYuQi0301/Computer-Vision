#include "ColorProcessing.hpp"

CImg<float> ColorProcessing::colorTransfer(string path1, string path2)
{
	CImg<float> img1(path1.c_str()), img2(path2.c_str());
	img1.display("before");
	img2.display("inter");

	//转换颜色空间：RGB到lab
	cout << "rgb2lab..." << endl;
	img1 = rgb2lab(img1);
	img2 = rgb2lab(img2);
// img1.save("lab1.jpg");
// img1.display("lab1");
	//计算统计量
	cout << "calculating mean..." << endl;
	float* mean1 = getMean(img1), *mean2 = getMean(img2);
	cout << "mean1: " << mean1[0] << " " << mean1[1] << " " << mean1[2] << endl;
	cout << "mean2: " << mean2[0] << " " << mean2[1] << " " << mean2[2] << endl;

	cout << "calculating sd..." << endl;
	float* sd_ratio = getStandardDviationRatio(img1, mean1, img2, mean2);
	cout << "sdr: " << sd_ratio[0] << " " << sd_ratio[1] << " " << sd_ratio[2] << endl;

	//颜色迁移
		//1. 将源图像原有的数据减掉源图像的均值
		//2. 得到的新数据按比例放缩，其放缩系数是两幅图像标准方差的比值
		//3. 得到的l’、a’、b’分别加上目标图像三个通道的均值，得到最终数据
	cout << "colorTransfering..." << endl;
	cimg_forXY(img1, x, y)
	{
		img1(x, y, 0) = sd_ratio[0]*(img1(x, y, 0) - mean1[0]) + mean2[0];
		img1(x, y, 1) = sd_ratio[1]*(img1(x, y, 1) - mean1[1]) + mean2[1];
		img1(x, y, 2) = sd_ratio[2]*(img1(x, y, 2) - mean1[2]) + mean2[2];
	}

	//转换颜色空间：lab到RGB
	cout << "lab2rgb..." << endl;
	img1 = lab2rgb(img1);

	delete[] mean1;
	delete[] mean2;
	delete[] sd_ratio;
	return img1;
}
CImg<float> ColorProcessing::rgb2lab(CImg<float> img)
{
	float matrix1[3][3] = {{0.3811, 0.5783, 0.0402},{0.1967, 0.7244, 0.0782},{0.0241, 0.1288, 0.8444}};
	float matrix2[3][3] = {{1, 1, 1},{1, 1,-2},{1,-1, 0}};
	float matrix3[3][3] = {{1/sqrt(3), 0, 0},{0, 1/sqrt(6), 0},{0, 0, 1/sqrt(2)}};
	cimg_forXY(img, x, y)
	{
		float LMS[3] = {0}, temp[3] = {0}, lab[3] = {0};
		//rgb 2 lms
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				LMS[i] += matrix1[i][j] * img(x, y, j); 
			}
			LMS[i]  = (LMS[i] == 0)?1:log10(LMS[i]);
		}
		//lms 2 lab
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				temp[i] += matrix2[i][j] * LMS[j]; 
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				lab[i] += matrix3[i][j] * temp[j]; 
			}
			img(x, y, i) = lab[i];
		}
	}
	return img;
}
CImg<float> ColorProcessing::lab2rgb(CImg<float> img)
{
	float matrix1[3][3] = {{4.4679, -3.5873, 0.1193},{-1.2186, 2.3809, -0.1624},{0.0497, -0.2439,  1.2045}};
	float matrix2[3][3] = {{1, 1, 1},{1, 1,-1},{1,-2, 0}};
	float matrix3[3][3] = {{sqrt(3)/3, 0, 0},{0, sqrt(6)/6, 0},{0, 0, sqrt(2)/2}};
	cimg_forXY(img, x, y)
	{
		float LMS[3] = {0}, temp[3] = {0}, rgb[3] = {0};
		//lab 2 lms
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				temp[i] += matrix3[i][j] * img(x, y, j); 
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				LMS[i] += matrix2[i][j] * temp[j]; 
			}
			LMS[i] = pow(10, LMS[i]);
		}
		//lms 2 rgb
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				rgb[i] += matrix1[i][j] * LMS[j]; 
			}
			img(x, y, i) = rgb[i];
		}
	}
	return img;
}
float* ColorProcessing::getMean(CImg<float> img)
{
	float* mean = new float[3];
	for (int i = 0; i < 3; ++i)
		mean[i] = 0;
	cimg_forXY(img, x, y)
	{
	for (int i = 0; i < 3; ++i)
		mean[i] += img(x, y, i);	
	}
	for (int i = 0; i < 3; ++i)
		mean[i] /= (img.height()*img.width());
	// for (int i = 0; i < img.height(); ++i)
	// {
	// 	double row_mean[3] = {0};
	// 	for (int k = 0; k < 3; ++k)
	// 		row_mean[k] = 0;

	// 	for (int j = 0; j < img.width(); ++j)
	// 		for (int k = 0; k < 3; ++k)
	// 			row_mean[k] += img(j, i, k);
		
	// 	for (int k = 0; k < 3; ++k)
	// 		mean[k] += (row_mean[k]/img.width());
	// }

	// for (int k = 0; k < 3; ++k)
	// 	mean[k] /= img.height();
	
	return mean;
}
float* ColorProcessing::getStandardDviationRatio(CImg<float> img1, float* mean1, CImg<float> img2, float* mean2)
{
	float* ratio = new float[3];
	float sd1[3] = {0}, sd2[3] = {0};

	cimg_forXY(img1, x, y)
	{
		for (int i = 0; i < 3; ++i)
		{
			sd1[i] += pow(img1(x, y, i)- mean1[i], 2);
		}
	}
	
	for (int i = 0; i < 3; ++i)
		sd1[i] = sqrt(sd1[i]/(img1.width()*img1.height() - 1));

	cout << "sd " << sd1[0]<< " " << sd1[1] << " " << sd1[2] << endl;

	cimg_forXY(img2, x, y)
	{
		for (int i = 0; i < 3; ++i)
			sd2[i] += pow(img2(x, y, i)- mean2[i], 2);     //计算像素值与均值差的平方和
	}
	
	for (int i = 0; i < 3; ++i)                            //计算1/N 和开方
		sd2[i] = sqrt( sd2[i] /(img2.width()*img2.height() - 1));

	cout << "sd " << sd2[0]<< " " << sd2[1] << " " << sd2[2] << endl;
	for (int i = 0; i < 3; ++i)
		ratio[i] = sd2[i]/sd1[i];
	return ratio;
} 


CImg<unsigned char> ColorProcessing::grayHisteq(string path, string root)
{
	cout << "gray histgram" << endl;
	CImg<unsigned char> img1(path.c_str());
	if (img1.spectrum() != 1)
		img1 = RGB2gray(img1);

	float* hist = getHist(img1, 4);
	showHist(hist);

	float* cdf = getCDF(hist);
	cimg_forXY(img1, x, y)
	{
		img1(x, y) = (int)255*cdf[img1(x, y)];
	}
	float* rhist = getHist(img1, 4);
	showHist(rhist);

	delete[] rhist;
	delete[] hist;
	delete[] cdf;
	return img1;
}
CImg<unsigned char> ColorProcessing::RGBHisteq(string path, string root)
{
	cout << "rgb histgram" << endl;
	CImg<unsigned char> img1(path.c_str());

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

float* ColorProcessing::getHist(CImg<unsigned char> &img1, int flag)
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

float* ColorProcessing::getCDF(float* hist) 
{
	float* cdf = new float[256];
	for (int i = 1; i < 256; ++i)
		cdf[i] = hist[i] + cdf[i-1];
	cout << "cdf" << cdf[100] << endl;
	return cdf;
}

void ColorProcessing::showHist(float* hist)
{
	CImg<unsigned char> fig = CImg<unsigned char>(256, 300, 1, 1, 0);
	cimg_forXY(fig, x, y)
	{	
		if (y>(300 - 1500*hist[x]))
			fig(x, y) = 255;
	}
	fig.display();
}

CImg<unsigned char> ColorProcessing::RGB2gray(CImg<unsigned char> img1)
{
	CImg<unsigned char> gray = CImg<unsigned char>(img1.width(), img1.height(), 1, 1, 0);
	cimg_forXY(img1, x, y)
	{
		gray(x, y) = (unsigned char)((img1(x, y, 0) * 0.2126 + img1(x, y, 1) * 0.7152 + img1(x, y, 2) * 0.0722));
	}
	return gray;
}