#ifndef _CANNY_
#define _CANNY_
#include "public.h"

using namespace std;
using namespace cimg_library;

class canny {
private:

    CImg<unsigned char> gFiltered; // Gradient
    CImg<unsigned char> sFiltered; //Sobel Filtered
    CImg<unsigned char> angles; //Angle Map
    CImg<unsigned char> non; // Non-maxima supp.
    CImg<unsigned char> result; // The result of the new function

public:
    canny(CImg<unsigned char>&, int, int);

    CImg<unsigned char> sobel(); //Sobel filtering
    CImg<unsigned char> nonMaxSupp(); //Non-maxima supp.
    CImg<unsigned char> threshold(CImg<unsigned char>, int, int); //Double threshold and finalize picture
    CImg<unsigned char> getResult();
    static CImg<unsigned char> newFunc(CImg<unsigned char> imgin, int length);
};

#endif