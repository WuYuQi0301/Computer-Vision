#include "ImageMorphing.hpp"

ImageMorphing::ImageMorphing(string spath1, string spath2, string rpath)
{
	CImg<unsigned char> img1(spath1.c_str()), img2(spath2.c_str());
	if (img1.width() != img2.width() || img1.height() != img2.height())
		img1 = scale(img1, img2.width(), img2.height());

	pset1 = loadControlPoint(img1, "../src/pset1.txt");
	pset2 = loadControlPoint(img2, "../src/pset2.txt");

	if (pset1.size() != pset2.size() ||pset1.empty() || pset2.empty())
	{
		cout << "pset error" << endl;
		continue;
	}


	for (int i = 0; i <= 10; ++i)
	{
		double a = i/(double)10;
		vector<Point> curPSet;
		CImg<unsigned char> midImg(img1.width(), img1.height(), 1, 3, 0);
		
		for (unsigned int j = 0; j < pset1.size(); ++j)
		{
			double x, y;
			x = (1 - a) * pset1[i].x + a * pset2[i].x;
			y = (1 - a) * pset1[i].y + a * pset2[i].y;
			Point temp(x, y);
			curPSet.push_back(temp);
		}
		Delaunay tmesh = triangleMesh(midImg, curPSet);

		//对于每一个三角形mesh， 计算它的映射矩阵
		for (unsigned int j = 0; j < tmesh.tset.size(); ++j)
		{
			int* p = tmesh.tset.ep;
			double **A = makeAMatrix(curPSet[p[0]], curPSet[p[1]], curPSet[p[2]]);
			double **invA = getInverse(A);
			double **U1 = makeAMatrix(pset1[v[0]], pset1[v[1]], pset1[v[2]]);
			double **U2 = makeAMatrix(pset2[v[0]], pset2[v[1]], pset2[v[2]]);
			tmesh.tset[i].m1 = getMatrixProduct(U1, invA);
			tmesh.tset[i].m2 = getMatrixProduct(U2, invA);
			deleteAMatrix(A);
			deleteAMatrix(invA);
			deleteAMatrix(U1);
			deleteAMatrix(U2);
		}

		cimg_forXY(img, x, y)
		{
			Point p(x, y);
			for (int j = 0; j < ; ++j)
			{
				int* endPoint = tmesh.tset[j].ep;
				//该点是否在当前三角形内？
				if(!PointinTriangle(curPSet[endPoint[0]], curPSet[endPoint[1]], curPSet[endPoint[2]], p))
					continue;

				double** M1 = tmesh.tset[j].m1;   //映射到原图的矩阵
				double** M2 = tmesh.tset[j].m2;   //映射到目标图像的矩阵

				double u1 = M1[0][0] * x + M1[0][1] * y + M1[0][2];
				double v1 = M1[1][0] * x + M1[1][1] * y + M1[1][2];
				unsigned char* rgb1 = BilinearFilter(img1, u1, v1);    //后向双线性插值

				double u2 = M2[0][0] * x + M2[0][1] * y + M2[0][2];
				double v2 = M2[1][0] * x + M2[1][1] * y + M2[1][2];
				unsigned char* rgb2 = BilinearFilter(img2, u2, v2);    //后向双线性插值

				for (int k = 0; k < 3; k++) {
					img(x, y, k) = (1 - a) * rgb1[k] + a * rgb2[k];//颜色中间值
				}

				delete[] rgb1, rgb2;
				break;
			}
		}

		img.display();
		img.save((rpath+str(i)+".bmp").c_str());
	}

}

unsigned char* ImageMorphing::BilinearFilter(CImg<unsigned char> &img1, double _x, double _y)
{
	unsigned char* rgb3 = new unsigned char[3];
	for (int i = 0; i < 3; ++i)
		rgb[i] = 0;

	Point p1(floor(_x), floor(_y)), p2(ceil(_x), floor(_y));
	Point p3(floor(_x), ceil(_y)), p4(ceil(_x), ceil(_y));
	Point p5(_x, floor(_y)), p6(_x, ceil(_y));

	if(p1.x < 0 || p1.x >= img1.width() || p4.x < 0 || p4.x >= img1.width()
		|| p1.y < 0 || p1.y >= img1.height() || p4.y < 0 || p4.y >= img1.height())
	{
		continue;
	}
	if (_x == p1.x)
	{
		if (_y == p1.y)
		{
			for (int i = 0; i < 3; ++i)
				rgb3[i] = img1(p1.x, p1.y, i);
		} else {  //p1p3 y插值一次 
			for (int i = 0; i < 3; ++i)
				rgb3[i] = ((_y - p1.y)/(p3.y-p1.y))*img1(p1.x, p1.y, i) 
								 + ((p3.y-_y)/(p3.y-p1.y))*img1(p3.x, p3.y, i);
		}
	}
	else if (_x == p2.x)
	{
		if( _y == p2.y)
		{
			for (int i = 0; i < 3; ++i)
				rgb3[i] = img1(p2.x, p2.y, i);
		} else {  //p2p4 y插值一次
			for (int i = 0; i < 3; ++i)
				rgb3[i] = ((_y-p2.y)/(p4.y-p2.y))*img1(p2.x, p2.y, i) 
							  + ((p4.y-_y)/(p4.y-p2.y))*img1(p4.x, p4.y, i);
		}
	}
	else if (_y == p1.y)  // p1p2 x插值一次
	{
		for (int i = 0; i < 3; ++i)
			rgb3[i] = ((_x-p1.x)/(p2.x-p1.x))*img1(p1.x, p1.y, i) 
						  + ((p2.x-_x)/(p2.x-p1.x))*img1(p2.x, p2.y, i);
	}
	else if (_y == p3.y)  // p3p4 x 插值一次
 	{
		for (int i = 0; i < 3; ++i)
			rgb3[i] = ((_x-p3.x)/(p4.x-p3.x))*img1(p3.x, p3.y, i) 
						  + ((p4.x-_x)/(p4.x-p3.x))*img1(p4.x, p4.y, i);
	}
	else        //并不落在任何两非对角点连成的直线上
	{
		for (int i = 0; i < 3; ++i)
		{
			p5.rgb[i] = ((p4.x - _x)/(p4.x - p3.x)) * img1(p4.x, p4.y, i) + ((_x - p3.x)/(p4.x - p3.x)) * img1(p3.x, p3.y, i);
			p6.rgb[i] = ((p2.x - _x)/(p2.x - p1.x)) * img1(p2.x, p2.y, i) + ((_x - p1.x)/(p2.x - p1.x)) * img1(p1.x, p1.y, i);
			rgb3[i] = (int)( ((p5.y - _y)/(p5.y - p6.y)) * p5.rgb[i] + ((_y - p6.y)/(p5.y - p6.y)) * p6.rgb[i]);
		}
	}
	return rgb3;
}


CImg<unsigned char> ImageMorphing::scale(CImg<unsigned char>img1, int w2, int h2)
{
	CImg<unsigned char> img3 = CImg<unsigned char>(w2, h2, 1, 3, 0);

	double w1 = img1.width(), h1 = img1.height();
	double a = w1/w2, b = h1/h2;
	cout << a << " " << b <<endl;

	cimg_forXY(img3, x, y)
	{
		double _x = x*a, _y = y*b;
			// if (y == 57 && x == 100) cout << _x << " " << _y << endl;
		unsigned char* rgb = BilinearFilter(img3, _x, _y);
		for (int i = 0; i < 3; ++i)
			img3(x, y, i) = rgb[i];

		delete rgb;
	}
	return img3;
}
vector<Point> ImageMorphing::loadControlPoint(CImg<unsigned char>img, string path)
{
	string line;
	stringstream ss;
	vector<Point> pset;

	ifstream fin(path.c_str());
	if (!fin)
	{
		cout << path <<" open file error" << endl;
		exit(1);
	}
	while(getline(fin, line))
	{
		ss.clear();
		ss.str(line);
		int _x = 0, _y = 0;
		ss >> _x >> _y;
		Point temp(_x, _y);
		pset.push_back(temp);
	}

	for (int i = 0; i < pset.size(); ++i)
	{
		cout << pset[i].x << " " << pset[i].y << endl;
		img(pset[i].x, pset[i].y, 0) = 255;
		img(pset[i].x, pset[i].y, 1) = 0;
		img(pset[i].x, pset[i].y, 2) = 0;
	}
	img.display();

	return pset;
}
Delaunay ImageMorphing::triangleMesh(CImg<unsigned char> img, vector<Point> pset)
{
	//1、构造超级三角形，包含所有散点，放入三角形链表。
	Point p1(0, 0), p2(img.width()-1, 0), p3(img.width()-1, img.height()-1), p4(0, img.height()-1);
	Delaunay tmesh(p1, p2, p3, p4);
	//2、将点集中的散点依次插入，在三角形链表中找出外接圆包含插入点的三角形（称为该点的影响三角形
	for (int i = 0; i < pset.size(); ++i)
	{
		tmesh.insertPoint(pset[i].x, pset[i].y);
	}
	//删除影响三角形的公共边，将插入点同影响三角形的全部顶点连接起来，完成一个点在Delaunay三角形链表中的插入。
	//3、根据优化准则对局部新形成的三角形优化。将形成的三角形放入Delaunay三角形链表。
	//4、循环执行上述第2步，直到所有散点插入完毕。

	unsigned char blue[3] = {0, 0, 255};
	for (int i = 0; i < tmesh.tset.size(); ++i)
	{
		Triangle temp = tmesh.tset[i];
		Point p5 = tmesh.pset[temp.ep[0]];
		Point p6 = tmesh.pset[temp.ep[1]];
		Point p7 = tmesh.pset[temp.ep[2]];
		img.draw_line(p5.x, p5.y, p6.x, p6.y, blue);
		img.draw_line(p6.x, p6.y, p7.x, p7.y, blue);
		img.draw_line(p5.x, p5.y, p7.x, p7.y, blue);
	}
	img.display();
	return tmesh;
}