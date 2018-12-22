#pragma once
#include "public.h"

class cutPiece{
public:
    cutPiece(CImg<unsigned char>);
    CImg<unsigned char> gray;
    CImg<unsigned char> resultGray;
    CImg<unsigned char> dividingImg;
    vector< vector<square> > getSquare(){return squareVec;}
private:
    CImg<unsigned char> threshold(CImg<unsigned char>&, float, int, int, float);
    void divideIntoBarItemImg(int);
    void findDividingLine(int, int);
    vector<int> getDivideLineXofSubImage(CImg<unsigned char>&,int);
    vector<int> getInflectionPosXs(vector<int>);
    vector< vector<square> > squareVec;
    CImg<int> HistogramImage;
    vector<int> inflectionPoints;
    vector<int> blackPixelSet;
};