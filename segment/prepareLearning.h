//
// Created by Chen Sihang on 2018/12/19.
//

#ifndef SEGMENT_PREPARELEARNING_H
#define SEGMENT_PREPARELEARNING_H
#include "public.h"
#include "segment.h"
#include "equalization.h"
#include "warp.h"
#include "prepareLearning.h"

class prepareLearning {
public:
    prepareLearning(CImg<unsigned char> input, vector< vector<square> > square, int);
    vector< vector< CImg<unsigned char> > > getResult(){return this->imgVec;}
    string subDir;
    static void doDilationForEachBarItemImg(CImg<unsigned char>& input);
    static int getDilationIntensityXXY(CImg<unsigned char>& Img, int x, int y);
    static int getDilationIntensityXY(const CImg<float>& Img, int x, int y);
private:
    bool thresholdDetect(CImg<unsigned char>&);
    vector< vector< CImg<unsigned char> > > imgVec;
    void failAccess(string);
    void storeImg();
    CImg<unsigned char> resizeNum(CImg<unsigned char>& input);
};


#endif //SEGMENT_PREPARELEARNING_H
