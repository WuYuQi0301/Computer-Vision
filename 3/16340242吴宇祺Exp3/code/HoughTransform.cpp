#include "HoughTransform.hpp"

HoughTransform::HoughTransform(string path, int threshold, string _root)
{
	this->root = _root;
	//读取图像
	img1.load_bmp(path.c_str());
	// img2.load_bmp(path.c_str());
	int w = img1.width();
	int h = img1.height();
	//量化theta
	int degreeInterval = 600, maxLength = sqrt(w*w + h*h)*3/2, gol;
	double coef = 2 * M_PI / degreeInterval;

	//生成霍夫空间
	houghSpace = generateHoughSpace(img1, degreeInterval, maxLength, coef);
	//查找霍夫空间中的局部极大值
	lines = findLocalMaximum(houghSpace, threshold);
	//将霍夫空间的theta和r反变换并绘制直线，I2
	lines_img_space = drawLines(img1, lines, maxLength, coef);
	//染色相关边缘点  I3
	drawRelativeEdgePoint(img1, lines_img_space);
	//计算并染色直线交点 I4
	drawPoints(img1, lines_img_space);
}

CImg<float> HoughTransform::generateHoughSpace(CImg<unsigned char> img, int& interval, int& maxLen, double coef)
{
	//初始化霍夫空间，横轴为theta，纵轴为r (degree, r)
	cout << "building up houghSpace ...";
	CImg<float> hough = CImg<float>(interval, maxLen, 1, 1, 0); 

	//生成霍夫空间
	for (int i = 0; i < img.height(); ++i)       //y
	{
		for (int j = 0; j < img.width(); ++j)    //x
		{
			//检查边缘点
			if (img.atXY(j, i) == 0)
				continue;
			//对于量化的每个角度值
			for (int k = 0; k < interval; ++k)
			{
				int r = (-j) * cos(coef * k) + i * sin(coef * k) + maxLen/2;
				if (r < 0 || r > maxLen)
					continue;
				hough.atXY(k, r) = (hough.atXY(k, r) == 255)?255:hough.atXY(k, r)+0.1;
			}
		}
	}
	hough.save((root+"houghSpace.bmp").c_str());
	cout << "done\n" << endl;
	return hough;
}
vector<pair<int, int> > HoughTransform::findLocalMaximum(CImg<float> hough, int threshold)
{
	//设定阈值，提取局部最大值
	cout << "finding local maximun ...";
	vector<pair<int, int> > line;
	for (int i = 0; i < hough.height(); ++i)
	{
		for (int j = 0; j < hough.width(); ++j)
		{
			if (hough.atXY(j, i) < threshold)
				continue;

			bool newLine = true;
			for (int k = 0; k < line.size(); ++k)
			{
				if (sqrt(pow(line[k].first - j, 2) + pow(line[k].second - i, 2)) < 10)
				{
					newLine = false;
					if (hough.atXY(line[k].first, line[k].second) < hough.atXY(j, i))
					{
						line[k].first = j;
						line[k].second = i;
					}
					break;
				}
			}
			if (newLine)
				line.push_back(pair<int, int>(j, i));
		}
	}
	
	for (int i = 0; i < line.size(); ++i)
		cout << " line : " << i << " " << line[i].first << " " << line[i].second << endl;
	cout << "done \n" << line.size() << endl;
	return line;
}

vector<pair<double, double> > HoughTransform::drawLines(CImg<unsigned char> &img, vector<pair<int, int> > line_in_hough, int maxLen, float coef)
{
	//将霍夫空间的theta和r反变换并绘制图形
	cout << "drawing line..." << " w: " << img.width() << " h: " << img.height() << endl;
	vector<pair<double, double> > line;
	vector<pair<int, int> > points;
	int count = 0, w = img.width(), h = img.height();
	for (int i = 0; i < line_in_hough.size(); ++i)
	{
		if (cos(coef*line_in_hough[i].first) != 0 && sin(coef*line_in_hough[i].first) != 0)
		{
			count++;

			double m = cos(coef*line_in_hough[i].first) / sin(coef*line_in_hough[i].first);
			double b = (line_in_hough[i].second - maxLen/2) / sin(coef*line_in_hough[i].first);

			int x0 = -b / m, y0 = 0, x1 = 0, y1 = b;

			if (x0 < 0)
			{
				if (w * m + b >= 0 && w * m + b < h)
				{
					x0 = w;
					y0 = w * m + b;
				}
				else {
					y0 = h;
					x0 = (h - b) / m;
				}
			}
			else if (x0 > w)
			{
				x0 = w;
				y0 = w * m + b;
			}
			if (y1 < 0)
			{
				if (w*m+b >= 0 && w*m+b < h)
				{
					x1 = w;
					y1 = w*m+b;
				}
				else
				{
					y1 = h;
					x1 = (h-b)/m;
				}
			}
			else if (y1 > h)
			{
				y1 = h;
				x1 = (h-b)/m;
			}

			// 删除过于相近的线条
			bool tooClose = false;
			for (int k = 0; k < points.size(); k+=2)
			{
				if (sqrt(pow(points[k].first - x0, 2) + pow(points[k].second - y0, 2)) < 3 
					&& sqrt(pow(points[k+1].first - x1, 2) + pow(points[k+1].second - y1, 2)) < 3)
					tooClose = true;
			}
			if (tooClose)
				continue;

			points.push_back(pair<int, int>(x0, y0));
			points.push_back(pair<int, int>(x1, y1));
			line.push_back(pair<double, double>(m, b));

			cout << "\n line " << i << endl 
				 << "parameter euqation: "<< (line_in_hough[i].second-maxLen/2) 
				 <<" = -x * cos(" <<(coef*line_in_hough[i].first)
				 <<") + y * sin(" << (coef*line_in_hough[i].first) 
				 << ")" << endl;  
			unsigned char blue[] = {0, 0, 255};
			img.draw_line(x1, y1, x0, y0, blue);
		}
	}
	cout<< " done " << count << endl;
	img.display();
	img.save((this->root + "I2.bmp").c_str());
	return line;
}
void HoughTransform::drawRelativeEdgePoint(CImg<unsigned char> &img, vector<pair<double, double> > line)
{
	//染色相关边缘点
	cimg_forXY(img, x, y)
	{
		if(img(x, y,0) == 255 && img(x, y, 1) == 255 && img(x, y, 2) == 255)
		{
			for (int i = 0; i < line.size(); ++i)
			{
				if (abs(y-(line[i].first*x + line[i].second)) < 2)
				{
					img(x, y, 1) = 0;
					img(x, y, 2) = 0; 
				}
			}
		}
	}
	img.display("relative edge");
	img.save((this->root + "I3.bmp").c_str());
}

void HoughTransform::drawPoints(CImg<unsigned char> &img, vector<pair<double, double> > line)
{
	//计算交点
	cout << line.size() << endl;
	unsigned char red[] = {255, 0, 0};
	for (int i = 0; i < line.size() - 1; ++i)
	{
		for (int j = i+1; j < line.size(); ++j)
		{
			double m1 = line[i].first, m2 = line[j].first;
			double b1 = line[i].second, b2 = line[j].second;
			int x = (b1 - b2) / (m2 - m1);
			int y = (m1*b2 - m2*b1) / (m1 - m2);
			// cout << x << " " << y << endl;
			if (x >= 0 && x < img.width() && y >= 0 && y < img.height())
				img1.draw_circle((int)x, (int)y, 5 , red);
		}
	}
	img1.display();
	img1.save((root+"I4.bmp").c_str());
}