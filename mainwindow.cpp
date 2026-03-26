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
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    //TODO: if left button is held inside the canvas, forward to onMouseDragged
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    //TODO: stroke for undo here??
}

//-------------mouse handler----------
void MainWindow::onMousePressed(int x, int y)
{
    //TODO
}

void MainWindow::onMouseDragged(int x, int y)
{
    //TODO
}

//----------------For coordinates, convert canvas widget pixel to sprite grid pixel
int MainWindow::toSpriteX(int widgetX)
{
    //TODO
}

int MainWindow::toSpriteY(int widgetY)
{
    //TODO
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
