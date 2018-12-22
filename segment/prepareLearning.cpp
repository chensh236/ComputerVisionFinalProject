//
// Created by Chen Sihang on 2018/12/19.
//
#include "prepareLearning.h"
prepareLearning::prepareLearning(CImg<unsigned char> input, vector<vector<square> > square, int imgSeq) {
    equalization eql(input);
    //input = eql.getGrayResult();
    //input.display("prepare");
    for(int i = 0; i < square.size(); i++){
        vector< CImg<unsigned char> > imgTmp;
        for(int j = 0; j < square[i].size(); j++){
            CImg<unsigned char> part(square[i][j].rt.x - square[i][j].lt.x + 1, square[i][j].lb.y - square[i][j].lt.y + 1, 1, 1, 0);
            for(int x = square[i][j].lt.x; x <= square[i][j].rt.x; x++){
                for(int y = square[i][j].lt.y; y <= square[i][j].lb.y; y++){
                    part(x - square[i][j].lt.x, y - square[i][j].lt.y) = input(x, y);
                }
            }
            CImg<unsigned char> gray = part;
            //gray.display();
            if(thresholdDetect(gray)){
                //doDilationForEachBarItemImg(gray);
                gray = resizeNum(gray);

                // strong
                cimg_forXY(gray, x, y){
                    if(gray(x, y) == 0) gray(x, y) = 255;
                    else if(gray(x, y) == 255) gray(x, y) = 0;
                }
//                gray.display();
                  gray = gray.resize(28, 28, true);
                //gray.display();
//                  gray.display();
                imgTmp.push_back(gray);
            }


        }
        imgVec.push_back(imgTmp);
    }

    //-------------IO-------------
    string dir="../temp";
    if (access(dir.c_str(), 0) == -1)
    {
        cout<<dir<<" is not existing"<<endl;
        cout<<"now make it"<<endl;
#ifdef WIN32
        int flag=mkdir(dir.c_str());
#endif
#ifdef linux
        int flag=mkdir(dir.c_str(), 0777);
#endif
        if (flag == 0)
        {
            cout<<"make successfully"<<endl;
        } else {
            cout<<"make errorly"<<endl;
        }
    }
    if(access(dir.c_str(), 0) == 0){
        subDir = "../temp/img" + to_string(imgSeq);
        if (access(subDir.c_str(), 0) == -1)
        {
            cout<<subDir<<" is not existing"<<endl;
            cout<<"now make it"<<endl;
#ifdef WIN32
            int flag=mkdir(subDir.c_str());
#endif
#ifdef linux
            int flag=mkdir(subDir.c_str(), 0777);
#endif
            if (flag == 0)
            {
                cout<<"make successfully"<<endl;
            } else {
                cout<<"make errorly"<<endl;
            }
        }
        if(access(subDir.c_str(), 0) == 0){
            storeImg();
        } else{
            failAccess(subDir);
        }
    } else{
        failAccess(dir);
    }
    cout<<"Image index:"<<imgSeq<<" processed success!"<<endl;

}

CImg<unsigned char> prepareLearning::resizeNum(CImg<unsigned char>& input){
    // 数字居中
    //    // 图像恢复
    int firstBlack_x = 0, firstBlack_y = 0;
    int lastBlack_x = 0, lastBlack_y = 0;
    bool flag = false;

    cimg_forX(input, x){
        cimg_forY(input, y){
            if(input(x ,y) != 255){
                firstBlack_x = x;
                flag = true;
                break;
            }
        }
        if(flag) break;

    }

    flag = false;
    cimg_forY(input, y){
        cimg_forX(input, x){
            if(input(x ,y) != 255){
                firstBlack_y = y;
                flag = true;
                break;
            }
        }
        if(flag) break;

    }
    flag = false;
    for(int x = input.width() - 1; x >= 0; x--){
        for(int y = 0; y < input.height(); y++){
            if(input(x, y) != 255){
                lastBlack_x = x;
                flag = true;
                break;
            }
        }
        if(flag) break;
    }

    flag = false;
    for(int y = input.height() - 1; y >= 0; y--){
        for(int x = 0; x < input.width(); x++){
            if(input(x, y) != 255){
                lastBlack_y = y;
                flag = true;
                break;
            }
        }
        if(flag) break;
    }

    // 得到中心
    //cout<<firstBlack_x<<" "<<lastBlack_x<<" "<<firstBlack_y<<" "<<lastBlack_y<<endl;
    int x_length = lastBlack_x - firstBlack_x + 1;
    int y_length = lastBlack_y - firstBlack_y + 1;
    CImg<unsigned char> tmp(x_length + 8, y_length + 8, 1, 1, 255);
    for(int i = 4, src_x = firstBlack_x; src_x <= lastBlack_x; i++, src_x++){
        for(int j = 4, src_y = firstBlack_y; src_y <= lastBlack_y; j++, src_y++){
            tmp(i, j) = input(src_x, src_y);
            //cout<<src_x<<" "<<src_y<<endl;
        }
    }
    return tmp;
}

void prepareLearning::storeImg(){
    //====guide.txt========
    string path = subDir + "/guide.txt";
    ofstream ofs(path.c_str());
    for(int i = 0; i < imgVec.size(); i++){
        for(int j = 0; j < imgVec[i].size(); j++){
            string finalPath = subDir + "/" + to_string(i) + "_" + to_string(j) + ".bmp";
            imgVec[i][j].save(finalPath.c_str());
        }
        ofs<<to_string(imgVec[i].size())<<endl;
    }
    ofs.close();
}

void prepareLearning::failAccess(string str){
    cout<<"failt to access the dir:"<<str<<endl;
    exit(0);
}

bool prepareLearning::thresholdDetect(CImg<unsigned char>& input){
    // 计算方差
    //input.display();
    double varSum = 0.0;
    cimg_forXY(input, x ,y){
        varSum += (double)input(x, y);
    }
    varSum /= (double)(input.width() * input.height());
    double var = 0.0;
    cimg_forXY(input, x, y){
        var += (input(x, y) - varSum) * (input(x, y) - varSum);
    }
    if(var <= 913.0) return false;


    //阈值
    set<int> numStore;
    cimg_forXY(input, x, y){
        numStore.insert(input(x, y));
    }
    int sum = 0;
    for(auto iter = numStore.begin(); iter != numStore.end(); iter++){
        sum += *(iter);
    }
    sum /= numStore.size();
    cimg_forXY(input, x, y){
        if(input(x, y) > sum ) input(x, y) = 255;
        else input(x, y) = 0;
    }

    return true;
}
//
//void prepareLearning::doDilationForEachBarItemImg(CImg<unsigned char>& input) {
//    //扩张Dilation -X-X-X-XYY方向
//    CImg<unsigned char> answerXXY = CImg<unsigned char>(input.width(), input.height(), 1, 1, 0);
//    cimg_forXY(input, x, y) {
//        int intensity = getDilationIntensityXXY(input, x, y);
//        answerXXY(x, y, 0) = intensity;
//    }
//
//    //扩张Dilation -X-X-X-XYY方向
//    CImg<unsigned char> answerXXY2 = CImg<unsigned char>(answerXXY.width(), answerXXY.height(), 1, 1, 0);
//    cimg_forXY(answerXXY, x, y) {
//        int intensity = getDilationIntensityXXY(answerXXY, x, y);
//        answerXXY2(x, y, 0) = intensity;
//    }
//
//    //扩张Dilation XY方向
//    CImg<unsigned char> answerXY = CImg<unsigned char>(answerXXY2.width(), answerXXY2.height(), 1, 1, 0);
//    cimg_forXY(answerXXY2, x, y) {
//        int intensity = getDilationIntensityXY(answerXXY2, x, y);
//        answerXY(x, y, 0) = intensity;
//    }
//
//    cimg_forXY(input, x, y) {
//        input(x, y, 0) = answerXY(x, y, 0);
//    }
//}
//
//// XY方向的正扩张
//int prepareLearning::getDilationIntensityXY(const CImg<float>& Img, int x, int y) {
//    int intensity = Img(x, y, 0);
//    if (intensity == 255) {
//        for (int i = -1; i <= 1; i++) {
//            for (int j = -1; j <= 1; j++) {
//                if (0 <= x + i && x + i < Img._width && 0 <= y + j && y + j < Img._height) {
//                    if (i != -1 && j != -1 || i != 1 && j != 1 || i != 1 && j != -1 || i != -1 && j != 1)
//                        if (Img(x + i, y + j, 0) == 0) {
//                            intensity = 0;
//                            break;
//                        }
//                }
//            }
//            if (intensity != 255)
//                break;
//        }
//    }
//    return intensity;
//}
//
//
//int prepareLearning::getDilationIntensityXXY(CImg<unsigned char>& Img, int x, int y){
//    int intensity = Img(x, y, 0);
//    if (intensity == 255) {    //若中间点为白色
//        int blackAccu = 0;
//
//        for (int i = -1; i <= 1; i++) {
//            if (0 <= y + i && y + i < Img._height) {    //垂直方向累加
//                if (Img(x, y + i, 0) == 0)
//                    blackAccu++;
//            }
//        }
//        for (int i = -2; i <= 2; i++) {
//            if (0 <= x + i && x + i < Img._width) {     //水平方向累减
//                if (Img(x + i, y, 0) == 0)
//                    blackAccu--;
//            }
//        }
//
//        if (blackAccu > 0)
//            intensity = 0;
//    }
//    return intensity;
//}