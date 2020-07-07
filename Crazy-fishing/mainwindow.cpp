#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include "gamewidget.h"
#include <QSound>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/i/res/item6.png"));
    this->setWindowTitle("海底钓鱼总动员");
    mybacksound = new QSound(":/wav/doudizhu.wav");
    mybacksound->play();
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
    QBrush(QPixmap(":/i/res/main_back.png").scaled(// 缩放背景图.
    this->size(),
    Qt::IgnoreAspectRatio,
    Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(palette);
    this->setFixedSize(QSize(1200,800));
    QMovie *movie = new QMovie(":/i/res/giphy.gif");
    movie->setScaledSize(QSize(430,320));
    ui->label->setMovie(movie);
    movie->start();
    ui->pushButton_back->setVisible(false);
    ui->label_3->setVisible(false);

    connect(ui->pushButton_start,&QPushButton::clicked,this,&MainWindow::start_game);
    connect(ui->pushButton_exit,&QPushButton::clicked,this,&MainWindow::close);
    connect(ui->pushButton_help,&QPushButton::clicked,[=](){ui->label_3->setVisible(true);ui->pushButton_back->setVisible(true);});
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){ui->label_3->setVisible(false);ui->pushButton_back->setVisible(false);});


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow :: start_game()
{
        mybacksound->stop();
        my_game = new Gamewidget();//这里直接定义的话容易crash，因为自毁，销毁自己的工具掌握在自己手里
        connect(my_game->mypushbutton_back,&QPushButton::clicked,[=]()
        {
            my_game->setVisible(false);
            delete my_game;
            mybacksound->play();
            this->setVisible(true);;
        });
        my_game->setVisible(true);
        this->setVisible(false);
}
