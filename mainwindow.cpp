#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , project(nullptr)
    , currentColor(Qt::black)
    , strokeSize(4)
    , opacity(255)
    , isMouseDown(false)
{
    ui->setupUi(this);
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
void MainWindow::on_colorButton1_clicked() { currentColor = Qt::black; }
void MainWindow::on_colorButton2_clicked() { currentColor = Qt::red;   }
void MainWindow::on_colorButton3_clicked() { currentColor = Qt::green; }
void MainWindow::on_colorButton4_clicked() { currentColor = Qt::blue;  }

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
void MainWindow::on_opacitySlider_valueChanged(int value) { opacity = value; }

//--------------canvas actions button slots----
void MainWindow::on_clearButton_clicked()
{

}

void MainWindow::on_replaceColorButton_clicked()
{

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
