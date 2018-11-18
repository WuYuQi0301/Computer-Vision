#ifndef SYMBOL
#define SYMBOL value

#include <iostream>
#include <cmath>
#include <vector>
#include "Point.hpp"
#include "CImg.h"
using namespace std;

struct Edge           //边
{
	int leftEnd;  //端点在point set中的下标
	int rightEnd; 
	int count;         //以该线段为边的三角形个数
	Edge(int _l, int _r, int _c)
	{
		leftEnd = _l;
		rightEnd = _r;
		count = _c;
	}
};

struct Triangle      //三角形类
{
	int ep[3];       //三个顶点
	Edge edge[3];       //三条边
	double centerX;  //外接圆圆心坐标
	double centerY;  
	double r;        //外接圆半径
	double** m1;     //到原图
	double** m2;     //到目标图像
};

class Delaunay
{
public:
	Delaunay(Point p1, Point p2, Point p3, Point p4);  //初始化边set和三角形set

	bool insertPoint(double _x, double _y);

	vector<Point> pset;    //所有点的集合
	vector<Edge> eset;     //所有边的集合
	vector<Triangle> tset; //所有三角形的集合
private:
	void computeCentre(double &x_centre,double &y_centre,double &radius,int n1,int n2,int n3);//计算三角形的外接圆圆心坐标和半径  
    void createTriangle(int n1,int n2,int n3);//生成指定顶点的三角形  
    bool inCircle(double xx,double yy,Triangle currentTris);//判断点是否在三角形外接圆内  
    void delTriangle(int n, vector<Edge> &BoundEdges);//删除指定的三角形  
	void sort(int& a, int& b, int& c);
  
};

#endif