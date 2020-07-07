#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "item.h"
#include <QLabel>
#include <vector>
#include "end_window.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QSound>


namespace Ui {
class Gamewidget;
}

class Gamewidget : public QWidget
{
    Q_OBJECT

public:
    explicit Gamewidget(QWidget *parent = nullptr);
    ~Gamewidget();
void paintEvent(QPaintEvent *);
void rotate();
void keyPressEvent(QKeyEvent *);
void to_grab();
void data_init();
void ship_init();
void backgroud_init();
void items_init(int num_init);
void items_added();//每隔一段时间自动生成最多 num_born_max个，最少1个
void items_born(int num_born);//init 和 added的过程只有个数不一样，所以公用items_born代码,
void game_time();
void game_resume_notime();
void game_pause();
void game_resume();
void game_restart();
void pause_clicked();
void help_clicked();
void resume_clicked();
void lable_display();
QPushButton *mypushbutton_back;

private:
    Ui::Gamewidget *ui;
    std::vector <Item*> my_items;//用于管理当前item的数据
    std::vector <QLabel*> my_lables;//用于管理当前item的图片
    QLabel *lable_ship = new QLabel(this);
    QPixmap pix =  QPixmap(":/i/res/gouzi.png");//用于钩子在qpainter中的运动显示
    QTimer *paint_timer = new QTimer(this);
    QTimer *grab_timer = new QTimer(this);
    QTimer *ship_timer = new QTimer(this);
    QTimer *zidong_born = new QTimer(this);
    QTimer *lable_timer = new QTimer(this);
    QTimer *seconds_timer = new QTimer(this);//秒表，管理剩余游戏时间和加速时间
    int game_time_last; //初始剩余一分钟时间；
    int game_time_add;//初始奖励时间；
    int jiasu_time_last;//初始剩余加速时间
    int add_time_interval;//用于控制自动产生的时间间隔
    int num_added_max; //用于控制自动产生的单次最大数量
    int num_init; //用于控制初始item数量
    double rate_kind1,rate_kind2,rate_kind3,rate_kind4,rate_kind5,rate_kind6;//均为百分比//随机生成种类的概率，那么kind0理所当然为剩下的

    bool need_rotating;
    bool is_rotating;
    bool need_grabing;
    bool is_grabing;
    bool has_grabsomething;
    int grab_item;
    bool game_is_stop;
    bool has_fast_grab;
    bool lable_dispaly_flag;

    int my_score;

    double grab_length;//初始状态
    double grab_speed; //一般情况下，可通过药水改变

    double grab_angle;
    double grab_angle_change;

    int current_kind;

    End_window *myend_window = new End_window();//两个窗口独立
    QSound *mysound_back;
    QSound *mysound_shenchuqu;
    QSound *mysound_ladao;
    QSound *mysound_haimianbaobao;
    QSound *mysound_get;

};

#endif // GAMEWIDGET_H

