#pragma once
#include "public.h"

class Projection{
public:
    // 将图像投影到柱面坐标上
    static CImg<unsigned char> imageProjection(const CImg<unsigned char> &);
    // 进行双线性插值计算
    // cimg : 输入图像
    // float : x
    // float : y
    // int : channel
    static unsigned char bilinearInterpolation(const CImg<unsigned char>&, double, double, int);
};