#pragma once
#include "public.h"
#define BOUNDER 8

class cutPiece{
public:
    cutPiece(CImg<unsigned char>);
    CImg<unsigned char> gray;
    CImg<unsigned char> dividingImg;
private:
    CImg<unsigned char> threshold(CImg<unsigned char>&, int, int, int, float);
    void divideIntoBarItemImg(int);
    vector< CImg<float> > getRowItemImgSet(const CImg<float>& , vector<int>);
    void findDividingLine(int, int);
    vector<int> getDivideLineXofSubImage(CImg<unsigned char>&,int);
    vector<int> getInflectionPosXs(vector<int>);

    CImg<int> HistogramImage;
    vector<int> inflectionPoints;
    vector<CImg<float>> subImageSet;     // 一行行数字图像
};