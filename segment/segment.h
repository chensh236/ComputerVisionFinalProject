#pragma once
#include "public.h"

class segment
{
private:
    position core1, core2;
    CImg<unsigned char> result;
    CImg<unsigned char> block;
    //判断两点是否相同
    bool isEqual(position&, position&);
    position getMeans(vector<position>& set, CImg<unsigned char>& src);
public:
    segment(string filepath, float sigma);
    void kmeans(CImg<unsigned char>&);
    CImg<unsigned char> getResult(){return this->result;}
    CImg<unsigned char> getSegment(){return this->block;}
    CImg<unsigned char> deleteEdge(CImg<unsigned char> imgin, int length);
    ~segment(){}
    
};