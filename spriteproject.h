#ifndef SPRITEPROJECT_H
#define SPRITEPROJECT_H
#include "frame.h"
#include "pixel.h"
#include <vector>
#include <stack>
#include <string>
using std::string;
using std::stack;
using std::vector;


class SpriteProject
{
private:
    int width, height, fps, currentFrameIndex;
    vector<Frame> frames;
    Pixel currentColor;
    stack<Frame> undoStack;
    stack<Frame> redoStack;
public:
    SpriteProject();
    void addFrame();
    void deleteFrame();
    Frame& getFrame(int index);
    Frame& getCurrentFrame();
    void setPixel(int x, int y, Pixel p);
    void resize(int width, int height);
    void replaceAllFramesColor(Pixel oldColor, Pixel newColor);
    void save(string filename);
    void load(string filename);
    void undo();
    void redo();
};

#endif // SPRITEPROJECT_H
