#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include "pixel.h"

class Frame
{
private:
    /**
     * A dynamic array that keeps
     * @brief pixels
     */
    std::vector<std::vector<Pixel>> pixels;

public:
    Frame();
    void setPixel(int x, int y, Pixel p);
    Pixel getPixel(int x, int y);
    void clear();
    void replaceColor(Pixel oldColor, Pixel newColor);
};

#endif // FRAME_H
