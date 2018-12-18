#pragma once
#include "public.h"

class Hough {
private:

    // Hough空间图像
    CImg<int> houghSpace;
    // 直线拟合的图像
    CImg<unsigned char> blueLineImage;
    // 原图拟合效果
    CImg<unsigned char> fitPointImage;
    // 角点图
    CImg<unsigned char> intersactionImage;
    // canny算法的结果
    CImg<unsigned char> cannyResult;

    vector<Hough_pos> peak;

    // 存储直线集合的信息
    vector<line> lines;

public:
    // 构造函数
    Hough(string filepath, string filename, double votingThreshold, double peakDistance, CImg<unsigned char>, double);
    // 由canny算法得到的图像进行Hough变换
    CImg<int> initialHoughFromCanny();

    void PointPeak(double, double);
    // 获得直线
    void getLine();
    //  求出交点
    void getInterLine();
    // 画出拟合和原来的匹配图像
    void getFitPoint(double);
    CImg<unsigned char> getResult(){return intersactionImage;}
    vector<Hough_pos> result;
};