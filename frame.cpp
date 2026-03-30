/**
 * Implementation file for the Frame class. Defines the functionality
 * for managing a pixel grid.
 *
 * @authors: Helen Zhang, Emily Wang, Aspen Tobler, Xinmiao Li, and Shaoze Yang
 * Class: CS 3505 Assignment 8
 * Last modified: March 30, 2026
 *
 * Checked by Aspen Tobler
 */

#include "frame.h"

Frame::Frame() {}

void Frame::setPixel(int x, int y, Pixel p) {
    // Ignore invalid coordinates so drawing/erasing stays inside the frame
    if (x < 0 || y < 0) {
        return;
    }
    if (x >= static_cast<int>(pixels.size())) {
        return;
    }
    if (y >= static_cast<int>(pixels[x].size())) {
        return;
    }

    // Update the target pixel with the new RGBA values
    pixels[x][y].setRGBA(p.getR(), p.getG(), p.getB(), p.getA());
}

Pixel Frame::getPixel(int x, int y) {
    // Return a default transparent pixel if the coordinates are out of bounds
    if (x < 0 || y < 0) {
        return Pixel();
    }
    if (x >= static_cast<int>(pixels.size())) {
        return Pixel();
    }
    if (y >= static_cast<int>(pixels[x].size())) {
        return Pixel();
    }

    return pixels[x][y];
}

void Frame::clear() {
    for(auto& row : pixels) {
        for(auto& pixel : row) {
            pixel.setRGBA(0,0,0,0);
        }
    }
}

void Frame::replaceColor(Pixel oldColor, Pixel newColor) {
    for(auto& row : pixels) {
        for(auto& pixel : row) {
            if(pixel.equals(oldColor)) {
                pixel.setRGBA(newColor.getR(), newColor.getG(), newColor.getB(), pixel.getA());
            }
        }
    }
}

void Frame::resize(int width, int height) {
    pixels.resize(width);
    for(auto& row : pixels) {
        row.resize(height);
    }
}

const std::vector<std::vector<Pixel>>& Frame::getPixels() const {
    return pixels;
}
