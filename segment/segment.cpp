#include "segment.h"

segment::segment(string filepath, float sigma){
    CImg<unsigned char> src(filepath.c_str());
    src = src.get_blur(sigma);
    //src.display();
    kmeans(src);
}

void segment::kmeans(CImg<unsigned char> &src) {
    // // 获得随机点
    // core1.getRandPos(src.width(), src.height());
    // core2.getRandPos(src.width(), src.height());
    // while(isEqual(core1, core2)){
    //     core1.getRandPos(src.width(), src.height());
    //     core2.getRandPos(src.width(), src.height());
    // }
    core1.setPosition(src.width() / 2, src.height() / 2);
    core2.setPosition(src.width() - 1, src.height() - 1);
    vector<position> set1;
    vector<position> set2;
    while(true){
        set1.clear();
        set2.clear();
        //获得两个簇
        cimg_forXY(src, x, y){
            int distance1 = abs((int)src(x, y, 0) - (int)src(core1.x, core1.y, 0)) +
                            abs((int)src(x, y, 1) - (int)src(core1.x, core1.y, 1)) +
                            abs((int)src(x, y, 2) - (int)src(core1.x, core1.y, 2));
            int distance2 = abs((int)src(x, y, 0) - (int)src(core2.x, core2.y, 0)) +
                            abs((int)src(x, y, 1) - (int)src(core2.x, core2.y, 1)) +
                            abs((int)src(x, y, 2) - (int)src(core2.x, core2.y, 2));
            position tmp(x, y);
            if(distance1 <= distance2){
                set1.push_back(tmp);
            } else{
                set2.push_back(tmp);
            }
        }
        position currentCore1 = getMeans(set1, src);
        position currentCore2 = getMeans(set2, src);
        if(isEqual(currentCore1, core1) && isEqual(currentCore2, core2)) break;
        core1.setPosition(currentCore1.x, currentCore1.y);
        core2.setPosition(currentCore2.x, currentCore2.y);
    }
    CImg<unsigned char> tmp = CImg<unsigned char>(src.width(), src.height(), 1, 1, 0);
    result = tmp;
    for(int i = 0; i < set1.size(); i++){
        tmp(set1[i].x, set1[i].y) = 255;
    }
    block = tmp;
    cimg_forXY(tmp, x, y){
        //马尔科夫领域
        if(x > 0.976 * tmp.width() || x < 0.024* tmp.width()) continue;
        if(tmp(x, y) == 0) continue;
        if((x - 1 >= 0 && y - 1 >= 0 && tmp(x - 1, y - 1) == 0)|| //0
            (y - 1 >= 0 && tmp(x, y - 1) == 0) || //1
            (x + 1 < tmp.width() && y - 1 >= 0 && tmp(x + 1, y - 1) == 0) || //2
            (x - 1 >= 0 && tmp(x - 1, y) == 0) ||//3
            (x + 1 < tmp.width() && tmp(x + 1, y) == 0) || //5
            (x - 1 >= 0 && y + 1 < tmp.height() && tmp(x - 1, y + 1) == 0) || //6
            (y + 1 < tmp.height() && tmp(x, y + 1) == 0) || //7
            (x + 1 < tmp.width() && y + 1 < tmp.height() && tmp(x + 1, y + 1) == 0) //8
            ){
            result(x, y) = 255;
            continue;
        }
    }
    result = deleteEdge(result, 40);
}

bool segment::isEqual(position& p1, position& p2){
    return(p1.x == p2.x && p1.y == p2.y);
}

position segment::getMeans(vector<position>& set, CImg<unsigned char>& src){
    float meansR = 0;
    float meansG = 0;
    float meansB = 0;
    // get the mean of RGB channels
    for(int i = 0; i < set.size(); i++){
        meansR += (float)src(set[i].x, set[i].y, 0);
        meansG += (float)src(set[i].x, set[i].y, 1);
        meansB += (float)src(set[i].x, set[i].y, 2);
    }

    meansR /= (float)set.size();
    meansG /= (float)set.size();
    meansB /= (float)set.size();

    float minDistance = 10000;
    position core;
    for(int i = 0; i < set.size(); ++i){
        float distance = abs((float)src(set[i].x, set[i].y, 0) - meansR) +
                        abs((float)src(set[i].x, set[i].y, 1) - meansG) +
                        abs((float)src(set[i].x, set[i].y, 2) - meansB);
        if(distance < minDistance){
            core.setPosition(set[i].x, set[i].y);
            minDistance = distance;
        }
    }
    return core;
}

CImg<unsigned char> segment::deleteEdge(CImg<unsigned char> imgin, int length)
{
    CImg<unsigned char> output = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 1, 0);
    stack<position> s;
    queue<position> q;
    cimg_forXY(imgin, x, y){
        // DFS begin
        if(imgin(x, y) == 255){
            s.push(position(x, y));
            q.push(position(x, y));
            imgin(x ,y) = 0;
            while(!s.empty()){
                position p = s.top();
                s.pop();
                // search in 8 different direcvtions
                if(p.x - 1 > 0 && p.y - 1 > 0 && imgin(p.x - 1, p.y - 1) == 255){
                    position np = position(p.x - 1, p.y - 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x - 1, p.y - 1) = 0;
                }

                if(p.y - 1 > 0 && imgin(p.x, p.y - 1) == 255){
                    position np = position(p.x, p.y - 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x, p.y - 1) = 0;
                }

                if(p.x + 1 < imgin.width() && p.y - 1 > 0 && imgin(p.x + 1, p.y - 1) == 255){
                    position np = position(p.x + 1, p.y - 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x + 1, p.y - 1) = 0;
                }

                if(p.x - 1 > 0 && imgin(p.x - 1, p.y) == 255){
                    position np = position(p.x - 1, p.y);
                    s.push(np);
                    q.push(np);
                    imgin(p.x - 1, p.y) = 0;
                }

                if(p.x + 1 < imgin.width() && imgin(p.x + 1, p.y) == 255){
                    position np = position(p.x + 1, p.y);
                    s.push(np);
                    q.push(np);
                    imgin(p.x + 1, p.y) = 0;
                }

                if(p.x - 1 > 0 && p.y + 1 < imgin.height() && imgin(p.x - 1, p.y + 1) == 255){
                    position np = position(p.x - 1, p.y + 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x - 1, p.y + 1) = 0;
                }

                if(p.y + 1 < imgin.height() && imgin(p.x, p.y + 1) == 255){
                    position np = position(p.x, p.y + 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x, p.y + 1) = 0;
                }

                if(p.x + 1 < imgin.width() && p.y + 1 < imgin.height() && imgin(p.x + 1, p.y + 1) == 255){
                    position np = position(p.x + 1, p.y + 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x + 1, p.y + 1) = 0;
                }
            }

            // No more element in the stack
            if (q.size() > length)
            {
                // The weak edge is not connected to any strong edge. Suppress it.
                while (!q.empty())
                {
                    position p = q.front();
                    q.pop();
                    output(p.x, p.y) = 255;
                }
            }else{
                // Clean the queue
                while (!q.empty()) q.pop();
            }
        }
    }
    return output;
}