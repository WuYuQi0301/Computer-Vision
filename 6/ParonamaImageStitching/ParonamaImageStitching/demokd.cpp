
int kdtree_bbf_knn(struct kd_node* kd_root, struct feature* feat, int k,
	struct feature*** nbrs, int max_nn_chks)
{
	struct kd_node* expl;
	struct min_pq* min_pq;
	struct feature* tree_feat, ** _nbrs;
	struct bbf_data* bbf_data;
	int i, t = 0, n = 0;

	minpq_insert(min_pq, kd_root, 0);  //�����ȶ����ڲ����һ����Ԫ��  
	while (min_pq->n > 0 && t < max_nn_chks)     //������в�Ϊ�����ڳ�ʱ������  
	{
		expl = (struct kd_node*)minpq_extract_min(min_pq);//�����ȶ�����ȡ��һ��Ԫ��  

		expl = explore_to_leaf(expl, feat, min_pq);// �ҵ���������KD��Ҷ�ӽڵ�λ�ã�������δ��ѯ�ļ������ȶ���  

		for (i = 0; i < expl->n; i++)     //������explΪ�����������нڵ�  
		{
			//printf("%x",expl->features[i].feature_data);  
			tree_feat = &expl->features[i];
			bbf_data = malloc(sizeof(struct bbf_data));

			//bbf_data->old_data �������û����;����Ϊ������������û��ʹ�õ�feature_data����Զ�������  
			bbf_data->old_data = tree_feat->feature_data;
			printf("%x", bbf_data->old_data);
			bbf_data->d = descr_dist_sq(feat, tree_feat);    //�������������ŷʽ����  
			tree_feat->feature_data = bbf_data;
			n += insert_into_nbr_array(tree_feat, _nbrs, n, k); //�ҵ�K�����ڵ������㣬��������_nbrs�У���С����ľ��룻  
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
struct min_pq* minpq_init()  //���г�ʼ��  
{
	struct min_pq* min_pq;

	min_pq = malloc(sizeof(struct min_pq));
	min_pq->pq_array = calloc(MINPQ_INIT_NALLOCD, sizeof(struct pq_node));//������еĿռ�  
	min_pq->nallocd = MINPQ_INIT_NALLOCD;
	min_pq->n = 0;//������Ԫ�صĸ���  

	return min_pq;
}

int minpq_insert(struct min_pq* min_pq, void* data, int key)//�����ȶ����в���Ԫ��  
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
	decrease_pq_node_key(min_pq->pq_array, min_pq->n, key); //����Ԫ�ص����ȶ�����,�������㷨  
	min_pq->n++;
	return 0;
}

static struct kd_node* explore_to_leaf(struct kd_node* kd_node,    //��kd_node��ʼ��ʼ��ѯֱ��Ҷ�ڵ�Ϊֹ  
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

		if (minpq_insert(min_pq, unexpl, ABS(kv - feat->descr[ki])))   //δ��ѯ���Ľ�㣬����ֵ��С�������ȶ���  
		{
			return NULL;
		}
	}

	return expl;
}