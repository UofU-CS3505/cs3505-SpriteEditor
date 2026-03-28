#ifndef PIXEL_H
#define PIXEL_H
#include <QJsonArray>

class Pixel
{
private:
    int red, green, blue, alpha;

public:
    /**
     * Generates a Pixel with 0 in all values.
     * @brief Pixel Basic constructor for pixel class.
     */
    Pixel();
    /**
     * Generates a pixel with specified values.
     * @brief Pixel Constructor for pixel class
     * @param r red value
     * @param g green value
     * @param b blue value
     * @param a alpha value
     */
    Pixel(int r, int g, int b, int a);
    /**
     * @brief getR getter for Red variable
     * @return returns the red variable of the Pixel class
     */
    int getR() const;
    /**
     * @brief getG getter for Green variable
     * @return returns the Green variable of the Pixel class
     */
    int getG() const;
    /**
     * @brief getB getter for Blue variable
     * @return returns the Blue variable of the Pixel class
     */
    int getB() const;
    /**
     * @brief getA getter for Alpha variable
     * @return returns the alpha (transparency) variable of the Pixel class
     */
    int getA() const;
    /**
     * Sets the values of the pixel.
     * @brief setRGBA complete setter of Pixel class
     * @param r red value
     * @param g green value
     * @param b blue value
     * @param a alpha value
     */
    void setRGBA(int r, int g, int b, int a);
    /**
     * Sets the alpha (opacity) of the pixel.
     * @brief setAlpha basic setter of pixel class
     * @param a the alpha value, or opacity
     */
    void setAlpha(int a);
    /**
     * Checks whether this pixel is equal to another in terms of color.
     * Opacity is not considered.
     * @brief equals equality checker for pixel color values
     * @param other the other pixel to compare to this one
     * @return whether this pixel has all the same color values
     */
    bool equals(Pixel other);
};

#endif // PIXEL_H
