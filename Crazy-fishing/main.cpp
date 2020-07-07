#include "mainwindow.h"
#include <QApplication>
#include <gamewidget.h>
#include <end_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    Gamewidget v;
//    v.show();
//    End_window v;
//    v.show();
    return a.exec();
}
