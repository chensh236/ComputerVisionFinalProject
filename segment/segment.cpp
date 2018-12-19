#include "segment.h"

segment::segment(string filepath, float sigma)
{
    CImg<unsigned char> src(filepath.c_str());
    grayImg = toGrayScale(src);
    grayImg = grayImg.get_blur(sigma);
    kmeans(grayImg);
}

CImg<unsigned char> segment::toGrayScale(CImg<unsigned char> &src)
{
    CImg<unsigned char> gray = CImg<unsigned char>(src.width(), src.height(), 1, 1, 0); //To one channel
    cimg_forXY(src, x, y)
    {
        int b = src(x, y, 0);
        int g = src(x, y, 1);
        int r = src(x, y, 2);

        double newValue = r * 0.2126 + g * 0.7152 + b * 0.0722;
        gray(x, y) = newValue;
    }
    return gray;
}

void segment::kmeans(CImg<unsigned char> &src)
{
    core1.setPosition(src.width() / 2, src.height() / 2);
    core2.setPosition(src.width() - 1, src.height() - 1);
    vector<position> set1;
    vector<position> set2;
    while (true)
    {
        set1.clear();
        set2.clear();
        //获得两个簇
        cimg_forXY(src, x, y)
        {
            int distance1 = abs((int)src(x, y) - (int)src(core1.x, core1.y));
            int distance2 = abs((int)src(x, y) - (int)src(core2.x, core2.y));

            position tmp(x, y);
            if (distance1 <= distance2)
            {
                set1.push_back(tmp);
            }
            else
            {
                set2.push_back(tmp);
            }
        }
        position currentCore1 = getMeans(set1, src);
        position currentCore2 = getMeans(set2, src);
        if (isEqual(currentCore1, core1) && isEqual(currentCore2, core2))
            break;
        core1.setPosition(currentCore1.x, currentCore1.y);
        core2.setPosition(currentCore2.x, currentCore2.y);
    }
    CImg<unsigned char> tmp = CImg<unsigned char>(src.width(), src.height(), 1, 1, 0);
    result = tmp;
    for (int i = 0; i < set1.size(); i++)
    {
        tmp(set1[i].x, set1[i].y) = 255;
    }
    tmp = generate(tmp);
    block = tmp;
    cimg_forXY(tmp, x, y)
    {
        if (tmp(x, y) == 0)
            continue;
        if ((x - 1 >= 0 && y - 1 >= 0 && tmp(x - 1, y - 1) == 0) ||                 //0
            (y - 1 >= 0 && tmp(x, y - 1) == 0) ||                                   //1
            (x + 1 < tmp.width() && y - 1 >= 0 && tmp(x + 1, y - 1) == 0) ||        //2
            (x - 1 >= 0 && tmp(x - 1, y) == 0) ||                                   //3
            (x + 1 < tmp.width() && tmp(x + 1, y) == 0) ||                          //5
            (x - 1 >= 0 && y + 1 < tmp.height() && tmp(x - 1, y + 1) == 0) ||       //6
            (y + 1 < tmp.height() && tmp(x, y + 1) == 0) ||                         //7
            (x + 1 < tmp.width() && y + 1 < tmp.height() && tmp(x + 1, y + 1) == 0) //8
        )
        {
            result(x, y) = 255;
        }
    }
    //result = deleteEdge(result, 20);
}

bool segment::isEqual(position &p1, position &p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}

CImg<unsigned char> segment::generate(CImg<unsigned char> &input)
{
    Hough_pos currentPos(input.width() / 2, input.height() / 2);
    posStack.push(currentPos);
    // DFS ready!
    for (int i = 0; i < input.width(); i++)
    {
        vector<bool> column;
        for (int j = 0; j < input.height(); j++)
        {
            bool flag = false;
            column.push_back(flag);
        }
        isVisted.push_back(column);
    }

    // DFS
    CImg<unsigned char> afterGenerate(input.width(), input.height(), 1, 1, 0);
    while (!posStack.empty())
    {
        Hough_pos currentPos = posStack.top();
        posStack.pop();
        if (isVisted[currentPos.x][currentPos.y])
            continue;
        isVisted[currentPos.x][currentPos.y] = true;
        afterGenerate(currentPos.x, currentPos.y) = 255;
        for (int i = currentPos.x - 1; i < currentPos.x + 2; i++)
        {
            for (int j = currentPos.y - 1; j < currentPos.y + 2; j++)
            {
                if (i >= 0 && i < input.width() && j >= 0 && j < input.height())
                {
                    if (i == currentPos.x && j == currentPos.y)
                        continue;
                    if (input(i, j) == 255)
                    {
                        Hough_pos nextPos(i, j);
                        posStack.push(nextPos);
                    }
                }
            }
        }
    }
    return afterGenerate;
}

position segment::getMeans(vector<position> &set, CImg<unsigned char> &src)
{
    float means = 0;
    // get the mean of RGB channels
    for (int i = 0; i < set.size(); i++)
    {
        means += (float)src(set[i].x, set[i].y);
    }

    means /= (float)set.size();
    float minDistance = 10000;
    position core;
    for (int i = 0; i < set.size(); ++i)
    {
        float distance = abs((float)src(set[i].x, set[i].y, 0) - means);
        if (distance < minDistance)
        {
            core.setPosition(set[i].x, set[i].y);
            minDistance = distance;
        }
    }
    return core;
}