#include "kdTree.h"

int main()
{
	string path = "keyPointSet/test.txt";
	ofstream fout(path.c_str());
	ifstream fin("leyPointSet/pano1_0010.txt");
	int count = 10, total = 0;
	fin >> total;
	fout << count;
	for (int i = 0; i < count; i++)
	{
		int l;
		double x, y, oric, ori;
		fin >> l >> x >> y >> oric;
		fout << l << " " << x << " " << y << " " << oric;
		for (int j = 0; j < oric;j++)
		{
			float des;
			for (int k = 0; k < 128; k++)
			{
				fin >> des;
				fout << des;
			}
		}
	}
	

	kdTree testkd;

	testkd.BuildKdTree()

	return 0;
}