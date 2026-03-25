#include "frame.h"

Frame::Frame() {}

void Frame::setPixel(int x, int y, Pixel p){
    pixels[x][y].setRGBA(p.getR(), p.getG(), p.getB(), p.getA());
}

Pixel Frame::getPixel(int x, int y){
    return pixels[x][y];
}

void Frame::clear(){
    for(auto& row : pixels){
        for(auto& pixel : row){
            pixel.setRGBA(0,0,0,0);
        }
    }
}

void Frame::replaceColor(Pixel oldColor, Pixel newColor){
    for(auto& row : pixels){
        for(auto& pixel : row){
            if(pixel.equals(oldColor)){
                pixel.setRGBA(newColor.getR(), newColor.getG(), newColor.getB(), pixel.getA());
            }
        }
    }
}

void Frame::resize(int width, int height){
    pixels.resize(width);
    for(auto& row : pixels){
        row.resize(height);
    }
}
