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
        PointPeak(votingThreshold, peakDistance);
        getLine();
        fitPointImage = blueLineImage;
        getFitPoint(fitPointDistance);
        intersactionImage = fitPointImage;
        getInterLine();
    } else{
        // randon
        cannyResult = input;
//        //cannyResult = cannyResult.get_blur(1);
//        cimg_forXY(cannyResult, x,  y ){
//            cannyResult(x, y) = 255 - cannyResult(x, y);
//        }

        int bor = 10;
        cimg_forXY(cannyResult, x, y){
            if(x <= bor || y <= bor || x >= cannyResult.width() - bor || y >= cannyResult.height() - bor){
                cannyResult(x, y) = 0;
            }
        }

        int count = 2;
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

        count = 60;
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
        count = 1;
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
        randonTheta = (double)pos / 2.0;
//        cimg_forX(houghSpace, x){
//            int counter = 0;
//            cimg_forY(houghSpace, y){
//                counter += houghSpace(x, y);
//            }
//            if(counter > max){
//                max = counter;
//                randonTheta = x;
//            }
//        }
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
    cimg_forXY(houghSpace, alpha, r)
    {
        // 判断票数知否大于阈值，筛去低于阈值的像素点
        if (houghSpace(alpha, r) > votingThreshold)
        {
            Hough_pos tmp(alpha, r, houghSpace(alpha, r));
            bool flag = true;
            for (int i = 0; i < peak.size(); i++)
            {
                // 如果距离相近，则选取较大的一个作为局部极大值
                if (sqrt((peak[i].x - alpha) * (peak[i].x - alpha) + (peak[i].y - r) * (peak[i].y - r)) < peakDistance)
                {
                    flag = false;
                    // find max
                    if (houghSpace(alpha, r) > peak[i].val)
                    {
                        peak[i] = tmp;
                    }
                }
            }
            // 符合条件，加入记录局部极大值的vector容器中
            if (flag)
            {
                peak.push_back(tmp);
            }
        }
    }
}

//计算出直线
void Hough::getLine()
{
    unsigned char BLUE[] = {0, 0, 255};
    for (int i = 0; i < peak.size(); i++)
    {

        double theta = (double)(peak[i].x) * cimg::PI / LOCAL_PI;
        double k = -cos(theta) / sin(theta); // 直线斜率
        double b = (double)(peak[i].y) / sin(theta);
        line templine(k, b);
        bool flag = true;
        for(int i = 0; i < lines.size(); i++){
            if(lines[i].k == k && lines[i].b == b){
                flag = false;
                break;
            }
        }
        if(!flag) continue;
        lines.push_back(templine);
        // 将直线转换为参数方程表示
        double alpha = atan(k);
        // cout << "x= " << (cos(alpha)) << "t" << endl;
        // cout << "y= " << b << " + " << (sin(alpha)) << "t" << endl;
    }

    // 画出直线
    for (int i = 0; i < lines.size(); i++)
    {
        const int x0 = (double)(0 - lines[i].b) / lines[i].k;
        const int x1 = (double)(blueLineImage.height() - 1 - lines[i].b) / lines[i].k;
        const int y0 = 0 * lines[i].k + lines[i].b;
        const int y1 = (blueLineImage.width() - 1) * lines[i].k + lines[i].b;

        // draw blue line
        if (abs(lines[i].k) > 1)
        {
            blueLineImage.draw_line(x0, 0, x1, blueLineImage.height() - 1, BLUE);
        }
        else
        {
            blueLineImage.draw_line(0, y0, blueLineImage.width() - 1, y1, BLUE);
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
    for(int i = 0; i < lines.size(); ++i){
        for(int j = 0; j < lines.size(); ++j){
                double k0 = lines[i].k;
                double k1 = lines[j].k;
                double b0 = lines[i].b;
                double b1 = lines[j].b;
                if((k0 - k1) == 0.0) continue;
                int x = (int)((b1 - b0) / (k0 - k1));
                int y = (int)((k0 * b1 - k1 * b0) / (k0 - k1));
                if(x < 0 || x >= intersactionImage.width() || y < 0 || y >= intersactionImage.height()) continue;
                bool flag = true;
                for(int i = 0; i < result.size(); i++){
                    if(result[i].x == x &&result[i].y == y){
                        flag = false;
                        break;
                    }
                }
                if(!flag) continue;
                Hough_pos pos(x, y, 0);
                result.push_back(pos);
                unsigned char YELLOW[] = {255, 255, 0};
                if(x >= 0 && x < intersactionImage.width() && y >= 0 && y < intersactionImage.height())
                    intersactionImage.draw_circle(x, y, 5, YELLOW);
        }
    }
}