#include "segment.h"
#include "Hough.h"
#include "warp.h"
#include "cutPiece.h"

int main() {
    int threshold = 72, a= 50;
    float sigma = 6;
    int sum = 10;
    for(int i = 0; i < sum; ++i){
        string filepath = "../data/";
        string filename = filepath + to_string(i + 1) + ".bmp";
        string segmentFilename = filepath + "segment_" + to_string(i + 1) + ".bmp";
        string edgeFilename = filepath + "edge_" + to_string(i + 1) + ".bmp";
        string pointFilename = filepath + "point_" + to_string(i + 1) + ".bmp";
        string resultFilename = filepath + "result_" + to_string(i + 1) + ".bmp";
        string cutFileName = filepath + "cut_" + to_string(i + 1) + ".bmp";
        segment seg(filename, sigma);

         Hough hough("", to_string(i + 1), threshold, a, seg.getResult(), 3);

        vector <Hough_pos> vec = hough.result;
        warp warping(vec, filename);
//        seg.getSegment().save(segmentFilename.c_str());
//        seg.getResult().save(edgeFilename.c_str());
//        warping.getResult().save(resultFilename.c_str());
//        hough.getResult().save(pointFilename.c_str());
        cutPiece cut(warping.getResult());
        cut.dividingImg.save(cutFileName.c_str());
//seg.getSegment().display();
//            seg.getResult().display();
//            hough.getResult().display();
//            warping.getResult().display();

    }
}