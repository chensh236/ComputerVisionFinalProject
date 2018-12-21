//
// Created by Chen Sihang on 2018/12/18.
//
#include "cutPiece.h"
#include "segment.h"
#include "prepareLearning.h"
#include "Hough.h"
#include "canny.h"


// 检查像素位置，防止超过图片宽度
int valueWidth(double srcX, int width) {
    if (srcX < 0) srcX = 0;
    if (srcX >= width) srcX--;
    return srcX;
}

// 检查像素位置，防止超过图片高度
int valueHeight(double srcY, int height) {
    if (srcY < 0) srcY = 0;
    if (srcY >= height) srcY--;
    return srcY;
}

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

/*
* 旋转图片，旋转角为theta，并采用双线性插值减少锯齿
*/
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


    //newImg.display();
    if(isBinary){
        cimg_forXY(newImg, x, y){
            if(newImg(x, y) < 180) newImg(x, y) = 0;
            else newImg(x, y) = 255;
        }
        newImg = canny::newFunc(newImg, 20);
    }

    return newImg;
}


cutPiece::cutPiece(CImg<unsigned char> input){
    gray = segment::toGrayScale(input);
    resultGray = gray;
    CImg<unsigned char> grayScale = gray;
    //gray.display();
    //单阈值检测
    gray = threshold(grayScale, 0.76, 23, 28, 0.76);
    CImg<unsigned char> dilationed = gray;
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
    }
    gray = dilationed;
   // gray = canny::newFunc(gray, 5);
    dilationed = canny::newFunc(dilationed, 60);
    dilationed = canny::newFunc(dilationed, 40);
    cimg_forXY(dilationed, x, y){
        if(dilationed(x, y) == 0) dilationed(x, y) = 255;
        else dilationed(x, y) = 0;
        if(gray(x, y) == 0) gray(x, y) = 255;
        else gray(x, y) = 0;
    }
    //gray.display();
    cout<<"begin!"<<endl;

    //canny cny(dilationed, 120, 160);
    //cny.getResult().display();
    //dilationed = cny.getResult();

    //dilationed = dilationed.resize(dilationed.width() / 2, dilationed.height() / 2, true);
    //dilationed.display();
   // prepareLearning::doDilationForEachBarItemImg(dilationed);
    Hough hough(0, 0, dilationed, 0, false);
    int theta = hough.randonTheta;
    cout<<theta<<endl;
    if(theta > 9 && theta < 13){
        theta-= 3;
        double ror = (double)theta / 180.0 * cimg::PI;
        gray = reotate_biliinar(gray, ror, true);
        resultGray = reotate_biliinar(resultGray, ror, false);
        //resultGray.display();
    }
    //gray.display();


    findDividingLine(5, 4);
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

    cimg_forXY(HistogramImage, x, y){
        if( x < blackPixelSet[y]){
            HistogramImage(x, y) = 0;
        }
    }
    HistogramImage.display();
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

        // 只有当黑色像素个数超过图像大小一定比例时，该航才可视作有数字
        if (blackPercent > (5.0/594.0)) {
            vector<square> squareTmp;
            vector<int> dividePosXset = getDivideLineXofSubImage(barItemImg, threshold);
            if(dividePosXset.empty())continue;

            for(int j = 1; j < dividePosXset.size(); j++){

                //去除污点！
                square squ(Hough_pos(dividePosXset[j - 1], inflectionPoints[i - 1]), Hough_pos(dividePosXset[j - 1], inflectionPoints[i]),
                        Hough_pos(dividePosXset[j], inflectionPoints[i - 1]), Hough_pos(dividePosXset[j], inflectionPoints[i]));
                int currentHeight = squ.lb.y - squ.lt.y + 1;
                vector<int> histogram;
                for(int i = squ.lt.y; i <= squ.lb.y; i++){
                    int counter = 0;
                    for(int j = squ.lt.x; j <= squ.rt.x; j++){
                        if(gray(j, i) != 255) counter++;
                    }
                    histogram.push_back(counter);
                }
                // 遍历
                int blackFirst, blackLast;
                for(int i = squ.lt.y; i <= squ.lb.y; i++){
                   if(histogram[i - squ.lt.y] != 0){
                       blackFirst = i;
                       break;
                   }
                }
                for(int i = squ.lb.y; i >= squ.lt.y; i--){
                    if(histogram[i - squ.lt.y] != 0){
                        blackLast = i;
                        break;
                    }
                }
                int distance = blackLast - blackFirst + 1;
                if((double)distance / (double)currentHeight < 0.2){
                    continue;
                }
                squareTmp.push_back(squ);
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
    //第一条往左挪动
    if(tmpVec[0] > 5) tmpVec[0] -= 5;
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
//    //距离分割(you bug!)
//    int max = 0;
//    for(int i = 1; i < tempInflectionPosXs.size(); i++){
//        if(tempInflectionPosXs[i] - tempInflectionPosXs[i - 1] > max) max = tempInflectionPosXs[i] - tempInflectionPosXs[i - 1];
//    }
//    int distance = 0;
//    for(int i = 1; i < tempInflectionPosXs.size(); i++){
//        distance += tempInflectionPosXs[i] - tempInflectionPosXs[i - 1];
//    }
//    distance -= max;
//
//    distance /= (tempInflectionPosXs.size() - 1);
//    for(auto iter = tempInflectionPosXs.begin(); iter != tempInflectionPosXs.end(); iter++){
//        if(iter != tempInflectionPosXs.begin() && *iter - *(iter - 1) >= 2 * distance){
//            int median = *iter - *(iter - 1);
//            median /= 2;
//            median += *(iter - 1);
//            tempInflectionPosXs.insert(iter, median);
//            iter = tempInflectionPosXs.begin();
//        }
//    }
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