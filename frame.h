#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include "pixel.h"
#include <QJsonArray>

class Frame
{
private:
    /**
     * A dynamic array that holds all the pixels.
     * @brief pixels 2D array of pixels
     */
    std::vector<std::vector<Pixel>> pixels;

public:
    /**
     * Basic frame constructor
     * @brief Frame constructs a frame.
     */
    Frame();
    /**
     * Sets a pixel in the pixel array to a specified color.
     * @brief setPixel Pixel setter
     * @param x The X value location of the pixel
     * @param y The Y value location of the pixel
     * @param p The color to set the pixel as
     */
    void setPixel(int x, int y, Pixel p);
    /**
     * Gets a pixel at a certain location.
     * @brief getPixel Pixel getter
     * @param x The X value of the pixel
     * @param y The Y value of the pixel
     * @return the Pixel at the specified location
     */
    Pixel getPixel(int x, int y);
    /**
     * Clears the frame, which sets all pixels to default state (0,0,0,0)
     * @brief clear clears the frame
     */
    void clear();
    /**
     * Replaces all instances of a specified color with another.
     * @brief replaceColor Replaces a color on the frame
     * @param oldColor the color you want replaced
     * @param newColor the color to replace it with
     */
    void replaceColor(Pixel oldColor, Pixel newColor);
    /**
     * Resizes the frame to a certain size.
     * @brief resize resizes the frame
     * @param width the new width of the frame
     * @param height the new height of the frame
     */
    void resize(int width, int height);

    /**
     * Returns a read-only version of the 2D vector of pixels.
     * @brief getPixels returns unmodifiable pixels
     * @return 2D vector of pixels
     */
    const std::vector<std::vector<Pixel>>& getPixels() const;
};

#endif // FRAME_H
