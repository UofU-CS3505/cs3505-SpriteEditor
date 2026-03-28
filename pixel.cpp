#include "pixel.h"
#include <QJsonArray>

Pixel::Pixel() {
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
}

Pixel::Pixel(int r, int g, int b, int a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

int Pixel::getR() const{
    return red;
}

int Pixel::getG() const{
    return green;
}

int Pixel::getB() const{
    return blue;
}

int Pixel::getA() const{
    return alpha;
}

void Pixel::setRGBA(int r, int g, int b, int a){
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

void Pixel::setAlpha(int a){
    alpha = a;
}

bool Pixel::equals(Pixel other){
    return (other.red == red && other.blue == blue && other.green == green);
}
