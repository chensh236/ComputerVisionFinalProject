//
// Created by Chen Sihang on 2018/12/18.
//
#include "cutPiece.h"
#include "segment.h"
#include "prepareLearning.h"
#include "Hough.h"
#include "canny.h"
#include "reshape.h"


cutPiece::cutPiece(CImg<unsigned char> input){
    gray = segment::toGrayScale(input);
    resultGray = gray;
    CImg<unsigned char> grayScale = gray;
    //单阈值检测
    //第一个高 多
    gray = threshold(grayScale, 24, 24, 0.80);
    CImg<unsigned char> dilationed = gray;
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
    }
    gray = dilationed;
    dilationed = canny::newFunc(dilationed, 140);
    gray = canny::newFunc(gray, 11);
    Hough hough(0, 0, dilationed, 0, false);
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
        if(gray(x, y) == 0) gray(x, y) = 255;
        else gray(x, y) = 0;
    }

    // 旋转角度
    double theta = hough.randonTheta;

    theta = (theta - 90.0);
    cout<<"theta:"<<theta<<endl;
    double ror = theta / 180.0 * cimg::PI;
    gray = reshape::reotate_biliinar(gray, ror, true);
    resultGray = reshape::reotate_biliinar(resultGray, ror, false);
    findDividingLine(5, 4);
    divideColumn(5);

}
void cutPiece::findDividingLine(int singleThreshold, int threshold) {
    CImg<int> histogram(gray.width(), gray.height(), 1, 1, 255);
    dividingImg = gray;

    // 扫描
    cimg_forY(gray, y){
        int blackPixel = 0;
        cimg_forX(gray, x) {
            if (gray(x, y) == 0)
                blackPixel++;
        }
        blackPixel = blackPixel > singleThreshold ? blackPixel : 0;
        blackPixelSet.push_back(blackPixel);
    }

//    cimg_forXY(histogram, x, y){
//        if(x < blackPixelSet[y]) histogram(x, y) = 0;
//    }
    //histogram.display();
    //去除不够长的边
    cimg_forY(gray, y){
        if(blackPixelSet[y] != 0){
            int pointer = y + 1;
            int counter = 0;
            while(pointer < gray.height() && blackPixelSet[pointer] != 0){
                pointer++;
                counter += blackPixelSet[pointer];
            }
            counter /= (pointer - y);
            //符合条件
            pointer--;
            //去除干扰
            if(pointer < gray.height() - 1 &&  blackPixelSet[pointer] != 0 && counter < threshold){
                int distance = pointer - y + 1;
                if(distance < threshold){
                    for(int i = y; i <= pointer; i++){
                        blackPixelSet[i] = 0;
                    }
                }
            }
        }
    }

    for(int y = 0; y < blackPixelSet.size(); y++) {
        if(y == 0) continue;

        if (blackPixelSet[y] == 0 && blackPixelSet[y - 1] != 0)
            linePosition.push_back(y);

        else if (blackPixelSet[y] != 0 && blackPixelSet[y - 1] == 0)
            linePosition.push_back(y - 1);
    }

    unsigned char lineColor[1] = {0};

    vector<int> tmpVec = linePosition;
    linePosition.clear();

    //零值淘汰，每一行只有一条中线
    for(int index = 0; index < tmpVec.size() - 1; index++){
        int counter = 0;
        for(int i = tmpVec[index]; i <= tmpVec[index + 1]; i++){
            if(blackPixelSet[i] != 0) counter++;
        }
        if((double)counter / (double)(tmpVec[index + 1] - tmpVec[index]) < 0.1) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            linePosition.push_back(median);
            index++;
        } else{
            linePosition.push_back(tmpVec[index]);
        }
    }
    linePosition.push_back(tmpVec[tmpVec.size() - 1]);
    tmpVec = linePosition;
    linePosition.clear();
    int distance = tmpVec[ tmpVec.size() - 1 ] - tmpVec[0];
    distance /= tmpVec.size();
    // 距离淘汰(太近合并)
    for(int index = 0; index < tmpVec.size() - 1; index++){
        if(tmpVec[index + 1] - tmpVec[index] < (distance / 3)) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            linePosition.push_back(median);
            index++;
        } else{
            linePosition.push_back(tmpVec[index]);
        }
    }

    linePosition.push_back(tmpVec[tmpVec.size() - 1]);
    if(linePosition[0] > 2){
        linePosition[0] -= 2;
    }

    if(linePosition[linePosition.size() - 1] + 2 < gray.height()){
        linePosition[linePosition.size() - 1] += 2;
    }


    for(int i = 0; i < linePosition.size(); i++){

        dividingImg.draw_line(0, linePosition[i], gray.width() - 1, linePosition[i], lineColor);
    }
}

// 根据行分割线划分图片
void cutPiece::divideColumn(int threshold) {

    for (int i = 1; i < linePosition.size(); i++) {
        int barHeight = linePosition[i] - linePosition[i - 1];
        int blackPixel = 0;
        CImg<unsigned char> barItemImg = CImg<unsigned char>(gray.width(), barHeight, 1, 1, 0);
        cimg_forXY(barItemImg, x, y) {
            barItemImg(x, y, 0) = gray(x, linePosition[i - 1] + 1 + y, 0);
            if (barItemImg(x, y, 0) == 0)
                blackPixel++;
        }
        
        double blackPercent = (double)blackPixel / (double)(gray.width() * barHeight);

        // 只有当黑色像素个数超过图像大小一定比例时，该航才可视作有数字
        if (blackPercent > (7.0/594.0)) {
            vector<square> squareTmp;
            vector<int> dividePosXset = getColumnLine(barItemImg, threshold);
            if(dividePosXset.empty())continue;

            for(int j = 1; j < dividePosXset.size(); j++){

                //去除污点！
                square squ(Hough_pos(dividePosXset[j - 1], linePosition[i - 1]), Hough_pos(dividePosXset[j - 1], linePosition[i]),
                           Hough_pos(dividePosXset[j], linePosition[i - 1]), Hough_pos(dividePosXset[j], linePosition[i]));
                int count = 0.0;
                for(int y = squ.lt.y; y<= squ.lb.y; y++){
                    bool flag = false;
                    for(int x = squ.lt.x; x <= squ.rt.x; x++){
                        if(gray(x, y) == 0){
                            flag = true;
                            break;
                        }
                    }
                    if(flag) count++;
                }

                if(count > 6){
                    squareTmp.push_back(squ);
                }
            }
            squareVec.push_back(squareTmp);
            // 绘制竖线
            for (int j = 0; j < dividePosXset.size(); j++) {
                dividingImg.draw_line(dividePosXset[j], linePosition[i - 1],
                                      dividePosXset[j], linePosition[i], lineColor);
            }
        }
    }

}

// 获取一行行的子图的水平分割线
vector<int> cutPiece::getColumnLine(CImg<unsigned char>& subImg, int threshold) {
    // 先绘制X方向灰度直方图
    vector<int> countVec;
    cimg_forX(subImg, x){
        int blackPixel = 0;
        cimg_forY(subImg, y) {
            if (subImg(x, y) == 0)
                blackPixel++;
        }

        bool flag = (blackPixel > threshold) ? true : false;
        if(x - 1 >= 0 && countVec[x - 1] != 0){
            flag = true;
        }
        if(!flag){
            blackPixel = 0;
        }
        countVec.push_back(blackPixel);
    }

    //不为0的数，判断空的航
    int counter = 0;
    for(int i = 0; i < countVec.size(); i++){
        if(countVec[i] != 0) counter++;
    }

    // CImg<int> Hou(subImg.width(), subImg.height(), 1, 1, 255);
    // cimg_forXY(Hou, x, y){
    //     if(y < countVec[x]){
    //         Hou(x, y) = 0;
    //     }
    // }
    //Hou.display();
    //去掉空的行
    if(counter == 0){
        vector<int> InflectionPosXs;
        InflectionPosXs.clear();
        return InflectionPosXs;
    }
    vector<int> InflectionPosXs = getColumnInflectionPoints(countVec);    //获取拐点
    for(int i = 0; i < InflectionPosXs.size(); i++){
        subImg.draw_line(InflectionPosXs[i], 0, InflectionPosXs[i], subImg.width() - 1, lineColor);
    }
   // subImg.display();
    return InflectionPosXs;
}

// 根据X方向直方图判断真实的拐点
vector<int> cutPiece::getColumnInflectionPoints(vector<int> counterVec) {
    vector<int> resultInflectionPosXs, tempInflectionPosXs;
    // 查找拐点
    for(int i = 0; i < counterVec.size(); i++){
        // 白转黑
        if (i > 0 && counterVec[i] != 0 && counterVec[i - 1] == 0)
            tempInflectionPosXs.push_back(i - 1);
            // 黑转白
        else if (i > 0 && counterVec[i] == 0 && counterVec[i - 1] != 0)
            tempInflectionPosXs.push_back(i);
    }

    // 合成！
    vector<int> tmpVec = tempInflectionPosXs;
    tempInflectionPosXs.clear();
    //第一条往左挪动
    if(tmpVec[0] > 5) tmpVec[0] -= 5;
    if(tmpVec[tmpVec.size() - 1] + 5 < gray.width()) tmpVec[tmpVec.size() - 1] += 5;
    tempInflectionPosXs.push_back(tmpVec[0]);

    for(int index = 1; index < tmpVec.size() - 1; index++){
        int counter = 0;
        for(int i = tmpVec[index]; i <= tmpVec[index + 1]; i++){
            if(counterVec[i] != 0) counter++;
        }
        //中间为空，合在一起
        if((double)counter / (double)(tmpVec[index + 1] - tmpVec[index]) < 0.05) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            tempInflectionPosXs.push_back(median);
            index++;
        } else{
            tempInflectionPosXs.push_back(tmpVec[index]);
        }
    }
    tempInflectionPosXs.push_back(tmpVec[tmpVec.size() - 1 ]);
    //距离分割(you bug!)
    int max = 0;
    for(int i = 1; i < tempInflectionPosXs.size(); i++){
        if(tempInflectionPosXs[i] - tempInflectionPosXs[i - 1] > max) max = tempInflectionPosXs[i] - tempInflectionPosXs[i - 1];
    }
    float distance = 0.0;
    for(int i = 1; i < tempInflectionPosXs.size(); i++){
        distance += tempInflectionPosXs[i] - tempInflectionPosXs[i - 1];
    }
    distance -= (float)max;

    distance /= (float)(tempInflectionPosXs.size() - 1);
   //cout<<"dis"<<distance<<endl;
    for(auto iter = tempInflectionPosXs.begin(); iter != tempInflectionPosXs.end(); iter++){
        if(iter != tempInflectionPosXs.begin() && (float)(*iter - *(iter - 1)) >= 2.077 * distance){
            int median = *iter - *(iter - 1);
            median /= 2;
            median += *(iter - 1);
            tempInflectionPosXs.insert(iter, median);
            iter = tempInflectionPosXs.begin();
        }
    }
    return tempInflectionPosXs;
}
// ostu算法求阈值
int cutPiece::OSTU(const CImg<unsigned char>& image) {
    int histogram[255] = {0};
    int pixelsNum = image.width() * image.height();
    cimg_forXY(image, i, j) {
        ++histogram[image(i, j)];
    }
    int threshold;
    double variance = 0.0;
    for (int i = 0; i < 256; i++) {
        double P1 = 0.0, P2 = 0.0, m1 = 0.0, m2 = 0.0;
        // 前景
        for (int j = 0; j <= i; j++) {
            
            P1 += (double)histogram[j];
            m1 += (double)j * histogram[j];
        }
        if (P1 == 0.0) continue;
        m1 /= P1;
        P1 /= pixelsNum;
        // 后景
        for (int j = i + 1; j < 256; j++) {
            P2 += (double)histogram[j];
            m2 += (double)j * histogram[j];
        }
        if (P2 == 0.0) continue;
        m2 /= P2;
        P2 /= pixelsNum;
        double temp_variance = P1 * P2 * (m1 - m2) * (m1 - m2);
        if (variance < temp_variance) {
            variance = temp_variance;
            threshold = i;
        }
    }
    return threshold;
}

CImg<unsigned char> cutPiece::threshold(CImg<unsigned char>& imgIn, int columnNumber, int rowNumber, float abandonThreshold)
{
    // 分块
    CImg<unsigned char> afterThreshold(imgIn.width(), imgIn.height(), 1, 1, 0);
    int columnSize = floor((float)imgIn.width() / (float)columnNumber);
    int rowSize = floor((float)imgIn.height() / (float)rowNumber);
    int resizeCol = imgIn.width() % columnNumber;
    int resizeRow = imgIn.height() % rowNumber;
    
    // 主体
    for(int i = 0; i < columnNumber; i++){
        for(int j = 0; j < rowNumber; j++){
            CImg<unsigned char> ostu(columnSize, rowSize, 1, 1, 255);
            for(int k = 0; k < columnSize; k++){
                for(int l = 0; l < rowSize; l++){
                    ostu(k, l) = imgIn(i * columnSize + k, j * rowSize + l);
                }
            }
            int threshold = OSTU(ostu);
            int count = 0;
            for(int k = 0; k < columnSize; k++){
                for(int l = 0; l < rowSize; l++){
                    if(imgIn(i * columnSize + k, j * rowSize + l) > threshold){
                        afterThreshold(i * columnSize + k, j * rowSize + l) = 255;
                        count++;
                    }
                    else afterThreshold(i * columnSize + k, j * rowSize + l) = 0;
                }
            }

            // 填充无用块为255
            if((float)count / (float)(columnSize * rowSize) < abandonThreshold){
                for(int k = 0; k < columnSize; k++){
                    for(int l = 0; l < rowSize; l++){
                        afterThreshold(i * columnSize + k, j * rowSize + l) = 255;
                    }
                }
            }
        }
    }
    // 剩余 x
    for(int j = 0; j < rowNumber; j++){
        CImg<unsigned char> ostu(resizeCol, rowSize, 1, 1, 255);
        for(int i = imgIn.width() - resizeCol; i < imgIn.width(); i++){
            for(int l = 0; l < rowSize; l++){
                ostu(i - imgIn.width() + resizeCol, l) = imgIn(i, j * rowSize + l);
            }
        }
        int threshold = OSTU(ostu);
        int count = 0;
        for(int i = imgIn.width() - resizeCol - 1; i < imgIn.width(); i++){
            for(int l = 0; l < rowSize; l++){
                if(imgIn(i, j * rowSize + l) > threshold){
                    afterThreshold(i, j * rowSize + l) = 255;
                    count++;
                }
                else afterThreshold(i, j * rowSize + l) = 0;
            }
        }
        // 填充无用块为255
        if((float)count / (float)(resizeCol * rowSize) < abandonThreshold){
            for(int i = imgIn.width() - resizeCol - 1; i < imgIn.width(); i++){
                for(int l = 0; l < rowSize; l++){
                    afterThreshold(i, j * rowSize + l) = 255;
                }
            }
        }
    }
    // 剩余 y
    for(int i = 0; i < columnNumber; i++){
        CImg<unsigned char> ostu(columnSize, rowSize, 1, 1, 255);
        for(int j = imgIn.height() - resizeRow; j < imgIn.height(); j++){
            for(int k = 0; k < columnSize; k++){
                ostu(k, j - imgIn.height() + resizeRow) = imgIn(i * columnSize + k, j);
            }
        }

        int threshold = OSTU(ostu);
        int count = 0;
        for(int j = imgIn.height() - resizeRow - 1; j < imgIn.height(); j++){
            for(int k = 0; k < columnSize; k++){
                if(imgIn(i * columnSize + k, j) > threshold){
                    afterThreshold(i * columnSize + k, j) = 255;
                    count++;
                }
                else afterThreshold(i * columnSize + k, j) = 0;
            }
        }
        // 填充无用块为255
        if((float)count / (float)(columnSize * resizeRow) < abandonThreshold){
            for(int j = imgIn.height() - resizeRow - 1; j < imgIn.height(); j++){
                for(int k = 0; k < columnSize; k++){
                    afterThreshold(i * columnSize + k, j) = 255;
                }
            }
        }
    }

    //膨胀
    CImg<unsigned char>  tmp(afterThreshold.width(), afterThreshold.height(), 1, 1, 255);
    cimg_forXY(afterThreshold, x, y){
        bool flag = false;
        for(int i = x - 1; i < x + 2; i++){
            for(int j = y - 1; j < y + 2; j++){
                if(j < 0 || j >= afterThreshold.height() || i < 0 || i >= afterThreshold.width()) continue;
                if(afterThreshold(i, j) == 0){
                    flag = true;
                    break;
                }
            }
        }
        if(flag) tmp(x, y) = 0;
    }
    afterThreshold = tmp;
    abandonThreshold = 0.52;
    // 舍弃
    // 主体
    for(int i = 0; i < columnNumber; i++){
        for(int j = 0; j < rowNumber; j++){
            int count = 0;
            for(int k = 0; k < columnSize; k++){
                for(int l = 0; l < rowSize; l++){
                    if(afterThreshold(i * columnSize + k, j * rowSize + l) == 255){
                        count++;
                    }
                }
            }

            // 填充无用块为255
            if((float)count / (float)(columnSize * rowSize) < abandonThreshold){
                for(int k = -1; k < columnSize + 1; k++){
                    for(int l = -1; l < rowSize + 1; l++){
                        if(i * columnSize + k < 0 || j * rowSize + l < 0
                        || i * columnSize + k >= afterThreshold.width() || j * rowSize + l >= afterThreshold.height()) continue;
                            afterThreshold(i * columnSize + k, j * rowSize + l) = 255;
                    }
                }
            }
        }
    }
    // 剩余 x
    for(int j = 0; j < rowNumber; j++){
        int count = 0;
        for(int i = imgIn.width() - resizeCol - 1; i < imgIn.width(); i++){
            for(int l = 0; l < rowSize; l++){
                if(afterThreshold(i, j * rowSize + l) == 255){
                    count++;
                }
            }
        }
        // 填充无用块为255
        if((float)count / (float)(resizeCol * rowSize) < abandonThreshold){
            for(int i = imgIn.width() - resizeCol - 1; i < imgIn.width(); i++){
                for(int l = 0; l < rowSize; l++){
                    afterThreshold(i, j * rowSize + l) = 255;
                }
            }
        }
    }
    // 剩余 y
    for(int i = 0; i < columnNumber; i++){
        int count = 0;
        for(int j = imgIn.height() - resizeRow - 1; j < imgIn.height(); j++){
            for(int k = 0; k < columnSize; k++){
                if(afterThreshold(i * columnSize + k, j) == 255){
                    count++;
                }
            }
        }
        // 填充无用块为255
        if((float)count / (float)(columnSize * resizeRow) < abandonThreshold){
            for(int j = imgIn.height() - resizeRow - 1; j < imgIn.height(); j++){
                for(int k = 0; k < columnSize; k++){
                    afterThreshold(i * columnSize + k, j) = 255;
                }
            }
        }
    }
    cimg_forXY(afterThreshold, x, y){
        if(x <= BOUNDER || y <= BOUNDER || x >= afterThreshold.width() - BOUNDER || y >= afterThreshold.height() - BOUNDER){
            afterThreshold(x, y) = 255;
        }
    }
    return afterThreshold;
}