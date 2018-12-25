#include "warp.h"

// 求出原图像到变换后的点
double warp::getXAfterWarping(double x, double y, Homography &H)
{
	return H.H[0][0] * x + H.H[0][1] * y + H.H[0][2] * x * y + H.H[1][0];
}

double warp::getYAfterWarping(double x, double y, Homography &H)
{
	return H.H[1][1] * x + H.H[1][2] * y + H.H[2][0] * x * y + H.H[2][1];
}

void warp::warpingImageByHomography(const CImg<unsigned char> &src, CImg<unsigned char> &dst, Homography &H)
{
	cimg_forXY(dst, x, y)
	{
		double newX = getXAfterWarping((double)x, (double)y, H);
		double newY = getYAfterWarping((double)x, (double)y, H);
		if (newX >= 0.0 && newX < (double)src.width() && newY >= 0.0 && newY < (double)src.height())
		{
			for (int i = 0; i < src.spectrum(); i++)
			{
				dst(x, y, i) = Projection::bilinearInterpolation(src, newX, newY, i);
			}
		}
	}
}

Homography warp::getHomography(vector<Hough_pos> &vec)
{
	double destPos[4][2] = {0};
	// 初始化标准A4纸坐标
	for (int i = 0; i < 4; i++)
	{
		if (i == 1 || i == 2)
			destPos[i][0] = 594.0;
		if (i == 2 || i == 3)
			destPos[i][1] = 841.0;
		srcPos[i][0] = 0;
		srcPos[i][1] = 0;
	}

	// 线段对
	vector<pointPair> posPairs;
	posPairs.clear();

	// 每两条求出距离
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			pointPair pair;
			pair.distance = sqrt(pow((vec[i].x - vec[j].x), 2) + pow((vec[i].y - vec[j].y), 2));
			pair.pos1[0] = vec[i].x;
			pair.pos1[1] = vec[i].y;
			pair.pos2[0] = vec[j].x;
			pair.pos2[1] = vec[j].y;
			posPairs.push_back(pair);
		}
	}

	// 排序，求出最短两条线段作为短边
	sort(posPairs.begin(), posPairs.end(), sorting);
	pointPair topPair, bottomPair;
	topPair = posPairs[0];
	bottomPair = posPairs[1];

    // 比较横向的x，如果bottom小于top，位置颠倒
    if (topPair.pos1[1] + topPair.pos2[1] > bottomPair.pos1[1] + bottomPair.pos2[1])
    {
        pointPair tmp = topPair;
        topPair = bottomPair;
        bottomPair = tmp;
    }
    // 如果y不符合上下位置，上面两点位置颠倒
    if (topPair.pos2[0] < topPair.pos1[0])
    {
        topPair.swap();
    }
    // 同样下面两点位置颠倒
    if (bottomPair.pos2[0] > bottomPair.pos1[0])
    {
        bottomPair.swap();
    }

	// 整理
	srcPos[0][0] = topPair.pos1[0];
	srcPos[0][1] = topPair.pos1[1];
	srcPos[1][0] = topPair.pos2[0];
	srcPos[1][1] = topPair.pos2[1];
	srcPos[2][0] = bottomPair.pos1[0];
	srcPos[2][1] = bottomPair.pos1[1];
	srcPos[3][0] = bottomPair.pos2[0];
	srcPos[3][1] = bottomPair.pos2[1];

	// 求解AH = b
	CImg<double> A(4, 4, 1, 1, 0);
	CImg<double> b(1, 4, 1, 1, 0);

	for (int i = 0; i < 4; i++)
	{
		A(0, i) = destPos[i][0];
		A(1, i) = destPos[i][1];
		A(2, i) = destPos[i][0] * destPos[i][1];
		A(3, i) = 1.0;
		b(0, i) = srcPos[i][0];
	}

	CImg<double> x1 = b.get_solve(A);

	for (int i = 0; i < 4; i++)
	{
		b(0, i) = srcPos[i][1];
	}

	CImg<double> x2 = b.get_solve(A);
	CImg<double> coff(1, 8, 1, 1, 0);
	for (int i = 0; i < 8; i++)
	{
		if (i < 4)
			coff(0, i) = x1(0, i);
		else
			coff(0, i) = x2(0, i - 4);
	}
	return Homography(x1(0, 0), x1(0, 1), x1(0, 2), x1(0, 3), x2(0, 0), x2(0, 1), x2(0, 2), x2(0, 3));
}

// choice->0 A4   choice->1 part
warp::warp(vector<Hough_pos> &vec, CImg<unsigned char>&src, int choice)
{
    if(choice == 0){
        Homography H = getHomography(vec);
        result = CImg<unsigned char>(595, 842, 1, 3);
        warpingImageByHomography(src, result, H);
    }else if(choice == 1){
        Homography H = getHomography2(vec);
        result = CImg<unsigned char>(28, 28, 1, 1, 0);
        warpingImageByHomography(src, result, H);
    }

}

Homography warp::getHomography2(vector<Hough_pos> &vec)
{
	double destPos[4][2] = {0};
	double srcPos[4][2] = {0};
	// 初始化标准A4纸坐标
	for (int i = 0; i < 4; i++)
	{
		if (i == 1 || i == 2)
			destPos[i][0] = 28.0;
		if (i == 2 || i == 3)
			destPos[i][1] = 28.0;
	}

	for(int i = 0; i < vec.size(); i++){
		srcPos[i][0] = vec[i].x;
		srcPos[i][1] = vec[i].y;
	}

	// 求解AH = b
	CImg<double> A(4, 4, 1, 1, 0);
	CImg<double> b(1, 4, 1, 1, 0);

	for (int i = 0; i < 4; i++)
	{
		A(0, i) = destPos[i][0];
		A(1, i) = destPos[i][1];
		A(2, i) = destPos[i][0] * destPos[i][1];
		A(3, i) = 1.0;
		b(0, i) = srcPos[i][0];
	}

	CImg<double> x1 = b.get_solve(A);

	for (int i = 0; i < 4; i++)
	{
		b(0, i) = srcPos[i][1];
	}

	CImg<double> x2 = b.get_solve(A);
	CImg<double> coff(1, 8, 1, 1, 0);
	for (int i = 0; i < 8; i++)
	{
		if (i < 4)
			coff(0, i) = x1(0, i);
		else
			coff(0, i) = x2(0, i - 4);
	}
	return Homography(x1(0, 0), x1(0, 1), x1(0, 2), x1(0, 3), x2(0, 0), x2(0, 1), x2(0, 2), x2(0, 3));
}