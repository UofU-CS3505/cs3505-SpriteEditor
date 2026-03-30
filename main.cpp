/**
 * Main file that creates an instance of MainWindow and
 * executes the Sprite editor program.
 *
 * @authors: Helen Zhang, Aspen Tobler, ...
 * Class: CS 3505 Assignment 8
 * Last modified: March 30, 2026
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
