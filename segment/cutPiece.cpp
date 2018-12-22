//
// Created by Chen Sihang on 2018/12/18.
//
#include "cutPiece.h"
#include "segment.h"
#include "prepareLearning.h"
#include "Hough.h"
#include "canny.h"


// 获得旋转后图片的像素对应于原图的像素位置，用于双线性插值
bool get_origin_pos(int x, int y, int srcWidth, int srcHeight, double theta,
                    double& srcX, double& srcY) {
    // 找到(x, y)在原图中对应的位置(srcX, srcY)
    srcX = (double)x * cos(theta) - (double)y * sin(theta);
    srcY = (double)x * sin(theta) + (double)y * cos(theta);
    if (srcX >= (0-srcWidth/2-1) && srcX <= srcWidth/2+1 && srcY >= (0-srcHeight/2-1) && srcY <= srcHeight/2+1) {
        srcX += srcWidth/2;
        srcY += srcHeight/2;
        return true;
    } else {
        return false;
    }
}


/**
* 采用双线性插值填充新图
*/
void bilinear_interpolation(CImg<unsigned char>& outImg,
                            const CImg<unsigned char>& srcImg, double theta) {

    int halfW = outImg.width() / 2;
    int halfH = outImg.height() / 2;
    cimg_forXY(outImg, x, y)
    {
        double src_x, src_y;
        if(get_origin_pos( x - halfW, y - halfH, srcImg.width(), srcImg.height(), theta, src_x, src_y)){
            src_x = (src_x >= (double)srcImg.width()) ? ((double)srcImg.width() - 1.0) : (src_x < 0.0) ? 0.0 : src_x;
            src_y = (src_y >= (double)srcImg.height()) ? ((double)srcImg.height() - 1.0) : (src_y < 0.0) ? 0.0 : src_y;
            outImg(x, y) = Projection::singleBilinearInterpolation(srcImg, src_x, src_y, 0);
        }
    }
}

CImg<unsigned char> reotate_biliinar(const CImg<unsigned char>& srcImg, double theta, bool isBinary) {
    int width = srcImg.width();
    int height = srcImg.height();
    // 原图的四个顶点坐标，这里以图片中心为坐标原点
    Hough_pos lt(0-width/2, 0+height/2), lb(0-width/2, 0-height/2),
            rt(0+width/2, 0+height/2), rb(0+width/2, 0-height/2);
    // 获得旋转后的图片的四个顶点坐标
    Hough_pos new_lt((int)(lt.x*cos(theta)+lt.y*sin(theta)), (int)(lt.y*cos(theta)-lt.x*sin(theta))),
            new_lb((int)(lb.x*cos(theta)+lb.y*sin(theta)), (int)(lb.y*cos(theta)-lb.x*sin(theta))),
            new_rt((int)(rt.x*cos(theta)+rt.y*sin(theta)), (int)(rt.y*cos(theta)-rt.x*sin(theta))),
            new_rb((int)(rb.x*cos(theta)+rb.y*sin(theta)), (int)(rb.y*cos(theta)-rb.x*sin(theta)));
    int newWidth = max(abs(new_rt.x-new_lb.x), abs(new_lt.x-new_rb.x));
    int newHeight = max(abs(new_lt.y-new_rb.y), abs(new_lb.y-new_rt.y));

    CImg<unsigned char> newImg(newWidth, newHeight, 1, 1, 255);
    // 开始填充新图片的灰度值
    bilinear_interpolation(newImg, srcImg, theta);

    if(isBinary){
        //newImg.display();
        cimg_forXY(newImg, x, y){
            if(newImg(x, y) > 200) newImg(x, y) = 255;
            else newImg(x, y) = 0;
        }
    }

    return newImg;
}


cutPiece::cutPiece(CImg<unsigned char> input){
    gray = segment::toGrayScale(input);
    resultGray = gray;
    CImg<unsigned char> grayScale = gray;
    //gray.display();
    //单阈值检测
    //第一个低 多 第二个高 多
    gray = threshold(grayScale, 0.76, 23, 28, 0.77);

    //gray.display();
    CImg<unsigned char> dilationed = gray;
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
    }
    gray = dilationed;
    dilationed = canny::newFunc(dilationed, 30);
    gray = canny::newFunc(dilationed, 8);
    Hough hough(0, 0, dilationed, 0, false);
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
        if(gray(x, y) == 0) gray(x, y) = 255;
        else gray(x, y) = 0;
    }

    double theta = hough.randonTheta;

    theta = (theta - 90.0);
    //cout<<theta<<endl;
    double ror = theta / 180.0 * cimg::PI;
    gray = reotate_biliinar(gray, ror, true);
    resultGray = reotate_biliinar(resultGray, ror, false);


    findDividingLine(3, 4);
    divideIntoBarItemImg(3);


}
void cutPiece::findDividingLine(int singleThreshold, int threshold) {
    HistogramImage = CImg<int>(gray.width(), gray.height(), 1, 1, 255);
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

    vector<int> tmpVec = inflectionPoints;
    inflectionPoints.clear();

    //零值淘汰，每一行只有一条中线
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
    // 距离淘汰(太近合并)
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
    if(inflectionPoints[0] > 2){
        inflectionPoints[0] -= 2;
    }
    if(inflectionPoints[inflectionPoints.size() - 1] + 2 < gray.height()){
        inflectionPoints[inflectionPoints.size() - 1] += 2;
    }
    for(int i = 0; i < inflectionPoints.size(); i++){

        dividingImg.draw_line(0, inflectionPoints[i], gray.width() - 1, inflectionPoints[i], lineColor);
    }
}

// 根据行分割线划分图片
void cutPiece::divideIntoBarItemImg(int threshold) {

    for (int i = 1; i < inflectionPoints.size(); i++) {
        int barHeight = inflectionPoints[i] - inflectionPoints[i - 1];
        int blackPixel = 0;
        CImg<unsigned char> barItemImg = CImg<unsigned char>(gray.width(), barHeight, 1, 1, 0);
        cimg_forXY(barItemImg, x, y) {
            barItemImg(x, y, 0) = gray(x, inflectionPoints[i - 1] + 1 + y, 0);
            if (barItemImg(x, y, 0) == 0)
                blackPixel++;
        }

        unsigned char lineColor[1] = {0};
        double blackPercent = (double)blackPixel / (double)(gray.width() * barHeight);

        // 只有当黑色像素个数超过图像大小一定比例时，该航才可视作有数字
        if (blackPercent > (7.0/594.0)) {
            vector<square> squareTmp;
            vector<int> dividePosXset = getDivideLineXofSubImage(barItemImg, threshold);
            if(dividePosXset.empty())continue;

            for(int j = 1; j < dividePosXset.size(); j++){

                //去除污点！
                square squ(Hough_pos(dividePosXset[j - 1], inflectionPoints[i - 1]), Hough_pos(dividePosXset[j - 1], inflectionPoints[i]),
                        Hough_pos(dividePosXset[j], inflectionPoints[i - 1]), Hough_pos(dividePosXset[j], inflectionPoints[i]));
                float count = 0.0;
                for(int x = squ.lt.x; x <= squ.rt.x; x++){
                    for(int y = squ.lt.y; y<= squ.lb.y; y++){
                        if(gray(x, y) == 0) count++;
                    }
                }
                float rate = count / (float)((squ.rt.x - squ.lt.x)*(squ.lb.y - squ.lt.y));
                cout<<"rate:"<<rate<<endl;
                if(rate > 0.016){
                    squareTmp.push_back(squ);
                }
                //                int currentHeight = squ.lb.y - squ.lt.y + 1;
//                vector<int> histogram;
//                for(int i = squ.lt.y; i <= squ.lb.y; i++){
//                    int counter = 0;
//                    for(int j = squ.lt.x; j <= squ.rt.x; j++){
//                        if(gray(j, i) != 255) counter++;
//                    }
//                    histogram.push_back(counter);
//                }
//                // 遍历
//                int blackFirst, blackLast;
//                for(int i = squ.lt.y; i <= squ.lb.y; i++){
//                   if(histogram[i - squ.lt.y] != 0){
//                       blackFirst = i;
//                       break;
//                   }
//                }
//                for(int i = squ.lb.y; i >= squ.lt.y; i--){
//                    if(histogram[i - squ.lt.y] != 0){
//                        blackLast = i;
//                        break;
//                    }
//                }
//                int distance = blackLast - blackFirst + 1;
//                if((double)distance / (double)currentHeight < 0.2){
//                    continue;
//                }

            }
            squareVec.push_back(squareTmp);
            // 绘制竖线
            for (int j = 0; j < dividePosXset.size(); j++) {
                dividingImg.draw_line(dividePosXset[j], inflectionPoints[i - 1],
                                      dividePosXset[j], inflectionPoints[i], lineColor);
            }
        }
    }

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

//    CImg<int> Hou(subImg.width(), subImg.height(), 1, 1, 255);
//    cimg_forXY(Hou, x, y){
//        if(y < countVec[x]){
//            Hou(x, y) = 0;
//        }
//    }
//    Hou.display();
    //空的行
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

// 根据X方向直方图判断真实的拐点
vector<int> cutPiece::getInflectionPosXs(vector<int> counterVec) {
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
    for(auto iter = tempInflectionPosXs.begin(); iter != tempInflectionPosXs.end(); iter++){
        if(iter != tempInflectionPosXs.begin() && (float)(*iter - *(iter - 1)) >= 2.17 * distance){
            int median = *iter - *(iter - 1);
            median /= 2;
            median += *(iter - 1);
            tempInflectionPosXs.insert(iter, median);
            iter = tempInflectionPosXs.begin();
        }
    }
    return tempInflectionPosXs;
}

CImg<unsigned char> cutPiece::threshold(CImg<unsigned char>& imgIn, float binaryThreshold, int columnNumber, int rowNumber, float abandonThreshold)
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
            int threshold = min + (max - min) * binaryThreshold;
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
        int threshold = min + (max - min) * binaryThreshold;
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
        int threshold = min + (max - min) * binaryThreshold;
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