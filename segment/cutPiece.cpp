//
// Created by Chen Sihang on 2018/12/18.
//
#include "cutPiece.h"
#include "segment.h"
cutPiece::cutPiece(CImg<unsigned char> input){
    gray = segment::toGrayScale(input);
    //单阈值检测
    gray = threshold(gray, 200, 23, 28, 0.76);
    findDividingLine(5, 4);
    divideIntoBarItemImg(3);
//    HistogramImage.display();
//    dividingImg.display();

}
void cutPiece::findDividingLine(int singleThreshold, int threshold) {
    HistogramImage = CImg<int>(gray.width(), gray.height(), 1, 1, 255);
    vector<int> blackPixelSet;
    dividingImg = gray;

    // 扫描
    cimg_forY(gray, y){
        int blackPixel = 0;
        cimg_forX(gray, x) {
            if (gray(x, y) == 0)
                blackPixel++;
        }
        bool flag = blackPixel > singleThreshold ? true : false;
        //邻域判断
        if(!flag && y - 1 >= 0 && blackPixelSet[y - 1] != 0){
            flag = true;
        }
        if(!flag && y - 2 >= 0 && blackPixelSet[y - 2] != 0){
            flag = true;
        }
        if(!flag) blackPixel = 0;
        if(blackPixel <= 2) blackPixel = 0;

        blackPixelSet.push_back(blackPixel);
    }

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

    cimg_forXY(HistogramImage, x, y){
        if( x < blackPixelSet[y]){
            HistogramImage(x, y) = 0;
        }
    }
    //HistogramImage.display();
    cimg_forY(HistogramImage, y) {
        if(y == 0) continue;
        // 求Y方向直方图，谷的最少黑色像素个数为0
        // 判断是否为拐点
        // 下白上黑：取下
        if (blackPixelSet[y] == 0 && blackPixelSet[y - 1] != 0)
            inflectionPoints.push_back(y);
            // 下黑上白：取上
        else if (blackPixelSet[y] != 0 && blackPixelSet[y - 1] == 0)
            inflectionPoints.push_back(y - 1);
    }
    unsigned char lineColor[1] = {0};
//    for(int i = 0; i < inflectionPoints.size(); i++){
//
//        dividingImg.draw_line(0, inflectionPoints[i],
//                              gray.width() - 1, inflectionPoints[i], lineColor);
//    }
//    dividingImg.display();
//    gray.display();
    //HistogramImage.display();
    // 舍去距离太近的两条线
//    for(auto iter = inflectionPoints.begin(); iter != inflectionPoints.end(); iter++){
//        if(iter + 1 == inflectionPoints.end()) break;
//        if(*(iter + 1) - *iter <= 8){
//            inflectionPoints.erase(iter + 1);
//            inflectionPoints.erase(iter);
//            iter--;
//        }
//    }
    // 宽度过大切分

//     while(true){
//         bool flag = false;
//         for(auto iter = inflectionPoints.begin(); iter != inflectionPoints.end(); iter++){
//             if(iter != inflectionPoints.begin()){
//                 if((*iter) - *(iter - 1) > 79) {
//                     cout<<(*(iter - 1))<<" "<<(*iter)<<endl;
//                     int counter = 0;
//                     for(int i = *(iter - 1); i < *iter; i++){
//                         if(blackPixelSet[i] != 0) counter++;
//                     }
//                     //空白推出
//                     cout<<"length!"<<endl;
//                     if((double)counter / (double)(*iter - *(iter - 1)) < 0.1) break;
//                     flag = true;
//                     double min = 10000.0;
//                     int line = -1;
//                     // ostu解决
//                     int threshold;
// //                    long sum0 = 0, sum1 = 0; //存储前景的灰度总和及背景灰度总和
// //                    long cnt0 = 0, cnt1 = 0; //前景的总个数及背景的总个数
// //                    double w0 = 0, w1 = 0; //前景及背景所占整幅图像的比例
// //                    double u0 = 0, u1 = 0;  //前景及背景的平均灰度
// //                    double variance = 0; //最大类间方差

//                     int sampleThrshold = 30;
//                     //double maxVariance = 0;
//                     int begin = *(iter - 1) + sampleThrshold;
//                     int end = (*iter) - sampleThrshold;
// //                    for (int i = ; i < ; i++) //一次遍历每个像素
// //                    {
// //
// ////                        sum0 = 0;
// ////                        sum1 = 0;
// ////                        cnt0 = 0;
// ////                        cnt1 = 0;
// ////                        w0 = 0;
// ////                        w1 = 0;
// ////                        for (int j = *(iter - 1) + sampleThrshold; j < i; j++) {
// ////                            cnt0 += blackPixelSet[j];
// ////                            sum0 += j * blackPixelSet[j];
// ////                        }
// ////                        if(cnt0 == 0) continue;
// ////                        u0 = (double) sum0 / (double)cnt0;
// ////                        if(cnt0 == 0) continue;
// ////                        w0 = (double) cnt0 / (double) (*iter - *(iter - 1));
// ////                        int val = (*iter);
// ////                        for (int j = i; j < val - sampleThrshold; j++) {
// ////                            cnt1 += blackPixelSet[j];
// ////                            sum1 += j * blackPixelSet[j];
// ////                        }
// ////
// ////                        u1 = (double) sum1 / cnt1;
// ////                        w1 = 1 - w0; // (double)cnt1 / size;
// ////
// ////                        variance = abs(w0 * w1 * (u0 - u1) * (u0 - u1));
// ////                        cout<<"vari:"<<variance<<endl;
// ////                        if (variance > maxVariance) {
// ////                            maxVariance = variance;
// ////                            threshold = i;
// ////                        }
// //                        threshold = (*iter - *(iter - 1)) / 2;
// //                        for()
// //                    }
//                     threshold = (end + begin) / 2;
//                     while(true){
//                         if(threshold == begin) threshold = begin + 1;
//                         if(threshold == end) threshold = end - 1;
//                         double sumFront = 0.0, sumBack = 0.0;
//                         for(int i = begin; i <= end; i++){
//                             if(i < threshold) sumFront += blackPixelSet[i];
//                             else sumBack += blackPixelSet[i];
//                         }
//                         sumFront /= (double)(threshold - begin);
//                         sumBack /= (double)(end - threshold);
//                         double median = (sumBack + sumFront) / 2.0;
//                         int distance = 10000;
//                         int pos = -1;
//                         for(int i = begin; i < end; i++){
//                             if(abs(blackPixelSet[i] - (int)median) < distance){
//                                 pos = i;
//                                 distance = abs(blackPixelSet[i] - (int)median);
//                             }
//                         }
//                         if(pos == threshold) break;
//                         else threshold = pos;
//                     }

//                     cout<<min<<" "<<(*(iter - 1))<<" "<<threshold<<" "<<(*iter)<<endl;
//                     iter = inflectionPoints.insert(iter, threshold - 1);
//                     inflectionPoints.insert(iter, threshold + 1);
//                     break;
//                 }
//             }
//         }
//         if(!flag)break;
//     }
//    while(true){
//        bool flag = false;
//        // 去重
//        for(auto iter = inflectionPoints.begin(); iter != inflectionPoints.end(); iter++){
//            if(iter != inflectionPoints.begin()){
//                if((*iter) - *(iter - 1) < 9){
//                    flag = true;
//                    int medium = (*iter + *(iter - 1)) / 2;
//                    *(iter - 1) = medium;
//                    inflectionPoints.erase(iter);
//                    iter--;
//                }
//            }
//        }
//        if(!flag)break;
//    }
    vector<int> tmpVec = inflectionPoints;
    inflectionPoints.clear();

    //零值淘汰
    for(int index = 0; index < tmpVec.size() - 1; index++){
        int counter = 0;
        for(int i = tmpVec[index]; i <= tmpVec[index + 1]; i++){
            if(blackPixelSet[i] != 0) counter++;
        }
        if((double)counter / (double)(tmpVec[index + 1] - tmpVec[index]) < 0.1) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            inflectionPoints.push_back(median);
            index++;
        } else{
            inflectionPoints.push_back(tmpVec[index]);
        }
    }
    inflectionPoints.push_back(tmpVec[tmpVec.size() - 1]);
    tmpVec = inflectionPoints;
    inflectionPoints.clear();
    int distance = tmpVec[ tmpVec.size() - 1 ] - tmpVec[0];
    distance /= tmpVec.size();
    // 距离淘汰
    for(int index = 0; index < tmpVec.size() - 1; index++){
        if(tmpVec[index + 1] - tmpVec[index] < (distance / 3)) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            inflectionPoints.push_back(median);
            index++;
        } else{
            inflectionPoints.push_back(tmpVec[index]);
        }
    }

    inflectionPoints.push_back(tmpVec[tmpVec.size() - 1]);
    for(int i = 0; i < inflectionPoints.size(); i++){

        dividingImg.draw_line(0, inflectionPoints[i],
                              gray.width() - 1, inflectionPoints[i], lineColor);
    }
    //dividingImg.display();
}

// 根据行分割线划分图片
void cutPiece::divideIntoBarItemImg(int threshold) {
    //vector<Hough_pos> tempDivideLinePointSet;
    for (int i = 1; i < inflectionPoints.size(); i++) {
        int barHeight = inflectionPoints[i] - inflectionPoints[i - 1];
        int blackPixel = 0;
        CImg<unsigned char> barItemImg = CImg<unsigned char>(gray.width(), barHeight, 1, 1, 0);
        cimg_forXY(barItemImg, x, y) {
            barItemImg(x, y, 0) = gray(x, inflectionPoints[i - 1] + 1 + y, 0);
            if (barItemImg(x, y, 0) == 0)
                blackPixel++;
        }
        //barItemImg.display();
        unsigned char lineColor[3] = {0, 0, 0};
        double blackPercent = (double)blackPixel / (double)(gray.width() * barHeight);

        // 只有当黑色像素个数超过图像大小一定比例0.001时，该航才可视作有数字
        if (blackPercent > (5.0/594.0)) {
            vector<int> dividePosXset = getDivideLineXofSubImage(barItemImg, threshold);
            if(dividePosXset.empty())continue;
//            vector<CImg<float>> rowItemImgSet = getRowItemImgSet(barItemImg, dividePosXset);
//
//            for (int j = 0; j < rowItemImgSet.size(); j++) {
//                subImageSet.push_back(rowItemImgSet[j]);
//                tempDivideLinePointSet.push_back(Hough_pos(dividePosXset[j], inflectionPoints[i - 1]));
//            }

//            if (i > 1) {
//                HistogramImage.draw_line(0, inflectionPoints[i - 1],
//                                         HistogramImage._width - 1, inflectionPoints[i - 1], lineColor);
//                dividingImg.draw_line(0, inflectionPoints[i - 1],
//                                      HistogramImage._width - 1, inflectionPoints[i - 1], lineColor);
//            }
            // 绘制竖线
            for (int j = 0; j < dividePosXset.size(); j++) {
                dividingImg.draw_line(dividePosXset[j], inflectionPoints[i - 1],
                                      dividePosXset[j], inflectionPoints[i], lineColor);
            }
        }
    }
    //dividingImg.display();

//    dividePoints.clear();
//    for (int i = 0; i < tempDivideLinePointSet.size(); i++)
//        dividePoints.push_back(tempDivideLinePointSet[i]);
}

// 获取一行行的子图的水平分割线
vector<int> cutPiece::getDivideLineXofSubImage(CImg<unsigned char>& subImg, int threshold) {
    // 先绘制X方向灰度直方图
    vector<int> countVec;
    cimg_forX(subImg, x){
        int blackPixel = 0;
        cimg_forY(subImg, y) {
            if (subImg(x, y) == 0)
                blackPixel++;
        }
        static int in = 0;
        in++;
        bool flag = (blackPixel > threshold) ? true : false;
        if(x - 1 >= 0 && countVec[x - 1] != 0){
            flag = true;
        }
        if(!flag){
            blackPixel = 0;
        }

//        if(blackPixel <= 2 && x - 2 >= 0 && x + 2 < subImg.width()){
//            if(countVec[x - 1] <= 2 && countVec[x - 2] <= 2 && countVec[x + 1] <= 2 && countVec[x + 2] <= 2){
//                blackPixel = 0;
//            }
//        }
        countVec.push_back(blackPixel);
    }



    //不为0的数
    int counter = 0;
    for(int i = 0; i < countVec.size(); i++){
        if(countVec[i] != 0) counter++;
    }

    CImg<int> Hou(subImg.width(), subImg.height(), 1, 1, 255);
    cimg_forXY(Hou, x, y){
        if(y < countVec[x]){
            Hou(x, y) = 0;
        }
    }
    //Hou.display();
    if(counter == 0){
        vector<int> InflectionPosXs;
      InflectionPosXs.clear();
        return InflectionPosXs;
    }
    vector<int> InflectionPosXs = getInflectionPosXs(countVec);    //获取拐点
    unsigned char lineColor[3] ={0, 0, 0};
    for(int i = 0; i < InflectionPosXs.size(); i++){
        subImg.draw_line(InflectionPosXs[i], 0, InflectionPosXs[i], subImg.width() - 1, lineColor);
    }
    //subImg.display();
    return InflectionPosXs;
}


// 分割行子图，得到列子图
vector< CImg<float> > cutPiece::getRowItemImgSet(const CImg<float>& lineImg, vector<int> _dividePosXset) {
    vector< CImg<float> > result;
    for (int i = 1; i < _dividePosXset.size(); i++) {
        int rowItemWidth = _dividePosXset[i] - _dividePosXset[i - 1];
        CImg<float> rowItemImg = CImg<float>(rowItemWidth, lineImg._height, 1, 1, 0);
        cimg_forXY(rowItemImg, x, y) {
            rowItemImg(x, y, 0) = lineImg(x + _dividePosXset[i - 1] + 1, y, 0);
        }
        result.push_back(rowItemImg);
    }

    return result;
}

// 根据X方向直方图判断真实的拐点
vector<int> cutPiece::getInflectionPosXs(vector<int> counterVec) {
    vector<int> resultInflectionPosXs, tempInflectionPosXs;
    int totalDist = 0, dist = 0;
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
    tempInflectionPosXs.push_back(tmpVec[0]);
    for(int index = 1; index < tmpVec.size() - 1; index++){
        int counter = 0;
        for(int i = tmpVec[index]; i <= tmpVec[index + 1]; i++){
            if(counterVec[i] != 0) counter++;
        }
        if((double)counter / (double)(tmpVec[index + 1] - tmpVec[index]) < 0.05) {
            int median = (tmpVec[index + 1] + tmpVec[index]) / 2;
            tempInflectionPosXs.push_back(median);
            index++;
        } else{
            tempInflectionPosXs.push_back(tmpVec[index]);
        }
    }
    tempInflectionPosXs.push_back(tmpVec[tmpVec.size() - 1 ]);
    //距离分割
    int max = 0;
    for(int i = 1; i < tempInflectionPosXs.size(); i++){
        if(tempInflectionPosXs[i] - tempInflectionPosXs[i - 1] > max) max = tempInflectionPosXs[i] - tempInflectionPosXs[i - 1];
    }
    int distance = 0;
    for(int i = 1; i < tempInflectionPosXs.size(); i++){
        distance += tempInflectionPosXs[i] - tempInflectionPosXs[i - 1];
    }
    distance -= max;

    distance /= (tempInflectionPosXs.size() - 1);
    for(auto iter = tempInflectionPosXs.begin(); iter != tempInflectionPosXs.end(); iter++){
        if(iter != tempInflectionPosXs.begin() && *iter - *(iter - 1) >= 2 * distance){
            int median = *iter - *(iter - 1);
            median /= 2;
            median += *(iter - 1);
            tempInflectionPosXs.insert(iter, median);
            iter = tempInflectionPosXs.begin();
        }
    }
    return tempInflectionPosXs;
}

CImg<unsigned char> cutPiece::threshold(CImg<unsigned char>& imgIn, int low, int columnNumber, int rowNumber, float abandonThreshold)
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
            int min = 256;
            int max = -1;
            for(int k = 0; k < columnSize; k++){
                for(int l = 0; l < rowSize; l++){
                    if(imgIn(i * columnSize + k, j * rowSize + l) > max) max = imgIn(i * columnSize + k, j * rowSize + l);
                    else if(imgIn(i * columnSize + k, j * rowSize + l) < min) min = imgIn(i * columnSize + k, j * rowSize + l);
                }
            }
            int threshold = min + (int)((float)(max - min) * ((float)low / 255.0));
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
        int min = 256;
        int max = -1;
        for(int i = imgIn.width() - resizeCol - 1; i < imgIn.width(); i++){
            for(int l = 0; l < rowSize; l++){
                if(imgIn(i, j * rowSize + l) > max) max = imgIn(i, j * rowSize + l);
                else if(imgIn(i, j * rowSize + l) < min) min = imgIn(i, j * rowSize + l);
            }
        }
        int threshold = min + (int)((float)(max - min) * ((float)low / 255.0));
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
        int min = 256;
        int max = -1;
        for(int j = imgIn.height() - resizeRow - 1; j < imgIn.height(); j++){
            for(int k = 0; k < columnSize; k++){
                if(imgIn(i * columnSize + k, j) > max) max = imgIn(i * columnSize + k, j);
                else if(imgIn(i * columnSize + k, j) < min) min = imgIn(i * columnSize + k, j);
            }
        }
        int threshold = min + (int)((float)(max - min) * ((float)low / 255.0));
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
    cimg_forXY(afterThreshold, x, y){
        if(x <= BOUNDER || y <= BOUNDER || x >= afterThreshold.width() - BOUNDER || y >= afterThreshold.height() - BOUNDER){
            afterThreshold(x, y) = 255;
        }
    }
    return afterThreshold;
}