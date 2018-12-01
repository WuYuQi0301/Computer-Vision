#include "kdTree.h"

kdTree::kdTree()
{
	total = 0;
	root = NULL;
	features = NULL;
}


Feature* kdTree::loadFeatures(string spPath)
{
	ifstream fin;
	fin.open(spPath.c_str());
	if (!fin) return NULL;

	fin >> total;
	Feature* feas = new Feature[total];

	for (int i = 0; i < total; i++)
	{
		fin >> feas[i].scale >> feas[i].x >> feas[i].y >> feas[i].ori_count;
		for (int j = 0; j < feas[i].ori_count; j++)
		{
			fin >> feas[i].ori[j];
			for (int k = 0; k < FEATURE_MAX_D; k++)
			{
				fin >> feas[i].descr[j][k];
			}
		}
	}

	fin.close();

	return feas;
}


Node* kdTree::BuildKdTree(string spPath)
{
	ifstream fin(spPath.c_str());
	if (!fin) return NULL;

	features = loadFeatures(spPath);

	//bulid up
	root = new Node();
	root->num = total;
	root->features = features; //feature�����ͷָ��
	buildupSubtree(root);
	
	return root;
}

bool kdTree::buildupSubtree(Node* node)
{
	//node ���Ѿ���ţ����ָ�����ݼ������ݸ���

	// leaf node
	if (node->num == 1 || node->num == 0)
	{
		node->leaf = 1;
		return true;
	}

	assignPartitionKey(node); //calculate split value;
	partitionFeatures(node);  //create chlidren;

	if (node->left)
		buildupSubtree(node->left);
	if (node->right)
		buildupSubtree(node->right);

	return true;
}

void kdTree::assignPartitionKey(Node *node)
{
	// have ensured node->num > 1
	node->split = 0, node->value = 0;
	int num = node->num;
	double maxSD = 0;

	// ���㵱ǰ��������󷽲��Ӧ�� split �� value��
	for (int i = 0; i < FEATURE_MAX_D; i++)
	{
		double sq_diff = 0, mean = 0;

		//�����ά�Ⱦ�ֵ
		for (int j = 0; j < num; j++)
		{
			Feature tmp = node->features[j];
			mean += tmp.descr[0][i];         //������ڸ����򣬽���������������
		}
		mean /= num;

		//�����ά�ȷ���
		for (int j = 0; j < num; j++)
		{
			Feature tmp = node->features[j];
			sq_diff += pow(tmp.descr[0][i] - mean, 2);
		}
		sq_diff /= num;

		//�������ֵ
		if (maxSD < sq_diff)
		{
			node->split = i;
			maxSD = sq_diff;
		}
	}

	node->value = median(node->features, num, node->split);//��������λ��
}


void kdTree::partitionFeatures(Node* node)
{
	if (node->num == 0 || node->split == -1 || node->value == -1)
		return;

	Feature* curFeas = node->features;
	int split = node->split, num = node->num;
	double value = node->value;

	int index = 0;
	//����value���ڵ�feature
	for (index = 0; index < num; ++index)
		if (curFeas[index].descr[0][split] == value)
			break;

	// ��splitά��valueΪ�Ƚϵ����һ�ο��� (x

	if (index != num - 1)
	{
		node->left = new Node(features, index + 1);
		node->right = new Node(features + index + 1, (num - index - 1));
	}
	else {
		node->leaf = 1;
	}
}



double kdTree::median(Feature* curFeas,int num, int split)
{	
	qSort(curFeas, 0, num - 1, split);

	return curFeas[num / 2].descr[0][split];
	//return 0;
}

void kdTree::qSort(Feature* fea, int low, int high, int split)
{
	if (low >= high)
		return;
	int pilot = qPatition(fea, low, high, split);

	qSort(fea, low, pilot-1, split);
	qSort(fea,pilot+1, high, split);
}
int kdTree::qPatition(Feature* fea, int low, int high, int split)
{
	int i = low, j = high - 1;
	Feature tmp = fea[low];
	double value = fea[low].descr[0][split];
	while (i < j)
	{
		while (fea[j].descr[0][split] > value && i < j)
			j--;
		fea[i] = fea[j];

		while (fea[i].descr[0][split] < value && i < j)
			i++;
		fea[j] = fea[i];
	}
	fea[i] = tmp;

	return i;
}

bool kdTree::DestoryKdTree()
{
	if (features != NULL)
	{
		delete[] features;
	}

	clearTree(root);
	return true;
}

void kdTree::clearTree(Node* node)
{
	if (node->left)
		clearTree(node->left);
	if (node->right)
		clearTree(node->right);
	delete node;
	node = NULL;
}
//root=kd�����ڵ㣻target=��ѯĿ�������㣬ans=��ѯ�����k=��ѯ������maxCheck=�����ȡ���д�������ʱ���ƣ�
//�ɹ����ظ��������򷵻�-1
int BFFSearch(Node* root, Feature* target, Feature** ans, int k, int maxCheck)
{
	Node* curNode;
	PriorityQueue pq;
	
	if (!ans || !target || !root)
	{
		cerr << "Error : null pointer" << endl;
		return -1;
	}
	int times = 0;
	ans = new Feature*[k];
	pq.insert(root, 0);
	while (!pq.empty() && times < maxCheck)
	{
		times++;
		curNode = pq.top();
		curNode = searchDownLeaf(curNode, target, pq);// �ҵ���������KD��Ҷ�ӽڵ�λ�ã�������δ��ѯ�ļ������ȶ���  
		//������curNodeΪ�������������нڵ�

	}
}