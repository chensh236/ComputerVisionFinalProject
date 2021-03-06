//
// Created by Chen Sihang on 2018/12/19.
//

#ifndef SEGMENT_PREPARELEARNING_H
#define SEGMENT_PREPARELEARNING_H
#include "public.h"
#include "segment.h"
#include "warp.h"
#include "cutPiece.h"
#include "prepareLearning.h"

class prepareLearning {
public:
    prepareLearning(CImg<unsigned char> input, vector< vector<square> > square, int, int);
    //vector< vector< CImg<unsigned char> > > getResult(){return this->imgVec;}
    string subDir;

private:
    bool thresholdDetect(CImg<unsigned char>&);
    vector< vector< CImg<unsigned char> > > imgVec;
    void failAccess(string);
    void storeImg(int);
    CImg<unsigned char> resizeNum(CImg<unsigned char>& input);
};


#endif //SEGMENT_PREPARELEARNING_H
