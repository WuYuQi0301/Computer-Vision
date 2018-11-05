			while(1)
			{
				//搜索八邻域内最大值
				for (int x = currentX-1; x <= currentX+1; ++x)
				{
					for (int y = currentY+1; y <= currentY+1; ++y)
					{
						if((x == currentX && y == currentY) || x < 0 || y < 0 
							|| x > img1.width() || y > img1.height() 
							|| visited.atXY(x, y) == 255)
							continue;
							
						visited.atXY(x, y) = 255;
						if (houghSpace.atXY(x, y) > houghSpace.atXY(maxX, maxY))
						{
							maxX = x;
							maxY = y;
						}
					}
				}
				//若当前值不是最大值，将八邻域内最大值作为当前值继续进行八邻域搜索
				if (maxX != currentX || maxY != currentY)
				{
					currentX = maxX, currentY = maxY;
					continue;
				}
				else {   //若当前值即八邻域内最大值，搜索其24邻域
					for (int x = currentX-2; x <= currentX+2; ++x)
					{
						for (int y = currentY+2; y <= currentY+2; ++y)
						{
							if((x == currentX && y == currentY) || x < 0 || y < 0 
								|| x > img1.width() || y > img1.height() 
								|| visited.atXY(x, y) == 255)
								continue;
								
							visited.atXY(x, y) = 255;
							if (houghSpace.atXY(x, y) > houghSpace.atXY(maxX, maxY))
							{
								maxX = x;
								maxY = y;
							}
						}
					}

					//若当前值是24邻域内最大值，搜索局部最大值成功并返回；
					if (maxX == currentX && maxY == currentY)
					{
						break;
					} //否则
					else {
						currentX = maxX, currentY = maxY;
						continue;
					}
				}
			}