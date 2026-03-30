/**
 * Header file for the MainWindow class. This class deals with user interface, or
 * the view, of the sprite editor. It manages user interactions, like drawing on
 * the canvas, managing frames, and controlling the animation preview. It connects
 * the UI components through signals and slots to the model.
 *
 * @authors: Helen Zhang, Aspen Tobler, ...
 * Class: CS 3505 Assignment 8
 * Last modified: March 30, 2026
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QTimer>
#include <QPushButton>
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
    /**
     * Constructs the MainWindow, initializes the UI, sets up tool buttons,
     * loads icons, creates a default 32x32 SpriteProject, and wires all signals and slots.
     * @brief MainWindow constructor
     * @param parent optional parent widget
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * Destructs the MainWindow and frees the UI and project resources.
     * @brief ~MainWindow destructor
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SpriteProject* project;
    int lastCanvasX;
    int lastCanvasY;
    bool hasLastCanvasPos;

    enum class ToolMode {
        Draw,
        Erase
    };

    ToolMode currentTool;
    QButtonGroup* toolGroup;

    QTimer* previewTimer;
    bool isPreviewing = false;

    // QColor from color picker
    QColor currentColor;
    // stroke size from the stroke slider
    int strokeSize;
    // opacity from the opacity slider
    int opacity;
    // drawing state
    bool isMouseDown;

    /**
     * Erases pixels in a square region centered at (x, y) based on the current stroke size.
     * @brief erasePixels erases pixels at the given sprite coordinate
     * @param x sprite X coordinate
     * @param y sprite Y coordinate
     */
    void erasePixels(int x, int y, bool refresh = true);

    /**
     * Sets the current tool mode to Draw.
     * @brief setDrawMode activates draw mode
     */
    void setDrawMode();

    /**
     * Sets the current tool mode to Erase.
     * @brief setEraseMode activates erase mode
     */
    void setEraseMode();

    // ---------helper method for mouse interactions-----------
    /**
     * Connects SpriteProject signals to MainWindow slots,
     * and wires up the frame list and fps spinbox interactions.
     * Must be called every time a new SpriteProject is created.
     * @brief connectProject wires model signals to view slots
     */
    void connectProject();

    /**
     * Handles a mouse press on the canvas. Saves the current frame state for undo,
     * then draws or erases at the given canvas-local coordinates.
     * @brief onMousePressed handles initial click on canvas
     * @param x canvas-local X position
     * @param y canvas-local Y position
     */
    void onMousePressed(int x, int y);

    /**
     * Handles mouse drag over the canvas while the left button is held.
     * Continues drawing or erasing at the given canvas-local coordinates.
     * @brief onMouseDragged handles drag painting on the canvas
     * @param x canvas-local X position
     * @param y canvas-local Y position
     */
    void onMouseDragged(int x, int y);

    /**
     * Converts a canvas widget X coordinate to the corresponding sprite grid column.
     * Accounts for centered rendering offset and uniform cell size scaling.
     * @brief toSpriteX maps widget X to sprite X
     * @param widgetX X position in canvas widget pixels
     * @return sprite grid column index, clamped to valid range
     */
    int toSpriteX(int widgetX);

    /**
     * Converts a canvas widget Y coordinate to the corresponding sprite grid row.
     * Accounts for centered rendering offset and uniform cell size scaling.
     * @brief toSpriteY maps widget Y to sprite Y
     * @param widgetY Y position in canvas widget pixels
     * @return sprite grid row index, clamped to valid range
     */
    int toSpriteY(int widgetY);

    //----------Pixel Drawing helpers------------
    /**
     * Paints a square brush stroke centered at (x, y) in sprite coordinates.
     * The brush size is determined by the current strokeSize value.
     * @brief changePixel draws a brush-sized block of pixels
     * @param x sprite X coordinate of brush center
     * @param y sprite Y coordinate of brush center
     * @param p the pixel color to paint
     */
    void changePixel(int x, int y, Pixel p, bool refresh = true);

    /**
     * Updates the current brush color using raw RGB values.
     * Does not affect opacity.
     * @brief changeBrushColor sets the brush color
     * @param r red channel (0-255)
     * @param g green channel (0-255)
     * @param b blue channel (0-255)
     */
    void changeBrushColor(int r, int g, int b);

    /**
     * Updates the current brush opacity and clamps the value to [0, 255].
     * @brief changeBrushOpacity sets the brush alpha
     * @param a alpha value (0-255)
     */
    void changeBrushOpacity(int a);

protected:
    // --------Mouse events, Qt calls these when user interacts with the window----------
    /**
     * Intercepts left mouse button press events over the canvas widget
     * and forwards them to onMousePressed with canvas-local coordinates.
     * @brief mousePressEvent handles Qt mouse press
     * @param event the mouse event
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * Intercepts left mouse drag events over the canvas widget
     * and forwards them to onMouseDragged with canvas-local coordinates.
     * @brief mouseMoveEvent handles Qt mouse move
     * @param event the mouse event
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * Resets the isMouseDown flag when the left mouse button is released.
     * @brief mouseReleaseEvent handles Qt mouse release
     * @param event the mouse event
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    // --------Slots connected to SpriteProject signals----------
    /**
     * Repaints the canvas widget to reflect the current frame's pixel data.
     * Each sprite pixel is drawn as a scaled square cell, centered in the widget.
     * Connected to SpriteProject::frameChanged.
     * @brief refreshCanvas redraws the canvas
     */
    void refreshCanvas();

    /**
     * Rebuilds the frame list widget with scaled thumbnail icons for each frame.
     * Connected to SpriteProject::frameListChanged and SpriteProject::frameChanged.
     * @brief refreshFrameList updates the frame list UI
     */
    void refreshFrameList();

    /**
     * Fully rebuilds the view after a project file is loaded
     * by calling refreshFrameList and refreshCanvas.
     * Connected to SpriteProject::projectLoaded.
     * @brief onProjectLoaded responds to a completed load
     */
    void onProjectLoaded();

    /**
     * Switches the active frame to the one at the given index.
     * Connected to QListWidget::currentRowChanged.
     * @brief onFrameSelected handles frame selection from the list
     * @param index the selected frame index
     */
    void onFrameSelected(int index);

    // --------Slots connected to UI buttons------------------
    /**
     * Opens a dialog to configure width, height, and FPS,
     * then creates a fresh SpriteProject with those settings.
     * @brief on_newProjectButton_clicked handles new project button
     */
    void on_newProjectButton_clicked();

    /**
     * Opens a file dialog and loads a .ssp project file into the current project.
     * @brief on_loadButton_clicked handles load button
     */
    void on_loadButton_clicked();

    /**
     * Opens a file dialog and saves the current project to a .ssp file.
     * @brief on_saveButton_clicked handles save button
     */
    void on_saveButton_clicked();

    /**
     * Exports the current project as an image.
     * @brief on_exportButton_clicked handles export button
     */
    void on_exportButton_clicked();

    /**
     * Sets the brush color to black (0, 0, 0) and activates draw mode.
     * @brief on_colorButton1_clicked handles color preset 1 (black)
     */
    void on_colorButton1_clicked();

    /**
     * Sets the brush color to red (255, 0, 0) and activates draw mode.
     * @brief on_colorButton2_clicked handles color preset 2 (red)
     */
    void on_colorButton2_clicked();

    /**
     * Sets the brush color to green (0, 255, 0) and activates draw mode.
     * @brief on_colorButton3_clicked handles color preset 3 (green)
     */
    void on_colorButton3_clicked();

    /**
     * Sets the brush color to blue (0, 0, 255) and activates draw mode.
     * @brief on_colorButton4_clicked handles color preset 4 (blue)
     */
    void on_colorButton4_clicked();

    /**
     * Opens a QColorDialog for the user to pick a custom color and opacity,
     * then updates the brush color, opacity, and the opacity slider accordingly.
     * @brief on_colorButtonCustom_clicked handles custom color picker button
     */
    void on_colorButtonCustom_clicked();

    /**
     * Updates the current stroke size to the slider's value.
     * @brief on_strokeSlider_valueChanged handles stroke size slider
     * @param value new stroke size
     */
    void on_strokeSlider_valueChanged(int value);

    /**
     * Updates the current brush opacity to the slider's value.
     * @brief on_opacitySlider_valueChanged handles opacity slider
     * @param value new opacity (0-255)
     */
    void on_opacitySlider_valueChanged(int value);

    /**
     * Clears all pixels on the current frame to transparent.
     * @brief on_clearButton_clicked handles clear canvas button
     */
    void on_clearButton_clicked();

    /**
     * Opens a dialog to enter new dimensions, then resizes
     * all frames in the project to the specified width and height.
     * @brief on_resizeButton_clicked handles canvas resize button
     */
    void on_resizeButton_clicked();

    /**
     * Opens a dialog with two color pickers (source and target),
     * then replaces all instances of the source color with the target
     * color across every frame in the project.
     * @brief on_replaceColorButton_clicked handles replace color button
     */
    void on_replaceColorButton_clicked();

    /**
     * Adds a new blank frame after the current frame and selects it.
     * @brief on_addFrameButton_clicked handles add frame button
     */
    void on_addFrameButton_clicked();

    /**
     * Deletes the currently selected frame if one is selected.
     * @brief on_deleteFrameButton_clicked handles delete frame button
     */
    void on_deleteFrameButton_clicked();

    /**
     * Toggles the animation preview on or off.
     * When on, cycles through frames at the current FPS using the preview timer.
     * @brief on_previewFrameButton_clicked handles preview toggle button
     */
    void on_previewFrameButton_clicked();

    /**
     * Updates the project's FPS to the new value and adjusts
     * the preview timer interval if the preview is currently running.
     * Connected to the fpsSpinBox valueChanged signal.
     * @brief onFpsChanged handles FPS spinbox changes
     * @param value new FPS value
     */
    void onFpsChanged(int value);

    /**
     * Undoes the last pixel change on the current frame.
     * @brief on_undoButton_clicked handles undo button
     */
    void on_undoButton_clicked();

    /**
     * Redoes the last undone pixel change on the current frame.
     * @brief on_redoButton_clicked handles redo button
     */
    void on_redoButton_clicked();

    /**
     * Activates erase mode so mouse interactions erase pixels.
     * @brief on_eraserButton_clicked handles eraser tool button
     */
    void on_eraserButton_clicked();

    /**
     * Activates draw mode with the pen tool (1px effective stroke).
     * @brief on_penButton_clicked handles pen tool button
     */
    void on_penButton_clicked();

    /**
     * Activates draw mode with the brush tool (larger stroke size).
     * @brief on_brushButton_clicked handles brush tool button
     */
    void on_brushButton_clicked();

    /**
     * Temporarily changes the button's color to create a flash effect.
     * @brief flashButton handles color button flash.
     * @param button The button to be flashed.
     * @param color The button's original background color.
     */
    void flashColorButton(QPushButton* button, const QString& color);

private:
    /**
     * Renders one frame of the project into a QPixmap.
     * @param frameIndex  index of the frame to render
     * @param actualSize  if true, render at 1:1 pixel scale; otherwise scale to fit targetSize
     * @param targetSize  the size to scale into when actualSize is false
     * @return a QPixmap containing the rendered frame
     */
    QPixmap buildFramePixmap(int frameIndex, bool actualSize, QSize targetSize) const;
};
#endif // MAINWINDOW_H
