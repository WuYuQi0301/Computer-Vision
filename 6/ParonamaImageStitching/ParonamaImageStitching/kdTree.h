#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

#define FEATURE_MAX_D 128
#define ORI_MAX_COUNT 2

//typedef float;

typedef struct Feature
{
	float x;                 // x
	float y;                 // y

	float scale;               // 尺度
	double ori[ORI_MAX_COUNT]; // 方向
	int ori_count;			   // 方向个数（可能有辅方向）
	
	double descr[ORI_MAX_COUNT][FEATURE_MAX_D];      //方向描述子
};

typedef struct Node
{
	Feature* features;		// 分割的数组的起始地址
	int split;				// 分割的维数：关键点直方图方差最大向量系列位置
	double value;			// 直方图方差最大向量系列中最中间模值
	int num;                // 以该节点为根的feature个数 
	int leaf;			    // 是否是叶节点 1/0

	Node* left;
	Node* right;

	Node(Feature* _feature = NULL, int _num = 0) {
		features = _feature;
		split = -1;
		value = -1;
		num = _num;
		leaf = 0;
		left = right = NULL;
	}
};

class kdTree
{
public:
	kdTree();
	Node* BuildKdTree(string spPath);
	//Node* createNode(Feature* dataset, int count);
	bool DestoryKdTree();
private:
	Node* root;
	Feature* features;
	int total;
	Feature* loadFeatures(string spPath);
	bool buildupSubtree(Node *);

	//calculate split and value
	void assignPartitionKey(Node*);
	double median(Feature*, int, int);
	void qSort(Feature* fea, int low, int high, int split);
	int qPatition(Feature* fea, int low, int high, int split);
	//create node's child and patition with split and value
	void partitionFeatures(Node*);

	//清空树
	void clearTree(Node*);
	//插入节点
	//bool Inser
	//删除节点
	//查找
	int BBFSearch(Node* root, Feature* target, Feature** ans, int k, int maxCheck);
	Feature* searchDownLeaf(Node*, Feature*, priority_queue<Node*>&);
	
	//计算特征点举例
	double getDist(Feature* f1, Feature* f2);
};