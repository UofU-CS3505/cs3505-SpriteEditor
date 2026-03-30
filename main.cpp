/**
 * Main file that creates an instance of MainWindow and
 * executes the Sprite editor program.
 *
 * @authors: Helen Zhang, Emily Wang, Aspen Tobler, Xinmiao Li, and Shaoze Yang
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
