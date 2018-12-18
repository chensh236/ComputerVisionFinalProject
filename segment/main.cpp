#include "segment.h"
#include "Hough.h"
#include "warp.h"

int main() {
    int threshold =165, a=40;
    float sigma = 4.0;
    int sum = 6;
    for(int i = 0; i < sum; ++i){
        string filepath = "../data/";
        string filename = filepath + to_string(i + 1) + ".bmp";
        string segmentFilename = filepath + "segment_" + to_string(i + 1) + ".bmp";
        string edgeFilename = filepath + "edge_" + to_string(i + 1) + ".bmp";
        string pointFilename = filepath + "point_" + to_string(i + 1) + ".bmp";
        string resultFilename = filepath + "result_" + to_string(i + 1) + ".bmp";
        segment seg(filename, sigma);

        Hough hough("", to_string(i + 1), threshold, a, seg.getResult(), 3);

        vector <Hough_pos> vec = hough.result;
        warp warping(vec, filename);
        seg.getSegment().save(segmentFilename.c_str());
        seg.getResult().save(edgeFilename.c_str());
        warping.getResult().save(resultFilename.c_str());
        hough.getResult().save(pointFilename.c_str());
        CImg<unsigned char> src(filename.c_str());
        src.display();
        seg.getSegment().display();
        seg.getResult().display();
        hough.getResult().display();
        warping.getResult().display();
    }
}