#pragma once
#include "public.h"

class segment
{
private:
    position core1, core2;
    CImg<unsigned char> grayImg;
    CImg<unsigned char> result;
    CImg<unsigned char> block;
    // 用于判断是否已经访问过了
    vector< vector<bool> > isVisted;
    stack<Hough_pos> posStack;
    //判断两点是否相同
    bool isEqual(position&, position&);
    position getMeans(vector<position>& set, CImg<unsigned char>& src);
public:
    segment(string filepath, float sigma);
    segment(CImg<unsigned char>& input){
        kmeans(input);
    }
    void kmeans(CImg<unsigned char>&);
    CImg<unsigned char> getResult(){return this->result;}
    CImg<unsigned char> getSegment(){return this->block;}
    CImg<unsigned char> generate(CImg<unsigned char>&);
    static CImg<unsigned char> toGrayScale(CImg<unsigned char>&);
    ~segment(){}
    
};