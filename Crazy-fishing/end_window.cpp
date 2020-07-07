#include "end_window.h"
#include "ui_end_window.h"

End_window::End_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::End_window)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/i/res/item6.png"));
    this->setWindowTitle("海底钓鱼总动员");
    this->setFixedSize(1200,800);
    pushbutton_restart = ui->pushButton;//按钮restart
    pushbutton_resume = ui->pushButton_2;//按钮resume
    score_lable = ui->label_2;//分数lable
    ui->lineEdit->setVisible(false);
    mylineedit = ui->lineEdit;
    ui->pushButton_4->setVisible(false);
    pushbutton_queding = ui->pushButton_4;
    tishi_lable = ui->label_4;//分数lable
    tishi_lable->setVisible(false);



    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
    QBrush(QPixmap(":/i/res/end_back.jpg").scaled(// 缩放背景图.
    this->size(),
    Qt::IgnoreAspectRatio,
    Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(palette);

    connect(ui->pushButton_3,&QPushButton::clicked,this,&QWidget::close);}

End_window::~End_window()
{
    delete ui;
}
