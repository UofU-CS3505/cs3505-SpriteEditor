#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spriteproject.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SpriteProject* project;

    // QColor from color picker
    QColor currentColor;
    // stroke size from the stroke slider
    int strokeSize;
    // opacity from the opacity slider
    int opacity;

    // helper method for mouse interactions
    void connectProject(); //connects signal(spriteProject) to slot(mainWindow)
    void onMousePressed(int x, int y);
    void onMouseDragged(int x, int y);
    int toSpriteX(int widgetX);
    int toSpriteY(int widgetY);

protected:
    // Mouse events, Qt calls these when user interacts with the window
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    // Slots connected to SpriteProject signals
    void refreshCanvas();       // connected to project::frameChanged
    void refreshFrameList();    // connected to project::frameListChanged
    void onProjectLoaded();     // connected to project::projectLoaded

    // Slots connected to UI buttons
    void on_newProjectButton_clicked();
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_exportButton_clicked();

    void on_colorButton1_clicked();
    void on_colorButton2_clicked();
    void on_colorButton3_clicked();
    void on_colorButton4_clicked();
    void on_colorButtonCustom_clicked();

    void on_strokeSlider_valueChanged(int value);
    void on_opacitySlider_valueChanged(int value);

    void on_clearButton_clicked();
    void on_replaceColorButton_clicked();

    void on_addFrameButton_clicked();
    void on_deleteFrameButton_clicked();
    void on_previewFrameButton_clicked();

    void on_undoButton_clicked();
    void on_redoButton_clicked();

};
#endif // MAINWINDOW_H
