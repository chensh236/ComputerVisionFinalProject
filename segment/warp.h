#pragma once

#include "public.h"
#include "Projection.h"

class warp{
public:
    warp(vector<Hough_pos>&, CImg<unsigned char>&, int);
    CImg<unsigned char> getResult(){return result;}
    ~warp(){}
    double srcPos[4][2];
private:

    // 求出原图像到变换后的点
    double getXAfterWarping(double, double, Homography &);
    double getYAfterWarping(double, double, Homography &);
    void warpingImageByHomography(const CImg<unsigned char> &, CImg<unsigned char> &, Homography &);
    Homography getHomography(vector<Hough_pos>&);
    Homography getHomography2(vector<Hough_pos>&);
    CImg<unsigned char> result;
};