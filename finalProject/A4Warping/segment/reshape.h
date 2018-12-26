//
// Created by Chen Sihang on 2018/12/21.
//

#ifndef SEGMENT_RESHAPE_H
#define SEGMENT_RESHAPE_H
#include "public.h"
#include "Projection.h"
class reshape {
public:
    static bool isLegal(int, int, int, int, double, double&, double&);

    static void bilinear(CImg<unsigned char>&, CImg<unsigned char>&, double);

    static CImg<unsigned char> reotate_biliinar(CImg<unsigned char>&, double, bool);
};


#endif //SEGMENT_RESHAPE_H
