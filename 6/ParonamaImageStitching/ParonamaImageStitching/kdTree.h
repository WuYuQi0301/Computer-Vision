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

	float scale;               // �߶�
	double ori[ORI_MAX_COUNT]; // ����
	int ori_count;			   // ��������������и�����
	
	double descr[ORI_MAX_COUNT][FEATURE_MAX_D];      //����������
};

typedef struct Node
{
	Feature* features;		// �ָ���������ʼ��ַ
	int split;				// �ָ��ά�����ؼ���ֱ��ͼ�����������ϵ��λ��
	double value;			// ֱ��ͼ�����������ϵ�������м�ģֵ
	int num;                // �Ըýڵ�Ϊ����feature���� 
	int leaf;			    // �Ƿ���Ҷ�ڵ� 1/0

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

	//�����
	void clearTree(Node*);
	//����ڵ�
	//bool Inser
	//ɾ���ڵ�
	//����
	int BBFSearch(Node* root, Feature* target, Feature** ans, int k, int maxCheck);
	Feature* searchDownLeaf(Node*, Feature*, priority_queue<Node*>&);
	
	//�������������
	double getDist(Feature* f1, Feature* f2);
};