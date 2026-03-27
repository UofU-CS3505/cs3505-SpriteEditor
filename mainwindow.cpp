#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , project(nullptr)
    , currentTool(ToolMode::Draw)
    , currentColor(Qt::black)
    , strokeSize(4)
    , opacity(255)
    , isMouseDown(false)
{
    ui->setupUi(this);

    QIcon icon1(":/new/prefix/Eraser.png");
    ui->eraserButton->setIcon(icon1);
    QIcon icon2(":/new/prefix/Brush.png");
    ui->brushButton->setIcon(icon2);
    QIcon icon3(":/new/prefix/Pen.png");
    ui->penButton->setIcon(icon3);
    QIcon icon4(":/new/prefix/undo.png");
    ui->undoButton->setIcon(icon4);
    QIcon icon5(":/new/prefix/redo.png");
    ui->redoButton->setIcon(icon5);
    QIcon icon6(":/new/prefix/clear.png");
    ui->clearButton->setIcon(icon6);
    QIcon icon7(":/new/prefix/save.png");
    ui->saveButton->setIcon(icon7);
    QIcon icon8(":/new/prefix/custom_color.png");
    ui->colorButtonCustom->setIcon(icon8);
    QIcon icon9(":/new/prefix/new_project.png");
    ui->newProjectButton->setIcon(icon9);

    ui->penButton->setIconSize(QSize(120, 120));
    ui->brushButton->setIconSize(QSize(120, 120));
    ui->eraserButton->setIconSize(QSize(120, 120));
    ui->undoButton->setIconSize(QSize(30, 30));
    ui->redoButton->setIconSize(QSize(30, 30));
    ui->clearButton->setIconSize(QSize(30, 30));
    ui->saveButton->setIconSize(QSize(20, 20));
    ui->colorButtonCustom->setIconSize(QSize(20, 20));
    ui->newProjectButton->setIconSize(QSize(16, 16));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete project;
}

//call everytime when creating a project, connects signals(model) and slots(view)
void MainWindow::connectProject()
{
    // model to view
    connect(project, &SpriteProject::frameChanged,     this, &MainWindow::refreshCanvas);
    connect(project, &SpriteProject::frameListChanged,  this, &MainWindow::refreshFrameList);
    connect(project, &SpriteProject::projectLoaded,    this, &MainWindow::onProjectLoaded);
}

void MainWindow::setDrawMode()
{
    currentTool = ToolMode::Draw;
}

void MainWindow::setEraseMode()
{
    currentTool = ToolMode::Erase;
}

//----------------mouse event-------------------
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    //TODO: if left button clicked inside the canvas, forward to onMousePressed
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    //TODO: if left button is held inside the canvas, forward to onMouseDragged
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    isMouseDown = false;
    QMainWindow::mouseReleaseEvent(event);
}

//-------------mouse handler----------
void MainWindow::onMousePressed(int x, int y)
{
    isMouseDown = true;

    if (currentTool == ToolMode::Erase) {
        erasePixels(x, y);
        return;
    }

    Pixel brushPixel(
        currentColor.red(),
        currentColor.green(),
        currentColor.blue(),
        opacity
        );

    changePixel(x, y, brushPixel);
}

void MainWindow::onMouseDragged(int x, int y)
{
    if (!isMouseDown) {
        return;
    }

    if (currentTool == ToolMode::Erase) {
        erasePixels(x, y);
        return;
    }

    Pixel brushPixel(
        currentColor.red(),
        currentColor.green(),
        currentColor.blue(),
        opacity
        );

    changePixel(x, y, brushPixel);
}

//----------------For coordinates, convert canvas widget pixel to sprite grid pixel
int MainWindow::toSpriteX(int widgetX)
{
    //TODO
    return widgetX;
}

int MainWindow::toSpriteY(int widgetY)
{
    //TODO
    return widgetY;
}

void MainWindow::changePixel(int x, int y, Pixel p)
{
    if (project == nullptr) {
        return;
    }

    int size = strokeSize;
    if (size < 1) {
        size = 1;
    }

    int start = -(size / 2);
    int end = start + size - 1;

    for (int dx = start; dx <= end; dx++) {
        for (int dy = start; dy <= end; dy++) {
            project->setPixel(x + dx, y + dy, p);
        }
    }
}

void MainWindow::erasePixels(int x, int y)
{
    if (project == nullptr) {
        return;
    }

    int size = strokeSize;
    if (size < 1) {
        size = 1;
    }

    int start = -(size / 2);
    int end = start + size - 1;

    for (int dx = start; dx <= end; dx++) {
        for (int dy = start; dy <= end; dy++) {
            project->erasePixel(x + dx, y + dy);
        }
    }
}

void MainWindow::changeBrushColor(int r, int g, int b)
{
    currentColor.setRgb(r, g, b, opacity);
}

void MainWindow::changeBrushOpacity(int a)
{
    if (a < 0) {
        a = 0;
    }
    if (a > 255) {
        a = 255;
    }

    opacity = a;
    currentColor.setAlpha(opacity);
}

//---------------helpers for connectProject(), connected to modal signals
void MainWindow::refreshCanvas()
{
    //TODO: draw all pixels from current frame to the canvas widget
}

void MainWindow::refreshFrameList()
{
    //TODO: rebuild to list inside the side pannel
}

void MainWindow::onProjectLoaded()
{
    refreshFrameList();
    refreshCanvas();
}

//----------------project manage button slots-------------
void MainWindow::on_newProjectButton_clicked()
{

}

void MainWindow::on_loadButton_clicked()
{

}

void MainWindow::on_saveButton_clicked()
{

}

void MainWindow::on_exportButton_clicked()
{

}

//-----------color button slots--------------
void MainWindow::on_colorButton1_clicked() {
    changeBrushColor(0, 0, 0);
    setDrawMode();
}
void MainWindow::on_colorButton2_clicked() {
    changeBrushColor(255, 0, 0);
    setDrawMode();
}
void MainWindow::on_colorButton3_clicked() {
    changeBrushColor(0, 255, 0);
    setDrawMode();
}
void MainWindow::on_colorButton4_clicked() {
    changeBrushColor(0, 0, 255);
    setDrawMode();
}

void MainWindow::on_colorButtonCustom_clicked()
{
    // Open a color dialog initialized with the current color, then update the color.
    QColor selectColor = QColorDialog::getColor(currentColor, this, "Select the color",
                                                QColorDialog::ShowAlphaChannel);
    if(!selectColor.isValid()) { return; }

    changeBrushColor(
        selectColor.red(),
        selectColor.green(),
        selectColor.blue()
        );
    changeBrushOpacity(selectColor.alpha());

    // update opacity in time, synchronize Corresponding Values
    ui->opacitySlider->setValue(opacity);
}

//--------------slider button slots--------------
void MainWindow::on_strokeSlider_valueChanged(int value) { strokeSize = value; }
void MainWindow::on_opacitySlider_valueChanged(int value) { changeBrushOpacity(value); }

//--------------canvas actions button slots----
void MainWindow::on_clearButton_clicked()
{

}

void MainWindow::on_replaceColorButton_clicked()
{
    // if(project == nullptr){
    //     return;
    // }

    QColor oldColor;
    QColor newColor;
    if(!newColor.isValid()) { return; }

    Pixel oldPixel(
        oldColor.red(),
        oldColor.green(),
        oldColor.blue(),
        oldColor.alpha()
        );

    Pixel newPixel(
        newColor.red(),
        newColor.green(),
        newColor.blue(),
        newColor.alpha()
        );

    project->replaceAllFramesColor(oldPixel, newPixel);
    refreshCanvas();
}

//----------frame manage button slots------------
void MainWindow::on_addFrameButton_clicked()
{

}

void MainWindow::on_deleteFrameButton_clicked()
{

}

void MainWindow::on_previewFrameButton_clicked()
{

}

//-----------undo/redo button slots---------
void MainWindow::on_undoButton_clicked()
{

}

void MainWindow::on_redoButton_clicked()
{

}

void MainWindow::on_eraserButton_clicked()
{
    setEraseMode();
}

void MainWindow::on_penButton_clicked()
{
    setDrawMode();
}
