#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
#include "Hough.h"
#define LOCAL_PI 180
#define X_MIN 0
#define Y_MIN 0
#define MAX 0x7FFFFFFF

#include "prepareLearning.h"
using namespace std;

Hough::Hough(double votingThreshold, double peakDistance, CImg<unsigned char> input, double fitPointDistance, bool flag)
{
    if(flag){
        cannyResult = input;
        // 由canny算法得到的结果转换为彩色图像
        CImg<unsigned char> tmp(input.width(), input.height(), 1, 3, 0);
        cimg_forXY(tmp, x, y){
            if(input(x, y) == 255){tmp(x, y, 0) = tmp(x, y, 1) = tmp(x, y, 2) = 255;}
        }
        blueLineImage = tmp;
        houghSpace = initialHoughFromCanny();
        int max = 0, min = 10000000;
        cimg_forXY(houghSpace, x, y){
            if(houghSpace(x, y) > max){
                max = houghSpace(x, y);
            }
            if(houghSpace(x, y) < min){
                min = houghSpace(x, y);
            }
        }
        double threshold = (max)*votingThreshold;
        PointPeak(threshold, peakDistance);
        getLine();
        fitPointImage = blueLineImage;
        getFitPoint(fitPointDistance);
        intersactionImage = fitPointImage;
        getInterLine();
    } else{
        // randon
        cannyResult = input;
        int bor = 10;
        cimg_forXY(cannyResult, x, y){
            if(x <= bor || y <= bor || x >= cannyResult.width() - bor || y >= cannyResult.height() - bor){
                cannyResult(x, y) = 0;
            }
        }

        int count = 90;
        while(count--){
            CImg<unsigned char>  tmp(cannyResult.width(), cannyResult.height(), 1, 1, 0);
            cimg_forXY(cannyResult, x, y){
                bool flag = false;
                for(int i = x - 1; i < x + 2; i++){
                    if(i < 0 || i >= cannyResult.width()) continue;
                    if(cannyResult(i, y) != 0){
                        flag = true;
                        break;
                    }
                }
                if(flag) tmp(x, y) = 255;
            }
            cannyResult = tmp;
        }
       //cannyResult.display();
        count = 10;
        while(count--){
            CImg<unsigned char>  tmp(cannyResult.width(), cannyResult.height(), 1, 1, 0);
            cimg_forXY(cannyResult, x, y){
                bool flag = true;
                for(int i = y - 1; i < y + 2; i++){
                    if(i < 0 || i >= cannyResult.height()) continue;
                    if(cannyResult(x, i) == 0){
                        flag = false;
                    }
                }
                // cout<<x<<" "<<y<<endl;
                if(flag) tmp(x, y) = 255;
            }
            cannyResult = tmp;
        }

       //cannyResult.display();
        // 对角线长度
        int diagonal = sqrt(cannyResult.width() * cannyResult.width() + cannyResult.height() * cannyResult.height());
        CImg<int> hough(LOCAL_PI * 4, diagonal, 1, 1, 0);
        //  对canny算法的结果进行遍历
        cimg_forXY(cannyResult, x, y)
        {
            if (cannyResult(x, y) == 255)
            {
                cimg_forX(hough, alpha)
                {
                    //转换为弧度制
                    double theta = ((double)(alpha)* 0.5 * cimg::PI) / LOCAL_PI;
                    int r = round((double)x * cos(theta) + (double)y * sin(theta));
                    // 符合条件则进行投票
                    if (r >= 0 && r < diagonal)
                    {
                        hough(alpha, r)++;
                    }
                }
            }
        }
        //hough.display();
        houghSpace = hough;
        int max = 0;
        int pos = 0;
        cimg_forXY(houghSpace, x, y){
            if(houghSpace(x, y) > max){
                max = houghSpace(x, y);
                pos = x;
            }
        }
        if(max == 0) randonTheta = 90.0;
        else randonTheta = (double)pos / 2.0;
    }

}

CImg<int> Hough::initialHoughFromCanny()
{
    // 对角线长度
    int diagonal = sqrt(cannyResult.width() * cannyResult.width() + cannyResult.height() * cannyResult.height());
    CImg<int> hough(LOCAL_PI * 2, diagonal, 1, 1, 0);
    //  对canny算法的结果进行遍历
    cimg_forXY(cannyResult, x, y)
    {
        if (cannyResult(x, y) == 255)
        {
            cimg_forX(hough, alpha)
            {
                //转换为弧度制
                double theta = ((double)alpha * cimg::PI) / LOCAL_PI;
                int r = round((double)x * cos(theta) + (double)y * sin(theta));
                // 符合条件则进行投票
                if (r >= 0 && r < diagonal)
                {
                    hough(alpha, r)++;
                }
            }
        }
    }
    return hough;
}

// peaking algorithm
void Hough::PointPeak(double votingThreshold, double peakDistance)
{
    //cout<<houghSpace.width()<<endl;
    cimg_forXY(houghSpace, alpha, r)
    {
        // 判断票数知否大于阈值，筛去低于阈值的像素点
        if (houghSpace(alpha, r) > votingThreshold)
        {
            int theta = alpha;
            Hough_pos tmp(theta, r, houghSpace(alpha, r));
           // cout<<alpha<<endl;
            bool flag = true;
            for (int i = 0; i < peak.size(); i++)
            {
                if (abs(peak[i].y - r) < houghSpace.height() / 20 &&
                    (abs(peak[i].x - alpha) < 20 || abs(peak[i].x - alpha) > houghSpace.width() - 20 ||
                     abs(abs(peak[i].x - alpha) - 180) < 10)) {
                    flag = false;
                    // find max
                    if (houghSpace(alpha, r) > peak[i].val)
                    {
                        peak[i].x = tmp.x;
                        peak[i].y = tmp.y;
                        peak[i].val = tmp.val;
                    }
                } else{
                    continue;
                }
            }
            // 符合条件，加入记录局部极大值的vector容器中
            if (flag)
            {
                //<<tmp.x<<" "<<tmp.y<<endl;
                peak.push_back(tmp);
            }
        }
    }
}

//计算出直线
void Hough::getLine()
{
    for(int i = 0; i < peak.size(); i++){
        cimg_forXY(blueLineImage, x, y){
            double theta = ((double)peak[i].x * cimg::PI) / 180.0; //角度转弧度
            int p = (int)(x *cos(theta) + y * sin(theta));
            if(abs(p - peak[i].y) < 2) blueLineImage(x, y, 2) = 255;
        }
    }
}

// canny结果与直线匹配的图像
void Hough::getFitPoint(double distance)
{
    cimg_forXY(cannyResult, x, y)
    {
        if (cannyResult(x, y) == 255)
        {
            for (int i = 0; i < lines.size(); ++i)
            {
                if (abs(lines[i].k * x + lines[i].b - y) <= distance)
                {
                    unsigned char RED[] = {255, 0, 0};
                    fitPointImage.draw_circle(x, y, 2, RED);
                }
            }
        }
    }
}

// 获得直线的交点作为纸张的角点
void Hough::getInterLine(){
    unsigned char yellow[3] = {255, 255, 0};
    for(int i = 0; i < peak.size(); i ++){
        for(int j = i + 1; j < peak.size(); j ++){
            int a1 = peak[i].x;
            int p1 = peak[i].y;
            double theta1 = ((double)a1*cimg::PI) / 180;
            int a2 = peak[j].x;
            int p2 = peak[j].y;
            double theta2 = ((double)a2*cimg::PI) / 180;
            cimg_forXY(intersactionImage, x, y) {
                if(intersactionImage(x, y, 0) == 255 && intersactionImage(x, y, 1) == 255 && intersactionImage(x, y, 2) == 0) {
                    continue;
                }
                if( p1 == (int)round(y * sin(theta1)) + (int)round(x * cos(theta1)) ) {
                    bool flag = false;
                    for(int xb = x - 1; xb < x + 2; xb++){
                        for(int yb = y - 1; yb < y + 2; yb++){
                            if(xb < 0 || yb < 0 || yb >= intersactionImage.height() || xb >= intersactionImage.width()) continue;
                            if( p2 == (int)round(yb * sin(theta2)) + (int)round(xb * cos(theta2)) ) {
                                intersactionImage.draw_circle(x, y, 5, yellow);
                                Hough_pos tmp(x, y);
                                result.push_back(tmp);
                                flag = true;
                                break;
                            }
                        }
                        if(flag) break;
                    }
                }
            }
        }
    }
}