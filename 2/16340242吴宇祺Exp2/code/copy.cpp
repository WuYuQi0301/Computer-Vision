#include "copy.hpp"
canny::canny(string filename, string root, int filter_size,int sigmaIn, int low , int high, int note)
{
	img.load_bmp(filename.c_str());
	
	if (!img.data()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
	}
	else
	{

		vector<vector<double>> filter = createFilter(filter_size, filter_size, sigmaIn);

	    //Print filter
	    for (int i = 0; i < filter.size(); i++) 
	    {
	        for (int j = 0; j < filter[i].size(); j++) 
	        {
	            cout << filter[i][j] << " ";
	        }
	        cout << endl;
	    }
		// img.display("Original");  

	    grayscaled = CImg<unsigned char>(toGrayScale()); //Grayscale the image
	    gFiltered = CImg<unsigned char>(useFilter(grayscaled, filter)); //Gaussian Filter
	    sFiltered = CImg<unsigned char>(sobel()); //Sobel Filter

	    non = CImg<unsigned char>(nonMaxSupp()); //Non-Maxima Suppression
	    thres = CImg<unsigned char>(threshold(non, low, high)); //Double Threshold and Finalize

	    // grayscaled.display("GrayScaled");
	    grayscaled.save((root + "gray.bmp").c_str());

	    // gFiltered.display("Gaussian Blur");
	    gFiltered.save((root + "Gaussian.bmp").c_str());
	    
	    // sFiltered.display("Sobel Filtered");
	    sFiltered.save((root + "sFiltered.bmp").c_str());
	    
	    // non.display("Non-Maxima Supp.");
	    non.save((root + "non.bmp").c_str());
	    
	    // thres.display("Thresholded");
	    thres.save((root + "Thresholded.bmp").c_str());

	    if (note == 1)
	    {
			final = CImg<unsigned char>(shortEdgeSupp(thres));
		    final.save((root + "Final.bmp").c_str());
		}
	}
}

CImg<unsigned char> canny::toGrayScale()
{
    CImg<unsigned char> result = CImg<unsigned char>(img.width(), img.height(), 1, 1, 0); //To one channel
	for (int i = 0; i < img.height(); i++)
		for (int j = 0; j < img.width(); j++)
		{
			int b = img(j, i, 2);
			int g = img(j, i, 1);
			int r = img(j, i, 0);

			double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
			result(j, i) = newValue;
		}
    return result;
}

vector<vector<double> > canny::createFilter(int row, int column, double sigmaIn)
{
	vector<vector<double>> filter;

	for (int i = 0; i < row; i++)
	{
        vector<double> col;
        for (int j = 0; j < column; j++)
        {
            col.push_back(-1);
        }
		filter.push_back(col);
	}

	float coordSum = 0;
	float constant = 2.0 * sigmaIn * sigmaIn;

	// Sum is for normalization
	float sum = 0.0;

	for (int x = - row/2; x <= row/2; x++)
	{
		for (int y = -column/2; y <= column/2; y++)
		{
			coordSum = (x*x + y*y);
			filter[x + row/2][y + column/2] = (exp(-(coordSum) / constant)) / (M_PI * constant);
			sum += filter[x + row/2][y + column/2];
		}
	}

	// Normalize the Filter
	for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            filter[i][j] /= sum;

	return filter;

}

CImg<unsigned char> canny::useFilter(CImg<unsigned char> img_in, vector<vector<double>> filterIn)
{
    int size = (int)filterIn.size()/2;
	CImg<unsigned char> filteredImg = CImg<unsigned char>(img_in.width() - 2*size, img_in.height() - 2*size, 1, 1, 0);
	for (int i = size; i < img_in.height() - size; i++)
	{
		for (int j = size; j < img_in.width() - size; j++)
		{
			double sum = 0;
            
			for (int x = 0; x < filterIn.size(); x++)
				for (int y = 0; y < filterIn.size(); y++)
				{
                    sum += filterIn[x][y] * (double)(img_in.atXY(j + y - size, i + x - size));
				}
            
            filteredImg.atXY(j-size, i-size) = sum;
		}

	}
	return filteredImg;
}

CImg<unsigned char> canny::sobel()
{

    //Sobel X Filter
    double x1[] = {-1.0, 0, 1.0};
    double x2[] = {-2.0, 0, 2.0};
    double x3[] = {-1.0, 0, 1.0};

    vector<vector<double> > xFilter(3);
    xFilter[0].assign(x1, x1+3);
    xFilter[1].assign(x2, x2+3);
    xFilter[2].assign(x3, x3+3);
    
    //Sobel Y Filter
    double y1[] = {1.0, 2.0, 1.0};
    double y2[] = {0, 0, 0};
    double y3[] = {-1.0, -2.0, -1.0};
    
    vector<vector<double> > yFilter(3);
    yFilter[0].assign(y1, y1+3);
    yFilter[1].assign(y2, y2+3);
    yFilter[2].assign(y3, y3+3);
    
    //Limit Size
    int size = (int)xFilter.size()/2;
    
	CImg<unsigned char> filteredImg = CImg<unsigned char>(gFiltered.width() - 2*size, gFiltered.height() - 2*size, 1, 1, 0);
    
    angles = CImg<unsigned char>(gFiltered.width() - 2*size, gFiltered.height() - 2*size, 1, 1, 0); //AngleMap

	for (int i = size; i < gFiltered.height() - size; i++)
	{
		for (int j = size; j < gFiltered.width() - size; j++)
		{
			double sumx = 0;
            double sumy = 0;
            
			for (int x = 0; x < xFilter.size(); x++)
				for (int y = 0; y < xFilter.size(); y++)
				{
                    sumx += xFilter[x][y] * (double)(gFiltered.atXY( j + y - size, i + x - size)); //Sobel_X Filter Value
                    sumy += yFilter[x][y] * (double)(gFiltered.atXY(j + y - size, i + x - size)); //Sobel_Y Filter Value
				}
            double sumxsq = sumx*sumx;
            double sumysq = sumy*sumy;
            
            double sq2 = sqrt(sumxsq + sumysq);
            
            if(sq2 > 255) //Unsigned Char Fix
                sq2 =255;
            filteredImg.atXY(j-size, i-size) = sq2;
 
            if(sumx==0) //Arctan Fix
                angles.atXY(j-size, i-size) = 90;
            else
                angles.atXY(j-size, i-size) = atan(sumy/sumx);
		}
	}
    
    return filteredImg;
}


CImg<unsigned char> canny::nonMaxSupp()
{
    CImg<unsigned char> nonMaxSupped = CImg<unsigned char>(sFiltered.width()-2, sFiltered.height()-2, 1, 1, 0);
    for (int i=1; i< sFiltered.height() - 1; i++) {
        for (int j=1; j<sFiltered.width() - 1; j++) {
            float Tangent = angles.atXY(j, i);

            nonMaxSupped.atXY(j-1, i-1) = sFiltered.atXY(j, i);
            //Horizontal Edge 
            if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((sFiltered.atXY(j, i) < sFiltered.atXY(j+1, i)) || (sFiltered.atXY(j, i) < sFiltered.atXY(j-1, i)))
                    nonMaxSupped.atXY(j-1, i-1) = 0;
            }
            //Vertical Edge
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((sFiltered.atXY(j, i) < sFiltered.atXY(j, i+1)) || (sFiltered.atXY(j, i) < sFiltered.atXY(j, i-1)))
                    nonMaxSupped.atXY(j-1, i-1) = 0;
            }
            
            //-45 Degree Edge
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((sFiltered.atXY(j, i) < sFiltered.atXY(j+1, i-1)) || (sFiltered.atXY(j, i) < sFiltered.atXY(j-1, i+1)))
                    nonMaxSupped.atXY(j-1, i-1) = 0;
            }
            
            //45 Degree Edge
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((sFiltered.atXY(j, i) < sFiltered.atXY(j+1, i+1)) || (sFiltered.atXY(j, i) < sFiltered.atXY(j-1, i-1)))
                    nonMaxSupped.atXY(j-1, i-1) = 0;
            }
        }
    }
    return nonMaxSupped;
}

CImg<unsigned char> canny::threshold(CImg<unsigned char> imgin,int low, int high)
{
    if(low > 255)
        low = 255;
    if(high > 255)
        high = 255;
    
    CImg<unsigned char> EdgeMat = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 1, 0);
    for (int i=0; i < imgin.height(); i++) 
    {
        for (int j = 0; j < imgin.width(); j++) 
        {
            EdgeMat.atXY(j, i) = imgin.atXY(j, i);  //强边缘点
            if(EdgeMat.atXY(j, i) > high)
                EdgeMat.atXY(j, i) = 255;
            else if(EdgeMat.atXY(j, i) < low)       //非边缘点
                EdgeMat.atXY(j, i) = 0;
            else                                    //弱边缘点的八邻域检测是否有强边缘点 
            {
                bool anyHigh = false;
                bool anyBetween = false;
                for (int x = i-1; x < i+2; x++) 
                {
                    for (int y = j-1; y<j+2; y++) 
                    {
                        if(x <= 0 || y <= 0 || x >= EdgeMat.height() || y >= EdgeMat.width()) //Out of bounds
                            continue;
                        else
                        {
                            if(EdgeMat.atXY(y,x) > high)        //若弱边缘点的八邻域中有强边缘点
                            {
                                EdgeMat.atXY(j, i) = 255;       //保留该弱边缘点为强边缘
                                anyHigh = true;
                                break;
                            }
                            else if(EdgeMat.atXY(y,x) <= high && EdgeMat.atXY(y,x) >= low)
                                anyBetween = true;             
                        }
                    }
                    if(anyHigh)
                        break;
                }
                if(!anyHigh && anyBetween)                      //若该弱边缘点的八邻域中没有强边缘点只有弱边缘点
                    for (int x = i-2; x < i+3; x++)             //查找4*4邻域
                    {
                        for (int y = j-1; y<j+3; y++) 
                        {
                            if(x < 0 || y < 0 || x >= EdgeMat.height() || y >= EdgeMat.width()) //Out of bounds
                                continue;
                            else
                            {
                                if(EdgeMat.atXY(y,x) > high)    //若4*4邻域中有强边缘点，保留该点作为强边缘
                                {
                                    EdgeMat.atXY(j, i) = 255;
                                    anyHigh = true;
                                    break;
                                }
                            }
                        }
                        if(anyHigh)
                            break;
                    }
                if(!anyHigh)                                    //若该弱边缘点的4*4邻域内都没有强点，删除之
                    EdgeMat.atXY(j, i) = 0;
            }
        }
    }
    return EdgeMat;
}

CImg<unsigned char> canny::shortEdgeSupp(CImg<unsigned char> imgin, int minLength)
{
	CImg<unsigned char> shortEdgeSupped = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 3, 0);
	CImg<unsigned char> flag = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 1, 0);

	for (int i = 0; i < imgin.height(); ++i)
	{
		for (int j = 0; j < imgin.width(); ++j)
		{
			if (flag.atXY(j, i) == 255)
				continue;
			flag.atXY(j, i) = 255;               //标志已经访问过
			
			if (imgin.atXY(j, i) == 0)           //非边缘点
				continue;
			
			vector<pair<int, int> > connected;
			connected.push_back(pair<int, int >(j, i));
			int index = -1;

			while(index != (connected.size() - 1))  //搜索完成条件
			{
				index++;

				int currentX = connected[index].first, currentY = connected[index].second;
				for (int x = currentX - 1; x <= currentX + 1; ++x)     //搜索当前点的八邻域
				{
					for (int y = currentY - 1; y <= currentY + 1; ++y)
					{

						if(x <= 0 || y <= 0 || x >= imgin.height() || y >= imgin.width() || flag.atXY(x, y) == 255) //Out of bounds
                            continue;

                       	flag.atXY(x, y) = 255;             //标志已经访问过
                       	if (imgin.atXY(x, y) == (unsigned char)255)       //八邻域中有边缘点
                       		connected.push_back(pair<int, int>(x, y));
					}
				}
			}

			if (connected.size() >= 20)
				for (int x = 0; x < connected.size(); ++x)
				{
					shortEdgeSupped.atXY(connected[x].first, connected[x].second, 0) = 255;
					shortEdgeSupped.atXY(connected[x].first, connected[x].second, 1) = 255;
					shortEdgeSupped.atXY(connected[x].first, connected[x].second, 2) = 255;
				}
		}
	}
	return shortEdgeSupped;
}
