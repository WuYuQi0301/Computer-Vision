
int kdtree_bbf_knn(struct kd_node* kd_root, struct feature* feat, int k,
	struct feature*** nbrs, int max_nn_chks)
{
	struct kd_node* expl;
	struct min_pq* min_pq;
	struct feature* tree_feat, ** _nbrs;
	struct bbf_data* bbf_data;
	int i, t = 0, n = 0;

	minpq_insert(min_pq, kd_root, 0);  //在优先队列在插入第一个根元素  
	while (min_pq->n > 0 && t < max_nn_chks)     //如果队列不为空且在超时次数内  
	{
		expl = (struct kd_node*)minpq_extract_min(min_pq);//在优先队列中取出一个元素  

		expl = explore_to_leaf(expl, feat, min_pq);// 找到特征点在KD树叶子节点位置，过程中未查询的加入优先队列  

		for (i = 0; i < expl->n; i++)     //遍历以expl为根的子树所有节点  
		{
			//printf("%x",expl->features[i].feature_data);  
			tree_feat = &expl->features[i];
			bbf_data = malloc(sizeof(struct bbf_data));

			//bbf_data->old_data 这个数据没有用途，因为特征点属性中没有使用到feature_data这个自定义类型  
			bbf_data->old_data = tree_feat->feature_data;
			printf("%x", bbf_data->old_data);
			bbf_data->d = descr_dist_sq(feat, tree_feat);    //计算两特征点的欧式距离  
			tree_feat->feature_data = bbf_data;
			n += insert_into_nbr_array(tree_feat, _nbrs, n, k); //找到K个近邻的特征点，存入数组_nbrs中，从小到大的距离；  
		}
		t++;
	}

	minpq_release(&min_pq);
	for (i = 0; i < n; i++)
	{
		bbf_data = _nbrs[i]->feature_data;
		_nbrs[i]->feature_data = bbf_data->old_data;
		free(bbf_data);
	}
	*nbrs = _nbrs;
	return n;

fail:
	minpq_release(&min_pq);
	for (i = 0; i < n; i++)
	{
		bbf_data = _nbrs[i]->feature_data;
		_nbrs[i]->feature_data = bbf_data->old_data;
		free(bbf_data);
	}
	free(_nbrs);
	*nbrs = NULL;
	return -1;
}
struct min_pq* minpq_init()  //队列初始化  
{
	struct min_pq* min_pq;

	min_pq = malloc(sizeof(struct min_pq));
	min_pq->pq_array = calloc(MINPQ_INIT_NALLOCD, sizeof(struct pq_node));//分配队列的空间  
	min_pq->nallocd = MINPQ_INIT_NALLOCD;
	min_pq->n = 0;//队列中元素的个数  

	return min_pq;
}

int minpq_insert(struct min_pq* min_pq, void* data, int key)//向优先队列中插入元素  
{
	int n = min_pq->n;

	/* double array allocation if necessary */
	if (min_pq->nallocd == n)
	{
		min_pq->nallocd = array_double((void**)&min_pq->pq_array,
			min_pq->nallocd,
			sizeof(struct pq_node));
		if (!min_pq->nallocd)
		{
			fprintf(stderr, "Warning: unable to allocate memory, %s, line %d\n",
				__FILE__, __LINE__);
			return 1;
		}
	}
	min_pq->pq_array[n].data = data;
	min_pq->pq_array[n].key = INT_MAX;
	decrease_pq_node_key(min_pq->pq_array, min_pq->n, key); //插入元素到优先队列中,堆排序算法  
	min_pq->n++;
	return 0;
}

static struct kd_node* explore_to_leaf(struct kd_node* kd_node,    //从kd_node开始开始查询直到叶节点为止  
	struct feature* feat,
	struct min_pq* min_pq)
{
	struct kd_node* unexpl, *expl = kd_node;
	double kv;
	int ki;

	while (expl && !expl->leaf)
	{
		ki = expl->ki;
		kv = expl->kv;

		if (ki >= feat->d)
		{
			return NULL;
		}
		if (feat->descr[ki] <= kv)
		{
			unexpl = expl->kd_right;
			expl = expl->kd_left;
		}
		else
		{
			unexpl = expl->kd_left;
			expl = expl->kd_right;
		}

		if (minpq_insert(min_pq, unexpl, ABS(kv - feat->descr[ki])))   //未查询到的结点，按差值大小加入优先队列  
		{
			return NULL;
		}
	}

	return expl;
}