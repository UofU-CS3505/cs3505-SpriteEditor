#include "spriteproject.h"
#include <QDebug>
#include <QJsonObject>
#include <QIODevice>
#include <QFile>

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
    return frames.at(index);
}

Frame& SpriteProject::getCurrentFrame(){
    return frames.at(currentFrameIndex);
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

    emit frameChanged();
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

void SpriteProject::saveCurrentFrameState() {
    undoStack.push(getCurrentFrame());

    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

int SpriteProject::getCurrentFrameIndex() {
    return currentFrameIndex;
}

void SpriteProject::clearCurrentFrame() {
    saveCurrentFrameState();

    Frame& frame = frames[currentFrameIndex];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            frame.setPixel(x, y, Pixel(0, 0, 0, 0));
        }
    }

    emit frameChanged();
}

void SpriteProject::setCurrentFrameIndex(int index, bool previewOn) {
    if (index < 0 || index >= (int)frames.size()) return;

    currentFrameIndex = index;

    // clear undo and redo stack when frame
    // is changed to avoid unwanted behavior.
    if (!previewOn) {
        while (!undoStack.empty()) {
            undoStack.pop();
        }

        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }

    emit frameChanged();
}

//////////////////////////////// saving/loading

void SpriteProject::save(const QString filename){
    QJsonDocument doc(toJson());

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing";
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void SpriteProject::load(const QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Parse JSON
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }


    QJsonObject obj = doc.object();
    resize(obj["width"].toInt(), obj["height"].toInt());
    fps = obj["fps"].toInt();
    currentFrameIndex = 0;

    //clears undos, redos, and frames
    std::stack<Frame> clearUndoStack;
    std::stack<Frame> clearRedoStack;
    std::vector<Frame> clearFrames;

    undoStack.swap(clearUndoStack);
    redoStack.swap(clearRedoStack);
    frames.swap(clearFrames);

    // handles all the frames
    QJsonArray frameArray = obj["frames"].toArray();
    for(const auto& frame : frameArray){
        QJsonArray frameObj = frame.toArray();
        frames.push_back(JSONToFrame(frameObj));
    }

    emit frameListChanged();
    emit frameChanged();
}

//////////////////////////////// JSON representations

QJsonArray SpriteProject::pixelToJSON(const Pixel &p) const{
    return QJsonArray{ p.getR(), p.getG(), p.getB(), p.getA() };
}

Pixel SpriteProject::JSONToPixel(QJsonArray& array){
    return Pixel(array[0].toInt(), array[1].toInt(), array[2].toInt(), array[3].toInt());
}

QJsonArray SpriteProject::SpriteProject::frameToJSON(const Frame &frame) const{
    QJsonArray rows;

    for (const auto &row : frame.getPixels()) {
        QJsonArray jsonRow;
        for (const auto &pixel : row) {
            jsonRow.append(pixelToJSON(pixel));
        }
        rows.append(jsonRow);
    }

    return rows;
}

Frame SpriteProject::JSONToFrame(QJsonArray& array){
    Frame frame;
    frame.resize(width, height);

    for(int col = 0; col < width; col++){
        //gets the 'row' of pixels
        QJsonArray rowArray = array[col].toArray();

        for(int rows = 0; rows < height; rows++){
            QJsonArray pixel = rowArray[rows].toArray();

            frame.setPixel(col, rows, JSONToPixel(pixel));
        }
    }

    return frame;
}

QJsonObject SpriteProject::toJson() const{
    QJsonArray frameArray;
    for (const auto &frame : frames) {
        frameArray.append(frameToJSON(frame));
    }

    QJsonObject obj;

    obj["width"] = width;
    obj["height"] = height;
    obj["fps"] = fps;
    obj["frames"] = frameArray;
    return obj;
}
