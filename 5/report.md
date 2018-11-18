HW5

Image warping 

几何变换

全局变换

控制点：控制点对的坐标决定了变换函数


变换类型：

前向变换和后向变换

最近邻：用欧氏距离最相近的原像素点替代

双线性插值：

Image **Morphing**

1. 输入控制点对
2. 用Delaunay算法定义三角形mesh
3. 变换每个三角形：仿射变换



#### 控制点对



| 标记   | 1        | 2    |
| ------ | -------- | ---- |
| 顶     | 242,268  |      |
| 左额角 |          |      |
| 左太阳 | 116,273  |      |
| 左耳   | 114.352  |      |
| 左颊   | 130,430, |      |
|        | 180,490  |      |
| 下巴   | 248,490  |      |
|        | 157,107  |      |
| 右颊   | 363,430  |      |
| 右耳   | 378,344  |      |
| 右太阳 | 377,268  |      |
| 右额角 |          |      |
| 左眉头 | 232,302  |      |
| 左眉峰 | 168,288  |      |
| 左眼   | 186,321  |      |
| 右眉头 | 274,305  |      |
| 右眉峰 | 329,285  |      |
| 右眼   | 306,322  |      |
| 鼻梁   | 250,328  |      |
| 鼻尖   | 249,380  |      |
| 左鼻孔 | 226,396  |      |
| 右鼻孔 | 270,397  |      |
| 唇峰   | 248,426  |      |
| 唇1    | 185,427  |      |
| 唇2    | 212.453  |      |
| 唇3    | 245,467  |      |
| 唇4    | 290,449  |      |
| 唇5    | 310,428  |      |

#### 三角剖分算法

> 【定义】Delaunay边：假设E中的一条边e（两个端点为a,b），e
>
> [![img](https://gss3.bdstatic.com/7Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D250/sign=3a2e6f0e0ef41bd5de53eff161da81a0/1f178a82b9014a90b133ce98a9773912b31bee82.jpg)](https://baike.baidu.com/pic/Delaunay%E4%B8%89%E8%A7%92%E5%89%96%E5%88%86%E7%AE%97%E6%B3%95/3779918/0/9f6e19086f08a225e92488e3?fr=lemma&ct=single)
>
> 若满足下列条件，则称之为Delaunay边：存在一个圆经过a,b两点，圆内(注意是圆内，圆上最多三点共圆)不含点集V中任何其他的点，这一特性又称空圆特性。
>
> 【定义】Delaunay三角剖分：如果点集V的一个三角剖分T只包含Delaunay边，那么该三角剖分称为Delaunay三角剖分。
>
> 优化处理:
>
> 理论上为了构造Delaunay[三角网](https://baike.baidu.com/item/%E4%B8%89%E8%A7%92%E7%BD%91) [2]  ，Lawson提出的局部优化过程LOP(Local Optimization Procedure)，一般三角网经过LOP处理，即可确保成为Delaunay三角网，其基本做法如下所示：
>
> 1.将两个具有共同边的三角形合成一个多边形。
>
> 2.以最大空圆准则作检查，看其第四个顶点是否在三角形的[外接圆](https://baike.baidu.com/item/%E5%A4%96%E6%8E%A5%E5%9C%86)之内。
>
> 3.如果在，修正对角线即将对角线对调，即完成局部优化过程的处理。

准则:

[![img](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=b80ab1c2912397ddd2799f066982b216/2cf5e0fe9925bc31815289bd5edf8db1cb13708c.jpg)](https://baike.baidu.com/pic/Delaunay%E4%B8%89%E8%A7%92%E5%89%96%E5%88%86%E7%AE%97%E6%B3%95/3779918/0/bd7faf35eec83f9ea61e12ed?fr=lemma&ct=single)

要满足Delaunay三角剖分的定义，必须符合两个重要的准则：

1、空圆特性：Delaunay[三角网](https://baike.baidu.com/item/%E4%B8%89%E8%A7%92%E7%BD%91)是唯一的（任意四点不能共圆），在Delaunay三角形网中任一三角形的外接圆范围内不会有其它点存在。如下图所示：

2、最大化最小角特性：在[散点](https://baike.baidu.com/item/%E6%95%A3%E7%82%B9)集可能形成的三角剖分中，Delaunay三角剖分所形成的三角形的最小角最大。从这个意义上讲，Delaunay三角网是“最接近于规则化的“的三角网。具体的说是指在两个相邻的三角形构成[凸四边形](https://baike.baidu.com/item/%E5%87%B8%E5%9B%9B%E8%BE%B9%E5%BD%A2)的对角线，在[相互交换](https://baike.baidu.com/item/%E7%9B%B8%E4%BA%92%E4%BA%A4%E6%8D%A2)后，六个内角的最小角不再增大。如下图所示：

[![img](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D250/sign=44260e0eb051f819f525044feab44a76/42166d224f4a20a43b17b4e290529822720ed08e.jpg)](https://baike.baidu.com/pic/Delaunay%E4%B8%89%E8%A7%92%E5%89%96%E5%88%86%E7%AE%97%E6%B3%95/3779918/0/504ec7f9aa1edd4c252df2ef?fr=lemma&ct=single)

特性:

以下是Delaunay剖分所具备的优异特性：

1.最接近：以最近的三点形成三角形，且各[线段](https://baike.baidu.com/item/%E7%BA%BF%E6%AE%B5)(三角形的边)皆不相交。L

2.唯一性：不论从区域何处开始构建，最终都将得到一致的结果。

3.最优性：任意两个相邻三角形形成的[凸四边形](https://baike.baidu.com/item/%E5%87%B8%E5%9B%9B%E8%BE%B9%E5%BD%A2)的对角线如果可以互换的话，那么两个三角形六个内角中最小的角度不会变大。

4.最规则：如果将[三角网](https://baike.baidu.com/item/%E4%B8%89%E8%A7%92%E7%BD%91)中的每个三角形的最小角进行升序排列，则Delaunay三角网的排列得到的数值最大。

5.区域性：新增、删除、移动某一个顶点时只会影响临近的三角形。

6.具有[凸多边形](https://baike.baidu.com/item/%E5%87%B8%E5%A4%9A%E8%BE%B9%E5%BD%A2)的外壳：三角网最外层的边界形成一个凸多边形的外壳。



1. 逐点插入

   基本原理：首先建立一个大的三角形或者多边形使其能够吧所有数据点包围起来。向其中插入一点，该点与包含它的三角形三个顶点相连，形成三个新三角形；对四个点逐个进行空外接圆检测，并LOP局部优化。

2. Watson算法基本步骤

   1. 构造一个包含所有散点的超级三角形，放入三角形列表；
   2. 插入一个散点，在三角形链表中找出*外接圆*包含插入点的三角形（影响三角形），删除影响三角形的公共边，将插入点同影响三角形的全部定点连接起来。
   3. LOP局部优化

### 仿射变换

对于每一个三角形
$$
U1 = \begin{bmatrix}
 pset1[k].x&pset1[k].x  &pset1[k].x \\ 
 pset1[k].y&pset1[k].y  &pset1[k].y \\ 
 1& 1 & 1
\end{bmatrix}
$$

$$
U2 = \begin{bmatrix}
 pset2[k].x&pset2[k].x  &pset2[k].x \\ 
 pset2[k].y&pset2[k].y  &pset2[k].y \\ 
 1& 1 & 1
\end{bmatrix}
$$

$$
A = \begin{bmatrix}
 p1.x&p2.x  &p3.x \\ 
 p1.y&p2.y  &p3.y \\ 
 1& 1 & 1
\end{bmatrix}
$$

$$
A^{-1}
$$


$$
m1 = U1*A^{-1}
$$

$$
m2 = U2*A^{-1}
$$

