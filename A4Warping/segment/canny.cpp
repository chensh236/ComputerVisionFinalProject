#include "canny.h"
#define _USE_MATH_DEFINES
using namespace std;

// canny::canny(CImg<unsigned char>& input, int tlow, int thigh)
// {
//     gFiltered = input;
//     gFiltered = gFiltered.get_blur(0.5);
//     sFiltered = sobel();                       //Sobel Filter
//     non = nonMaxSupp();                        //Non-Maxima Suppression
//     result = threshold(non, tlow, thigh);            //Double Threshold and Finalize 20 40
//     //result = newFunc(result, 5);
//     cimg_forXY(result, x, y){
//         if(x <= BOUNDER || y <= BOUNDER || x >= result.width() - BOUNDER || y >= result.height() - BOUNDER){
//             result(x, y) = 0;
//         }
//     }
// }

// CImg<unsigned char> canny::getResult(){return result;}

// CImg<unsigned char> canny::sobel()
// {

//     //Sobel X Filter
//     double x1[] = {-1.0, 0, 1.0};
//     double x2[] = {-2.0, 0, 2.0};
//     double x3[] = {-1.0, 0, 1.0};

//     vector<vector<double>> xFilter(3);
//     xFilter[0].assign(x1, x1 + 3);
//     xFilter[1].assign(x2, x2 + 3);
//     xFilter[2].assign(x3, x3 + 3);

//     //Sobel Y Filter
//     double y1[] = {1.0, 2.0, 1.0};
//     double y2[] = {0, 0, 0};
//     double y3[] = {-1.0, -2.0, -1.0};

//     vector<vector<double>> yFilter(3);
//     yFilter[0].assign(y1, y1 + 3);
//     yFilter[1].assign(y2, y2 + 3);
//     yFilter[2].assign(y3, y3 + 3);

//     //Limit Size
//     int size = (int)xFilter.size() / 2;

//     CImg<unsigned char> filteredImg = CImg<unsigned char>(gFiltered.width() - 2 * size, gFiltered.height() - 2 * size, 1, 1, 0);

//     angles = CImg<unsigned char>(gFiltered.width() - 2 * size, gFiltered.height() - 2 * size, 1, 1, 0); //AngleMap

//     for (int i = size; i < gFiltered.width() - size; i++)
//     {
//         for (int j = size; j < gFiltered.height() - size; j++)
//         {
//             double sumx = 0;
//             double sumy = 0;

//             for (int x = 0; x < xFilter.size(); x++)
//                 for (int y = 0; y < xFilter.size(); y++)
//                 {
//                     sumx += xFilter[x][y] * (double)(gFiltered(i + x - size, j + y - size)); //Sobel_X Filter Value
//                     sumy += yFilter[x][y] * (double)(gFiltered(i + x - size, j + y - size)); //Sobel_Y Filter Value
//                 }
//             double sumxsq = sumx * sumx;
//             double sumysq = sumy * sumy;

//             double sq2 = sqrt(sumxsq + sumysq);

//             if (sq2 > 255) //Unsigned Char Fix
//                 sq2 = 255;
//             filteredImg(i - size, j - size) = sq2;

//             if (sumx == 0) //Arctan Fix
//                 angles(i - size, j - size) = 90;
//             else
//                 angles(i - size, j - size) = atan(sumy / sumx);
//         }
//     }

//     return filteredImg;
// }

// CImg<unsigned char> canny::nonMaxSupp()
// {
//     CImg<unsigned char> nonMaxSupped = CImg<unsigned char>(sFiltered.width() - 2, sFiltered.height() - 2, 1, 1, 0);
//     for (int i = 1; i < sFiltered.width() - 1; i++)
//     {
//         for (int j = 1; j < sFiltered.height() - 1; j++)
//         {
//             float Tangent = angles(i, j);

//             nonMaxSupped(i - 1, j - 1) = sFiltered(i, j);
//             //Horizontal Edge
//             if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
//             {
//                 if ((sFiltered(i, j) < sFiltered(i, j + 1)) || (sFiltered(i, j) < sFiltered(i, j - 1)))
//                     nonMaxSupped(i - 1, j - 1) = 0;
//             }
//             //Vertical Edge
//             if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
//             {
//                 if ((sFiltered(i, j) < sFiltered(i + 1, j)) || (sFiltered(i, j) < sFiltered(i - 1, j)))
//                     nonMaxSupped(i - 1, j - 1) = 0;
//             }

//             //-45 Degree Edge
//             if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
//             {
//                 if ((sFiltered(i, j) < sFiltered(i - 1, j + 1)) || (sFiltered(i, j) < sFiltered(i + 1, j - 1)))
//                     nonMaxSupped(i - 1, j - 1) = 0;
//             }

//             //45 Degree Edge
//             if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
//             {
//                 if ((sFiltered(i, j) < sFiltered(i + 1, j + 1)) || (sFiltered(i, j) < sFiltered(i - 1, j - 1)))
//                     nonMaxSupped(i - 1, j - 1) = 0;
//             }
//         }
//     }
//     return nonMaxSupped;
// }

// CImg<unsigned char> canny::threshold(CImg<unsigned char> imgin, int low, int high)
// {
//     if (low > 255)
//         low = 255;
//     if (high > 255)
//         high = 255;

//     CImg<unsigned char> EdgeMat = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 1, 0);
//     cimg_forXY(imgin, i, j)
//     {
//         EdgeMat(i, j) = imgin(i, j);
//         if (EdgeMat(i, j) > high)
//             EdgeMat(i, j) = 255;
//         else if (EdgeMat(i, j) < low)
//             EdgeMat(i, j) = 0;
//         else
//         {
//             bool anyHigh = false;
//             bool anyBetween = false;
//             for (int x = i - 1; x < i + 2; x++)
//             {
//                 for (int y = j - 1; y < j + 2; y++)
//                 {
//                     if (x <= 0 || y <= 0 || EdgeMat.width() || y > EdgeMat.height()) //Out of bounds
//                         continue;
//                     else
//                     {
//                         if (EdgeMat(x, y) > high)
//                         {
//                             EdgeMat(i, j) = 255;
//                             anyHigh = true;
//                             break;
//                         }
//                         else if (EdgeMat(x, y) <= high && EdgeMat(x, y) >= low)
//                             anyBetween = true;
//                     }
//                 }
//                 if (anyHigh)
//                     break;
//             }
//             if (!anyHigh && anyBetween)
//                 for (int x = i - 2; x < i + 3; x++)
//                 {
//                     for (int y = j - 1; y < j + 3; y++)
//                     {
//                         if (x < 0 || y < 0 || x > EdgeMat.width() || y > EdgeMat.height()) //Out of bounds
//                             continue;
//                         else
//                         {
//                             if (EdgeMat(x, y) > high)
//                             {
//                                 EdgeMat(i, j) = 255;
//                                 anyHigh = true;
//                                 break;
//                             }
//                         }
//                     }
//                     if (anyHigh)
//                         break;
//                 }
//             if (!anyHigh)
//                 EdgeMat(i, j) = 0;
//         }
//     }
//     return EdgeMat;
// }

CImg<unsigned char> canny::newFunc(CImg<unsigned char> imgin, int length)
{
    CImg<unsigned char> output = CImg<unsigned char>(imgin.width(), imgin.height(), 1, 1, 0);
    stack<Hough_pos> s;
    queue<Hough_pos> q;
    cimg_forXY(imgin, x, y){
        // DFS begin
        if(imgin(x, y) == 255){
            s.push(Hough_pos(x, y));
            q.push(Hough_pos(x, y));
            imgin(x ,y) = 0;
            while(!s.empty()){
                Hough_pos p = s.top();
                s.pop();
                // search in 8 different direcvtions
                if(p.x - 1 > 0 && p.y - 1 > 0 && imgin(p.x - 1, p.y - 1) == 255){
                    Hough_pos np = Hough_pos(p.x - 1, p.y - 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x - 1, p.y - 1) = 0;
                }

                if(p.y - 1 > 0 && imgin(p.x, p.y - 1) == 255){
                    Hough_pos np = Hough_pos(p.x, p.y - 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x, p.y - 1) = 0;
                }

                if(p.x + 1 < imgin.width() && p.y - 1 > 0 && imgin(p.x + 1, p.y - 1) == 255){
                    Hough_pos np = Hough_pos(p.x + 1, p.y - 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x + 1, p.y - 1) = 0;
                }

                if(p.x - 1 > 0 && imgin(p.x - 1, p.y) == 255){
                    Hough_pos np = Hough_pos(p.x - 1, p.y);
                    s.push(np);
                    q.push(np);
                    imgin(p.x - 1, p.y) = 0;
                }

                if(p.x + 1 < imgin.width() && imgin(p.x + 1, p.y) == 255){
                    Hough_pos np = Hough_pos(p.x + 1, p.y);
                    s.push(np);
                    q.push(np);
                    imgin(p.x + 1, p.y) = 0;
                }

                if(p.x - 1 > 0 && p.y + 1 < imgin.height() && imgin(p.x - 1, p.y + 1) == 255){
                    Hough_pos np = Hough_pos(p.x - 1, p.y + 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x - 1, p.y + 1) = 0;
                }

                if(p.y + 1 < imgin.height() && imgin(p.x, p.y + 1) == 255){
                    Hough_pos np = Hough_pos(p.x, p.y + 1);
                    s.push(np);
                    q.push(np);
                    imgin(p.x, p.y + 1) = 0;
                }

                if(p.x + 1 < imgin.width() && p.y + 1 < imgin.height() && imgin(p.x + 1, p.y + 1) == 255){
                    Hough_pos np = Hough_pos(p.x + 1, p.y + 1);
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
                    Hough_pos p = q.front();
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