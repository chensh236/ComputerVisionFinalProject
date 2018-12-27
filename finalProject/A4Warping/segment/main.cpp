#include "segment.h"
#include "Hough.h"
#include "warp.h"
#include "prepareLearning.h"
#include "cutPiece.h"
#include <math.h>
#include <thread>
#include <windows.h>

static void threadProcess(int i){
    double threshold = 0.25;
    int a= 60;
    float sigma = 6.0;
    string filepath = "../data/";
    string filename = filepath + to_string(i + 1) + ".bmp";
    string segmentFilename = filepath + "segment_" + to_string(i + 1) + ".bmp";
    string edgeFilename = filepath + "edge_" + to_string(i + 1) + ".bmp";
    string pointFilename = filepath + "point_" + to_string(i + 1) + ".bmp";
    string resultFilename = filepath + "result_" + to_string(i + 1) + ".bmp";
    string cutFileName = filepath + "cut_" + to_string(i + 1) + ".bmp";
    segment seg(filename, sigma);
//
    Hough hough(threshold, a, seg.getResult(), 3, true);
    //hough.getResult().display();
    CImg<unsigned char> src(filename.c_str());
    vector <Hough_pos> vec = hough.result;
    if(vec.size() != 4){
        cout<<"ERROR!!!!!! index:"<<i<<endl;
        return;
    }
    warp warping(vec, src, 0);
    seg.getSegment().save(segmentFilename.c_str());
   // seg.getResult().display();
    seg.getResult().save(edgeFilename.c_str());
    warping.getResult().save(resultFilename.c_str());
    hough.getResult().save(pointFilename.c_str());
    cutPiece cut(warping.getResult(), i);
    cut.dividingImg.save(cutFileName.c_str());

//
    vector< vector<square> > square = cut.getSquare();
    prepareLearning pl(cut.gray, square, i, i);
    string dst = pl.subDir + "/points.txt";
    ofstream ofs(dst.c_str());
    for(int i = 0; i < 4; i++){
        int x = (int)warping.srcPos[i][0];
        int y = (int)warping.srcPos[i][1];
        ofs<<x<<" "<<y<<endl;
    }
    ofs.close();
}

int main() {

    threadProcess(0);
    for (int i = 1; i < 99; i += 11) {
        thread t1(threadProcess, i);
        thread t2(threadProcess, i + 1);
        thread t3(threadProcess, i + 2);
        thread t4(threadProcess, i + 3);
        thread t5(threadProcess, i + 4);
        thread t6(threadProcess, i + 5);
        thread t7(threadProcess, i + 6);
        thread t8(threadProcess, i + 7);
        thread t9(threadProcess, i + 8);
        thread t10(threadProcess, i + 9);
        thread t11(threadProcess, i + 10);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();
        t10.join();
        t11.join();
    }

}