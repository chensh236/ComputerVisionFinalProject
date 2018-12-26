#pragma once
#include "public.h"

class cutPiece{
public:
    cutPiece(CImg<unsigned char>);
    CImg<unsigned char> gray;
    CImg<unsigned char> resultGray;
    CImg<unsigned char> dividingImg;
    vector< vector<square> > getSquare(){return squareVec;}
    static int OSTU(const CImg<unsigned char>&);
private:
    CImg<unsigned char> threshold(CImg<unsigned char>&, int, int, float);
    void divideColumn(int);
    void findDividingLine(int, int);
    vector<int> getColumnLine(CImg<unsigned char>&,int);
    vector<int> getColumnInflectionPoints(vector<int>);
    vector< vector<square> > squareVec;
    vector<int> linePosition;
    vector<int> blackPixelSet;
    unsigned char lineColor[1] = {0};
};