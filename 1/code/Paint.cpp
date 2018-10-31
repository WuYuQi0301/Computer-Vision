#include "Paint.hpp"
#include <iostream>

Paint::Paint()
{
	this->SrcImg.load_bmp("1.bmp");
}
void Paint::Display()
{
	this->SrcImg.display();
}

void Paint::Save(string fname)
{
	this->SrcImg.save(fname.c_str());
}

void Paint::ChangeColor()
{
	//one-by-one piecx processing:grey to white
	cimg_forXY(SrcImg, x, y)
	{
		//turn it red if the color is white
		if(SrcImg(x, y, 0) == 255 && SrcImg(x, y, 1) == 255 && SrcImg(x, y, 2) == 255)
		{
			SrcImg(x, y, 0) = 255;
			SrcImg(x, y, 1) = 0;
			SrcImg(x, y, 2) = 0;

		} //turn it green if the color is black
		else if(SrcImg(x, y, 0) == 0 && SrcImg(x, y, 1) == 0 && SrcImg(x, y, 2) == 0)
		{
			SrcImg(x, y, 0) = 0;
			SrcImg(x, y, 1) = 255;
			SrcImg(x, y, 2) = 0;
		}
	}
	SrcImg.display("task2");
}

void Paint::BlueCircle()
{
	unsigned char blue[] = {0,0,255};
	//class api with parameters center point x, y, r, filled color
    SrcImg.draw_circle(50, 50, 30, blue);
	SrcImg.display("task3_lib");
}
void Paint::MyBlueCircle()
{ 
	cimg_forXY(SrcImg, x, y)
	{
		if ((pow((x - 50), 2) + pow((y - 50), 2)) <= 900) //change color of pixel that inside the circle
		{
			SrcImg(x, y, 0) = 0;
			SrcImg(x, y, 1) = 0;
			SrcImg(x, y, 2) = 255;
		}
	}
	SrcImg.display("task3_nlib");
}

void Paint::MyellowCircle()
{
	cimg_forXY(SrcImg, x, y)
	{
		if ((pow((x - 50), 2) + pow((y - 50), 2)) <= 9)
		{
			SrcImg(x, y, 0) = 255;
			SrcImg(x, y, 1) = 255;
			SrcImg(x, y, 2) = 0;
		}
	}
	SrcImg.display("task4_nlib");
}
void Paint::YellowCircle()
{
	unsigned char yellow[] = {255, 255, 0};
    SrcImg.draw_circle(50, 50, 3, yellow);
	SrcImg.display("task4_lib");
}

void Paint::BlueLine()
{

	unsigned char blue[] = {0, 0, 255};
	double x0 = 100 * cos(35 * M_PI / 180), y0 = 100 * sin(35 * M_PI / 180);
	//五个参数：线段起点x,y，线段终点x0,y0, 填充色RGB数组
    SrcImg.draw_line(0, 0, x0, y0, blue);
	SrcImg.display("task5_lib");
}

void Paint::MyBlueLine()
{
	//coef：35度的tan函数值，x0 y0：直线段终点坐标
	double coef = tan(35 * M_PI / 180), x0 = 100 * cos(35*M_PI/180), y0 = 100*sin(35*M_PI/180);
	cimg_forXY(SrcImg, x, y)
	{	
		if ((y == 0 && x == 0) || (abs(y - x * coef) < 0.5 && x <= x0 && y <= y0))
		{
			SrcImg(x, y, 0) = 0;
			SrcImg(x, y, 1) = 0;
			SrcImg(x, y, 2) = 255;
		}
	}
	SrcImg.display("task5_nlib");
}