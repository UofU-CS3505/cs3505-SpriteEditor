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
    Frame newFrame;
    newFrame.resize(width, height);
    int insertFrameIndex = currentFrameIndex + 1;
    frames.insert(frames.begin() + insertFrameIndex, newFrame);

    currentFrameIndex = insertFrameIndex;

    emit frameListChanged();
    emit frameChanged();
}

void SpriteProject::deleteFrame(){
    if (frames.size() <= 1) return;

    frames.erase(frames.begin() + currentFrameIndex);

    if (currentFrameIndex >= (int) frames.size()) {
        currentFrameIndex = frames.size() - 1;
    }

    emit frameListChanged();
    emit frameChanged();
}

Frame& SpriteProject::getFrame(int index){
    return frames[index];
}

Frame& SpriteProject::getCurrentFrame(){
    return frames[currentFrameIndex];
}

void SpriteProject::setPixel(int x, int y, Pixel p){
    undoStack.push(getCurrentFrame());
    // clear stack whenever the frame changes
    while (!redoStack.empty()) {
        redoStack.pop();
    }

    frames[currentFrameIndex].setPixel(x, y, p);
    emit frameChanged();
}

void SpriteProject::erasePixel(int x, int y){
    undoStack.push(getCurrentFrame());
    // clear stack whenever the frame changes
    while (!redoStack.empty()) {
        redoStack.pop();
    }

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
    if (undoStack.empty()) return;

    redoStack.push(getCurrentFrame());
    frames[currentFrameIndex] = undoStack.top();
    undoStack.pop();

    emit frameChanged();
}

void SpriteProject::redo(){
    if (redoStack.empty()) return;

    undoStack.push(getCurrentFrame());
    frames[currentFrameIndex] = redoStack.top();
    redoStack.pop();

    emit frameChanged();
}
