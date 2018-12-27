//
// Created by Chen Sihang on 2018/12/18.
//
#include "cutPiece.h"
#include "segment.h"
#include "prepareLearning.h"
#include "Hough.h"
#include "canny.h"
#include "reshape.h"


cutPiece::cutPiece(CImg<unsigned char> input, int seq){
    this->seq = seq;
    gray = segment::toGrayScale(input);
    CImg<unsigned char> grayScale = gray;
    //单阈值检测
    //第一个高 多
    gray = threshold(grayScale, 15, 0.08);
    CImg<unsigned char> dilationed = gray;
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
    }
    gray = dilationed;
    gray = canny::newFunc(gray, 6);
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
    // 减少计算量
    //if(theta <= 1.0 && theta >= -1.0) theta = 0.0;
    cout<<"theta:"<<theta<<endl;
    double ror = theta / 180.0 * cimg::PI;
    gray = reshape::reotate_biliinar(gray, ror);
    findDividingLine(3, 4);
    divideColumn(4);
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
//    histogram.display();
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
        if((double)counter / (double)(tmpVec[index + 1] - tmpVec[index]) < 0.3) {
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
    //dividingImg.display();
}

// 根据行分割线划分图片
void cutPiece::divideColumn(int threshold) {

    for (int i = 1; i < linePosition.size(); i++) {
        int barHeight = linePosition[i] - linePosition[i - 1];
        CImg<unsigned char> barItemImg = CImg<unsigned char>(gray.width(), barHeight, 1, 1, 0);
        cimg_forXY(barItemImg, x, y) {
            barItemImg(x, y, 0) = gray(x, linePosition[i - 1] + 1 + y, 0);
        }

        vector<square> squareTmp;
        vector<int> dividePosXset = getColumnLine(barItemImg, threshold);
        if(dividePosXset.empty())continue;
        int everyCol = 0;
        for(int j = 1; j < dividePosXset.size(); j++){
            //去除污点！
            square squ(Hough_pos(dividePosXset[j - 1], linePosition[i - 1]), Hough_pos(dividePosXset[j - 1], linePosition[i]),
                       Hough_pos(dividePosXset[j], linePosition[i - 1]), Hough_pos(dividePosXset[j], linePosition[i]));
            int count = 0;
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
                everyCol++;
                squareTmp.push_back(squ);
            }
        }
        if(everyCol > 4){
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
        if(!flag) {
            blackPixel = 0;
        }
        countVec.push_back(blackPixel);
    }

    //不为0的数，判断空的航
    int counter = 0;
    for(int i = 0; i < countVec.size(); i++){
        if(countVec[i] != 0) counter++;
    }

//     CImg<int> Hou(subImg.width(), subImg.height(), 1, 1, 255);
//     cimg_forXY(Hou, x, y){
//         if(y < countVec[x]){
//             Hou(x, y) = 0;
//         }
//     }
//    Hou.display();
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
    //subImg.display();
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
        if(counter == 0) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            tempInflectionPosXs.push_back(median);
            index++;
        } else{
            tempInflectionPosXs.push_back(tmpVec[index]);
        }
    }
    tempInflectionPosXs.push_back(tmpVec[tmpVec.size() - 1 ]);
    if(seq < 10){
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

CImg<unsigned char> cutPiece::threshold(CImg<unsigned char>& imgIn, int halfSize, float threshold)
{
    CImg<unsigned char> result(imgIn.width(), imgIn.height(), 1, 1, 255);
    CImg<int> integral(imgIn.width(), imgIn.height(), 1, 1, 0);
    cimg_forY(result, y){
        int sum = 0;
        cimg_forX(result, x){
            sum += imgIn(x, y);
            if(y == 0){
                integral(x, y) = sum;
            }else{
                integral(x, y) = integral(x, y - 1) + sum;
            }
        }
    }

    //自适应阈值
    cimg_forY(imgIn, y) {
        int y1 = (y - halfSize > 0) ?y - halfSize : 0;
        int y2 = (y + halfSize < imgIn.height()) ? (y + halfSize) : (imgIn.height() - 1);
        cimg_forX(imgIn, x) {
            int x1 = (x - halfSize > 0) ? x - halfSize : 0;
            int x2 = (x + halfSize < imgIn.width()) ? (x + halfSize) : (imgIn.width() - 1);
            int count = (x2 - x1) * (y2 - y1);
            int sum = integral(x2, y2) - integral(x1, y2) -
                            integral(x2, y1) + integral(x1, y1);
            if (imgIn(x, y) * count < sum * (1.0 - threshold)) {
                result(x, y) = 0;
            }
        }
    }

    CImg<bool> isVisited(imgIn.width(), imgIn.height(), 1, 1, false);
    //isVisited.display();
    cimg_forXY(isVisited, x, y){
        if(x > BOUNDER && x < (isVisited.width() - BOUNDER) && y > BOUNDER && y < (isVisited.height() - BOUNDER)) continue;
        if(isVisited(x, y) || result(x, y) != 0) continue;
        Hough_pos currentPos(x, y);
        stack <Hough_pos> posStack;
        posStack.push(currentPos);
        isVisited(x, y) = true;
        //DFS
        while (!posStack.empty())
        {
            Hough_pos currentPos = posStack.top();
            posStack.pop();
            if (isVisited(currentPos.x,currentPos.y))
                continue;
            isVisited(currentPos.x,currentPos.y) = true;
            for (int i = currentPos.x - 1; i < currentPos.x + 2; i++)
            {
                for (int j = currentPos.y - 1; j < currentPos.y + 2; j++)
                {
                    if (i >= 0 && i < isVisited.width() && j >= 0 && j < isVisited.height())
                    {
                        if (i == currentPos.x && j == currentPos.y)
                            continue;
                        if (result(i, j) == 0)
                        {
                            Hough_pos nextPos(i, j);
                            posStack.push(nextPos);
                        }
                    }
                }
            }
        }
    }
    cimg_forXY(result, x, y){
        if(isVisited(x, y)){
            result(x, y) = 255;
        }
    }
    return result;
}