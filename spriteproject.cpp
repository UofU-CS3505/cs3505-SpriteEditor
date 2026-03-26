#include "spriteproject.h"

SpriteProject::SpriteProject(int width, int height, int fps, QObject* parent)
    : QObject(parent)
    , width(width)
    , height(height)
    , fps(fps)
    , currentFrameIndex(0)
{
    Frame f;
    f.resize(width, height);
    frames.push_back(f);
}

void SpriteProject::addFrame(){

}

void SpriteProject::deleteFrame(){

}

Frame& SpriteProject::getFrame(int index){
    return frames[index];
}

Frame& SpriteProject::getCurrentFrame(){
    return frames[currentFrameIndex];
}

void SpriteProject::setPixel(int x, int y, Pixel p){
    frames[currentFrameIndex].setPixel(x, y, p);
    emit frameChanged();
}

void SpriteProject::erasePixel(int x, int y){
    Pixel transparent(0, 0, 0, 0);
    frames[currentFrameIndex].setPixel(x, y, transparent);
    emit frameChanged();
}

void SpriteProject::resize(int width, int height){
    this->width = width;
    this->height = height;
    for(auto& frame : frames){
        frame.resize(width, height);
    }
}

void SpriteProject::replaceAllFramesColor(Pixel oldColor, Pixel newColor){
    for(auto& frame : frames){
        frame.replaceColor(oldColor, newColor);
    }
}

void SpriteProject::save(string filename){
    //TODO
}
void SpriteProject::load(string filename){
    //TODO
}

void SpriteProject::undo(){
    //TODO
}

void SpriteProject::redo(){
    //TODO
}
