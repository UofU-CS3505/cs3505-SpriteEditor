#ifndef SPRITEPROJECT_H
#define SPRITEPROJECT_H

#include <QObject>
#include "frame.h"
#include "pixel.h"
#include <vector>
#include <stack>
#include <string>
using std::string;
using std::stack;
using std::vector;

class SpriteProject : public QObject
{
    Q_OBJECT

private:
    int width, height, fps, currentFrameIndex;
    vector<Frame> frames;
    Pixel currentColor;
    stack<Frame> undoStack;
    stack<Frame> redoStack;

    QJsonArray pixelToJSON(const Pixel &p) const;
    Pixel JSONToPixel(QJsonArray& array);
    QJsonArray frameToJSON(const Frame &frame) const;
    Frame JSONToFrame(QJsonArray& array);


    QJsonObject toJson() const;

public:
    SpriteProject(int width, int height, int fps, QObject* parent = nullptr);
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    // getter and setter for Frames per Second
    void setFps(int fps) {this->fps = fps; }
    int getFps() const { return fps; }

    /**
     * Adds a new frame after the currentFrameIndex
     * @brief addFrame creates a new frame
     */
    void addFrame();
    /**
     * Deletes the frame that is at the currentFrameIndex.
     * @brief deleteFrame deletes a frame
     */
    void deleteFrame();
    /**
     * Gets the frame at a specified index
     * @brief getFrame gets a specified frame
     * @param index the index of the frame you want
     * @return the frame reference
     */
    Frame& getFrame(int index);
    /**
     * Gets the frame that is at the currentFrameIndex.
     * @brief getCurrentFrame gets the current frame
     * @return the current frame reference
     */
    Frame& getCurrentFrame();
    /**
     * Sets a pixel in the pixel array of the current frame to a specified color.
     * @brief setPixel Pixel setter
     * @param x The X value location of the pixel on the current frame
     * @param y The Y value location of the pixel on the current frame
     * @param p The color to set the pixel as
     */
    void setPixel(int x, int y, Pixel p);
    /**
     * Erases a pixel in the pixel array
     * @brief erasePixel Pixel eraser
     * @param x The X value location of the pixel on the current frame
     * @param y The Y value location of the pixel on the current frame
     */
    void erasePixel(int x, int y);
    /**
     * Resizes all frames to a specified size.
     * @brief resize resizes all frames
     * @param width the new width of the project
     * @param height the new height of the project
     */
    void resize(int width, int height);
    /**
     * TODO
     * @brief replaceAllFramesColor
     * @param oldColor
     * @param newColor
     */
    void replaceAllFramesColor(Pixel oldColor, Pixel newColor);
    /**
     * Saves the spriteProject as a JSON to a specified file.
     * @brief save saves the spriteProject
     * @param filename the file name
     */
    void save(const QString filename);
    /**
     * Loads the project from a specified file name.
     * @brief load loads the spriteProject
     * @param filename the file that contains the spriteProjectJSON
     */
    void load(const QString filename);
    /**
     * Undos an action such as setPixel.
     * @brief undo undos an action
     */
    void undo();
    /**
     * Redos an action such as setPixel.
     * @brief redos an action
     */
    void redo();
    /**
     * Saves the current frame state to push onto the undoStack.
     * @brief saves frame state
     */
    void saveCurrentFrameState();
    /**
     * Gets the index of the current frame.
     * @brief gets the current frame index
     * @return index number
     */
    int getCurrentFrameIndex();
    /**
     * Updated the frame index number to the desired index.
     * @brief Sets the current frame index
     * @param index
     * @param previewOn If preview is on, prevents stacks from being messed with
     */
    void setCurrentFrameIndex(int index, bool previewOn = false);
    /**
     * Clears the canvas of the selected frame.
     * @brief clears the current frame
     */
    void clearCurrentFrame();

signals:
    void frameChanged();      // emit when any pixel on current frame changed
    void frameListChanged();  // emit when frame added or deleted
    void projectLoaded();     // emit when load() finished, view should fully rebuild
};

#endif // SPRITEPROJECT_H
