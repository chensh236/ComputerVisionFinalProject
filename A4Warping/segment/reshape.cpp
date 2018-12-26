//
// Created by Chen Sihang on 2018/12/21.
//

#include "reshape.h"

// 判断图像边界
bool reshape::isLegal(int x, int y, int srcWidth, int srcHeight, double theta,
                             double &srcX, double &srcY)
{
    // 找到(x, y)在原图中对应的位置(srcX, srcY)
    srcX = (double)x * cos(theta) - (double)y * sin(theta);
    srcY = (double)x * sin(theta) + (double)y * cos(theta);
    if (srcX >= (0 - srcWidth / 2 - 1) && srcX <= srcWidth / 2 + 1 && srcY >= (0 - srcHeight / 2 - 1) && srcY <= srcHeight / 2 + 1)
    {
        srcX += srcWidth / 2;
        srcY += srcHeight / 2;
        return true;
    }
    else
    {
        return false;
    }
}


void reshape::bilinear(CImg<unsigned char> &src, CImg<unsigned char> &output, double theta)
{
    cimg_forXY(output, x, y)
    {
        double src_x, src_y;
        // 判断边界合法性
        if (isLegal(x - output.width() / 2, y - output.height() / 2, src.width(), src.height(), theta, src_x, src_y))
        {
            src_x = (src_x >= (double)src.width()) ? ((double)src.width() - 1.0) : (src_x < 0.0) ? 0.0 : src_x;
            src_y = (src_y >= (double)src.height()) ? ((double)src.height() - 1.0) : (src_y < 0.0) ? 0.0 : src_y;
            // 双线性插值
            output(x, y) = Projection::singleBilinearInterpolation(src, src_x, src_y, 0);
        }
    }
}

CImg<unsigned char> reshape::reotate_biliinar(CImg<unsigned char> &src, double theta, bool isBinary)
{
    // 获得图像旋转中心
    Hough_pos lt(0 - src.width() / 2, 0 + src.height() / 2), lb(0 - src.width() / 2, 0 - src.height() / 2),
        rt(0 + src.width() / 2, 0 + src.height() / 2), rb(0 + src.width() / 2, 0 - src.height() / 2);

    // 获得旋转后坐标
    Hough_pos new_lt((int)(lt.x * cos(theta) + lt.y * sin(theta)), (int)(lt.y * cos(theta) - lt.x * sin(theta))),
        new_lb((int)(lb.x * cos(theta) + lb.y * sin(theta)), (int)(lb.y * cos(theta) - lb.x * sin(theta))),
        new_rt((int)(rt.x * cos(theta) + rt.y * sin(theta)), (int)(rt.y * cos(theta) - rt.x * sin(theta))),
        new_rb((int)(rb.x * cos(theta) + rb.y * sin(theta)), (int)(rb.y * cos(theta) - rb.x * sin(theta)));

    int width = abs(new_rt.x - new_lb.x) > abs(new_lt.x - new_rb.x) ? abs(new_rt.x - new_lb.x) : abs(new_lt.x - new_rb.x);
    int height = abs(new_lt.y - new_rb.y) > abs(new_lb.y - new_rt.y) ? abs(new_lt.y - new_rb.y) : abs(new_lb.y - new_rt.y);
    CImg<unsigned char> output(width, height, 1, 1, 255);
    // 开始填充新图片的灰度值
    bilinear(src, output, theta);

    // 如果是二值化图，进行处理，否则不进行处理
    if (isBinary)
    {
        cimg_forXY(output, x, y)
        {
            if (output(x, y) > 200)
                output(x, y) = 255;
            else
                output(x, y) = 0;
        }
    }

    return output;
}