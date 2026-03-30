#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include <QPixmap>
#include <QIcon>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <algorithm>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , project(nullptr)
    , lastCanvasX(0)
    , lastCanvasY(0)
    , hasLastCanvasPos(false)
    , currentTool(ToolMode::Draw)
    , currentColor(Qt::black)
    , strokeSize(4)
    , opacity(255)
    , isMouseDown(false)
{
    ui->setupUi(this);

    // Set icons for buttons
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
    QIcon icon10(":/new/prefix/Export.png");
    ui->exportButton->setIcon(icon10);
    QIcon icon11(":/new/prefix/load_project.png");
    ui->loadButton->setIcon(icon11);
    QIcon icon12(":/new/prefix/resize.png");
    ui->resizeButton->setIcon(icon12);

    // Set size for buttons
    ui->penButton->setIconSize(QSize(120, 120));
    ui->brushButton->setIconSize(QSize(120, 120));
    ui->eraserButton->setIconSize(QSize(120, 120));
    ui->undoButton->setIconSize(QSize(30, 30));
    ui->redoButton->setIconSize(QSize(30, 30));
    ui->clearButton->setIconSize(QSize(30, 30));
    ui->saveButton->setIconSize(QSize(20, 20));
    ui->colorButtonCustom->setIconSize(QSize(20, 20));
    ui->newProjectButton->setIconSize(QSize(16, 16));
    ui->exportButton->setIconSize(QSize(20, 20));
    ui->loadButton->setIconSize(QSize(20, 20));
    ui->resizeButton->setIconSize(QSize(20, 20));

    // Only one tool can be activated
    toolGroup = new QButtonGroup(this);
    toolGroup->addButton(ui->penButton);
    toolGroup->addButton(ui->brushButton);
    toolGroup->addButton(ui->eraserButton);
    toolGroup->setExclusive(true);
    ui->penButton->setChecked(true);

    project = new SpriteProject(32, 32, 8, this);

    // Set original fps value
    ui->fpsSpinBox->setValue(project->getFps());

    previewTimer = new QTimer(this);

    connectProject();
    refreshCanvas();
    refreshFrameList();
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
    connect(project, &SpriteProject::frameChanged,     this, &MainWindow::refreshFrameList);
    connect(project, &SpriteProject::frameListChanged,  this, &MainWindow::refreshFrameList);
    connect(project, &SpriteProject::projectLoaded,    this, &MainWindow::onProjectLoaded);
    connect(ui->frameList, &QListWidget::currentRowChanged, this, &MainWindow::onFrameSelected);
    connect(ui->fpsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onFpsChanged);
}

// Switch the current tool to draw mode
void MainWindow::setDrawMode()
{
    currentTool = ToolMode::Draw;
}

// Switch the current tool to erase mode
void MainWindow::setEraseMode()
{
    currentTool = ToolMode::Erase;
}

//----------------mouse event-------------------
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        QMainWindow::mousePressEvent(event);
        return;
    }

    QPoint globalPos    = event->globalPosition().toPoint();
    QPoint localPos     = ui->canvasWidget->mapFromGlobal(globalPos);
    bool   insideCanvas = ui->canvasWidget->rect().contains(localPos);

    if (insideCanvas) {
        onMousePressed(localPos.x(), localPos.y());
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        QMainWindow::mouseMoveEvent(event);
        return;
    }

    QPoint globalPos    = event->globalPosition().toPoint();
    QPoint localPos     = ui->canvasWidget->mapFromGlobal(globalPos);
    bool   insideCanvas = ui->canvasWidget->rect().contains(localPos);

    if (insideCanvas) {
        onMouseDragged(localPos.x(), localPos.y());
    }

    QMainWindow::mouseMoveEvent(event);
}

// End the current mouse stroke and clear the last tracked canvas position
void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    isMouseDown = false;
    hasLastCanvasPos = false;
    QMainWindow::mouseReleaseEvent(event);
}

//-------------mouse handler----------
void MainWindow::onMousePressed(int x, int y)
{
    // Ignore drawing input if no project is loaded
    if (project == nullptr) {
        return;
    }

    isMouseDown = true;
    hasLastCanvasPos = true;
    lastCanvasX = x;
    lastCanvasY = y;

    project->saveCurrentFrameState();

    int spriteX = toSpriteX(x);
    int spriteY = toSpriteY(y);

    // Erase if the eraser tool is selected
    if (currentTool == ToolMode::Erase) {
        erasePixels(toSpriteX(x), toSpriteY(y));
        return;
    }

    // Build the brush pixel using the current color and opacity
    Pixel brushPixel(
        currentColor.red(),
        currentColor.green(),
        currentColor.blue(),
        opacity
        );

    changePixel(spriteX, spriteY, brushPixel, true);
}

void MainWindow::onMouseDragged(int x, int y)
{
    if (!isMouseDown || project == nullptr) {
        return;
    }

    // If this is the first tracked point, initialize it and wait for the next move
    if (!hasLastCanvasPos) {
        lastCanvasX = x;
        lastCanvasY = y;
        hasLastCanvasPos = true;
        return;
    }

    // Measure how far the mouse moved on the canvas since the last event
    int dx = x - lastCanvasX;
    int dy = y - lastCanvasY;

    // between the two mouse positions so fast dragging does not leave gaps
    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps < 1) {
        steps = 1;
    }

    Pixel brushPixel(
        currentColor.red(),
        currentColor.green(),
        currentColor.blue(),
        opacity
        );

    int lastSpriteX = -1;
    int lastSpriteY = -1;

    for (int i = 1; i <= steps; i++) {
        double t = static_cast<double>(i) / steps;

        int sampleCanvasX = static_cast<int>(std::lround(lastCanvasX + dx * t));
        int sampleCanvasY = static_cast<int>(std::lround(lastCanvasY + dy * t));

        int spriteX = toSpriteX(sampleCanvasX);
        int spriteY = toSpriteY(sampleCanvasY);

        // Skip duplicate sprite cells to avoid unnecessary work while dragging
        if (spriteX == lastSpriteX && spriteY == lastSpriteY) {
            continue;
        }

        if (currentTool == ToolMode::Erase) {
            erasePixels(spriteX, spriteY, false);
        } else {
            changePixel(spriteX, spriteY, brushPixel, false);
        }

        lastSpriteX = spriteX;
        lastSpriteY = spriteY;
    }

    emit project->frameChanged();

    lastCanvasX = x;
    lastCanvasY = y;
}

//----------------For coordinates, convert canvas widget pixel to sprite grid pixel
int MainWindow::toSpriteX(int widgetX)
{
    if (!project) return 0;

    int spriteWidth  = project->getWidth();
    int spriteHeight = project->getHeight();
    int canvasWidth  = ui->canvasWidget->width();
    int canvasHeight = ui->canvasWidget->height();

    // Use the same square cell size as refreshCanvas()
    int cellSize     = qMin(canvasWidth / spriteWidth, canvasHeight / spriteHeight);
    if (cellSize <= 0) return 0;

    // Account for the horizontal offset when the sprite is centered
    int offsetX = (canvasWidth - cellSize * spriteWidth) / 2;
    int spriteX = (widgetX - offsetX) / cellSize;

    // Clamp the result so it always stays inside the sprite bounds
    if (spriteX < 0) spriteX = 0;
    if (spriteX >= spriteWidth) spriteX = spriteWidth - 1;

    return spriteX;
}

int MainWindow::toSpriteY(int widgetY)
{
    if (!project) return 0;

    int spriteWidth  = project->getWidth();
    int spriteHeight = project->getHeight();
    int canvasWidth  = ui->canvasWidget->width();
    int canvasHeight = ui->canvasWidget->height();

    // Use the same square cell size as refreshCanvas()
    int cellSize     = qMin(canvasWidth / spriteWidth, canvasHeight / spriteHeight);
    if (cellSize <= 0) return 0;

    // Account for the vertical offset when the sprite is centered
    int offsetY = (canvasHeight - cellSize * spriteHeight) / 2;
    int spriteY = (widgetY - offsetY) / cellSize;

    // Clamp the result so it always stays inside the sprite bounds
    if (spriteY < 0) spriteY = 0;
    if (spriteY >= spriteHeight) spriteY = spriteHeight - 1;

    return spriteY;
}

void MainWindow::changePixel(int x, int y, Pixel p, bool refresh)
{
    if (project == nullptr) {
        return;
    }

    int size = strokeSize;
    if (size < 1) {
        size = 1;
    }

    // Compute the brush coverage around the center pixel
    int start = -(size / 2);
    int end = start + size - 1;

    // Apply the selected pixel color to every pixel inside the brush area
    for (int dx = start; dx <= end; dx++) {
        for (int dy = start; dy <= end; dy++) {
            project->setPixel(x + dx, y + dy, p);
        }
    }

    if (refresh) {
        emit project->frameChanged();
    }
}

void MainWindow::erasePixels(int x, int y, bool refresh)
{
    if (project == nullptr) {
        return;
    }

    int size = strokeSize;
    if (size < 1) {
        size = 1;
    }

    // Compute the brush coverage around the center pixel
    int start = -(size / 2);
    int end = start + size - 1;

    // Erase every pixel inside the brush area
    for (int dx = start; dx <= end; dx++) {
        for (int dy = start; dy <= end; dy++) {
            project->erasePixel(x + dx, y + dy);
        }
    }

    if (refresh) {
        emit project->frameChanged();
    }
}

// Update the brush color while keeping the current opacity value
void MainWindow::changeBrushColor(int r, int g, int b)
{
    currentColor.setRgb(r, g, b, opacity);
}

void MainWindow::changeBrushOpacity(int a)
{
    // Clamp opacity to the valid RGBA range
    if (a < 0) {
        a = 0;
    }
    if (a > 255) {
        a = 255;
    }

    // Store the new opacity and keep currentColor synchronized with it
    opacity = a;
    currentColor.setAlpha(opacity);
}

//---------------helpers for connectProject(), connected to modal signals
// Redraws the canvas by painting each sprite pixel as a scaled cell on the QPixmap.
void MainWindow::refreshCanvas()
{
    if (!project) return;

    // Get sprite and canvas dimensions
    int spriteWidth  = project->getWidth();
    int spriteHeight = project->getHeight();
    int canvasWidth  = ui->canvasWidget->width();
    int canvasHeight = ui->canvasWidget->height();

    // Use a uniform cell size so pixels are always square
    int cellSize = qMin(canvasWidth / spriteWidth, canvasHeight / spriteHeight);
    if (cellSize < 1) cellSize = 1;

    // Center the drawn sprite within the canvas widget
    int drawWidth  = cellSize * spriteWidth;
    int drawHeight = cellSize * spriteHeight;
    int offsetX    = (canvasWidth  - drawWidth)  / 2;
    int offsetY    = (canvasHeight - drawHeight) / 2;

    // Grey background for the area outside the sprite boundary
    QPixmap pixmap(canvasWidth, canvasHeight);
    pixmap.fill(QColor(180, 180, 180));
    QPainter painter(&pixmap);

    // White fill inside the sprite canvas
    painter.fillRect(offsetX, offsetY, drawWidth, drawHeight, Qt::white);

    // Draw each sprite pixel as a filled square cell on the pixmap
    Frame& frame = project->getCurrentFrame();
    for (int x = 0; x < spriteWidth; x++) {
        for (int y = 0; y < spriteHeight; y++) {
            Pixel p = frame.getPixel(x, y);
            QColor color(p.getR(), p.getG(), p.getB(), p.getA());
            painter.fillRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, color);
        }
    }

    ui->canvasWidget->setPixmap(pixmap);
}

void MainWindow::refreshFrameList()
{
    if (!project) return;
    ui->frameList->blockSignals(true);
    ui->frameList->clear();

    int iconSize = 130;
    ui->frameList->setIconSize(QSize(iconSize, iconSize));

    int index = 0;
    while (true) {
        try {
            QListWidgetItem* item = new QListWidgetItem();

            QPixmap preview(iconSize, iconSize);
            preview.fill(Qt::transparent);
            QPainter painter(&preview);

            int spriteW  = project->getWidth();
            int spriteH  = project->getHeight();
            double scale = qMin(static_cast<double>(iconSize) / spriteW,
                                static_cast<double>(iconSize) / spriteH);
            int offX = (iconSize - static_cast<int>(scale * spriteW)) / 2;
            int offY = (iconSize - static_cast<int>(scale * spriteH)) / 2;

            for (int x = 0; x < spriteW; x++) {
                for (int y = 0; y < spriteH; y++) {
                    Pixel p = project->getFrame(index).getPixel(x, y);
                    QColor color(p.getR(), p.getG(), p.getB(), p.getA());
                    painter.fillRect(offX + x * scale, offY + y * scale, scale, scale, color);
                }
            }

            item->setIcon(QIcon(preview));
            ui->frameList->addItem(item);
            index++;
        }
        catch (std::out_of_range&) {
            break;
        }
    }

    ui->frameList->setCurrentRow(project->getCurrentFrameIndex());
    ui->frameList->blockSignals(false);
}

void MainWindow::onFrameSelected(int index) {
    if (!project || index < 0) return;

    project->setCurrentFrameIndex(index);
}

void MainWindow::onProjectLoaded()
{
    refreshFrameList();
    refreshCanvas();
}

//----------------project manage button slots-------------
void MainWindow::on_newProjectButton_clicked()
{
    // Build a dialog with three input fields
    QDialog dialog(this);
    dialog.setWindowTitle("New Project");

    QFormLayout form(&dialog);

    QSpinBox* widthBox = new QSpinBox(&dialog);
    widthBox->setRange(1, 512);
    widthBox->setValue(32);
    form.addRow("Width:", widthBox);

    QSpinBox* heightBox = new QSpinBox(&dialog);
    heightBox->setRange(1, 512);
    heightBox->setValue(32);
    form.addRow("Height:", heightBox);

    QSpinBox* fpsBox = new QSpinBox(&dialog);
    fpsBox->setRange(1, 60);
    fpsBox->setValue(8);
    form.addRow("FPS:", fpsBox);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&buttons);

    if (dialog.exec() != QDialog::Accepted) return;

    int width  = widthBox->value();
    int height = heightBox->value();
    int fps    = fpsBox->value();

    delete project;
    project = new SpriteProject(width, height, fps, this);
    connectProject();
    refreshCanvas();
    refreshFrameList();
}

void MainWindow::on_loadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Loading File...", "", "*.ssp");

    if(filePath != ""){
        project->load(filePath);
    }
}

void MainWindow::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Saving File...", "", "*.ssp");
    project->save(filePath);
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

void MainWindow::on_resizeButton_clicked()
{
    if (!project) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Resize Canvas");

    QFormLayout form(&dialog);

    QSpinBox* widthBox = new QSpinBox(&dialog);
    widthBox->setRange(1, 512);
    widthBox->setValue(project->getWidth());
    form.addRow("Width:", widthBox);

    QSpinBox* heightBox = new QSpinBox(&dialog);
    heightBox->setRange(1, 512);
    heightBox->setValue(project->getHeight());
    form.addRow("Height:", heightBox);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow(&buttons);

    if (dialog.exec() != QDialog::Accepted) return;

    project->resize(widthBox->value(), heightBox->value());
    refreshCanvas();
    refreshFrameList();
}

//--------------canvas actions button slots----
void MainWindow::on_clearButton_clicked()
{
    project->clearCurrentFrame();
}

void MainWindow::on_replaceColorButton_clicked()
{
    // Build a dialog with three input fields
    QDialog dialog(this);
    dialog.setWindowTitle("Replace Color");

    QFormLayout form(&dialog);

    QColor oldColor = currentColor;
    QColor newColor = currentColor;

    auto colorStyle = [](QColor color){
        return (
            "background-color: rgba(" +
            QString::number(color.red()) + ", " +
            QString::number(color.green()) + ", " +
            QString::number(color.blue()) + ", " +
            QString::number(color.alpha()) + "); "
            "border: 1px solid black;"
            );
    };

    // Create two buttons inside the dialog.
    // oldColorButton: Color to be replaced. newColorButton: Color after replacement.
    QPushButton *oldColorButton = new QPushButton(&dialog);
    QPushButton *newColorButton = new QPushButton(&dialog);
    oldColorButton->setStyleSheet(colorStyle(oldColor));
    newColorButton->setStyleSheet(colorStyle(newColor));

    // click each button to choose the color in the canvas and replace the chosen color.
    connect(oldColorButton, &QPushButton::clicked, [&]() {
        QColor chosenColor = QColorDialog::getColor(oldColor, &dialog, "Select source color",
                                              QColorDialog::ShowAlphaChannel);
        if (!chosenColor.isValid()) return;
        oldColor = chosenColor;
        oldColorButton->setStyleSheet(colorStyle(oldColor));
    });

    connect(newColorButton, &QPushButton::clicked, [&]() {
        QColor chosenColor = QColorDialog::getColor(newColor, &dialog, "Select target color",
                                              QColorDialog::ShowAlphaChannel);
        if (!chosenColor.isValid()) return;
        newColor = chosenColor;
        newColorButton->setStyleSheet(colorStyle(newColor));
    });

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form.addRow("Color to be replaced:", oldColorButton);
    form.addRow("Color after replacement:", newColorButton);
    form.addRow(&buttons);

    if (dialog.exec() != QDialog::Accepted) { return; }

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

    project->saveCurrentFrameState();

    project->replaceAllFramesColor(oldPixel, newPixel);
    refreshCanvas();
}

//----------frame manage button slots------------
void MainWindow::on_addFrameButton_clicked()
{
    project->addFrame();
}

void MainWindow::on_deleteFrameButton_clicked()
{
    if (ui->frameList->currentRow() < 0) return;

    project->deleteFrame();
}

QPixmap MainWindow::buildFramePixmap(int frameIndex, bool actualSize, QSize targetSize) const
{
    if (!project) return QPixmap();

    int spriteWidth = project->getWidth();
    int spriteHeight = project->getHeight();

    // Calculate pixmap dimensions
    int pixWidth, pixHeight, cellSize, offsetX, offsetY;
    if (actualSize) {
        // one sprite pixel = one screen pixel
        pixWidth     = spriteWidth;
        pixHeight     = spriteHeight;
        cellSize = 1;
        offsetX = offsetY = 0;
    } else {
        // Scale to fit size and keeping pixels square
        pixWidth = targetSize.width();
        pixHeight = targetSize.height();
        cellSize = qMax(1, qMin(pixWidth / spriteWidth, pixHeight / spriteHeight));
        offsetX = (pixWidth - cellSize * spriteWidth) / 2;
        offsetY = (pixHeight - cellSize * spriteHeight) / 2;
    }

    QPixmap pixmap(pixWidth, pixHeight);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    // Draw the Checkerboard background
    const int checkerSquareSize = qMax(1, cellSize * 2);
    for (int squareTop = 0; squareTop < pixHeight; squareTop += checkerSquareSize) {
        for (int squareLeft = 0; squareLeft < pixWidth; squareLeft += checkerSquareSize) {
            int col = squareLeft / checkerSquareSize;
            int row = squareTop  / checkerSquareSize;
            bool isLightSquare = ((col + row) % 2 == 0);
            QColor squareColor = isLightSquare ? QColor(200, 200, 200) : Qt::white;
            painter.fillRect(squareLeft, squareTop, checkerSquareSize, checkerSquareSize, squareColor);
        }
    }

    // Draw sprite pixels
    Frame& frame = const_cast<SpriteProject*>(project)->getFrame(frameIndex);
    for (int x = 0; x < spriteWidth; x++) {
        for (int y = 0; y < spriteHeight; y++) {
            Pixel p = frame.getPixel(x, y);
            if (p.getA() == 0) continue;
            QColor color(p.getR(), p.getG(), p.getB(), p.getA());
            painter.fillRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, color);
        }
    }

    return pixmap;
}

void MainWindow::on_previewFrameButton_clicked()
{
    if (!project) return;

    // Setup Dialog window
    QDialog* previewDialog = new QDialog(this);
    previewDialog->setWindowTitle("Animation Preview");
    previewDialog->setAttribute(Qt::WA_DeleteOnClose);
    previewDialog->setWindowFlags(previewDialog->windowFlags() | Qt::Window);

    // Load the canvas
    QLabel* animationCanvas = new QLabel(previewDialog);
    animationCanvas->setAlignment(Qt::AlignCenter);
    animationCanvas->setMinimumSize(256, 256);
    animationCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Selectes between scale to fit dialog and actual size
    QCheckBox* actualSizeToggle = new QCheckBox("Actual Size (1:1 pixels)", previewDialog);
    actualSizeToggle->setChecked(false);

    // Change FPS in real time
    QLabel* fpsTextLabel  = new QLabel("FPS:", previewDialog);
    QSpinBox* fpsSpinBox  = new QSpinBox(previewDialog);
    fpsSpinBox->setRange(1, 60);
    fpsSpinBox->setValue(project->getFps());

    // Shows Frame counter so the user knows which frame they are now
    QLabel* frameCounterLabel = new QLabel("Frame: 1", previewDialog);

    QPushButton* closeButton = new QPushButton("Close", previewDialog);

    // Dialog bottom layout
    QHBoxLayout* bottomBarLayout = new QHBoxLayout();
    bottomBarLayout->addWidget(actualSizeToggle);
    bottomBarLayout->addStretch();
    bottomBarLayout->addWidget(fpsTextLabel);
    bottomBarLayout->addWidget(fpsSpinBox);
    bottomBarLayout->addStretch();
    bottomBarLayout->addWidget(frameCounterLabel);
    bottomBarLayout->addStretch();
    bottomBarLayout->addWidget(closeButton);

    QVBoxLayout* dialogLayout = new QVBoxLayout(previewDialog);
    dialogLayout->addWidget(animationCanvas);
    dialogLayout->addLayout(bottomBarLayout);

    // Frame index tracker
    int* playbackFrameIndex = new int(0);

    // Redraws animationCanvas to show the frame at *playbackFrameIndex
    auto drawCurrentFrame = [=]() {
        int totalFrames = ui->frameList->count();
        if (totalFrames == 0) return;

        // Wrap index in case frames were deleted while the dialog is open
        *playbackFrameIndex = *playbackFrameIndex % totalFrames;

        bool useActualSize      = actualSizeToggle->isChecked();
        QSize canvasDisplaySize = animationCanvas->size();
        QPixmap framePixmap     = buildFramePixmap(*playbackFrameIndex, useActualSize, canvasDisplaySize);

        animationCanvas->setPixmap(framePixmap);
        frameCounterLabel->setText(QString("Frame: %1 / %2")
                                       .arg(*playbackFrameIndex + 1)
                                       .arg(totalFrames));
    };

    // Playback timer fires once per frame at the current FPS
    int msPerFrame = 1000 / project->getFps();
    QTimer* playbackTimer = new QTimer(previewDialog);
    playbackTimer->setInterval(msPerFrame);
    playbackTimer->start();

    connect(playbackTimer, &QTimer::timeout, previewDialog, [=]() {
        int totalFrames = ui->frameList->count();
        if (totalFrames == 0) return;
        *playbackFrameIndex = (*playbackFrameIndex + 1) % totalFrames;
        drawCurrentFrame();
    });

    // FPS spinbox update timer speed
    connect(fpsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), previewDialog, [=](int newFps) {
        playbackTimer->setInterval(1000 / newFps);
        project->setFps(newFps);
        ui->fpsSpinBox->setValue(newFps);
    });

    // Actual size toggle resize the dialog and label then redraw
    connect(actualSizeToggle, &QCheckBox::toggled, previewDialog, [=](bool) {
        if (actualSizeToggle->isChecked()) {
            // Lock label to exact sprite dimensions
            int spriteWidth  = project->getWidth();
            int spriteHeight = project->getHeight();
            animationCanvas->setFixedSize(spriteWidth, spriteHeight);
            previewDialog->resize(qMax(320, spriteWidth + 40), spriteHeight + 80);
        } else {
            // free-resizing behaviour
            animationCanvas->setMinimumSize(256, 256);
            animationCanvas->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
            previewDialog->resize(320, 360);
        }
        drawCurrentFrame();
    });

    connect(closeButton, &QPushButton::clicked, previewDialog, &QDialog::close);

    drawCurrentFrame();

    previewDialog->resize(320, 360);
    previewDialog->show();

    // Free the frame index on heap
    connect(previewDialog, &QDialog::destroyed, previewDialog, [playbackFrameIndex]() {
        delete playbackFrameIndex;
    });
}

void MainWindow::onFpsChanged(int value) {
    project->setFps(value);

    if (isPreviewing && previewTimer) {
        int fps = 1000 / value;
        previewTimer->setInterval(fps);
    }
}

//-----------undo/redo button slots---------
void MainWindow::on_undoButton_clicked()
{
    project->undo();
}

void MainWindow::on_redoButton_clicked()
{
    project->redo();
}

//-----------tool selection button slots-----------
void MainWindow::on_eraserButton_clicked()
{
    setEraseMode();
}

void MainWindow::on_penButton_clicked()
{
    setDrawMode();
}

void MainWindow::on_brushButton_clicked()
{
    setDrawMode();
}
