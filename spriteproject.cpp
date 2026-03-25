#include "spriteproject.h"

SpriteProject::SpriteProject() {


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

}
void SpriteProject::resize(int width, int height){

}
void SpriteProject::replaceAllFramesColor(Pixel oldColor, Pixel newColor){

}
void SpriteProject::save(string filename){

}
void SpriteProject::load(string filename){

}

void SpriteProject::undo(){

}

void SpriteProject::redo(){

}
