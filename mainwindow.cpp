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

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    isMouseDown = false;
    QMainWindow::mouseReleaseEvent(event);
}

//-------------mouse handler----------
void MainWindow::onMousePressed(int x, int y)
{
    isMouseDown = true;

    project->saveCurrentFrameState();

    if (currentTool == ToolMode::Erase) {
        erasePixels(toSpriteX(x), toSpriteY(y));
        return;
    }

    Pixel brushPixel(
        currentColor.red(),
        currentColor.green(),
        currentColor.blue(),
        opacity
        );

    changePixel(toSpriteX(x), toSpriteY(y), brushPixel);
}

void MainWindow::onMouseDragged(int x, int y)
{
    if (!isMouseDown) {
        return;
    }

    if (currentTool == ToolMode::Erase) {
        erasePixels(toSpriteX(x), toSpriteY(y));
        return;
    }

    Pixel brushPixel(
        currentColor.red(),
        currentColor.green(),
        currentColor.blue(),
        opacity
        );

    changePixel(toSpriteX(x), toSpriteY(y), brushPixel);
}

//----------------For coordinates, convert canvas widget pixel to sprite grid pixel
int MainWindow::toSpriteX(int widgetX)
{
    if (!project) return 0;

    int spriteWeight = project->getWidth();
    int canvasWeight = ui->canvasWidget->width();
    return widgetX * spriteWeight / canvasWeight;
}

int MainWindow::toSpriteY(int widgetY)
{
    if (!project) return 0;

    int spriteHeight = project->getHeight();
    int canvasHeight = ui->canvasWidget->height();
    return widgetY * spriteHeight / canvasHeight;
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
    emit project->frameChanged();
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
// Redraws the canvas by painting each sprite pixel as a scaled cell on the QPixmap.
void MainWindow::refreshCanvas()
{
    if (!project) return;

    // Get sprite and canvas dimensions
    int spriteWeight = project->getWidth();
    int spriteHeight = project->getHeight();
    int canvasWeight = ui->canvasWidget->width();
    int canvasHeight = ui->canvasWidget->height();

    // Each sprite pixel maps to a cell of this size on the canvas
    int cellW = canvasWeight / spriteWeight;
    int cellH = canvasHeight / spriteHeight;

    // Create a blank white pixmap the size of the canvas
    QPixmap pixmap(canvasWeight, canvasHeight);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);

    // Draw each sprite pixel as a filled rectangle on the pixmap
    Frame& frame = project->getCurrentFrame();
    for (int x = 0; x < spriteWeight; x++) {
        for (int y = 0; y < spriteHeight; y++) {
            Pixel p = frame.getPixel(x, y);
            QColor color(p.getR(), p.getG(), p.getB(), p.getA());
            painter.fillRect(x * cellW, y * cellH, cellW, cellH, color);
        }
    }

    ui->canvasWidget->setPixmap(pixmap);
}

void MainWindow::refreshFrameList()
{
    if (!project) return;

    ui->frameList->clear();

    int index = 0;
    while (true) {
        try {
            Frame& frame = project->getFrame(index);

            QListWidgetItem* item = new QListWidgetItem();
            item->setText("Frame " + QString::number(index + 1));

            QPixmap preview(32, 32);
            preview.fill(Qt::gray);

            item->setIcon(QIcon(preview));
            ui->frameList->addItem(item);
            index++;
        }
        catch (std::out_of_range&) {
            break;
        }
    }

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

}

void MainWindow::on_previewFrameButton_clicked()
{

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

void MainWindow::on_eraserButton_clicked()
{
    setEraseMode();
}

void MainWindow::on_penButton_clicked()
{
    setDrawMode();
}
