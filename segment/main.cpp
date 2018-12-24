#include "segment.h"
#include "Hough.h"
#include "warp.h"
#include "prepareLearning.h"
#include "cutPiece.h"
#include <math.h>
#include <thread>

static void threadProcess(int i){
    int threshold = 72, a= 50;
    float sigma = 6;
    string filepath = "../data/";
    string filename = filepath + to_string(i + 1) + ".bmp";
    string segmentFilename = filepath + "segment_" + to_string(i + 1) + ".bmp";
    string edgeFilename = filepath + "edge_" + to_string(i + 1) + ".bmp";
    string pointFilename = filepath + "point_" + to_string(i + 1) + ".bmp";
    string resultFilename = filepath + "result_" + to_string(i + 1) + ".bmp";
    string cutFileName = filepath + "cut_" + to_string(i + 1) + ".bmp";
    segment seg(filename, sigma);

    Hough hough(threshold, a, seg.getResult(), 3, true);
    CImg<unsigned char> src(filename.c_str());
    vector <Hough_pos> vec = hough.result;
    warp warping(vec, src, 0);
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


    vector< vector<square> > square = cut.getSquare();
    prepareLearning pl(cut.resultGray, square, i, i);
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
    int sum = 10;
//        thread t1(threadProcess, 0);
//        thread t2(threadProcess, 1);
//        thread t3(threadProcess, 2);
//        thread t4(threadProcess, 3);
//        thread t5(threadProcess, 4);
//        thread t6(threadProcess, 5);
//        thread t7(threadProcess, 6);
//        thread t8(threadProcess, 7);
//
//        t1.join();
//        t2.join();
//        t3.join();
//        t4.join();
//        t5.join();
//        t6.join();
//        t7.join();
//        t8.join();


        thread t10(threadProcess, 9);
        t10.join();
    thread t9(threadProcess, 8);
    t9.join();
    for(int i = 0; i < 8; i += 4){
        thread t1(threadProcess, i);
        thread t2(threadProcess, i + 1);
        thread t3(threadProcess, i + 2);
        thread t4(threadProcess, i + 3);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }

//
//threadProcess(6);
//for(int i = 0; i < 10; i++){
//    threadProcess(i);
//}

}