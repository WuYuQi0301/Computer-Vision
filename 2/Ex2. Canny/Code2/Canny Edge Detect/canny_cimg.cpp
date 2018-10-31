#include "canny_cimg.hpp"

canny_cimg::canny_cimg(string filename, int filter_size,int sigmaIn, int low, int high)
{
	string pathRoot = "./result/";
	img.load_bmp(filename.c_str());
	if (!img.data())   //Return a pointer to the first pixel value. 
	{
		cout << "Could not open or find the image" << std::endl;
	}
	else 
	{
		img.display();

		vector<vector<double>> filter;
		createFilter(filter, filter_size, filter_size, sigmaIn);
		    //Print filter
	    for (int i = 0; i < filter.size(); i++) 
	    {
	        for (int j = 0; j < filter[i].size(); j++) 
	        {
	            cout << filter[i][j] << " ";
	        }
	        cout << endl;
	    }

	    cout << "width: " << img.width() << " height: " << img.height() <<endl;
	    // grayscaled = this->toGrayScale(); //Grayscale the image
	    toGrayScale(); //Grayscale the image
		grayscaled.display("GrayScaled");
		string path = pathRoot + "GrayScaled.bmp";
		grayscaled.save(path.c_str());

   		// gFiltered = useFilter(grayscaled, filter); //Gaussian Filter
 		useFilter(grayscaled, filter); //Gaussian Filter
		gFiltered.display("Gaussian Blur");
		path = pathRoot + "Gaussian-Blur.bmp";
		gFiltered.save(path.c_str());

    	// sFiltered = sobel(); //Sobel Filter
    	sobel(); //Sobel Filter
		sFiltered.display("Sobel Filter");
		path = pathRoot + "Sobel-Filter.bmp";
		sFiltered.save(path.c_str());
    	
    	// non = nonMaxSupp(); //Non-Maxima Suppression
    	nonMaxSupp(); //Non-Maxima Suppression
		non.display("Non-Maxima Suppression");
		path = pathRoot + "Non-Maxima-Supped.bmp";
		non.save(path.c_str());
    	
    	// thres = threshold(non, 20, 40); //Double Threshold and Finalize
    	thres = threshold(non, low, high); //Double Threshold and Finalize
		thres.display("Final");
		path = pathRoot + "Final.bmp";
		thres.save(path.c_str());
	}
}
CImg<unsigned char> canny_cimg::toGrayScale()
{
	//考虑存double值
	grayscaled = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0);//To one channel
	for (int i = 0; i < img.height(); ++i)
	{
		for (int j = 0; j < img.width(); ++j)
		{
			int r = img.atXYZC(i, j, 1, 0);
			int g = img.atXYZC(i, j, 1, 1);
			int b = img.atXYZC(i, j, 1, 2);
			// cout << i << " " << j <<endl;
			grayscaled.atXYZC(i, j, 1, 0) = (unsigned char)(r * 0.2126 + g * 0.7152 + b * 0.0722);
		}
	}

	return grayscaled;
}
void canny_cimg::createFilter(vector<vector<double>> &filter, int row, int col, double sigmaIn)
{
	// vector<vector<double>> filter;
	for (int i = 0; i < row; ++i)
	{
		std::vector<double> column;
		for (int j = 0; j < col; ++j)
		{
			column.push_back(-1);
		}
		filter.push_back(column);
	}

	float coordSum = 0;
	float constant = 2.0 * sigmaIn * sigmaIn;

	float sum = 0.0;
	for (int x = -row/2; x <= row/2; ++x)
	{
		for (int y = -col; y <= col/2 ; ++y)
		{
			coordSum = (x*x + y*y);  //这系数好像怪怪的
			filter[x + row/2][y + col/2] = (exp(-(coordSum) / constant)) / (M_PI * constant);
			sum += filter[x + row/2][y + col/2];
		}	
	}

	// Normalize the Filter
	for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            filter[i][j] /= sum;

	// return filter;
}
//高斯平滑
CImg<unsigned char> canny_cimg::useFilter(CImg<unsigned char> img_in, vector<vector<double>> filterIn)
{
	int size = (int)filterIn.size()/2;
	gFiltered = CImg<unsigned char>(img_in.width()-2*size, img_in.height()-2*size, 1, 1, 0);
	for (int i = size; i < img_in.height() - size; i++)
	{
		for (int j = size; j < img_in.width() - size; ++j)
		{

			double sum = 0;
            
			for (int x = 0; x < filterIn.size(); x++)
				for (int y = 0; y < filterIn.size(); y++)
				{
                    sum += filterIn[x][y] * (double)(img_in.atX(i + x - size, j + y - size, 0));
				}
            
            gFiltered.atXY(i-size, j-size) = sum;	
		}
	}
	return gFiltered;
}
CImg<unsigned char> canny_cimg::sobel()
{
    //Sobel X Filter
    double x1[] = {-1.0, 0, 1.0};
    double x2[] = {-2.0, 0, 2.0};
    double x3[] = {-1.0, 0, 1.0};

    vector<vector<double>> xFilter(3);
    xFilter[0].assign(x1, x1+3);
    xFilter[1].assign(x2, x2+3);
    xFilter[2].assign(x3, x3+3);
    
    //Sobel Y Filter
    double y1[] = {1.0, 2.0, 1.0};
    double y2[] = {0, 0, 0};
    double y3[] = {-1.0, -2.0, -1.0};
    
    vector<vector<double>> yFilter(3);
    yFilter[0].assign(y1, y1+3);
    yFilter[1].assign(y2, y2+3);
    yFilter[2].assign(y3, y3+3);

    
    //size = 1;
    int size = (int)xFilter.size()/2;
    //建立一个滤波结果image，大小为梯度矩阵小2行
    sFiltered = CImg<unsigned char>(gFiltered.width() - 2*size, gFiltered.height() - 2*size, 1, 1, 0);
    //初始化angles图像尺寸同上；
    angles = CImg<unsigned char>(gFiltered.width() - 2*size, gFiltered.height() - 2*size, 1, 1, 0); //AngleMap
    //0~rows-1
    for (int i = size; i < gFiltered.height() - size; ++i)   //rows
    {
    	for (int j = size; j < gFiltered.width() - size; ++j) //cols
    	{
    		double sumx = 0;
    		double sumy = 0;

    		for (int x = 0; x < xFilter.size(); ++x)
    		{
    			for (int y = 0; y < xFilter.size(); ++y)
    			{
                    sumx += xFilter[x][y] * (double)(gFiltered.atXY(i + x - size, j + y - size)); //Sobel_X Filter Value
                    sumy += yFilter[x][y] * (double)(gFiltered.atXY(i + x - size, j + y - size)); //Sobel_Y Filter Value
    			}
    		}

    		// double sumxsq = sumx * sumx;
    		// double sumysq = sumy * sumy;
    		double sq2 = sqrt(sumx * sumx + sumy * sumy);

    		if(sq2 > 255)
    			sq2 = 255;
    		sFiltered.atXY(i - size, j - size) = sq2;//0~（480-2-2）
    		
    		if(sumx == 0)                              //梯度方向矩阵
    			angles.atXY(i - size, j - size) = 90;  //tan(y算子/x算子)
    		else
    			angles.atXY(i - size, j - size) = atan(sumy/sumx)*(180.0/M_PI); // 返回值是弧度制。需要乘上180/PI,后面的计算使用角度制的
    	}
    }
    return sFiltered;
}
CImg<unsigned char> canny_cimg::nonMaxSupp()
{
	non = CImg<unsigned char>(sFiltered.width() - 2, sFiltered.height() - 2, 1,1,0);
	for (int i = 1; i < sFiltered.height() - 1; ++i)
	{
		for (int j = 1; j < sFiltered.width() - 1; ++j)   //这里可能把行列换了
		{
			float Tangent = angles.atXY(i, j);   //(row, col)

			non.atXY(i-1, j-1) = sFiltered.atXY(i, j);

			//不使用线性插值，用45度区间的临近值（像素）近似代替
            //Horizontal Edge 
			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((sFiltered.atXY(i,j) < sFiltered.atXY(i,j+1)) || (sFiltered.atXY(i,j) < sFiltered.atXY(i,j-1)))
                    non.atXY(i-1, j-1) = 0;
            }
            //Vertical Edge
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((sFiltered.atXY(i,j) < sFiltered.atXY(i+1,j)) || (sFiltered.atXY(i,j) < sFiltered.atXY(i-1,j)))
                    non.atXY(i-1, j-1) = 0;
            }
            
            //-45 Degree Edge
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((sFiltered.atXY(i,j) < sFiltered.atXY(i-1,j+1)) || (sFiltered.atXY(i,j) < sFiltered.atXY(i+1,j-1)))
                    non.atXY(i-1, j-1) = 0;
            }
            
            //45 Degree Edge
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((sFiltered.atXY(i,j) < sFiltered.atXY(i+1,j+1)) || (sFiltered.atXY(i,j) < sFiltered.atXY(i-1,j-1)))
                    non.atXY(i-1, j-1) = 0;
            }
        }
    }
    return non;
}

CImg<unsigned char> canny_cimg::threshold(CImg<unsigned char> imgin, int low, int high)
{
	if(low > 255)
        low = 255;
    if(high > 255)
        high = 255;

    thres = CImg<unsigned char>(imgin.width(), imgin.height(),1,1,0);

    for (int i=0; i < imgin.height(); i++) 
    {
        for (int j = 0; j < imgin.width(); j++) 
		{
			//turn it red if the color is white
			thres.atXY(i, j) = imgin.atXY(i, j);
			if (thres.atXY(i, j) > high)
				thres.atXY(i, j) = 255;   //strong edge
			else if (thres.atXY(i, j) < low) 
				thres.atXY(i, j) = 0;     //weak edge
			else                            //supress
			{
				bool anyHigh = false;
				bool anyBetween = false;

                for (int x=i-1; x < i+2; x++) 
                {
                    for (int y = j-1; y<j+2; y++) 
                    {
	                	if(x <= 0 || y <= 0 || x > thres.height() || y > thres.width()) //Out of bounds
                            continue;
                        if(thres.atXY(x, y) > high)   //若八邻域内有high值，该点为255
                        {
                        	thres.atXY(i, j) = 255;
                        	anyHigh = true;
                        	break;
                        }
                        else if(thres.atXY(x, y) <= high && thres.atXY(x, y) >= low)
                        	anyBetween = true;
   	                }
	            if (anyHigh)
	            	break;
	            }
	            if (!anyHigh && anyBetween)  //若八邻域内没有极大值只有中间值
	            {
	            	for (int x = i - 2; x <= i + 2; x++)
	            	{
                        for (int y = j - 1; y <= j + 2; y++) //为什么是1不是2（24邻域）
	            		{
	            			if(x <= 0 || y <= 0 || x > thres.height() || y > thres.width()) //Out of bounds
                        	    continue;
	                        if(thres.atXY(x, y) > high)   //若24邻域内有high值，该点为255
	                        {
	                        	thres.atXY(i, j) = 255;
	                        	anyHigh = true;
	                        	break;
	                        }
	            		}
	            		if (anyHigh)
	            			break;
	            	}
	            	if (!anyHigh)
	            		thres.atXY(i, j) = 0;   //若24邻域内仍没有大于high的极大值，则设为0；
	            }
			}
		}
	}	 
	return thres;
}