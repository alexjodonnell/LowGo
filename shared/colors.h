//
// Created by Alex O'Donnell on 2018-03-26.
//

#ifndef LOWGO_COLOURS_H
#define LOWGO_COLOURS_H

#include "TGAImage.h"

class colors{
public:
    const TGAColor white    = TGAColor(255, 255, 255, 255);
    const TGAColor black    = TGAColor(  0,   0,   0, 255);
    const TGAColor red      = TGAColor(255,   0,   0, 255);
    const TGAColor green    = TGAColor(  0, 255,   0, 255);
    const TGAColor blue     = TGAColor(  0,   0, 255, 255);
    const TGAColor cyan     = TGAColor(  0, 255, 255, 255);
    const TGAColor yellow   = TGAColor(255, 255,   0, 255);
    const TGAColor magenta  = TGAColor(255,   0, 255, 255);
    const TGAColor forest   = TGAColor( 34, 139,  34, 255);
    const TGAColor charcoal = TGAColor( 34,  34,  34, 255);
};
#endif //LOWGO_COLOURS_H
