#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Point.hpp"
using namespace std;

#define MYDIM 3    //测试矩阵维数定义

//得到一个空矩阵
extern double** getAMatrix(int row, int col);

//得到三个点的值组成的矩阵
extern double** makeAMatrix(Point &p0, Point &p1, Point &p2);

//删除矩阵
extern void deleteAMatrix(double **matrix);

extern double getA(double** arcs, int n);

//计算每一行每一列的每个元素所对应的余子式，组成A*
extern void getAStart(double** arcs, int n, double** ans);

//得到给定矩阵src的逆矩阵保存到des中。
extern bool GetMatrixInverse(double** src, int n, double** des);

//求逆
extern double** getInverse(double** matrix_before);

//求矩阵的积
extern double** getMatrixProduct(double** m1, double** m2);

#endif
