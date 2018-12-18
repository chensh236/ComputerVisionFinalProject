#pragma once
#include "CImg.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stack>
#include <queue>
using namespace std;
using namespace cimg_library;

//---------------Hough-----------------------
// Hough空间中的位置信息
typedef struct Hough_pos{
    int x;
    int y;
    int val;
    Hough_pos(int _x, int _y, int _val):x(_x),y(_y),val(_val){}
    Hough_pos(){}
} Hough_pos;

// 直线的信息
typedef struct line{
    double k;
    double b;
    line(double _k, double _b):k(_k),b(_b){}
} line;
//-------------Projection------------------
#define ANGLE 15
#define CHANNEL_NUM 3

// 存储像素的RGB值
typedef struct Pixel{
    unsigned char val[CHANNEL_NUM];
    Pixel(){}
    Pixel(unsigned char _r, unsigned char _g, unsigned char _b){
        val[0] = _r;
        val[1] = _g;
        val[2] = _b;
    }
} Pixel;

//-----------------Segment----------------------
typedef struct position{
    int x;
    int y;
    position(){
        x = 0;
        y = 0;
    }
    position(int _x, int _y):x(_x), y(_y){}
    void getRandPos(int width, int height){
        x = rand() % width;
        y = rand() % height;
    }
    void setPosition(int _x, int _y){
        x = _x;
        y = _y;
    }
}position;

//-----------------Warp---------------------
struct Homography
{
    // 单应性矩阵
    double H[3][3];
    Homography(double w11, double w12, double w13, double w21, double w22, double w23, double w31, double w32)
    {
        H[0][0] = w11;
        H[0][1] = w12;
        H[0][2] = w13;
        H[1][0] = w21;
        H[1][1] = w22;
        H[1][2] = w23;
        H[2][0] = w31;
        H[2][1] = w32;
        H[2][2] = 1;
    }
    Homography(){}
};

typedef struct pointPair
{
    int pos1[2];
    int pos2[2];
    double distance;
    pointPair() {}
    void swap()
    {
        int tmp[2];
        tmp[0] = pos1[0];
        tmp[1] = pos1[1];
        pos1[0] = pos2[0];
        pos1[1] = pos2[1];
        pos2[0] = tmp[0];
        pos2[1] = tmp[1];
    }
} pointPair;

static bool sorting(pointPair &a, pointPair &b)
{
    return (a.distance < b.distance);
}