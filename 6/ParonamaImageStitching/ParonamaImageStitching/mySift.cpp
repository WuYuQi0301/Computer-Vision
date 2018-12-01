#include "mySift.h"

mySift::mySift(){ }
bool mySift::SiftProcessing(string imgName, string imgPath, string spPath, string siPath)
{
	if (imgName == "" || imgPath == "" || spPath == "" || siPath == "")
		return false;

	fstream tmp;
	tmp.open((spPath+"tmp.txt").c_str(), ios::out);
	if (!tmp) return false;

	VL_PRINT(("hello, VLFeat :  " + imgName + "\n").c_str());

	Mat img1 = imread((imgPath+imgName+".bmp").c_str(), 0);
	int min = 0, noctaves = 4, nlevels = 2;

	//typedef float vl_sift_pix���������� sift.�� 
	//float���飬���ϵ��´����Ҵ洢����ͼ��ĻҶ�ֵ
	vl_sift_pix *ImageData = new vl_sift_pix[img1.cols*img1.rows];
	for (int i = 0; i < img1.rows; i++)
	{
		uchar *pixels = img1.ptr<uchar>(i);
		for (int j = 0; j < img1.cols; j++)
			ImageData[i*img1.cols + j] = pixels[j];
	}

	//����sift�˲���ָ�� ������һ���µ�sift�˲���
	VlSiftFilt *siftFilt = vl_sift_new(img1.cols, img1.rows, noctaves, nlevels, min);

	int keyPoint = 0, index = 0, scale = 0;
	int total = 0;

	if (vl_sift_process_first_octave(siftFilt, ImageData) != VL_ERR_EOF)
	{
		while (1)
		{
			scale++;
			//����ͼ�йؼ���
			vl_sift_detect(siftFilt);
			//����������ÿ����
			keyPoint += siftFilt->nkeys; //��⵽�Ĺؼ�����Ŀ
			cout << " number of key point : " << keyPoint << endl;
			total += keyPoint;

			VlSiftKeypoint *pKeyPoint = siftFilt->keys;
			for (int i = 0; i < siftFilt->nkeys; i++)
			{
				VlSiftKeypoint tempKeyPoint = *pKeyPoint;
				pKeyPoint++;
				//void cvCircle( CvArr* img, CvPoint center, int radius, CvScalar color,int thickness = 1, int line_type = 8, int shift = 0 );
				circle(img1, Point(tempKeyPoint.x, tempKeyPoint.y), tempKeyPoint.sigma / 2, CV_RGB(255, 255, 0));
				index++;
				//���㲢����ÿ����ķ���
				double angles[4];
				//����ؼ���ķ���
				int angleCount = vl_sift_calc_keypoint_orientations(siftFilt, angles, &tempKeyPoint);

				//���� �ؼ��� ���ڳ߶ȣ�λ�ã����꣩��������������Ϣ
				tmp << scale << " " << tempKeyPoint.x << " " << tempKeyPoint.y << " " << angleCount << endl;

				for (int j = 0; j < angleCount; j++)
				{
					double tempAngle = angles[j];
					tmp << tempAngle << endl;
					//����ÿ�����������
					float *descriptor = new float[128];
					vl_sift_calc_keypoint_descriptor(siftFilt, descriptor, &tempKeyPoint, tempAngle);

					for (int k = 0; k < 128; k++)
						tmp << descriptor[k] << " ";
					tmp << endl;

					delete[] descriptor;
					descriptor = NULL;
				}
			}
			if (vl_sift_process_next_octave(siftFilt) == VL_ERR_EOF)
				break;
			keyPoint = NULL;
		}
	}
	vl_sift_delete(siftFilt);
	delete[] ImageData;
	ImageData = NULL;
	//namedWindow("Source Image", 1);
	//imshow("Source Image", img1);

	imwrite((siPath + imgName + ".bmp").c_str(), img1);
	waitKey(0);

	tmp.close();
	tmp.open((spPath + "tmp.txt").c_str(), ios::in);
	if (!tmp) return false;

	std::fstream fout;
	fout.open((spPath + imgName + ".txt").c_str(), ios::out);
	if (!fout) return false;

	cout << "total: " << total << endl << endl;
	fout << total << endl;

	int	Max = 2048;
	char buffer[2048];
	tmp.seekg(0, ios::beg);
	while (tmp)
	{
		tmp.getline(buffer, Max);
		fout << buffer << endl;
	}

	fout.close();
	tmp.close();
	
	remove((spPath + "tmp.txt").c_str());
	return true;
}