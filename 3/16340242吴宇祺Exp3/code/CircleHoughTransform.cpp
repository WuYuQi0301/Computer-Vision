#include "CircleHoughTransform.hpp"
CircleHoughTransform::CircleHoughTransform(string path, int minR, int maxR, int threshold, int deg_scale)
{
	img1.load_bmp(path.c_str());
	img1.display();

	vector<int > radius = findPossibleRadius(img1, minR, maxR, threshold);
	int count = 0;
	for (int i = 0; i < radius.size(); ++i)
	{
		CImg<float> houghSpace = generateCircleHoughSpace(img1, radius[i], deg_scale);
		vector<pair<int, int > > centers = findCircleLocalMaximum(houghSpace, threshold);
		count += centers.size();
		drawCircle(img1, centers, radius[i]);
	}
	img1.display("houghed");
	img1.save("../result/circle/test.bmp");
	cout << "Total number of coins = " << count << endl;
}
vector<int> CircleHoughTransform::findPossibleRadius(CImg<unsigned char> img, int minR, int maxR,int threshold, int deg_scale)
{
	cout << "finding Possible Radius...";
	int col = img.width(), row = img.height();
	//设定阈值查找可能的r
	float coef = 2*M_PI / deg_scale;
	vector<int> rs;
	for (int r = minR; r < maxR; r+=5)
	{
		//生成霍夫空间
		CImg<float> houghSpace = generateCircleHoughSpace(img, r, deg_scale);

		//搜索当前r的最大投票值
		float max_vote = 0;
		int a = 0, b = 0;
		cimg_forXY(houghSpace, x, y)
		{
			if(houghSpace(x, y) > max_vote)
			{
				a = x;
				b = y;
				max_vote = houghSpace(x, y);
			}
		}
		if (max_vote > threshold) //或者排序取最大值？或者pi*r?
			rs.push_back(r);
	}
	cout << "done" << endl;
	return rs;
}

CImg<float> CircleHoughTransform::generateCircleHoughSpace(CImg<unsigned char> &img, int r, int deg_scale)
{
	int col = img.width(), row = img.height();
	float coef = 2*M_PI / deg_scale;
	CImg<float> houghSpace = CImg<float>(col, row, 1, 1, 0);
	cimg_forXY(img, x, y)
	{
		if (img(x, y) == 0)
			continue;
		for (int j = 0; j < deg_scale; j++)
		{
			float a = x - r * cos(coef*j);
			float b = y - r * sin(coef*j);
			if (a >= 0 && a < col && b >= 0 && b < row)
				houghSpace(a, b) = (houghSpace(a, b) == 255) ? 255:houghSpace(a, b)+1;
		}
	}

	return houghSpace;
}

vector<pair<int, int> > CircleHoughTransform::findCircleLocalMaximum(CImg<float> hough, int threshold)
{
	//设定阈值，提取局部最大值
	cout << "finding local maximun ...";
	vector<pair<int, int> > center;
	for (int i = 0; i < hough.height(); ++i)
	{
		for (int j = 0; j < hough.width(); ++j)
		{
			if (hough.atXY(j, i) < threshold)
				continue;

			bool newCenter = true;
			for (int k = 0; k < center.size(); ++k)
			{
				if (sqrt(pow(center[k].first - j, 2) + pow(center[k].second - i, 2)) < 10)
				{
					newCenter = false;
					if (hough.atXY(center[k].first, center[k].second) < hough.atXY(j, i))
					{
						center[k].first = j;
						center[k].second = i;
					}
					break;
				}
			}
			if (newCenter)
				center.push_back(pair<int, int>(j, i));
		}
	}
	
	return center;
}

void CircleHoughTransform::drawCircle(CImg<unsigned char> &img, vector<pair<int, int> > center, int r)
{
	unsigned char blue[] = {0, 0, 255};
	for (int i = 0; i < center.size(); ++i)
		img.draw_circle(center[i].first, center[i].second, r, blue, 0.5);
}