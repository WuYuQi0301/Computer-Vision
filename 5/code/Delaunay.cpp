#include "Delaunay.hpp"
Delaunay::Delaunay(Point p1, Point p2, Point p3, Point p4)  //初始化边set和三角形set
{
	//将边框4点加入点集
	pset.push_back(p1);
	pset.push_back(p2);
	pset.push_back(p3);
	pset.push_back(p4);
	//将边框4遍加入边集
	Edge l1(0, 1, -1);
	Edge l2(1, 2, -1);
	Edge l4(2, 3, -1);
	Edge l3(0, 3, -1);
	eset.push_back(l1);
	eset.push_back(l2);
	eset.push_back(l3);
	eset.push_back(l4);
	//创建以对角线为边的两个超级三角形()并加入三角形集合
	createTriangle(0, 1, 2);
	createTriangle(0, 2, 3);
}
Delaunay::~Delaunay()
{

}

bool Delaunay::insertPoint(double _x, double _y)
{
	vector<Edge> BoundEdges;
	Point newPoint(_x, _y);
	pset.push_back(newPoint);

	vector<int> toDelete;
	//遍历tset 若点在该三角形外接圆内，需要删除之
	for (unsigned int i = 0; i < tset.size(); ++i)
		if(inCircle(_x, _y, tset[i]))
			toDelete.push_back(i);

	for (unsigned int i = 0; i < toDelete.size(); ++i)
	{
		delTriangle(toDelete[i], BoundEdges);
		//删除后，之后的各序号均减去1
		for (unsigned int j = i + 1; j < toDelete.size(); ++j)
			toDelete[j] -= 1;
	}
	//构建新三角形并加入到集合中
	int psize = (int)pset.size();
	for (unsigned int i = 0; i < BoundEdges.size(); ++i)
	{
		createTriangle(psize - 1, BoundEdges[i].leftEnd, BoundEdges[i].rightEnd);
	}
	return true;
}
//计算三角形的外接圆圆心坐标和半径  
void Delaunay::computeCentre(double &x_centre,double &y_centre,double &radius,int n1,int n2,int n3)
{
	double x1, x2, x3, y1, y2, y3;
	x1 = pset[n1].x;
	y1 = pset[n1].y;
	x2 = pset[n2].x;
	y2 = pset[n2].y;
	x3 = pset[n3].x;
	y3 = pset[n3].y;
	// x_centre = ((y2 - y1)*(y3*y3 - y1*y1 + x3*x3 - x1*x1) - (y3 - y1)*(y2*y2 - y1*y1 + x2*x2 - x1*x1)) / (2 * (x3 - x1)*(y2 - y1) - 2 * ((x2 - x1)*(y3 - y1)));
	// y_centre = ((x2 - x1)*(x3*x3 - x1*x1 + y3*y3 - y1*y1) - (x3 - x1)*(x2*x2 - x1*x1 + y2*y2 - y1*y1)) / (2 * (y3 - y1)*(x2 - x1) - 2 * ((y2 - y1)*(x3 - x1)));
	// radius = sqrt((x1 - x_centre)*(x1 - x_centre) + (y1 - y_centre)*(y1 - y_centre));


	//求外接圆半径
	double a = sqrt(pow((x1-x2), 2) + pow((y1-y2), 2));
	double b = sqrt(pow((x1-x3), 2) + pow((y1-y3), 2));
	double c = sqrt(pow((x3-x2), 2) + pow((y3-y2), 2));
	double p = (a + b + c) / 2;
	double S = sqrt(p * (p-a) * (p-b) * (p-c)); //海伦公式
	radius = a * b * c / (4 * S);
	//求外接圆圆心
	double t1 = x1*x1 + y1*y1;
	double t2 = x2*x2 + y2*y2;
	double t3 = x3*x3 + y3*y3;
	double temp = x1*y2 + x2*y3 + x3*y1 - x1*y3 - x2*y1 - x3*y2;
	x_centre = (t2*y3 + t1*y2 + t3*y1 - t2*y1 - t3*y2 - t1*y3)/(temp*2);
	y_centre = (t3*x2 + t2*x1 + t1*x3 - t1*x2 - t2*x3 - t3*x1)/(temp*2);	
}

//生成指定顶点的三角形 
void Delaunay::createTriangle(int n1, int n2,int n3)
{
	//n1,n2,n3从小到大排列
	sort(n1, n2, n3);

	double xc = 0, yc = 0, r = 0;
	computeCentre(xc, yc, r, n1, n2, n3);
	Triangle newTri = { { n1,n2,n3 },{ { n1,n2,1 },{ n2,n3,1 },{ n1,n3,1 } }, xc, yc, r, NULL, NULL };
	tset.push_back(newTri);

	int flag = 1;
	for (int i = 0; i < 3; ++i)
	{
		flag = 1;
		for (unsigned int j = 0; j < eset.size(); ++j)
		{
			if (newTri.edge[i].leftEnd == eset[j].leftEnd && newTri.edge[i].rightEnd == eset[j].rightEnd)
			{
				if (eset[j].count != -1)
				{
					flag = 0;
					eset[j].count += 1;
					break;
				}
				else {
					flag = 0;
					break;
				}
			}
		}
		if (flag == 1)
			eset.push_back(newTri.edge[i]);
	}
}
//判断点是否在三角形外接圆内  
bool Delaunay::inCircle(double _x,double _y, Triangle currentTris)
{
	return (currentTris.r >= sqrt(pow((_x - currentTris.centerX), 2) 
				+ pow(_y - currentTris.centerY, 2)));
}

//删除指定的三角形  
void Delaunay::delTriangle(int n, vector<Edge> &BoundEdges)
{
	for (int i = 0; i < 3; ++i)
	{
		for (unsigned int j = 0; j < eset.size(); ++j)
		{
			if (eset[j].leftEnd == tset[n].edge[i].leftEnd && eset[j].rightEnd == tset[n].edge[i].rightEnd)
			{
				switch (eset[j].count)
				{
					case 2:                            //若包含这条边的值为
						eset[j].count = 1;
						BoundEdges.push_back(eset[j]);
						break;
					case 1:
						for (unsigned int k = 0; k < BoundEdges.size(); ++k)
						{
							if(BoundEdges[k].leftEnd == eset[j].leftEnd && BoundEdges[k].rightEnd == eset[j].rightEnd)
							{
								BoundEdges.erase(BoundEdges.begin() + k);
								break;
							}
						}
						eset.erase(eset.begin() + j);
						j--;
						break;
					case -1:
						BoundEdges.push_back(eset[j]);
						break;
					default:
						break;
				}
			}
		}
	}
	tset.erase(tset.begin() + n);
}
void Delaunay::delFrame()
{
	vector<Edge> BoundEdges;
	for (int i = 0; i<4; i++) pset.erase(pset.begin());
	for (int i = 0; i<(int)tset.size(); i++)
	{
		if (tset[i].ep[0] == 0 || tset[i].ep[0] == 1 || tset[i].ep[0] == 2 || tset[i].ep[0] == 3)
		{
			delTriangle(i, BoundEdges);
			BoundEdges.resize(0);
			i--;
		}
		else
		{
			for (int j = 0; j<3; j++)
			{
				tset[i].ep[j] -= 4;
				tset[i].edge[j].leftEnd -= 4;
				tset[i].edge[j].rightEnd -= 4;
			}
		}
	}
	for (int i = 0; i < 4; i++) 
		eset.erase(eset.begin());
	for (int i = 0; i < (int)eset.size(); i++)
	{
		eset[i].leftEnd -= 4;
		eset[i].rightEnd -= 4;
	}
}
void Delaunay::sort(int& a, int& b, int& c)
{
	if(a > b)
	{
		int temp = a;
		a = b;
		b = temp; 
	}
	if(a > c)
	{
		int temp = a;
		a = c;
		c = temp;
	}
	if(b > c)
	{
		int temp = b;
		b = c;
		c = temp;
	}
}