#include <iostream>

#include "tga/TGAImage.h" // this is a really basic include that pretty much just lets you change a single pixel in a tgaimage
#include "tga/DWGTool.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
int main(int argc, char** argv) {

    TGAImage image(100, 100, TGAImage::RGB);
    TGAColor color = TGAColor(0, 255, 255, 128);

    DWGTool::line(0, 0, 50, 50, image, color);

    DWGTool::line(90, 0, 100, 100, image, color);

    // save our image
    image.write_tga_file("output.tga");
    return 0;
}