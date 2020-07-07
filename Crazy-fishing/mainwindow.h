#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void start_game();

private:
    Ui::MainWindow *ui;
    Gamewidget *my_game;
    QSound *mybacksound;

};

#endif // MAINWINDOW_H
