#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QTime>
#include <math.h>
#define PI 3.1415927
#define random(x) (qrand()%x) //用于产生绝对随机数

Gamewidget::Gamewidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gamewidget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/i/res/item6.png"));
    this->setWindowTitle("海底钓鱼总动员");
    mysound_back = new QSound(":/wav/doudizhu.wav");mysound_back->setLoops(QSound::Infinite);//背景音乐的音效
    mysound_shenchuqu = new QSound(":/wav/shenchuqu.wav");mysound_back->setLoops(1);//伸出绳子的音效
    mysound_ladao = new QSound(":/wav/ladao.wav");mysound_back->setLoops(QSound::Infinite);//拉到的音效，所有
    mysound_get = new QSound(":/wav/get.wav");mysound_back->setLoops(1); //拉回来之后的音效，除了海绵宝宝
    mysound_haimianbaobao = new QSound(":/wav/haimiaobaobao.wav");mysound_back->setLoops(1); //拉回来海绵宝宝的音效
    mypushbutton_back = ui->pushButton_back;
    myend_window->setVisible(false);
    setFixedSize(1200,800);//设置固定大小

    data_init();//所有数据初始化
    backgroud_init();//背景图片初始化
    ship_init();//船图片初始化
    items_init(num_init);//初始化item个数

    connect(ui->pushButton_help,&QPushButton::clicked,this,&Gamewidget::help_clicked);
    connect(ui->pushButton_pause,&QPushButton::clicked,this,&Gamewidget::pause_clicked);
    connect(ui->pushButton_resume,&QPushButton::clicked,this,&Gamewidget::resume_clicked);
    //connect(ui->pushButton_back,&QPushButton::clicked,[=](){//mysound_back->stop();});
    connect(zidong_born,&QTimer::timeout,this,&Gamewidget::items_added); //自动添加
    connect(paint_timer,&QTimer::timeout,this,&Gamewidget::rotate);//控制旋转时的图像刷新
    connect(grab_timer,&QTimer::timeout,this,&Gamewidget::to_grab);//控制抓取时的图像刷新
    connect(seconds_timer,&QTimer::timeout,this,&Gamewidget::game_time);//秒表，给游戏时间和药水时间计时
//  用于换行显示
//  ui->label_2->setGeometry(QRect(100, 10, 329, 27*4));  //四倍行距
//  ui->label_2->setWordWrap(true);
//  ui->label_2->setAlignment(Qt::AlignTop);
    connect(lable_timer,&QTimer::timeout,[=](){ui->label_2->setVisible(false);
        lable_timer->stop();});//显示完加分信息之后停止计时
    connect(paint_timer,&QTimer::timeout,[=](){
        ui->lcdNumber_2->display(game_time_last);

        if(jiasu_time_last != 0)
        {
            ui->label_4->setVisible(true);//显示出加速剩余时间
            ui->lcdNumber_3->setVisible(true);
            ui->lcdNumber_3->display(jiasu_time_last);}
        else
        {
            ui->label_4->setVisible(false);//显示出加速剩余时间
            ui->lcdNumber_3->setVisible(false);
        }});//控制旋转时的图像刷新

    connect(myend_window->getPushbutton_resume(),&QPushButton::clicked,this,&Gamewidget::game_resume);
    connect(myend_window->getPushbutton_rstart(),&QPushButton::clicked,this,&Gamewidget::game_restart);
    connect(myend_window->getPushbutton_queding(),&QPushButton::clicked,this,&Gamewidget::game_resume_notime);
}

Gamewidget::~Gamewidget()
{
    delete ui;
}




//仅用于显示，至于运动的数据处理交给其他函数，然后调用update来画图

void Gamewidget :: backgroud_init()
{
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
    QBrush(QPixmap(":/i/res/mysea.png").scaled(// 缩放背景图.
    this->size(),
    Qt::IgnoreAspectRatio,
    Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(palette);
}
void Gamewidget :: ship_init()
{
    ship_timer->start(500);

    QImage *ship_image = new QImage(":/i/res/ship.png");
    QPixmap pixmap_ship = QPixmap::fromImage(ship_image->scaled(70,70,Qt::KeepAspectRatio));
    lable_ship->setPixmap(pixmap_ship); //加载船
    lable_ship->move(570,5);

    connect(ship_timer,&QTimer::timeout,[=](){
        static int flag_ship = 1;
        if (flag_ship == 1)
        {
            lable_ship->move(567,5);
            flag_ship = -flag_ship;
        }
        else {
            lable_ship->move(570,5);
            flag_ship = -flag_ship;
        }
    });
}

void Gamewidget :: data_init(){
    game_time_last = 120; //初始剩余一分钟时间；
    game_time_add = 0;//初始奖励时间；
    jiasu_time_last = 0;//初始剩余加速时间
    add_time_interval = 8;//用于控制自动产生的时间间隔
    num_added_max = 3; //用于控制自动产生的单次最大数量
    num_init = 10; //用于控制初始item数量

    //概率控制
    rate_kind1 = 0.30;//中鱼0.3
    rate_kind2 = 0.20;//大鱼0.2
    rate_kind3 = 0.07;//加速0.07
    rate_kind4 = 0.04;//时间0.04
    rate_kind5 = 0.04;//鲨鱼0.04
    rate_kind6 = 0.03;//海绵宝宝0.03

    //mysound_back->play();

    need_rotating = true;
    is_rotating = true;
    need_grabing = false;
    is_grabing = false;
    has_grabsomething = false;
    grab_item = 0;
    game_is_stop = false;
    has_fast_grab = false;
    lable_dispaly_flag = false;
    game_is_stop = false;

    my_score = 0;

    grab_length = 0;//初始状态
    grab_speed = 2; //一般情况下，可通过药水改变

    //必须刚开始有东西显现出来,第一次拉到东西之后lable显示才不会卡顿，可能是第一次调用字体库的缘故
    ui->label_2->setText(".");
    ui->label_2->setVisible(true);//加分的提示语句
    ui->label_4->setVisible(false);//“剩余加速时间”label
    ui->lcdNumber->display("0");
    ui->lcdNumber_3->setVisible(false);//剩余加速时间的lcd


    grab_angle = 0;
    grab_angle_change = 0.5;
    current_kind = 0;

    paint_timer->start(15);//图像15ms刷新一次
    grab_timer->start(10);//100ms对grab_length进行控制，用于控制速度
    seconds_timer->start(1000);
    zidong_born->start(1000*add_time_interval);

    ui->label_5->setVisible(false);
    ui->pushButton_resume->setVisible(false);
    ui->pushButton_pause->setVisible(true);
    ui->pushButton_help->setVisible(true);
    for (int i = 0; i < my_items.size(); i++)
    {
        delete my_items[i];
        delete my_lables[i];
}

    my_items.clear();
    my_lables.clear();
}

void Gamewidget :: items_init(int num_init)
{
    my_items.clear();
    my_lables.clear();
    items_born(num_init);
}
void Gamewidget :: items_born(int num_born)
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    for (int i = 0; i < num_born; i++)
    {
        QImage image;
//       七个参数分别表示
//        x = a;
//        y = b;
//        height = c;
//        width = d;
//        price = e;
//        kind = f;
//        speed = g;
//        visible = h;
        int a=0,b=0,c = 0,d = 0,e=0,f = 0;double g = 0; bool h = false;
        int n = random(100); //代表刚开始随机生成的种类
        int xx = random(1100)+10;//用于随机的坐标确定
        QString pic_path;
        double rate_kind0 = 1-rate_kind1-rate_kind2-rate_kind3-rate_kind4-rate_kind5-rate_kind6;
        int yy = random(630)+70;//用于随机的坐标确定
        //qDebug() << n << xx <<yy;
//        switch (n)
//        {
//        case 0 :a = xx;b = yy;c = 80; d = 50; e = 10; f = 0; g = 10; h = true; pic_path = ":/i/res/item0.png";break;
//        case 1 :a = xx;b = yy;c = 85; d = 60; e = 20; f = 1; g = 8; h = true; pic_path = ":/i/res/item1.png";break;
//        case 2 :a = xx;b = yy;c = d = 93; e = 30; f = 2; g = 5; h = true; pic_path = ":/i/res/item2.png";break;
//        case 3 :a = xx;b = yy;c = d = 80; e = 0; f = 3; g = 8; h = true; pic_path = ":/i/res/item3_jiasu.png";break;
//        case 4 :a = xx;b = yy;c = 50; d = 80; e = 0; f = 4; g = 8; h = true; pic_path = ":/i/res/item4_shoubiao.png";break;
//        case 5 :a = xx;b = yy;c = d = 100; e = -100; f = 5; g = 100; h = true; pic_path = ":/i/res/item5_jianfen.png";break;
//        case 6 :a = xx;b = yy;c = d = 120; e = 999; f = 6; g = 1; h = true; pic_path = ":/i/res/item6.png";break;
//        }
        //概率计算处理部分
                if(n >=0 && n <100*rate_kind0)
        { a = xx;b = yy;c = 80; d = 50; e = 10; f = 0; g = 10; h = true; pic_path = ":/i/res/item0.png";}
                else if(n >=(100*rate_kind0) && n <100*(rate_kind1 + rate_kind0))
        {a = xx;b = yy;c = 85; d = 60; e = 20; f = 1; g = 8; h = true; pic_path = ":/i/res/item1.png";}
                else if(n >=100*(rate_kind1 + rate_kind0) && n <100*(rate_kind2+rate_kind1 + rate_kind0))
        {a = xx;b = yy;c = d = 93; e = 30; f = 2; g = 5; h = true; pic_path = ":/i/res/item2.png";}
                else if(n >=100*(rate_kind2+rate_kind1 + rate_kind0) && n <100*(rate_kind3+rate_kind2+rate_kind1 + rate_kind0))
        {a = xx;b = yy;c = d = 80; e = 0; f = 3; g = 8; h = true; pic_path = ":/i/res/item3_jiasu.png";}
                else if(n >=100*(rate_kind3+rate_kind2+rate_kind1 + rate_kind0) && n <100*(rate_kind3+rate_kind2+rate_kind1 + rate_kind0 +rate_kind4))
        {a = xx;b = yy;c = 50; d = 80; e = 0; f = 4; g = 8; h = true; pic_path = ":/i/res/item4_shoubiao.png";}
                else if(n >=100*(rate_kind4+rate_kind3+rate_kind2+rate_kind1 + rate_kind0) && n <100*(rate_kind5 +rate_kind3+rate_kind2+rate_kind1 + rate_kind0 +rate_kind4))
        {a = xx;b = yy;c = d = 100; e = -100; f = 5; g = 100; h = true; pic_path = ":/i/res/item5_jianfen.png";}
                else if(n >=100*(rate_kind5+ rate_kind4+rate_kind3+rate_kind2+rate_kind1 + rate_kind0) && n <100*(rate_kind6+ rate_kind5 +rate_kind3+rate_kind2+rate_kind1 + rate_kind0 +rate_kind4))
        {a = xx;b = yy;c = d = 120; e = 999; f = 6; g = 1; h = true; pic_path = ":/i/res/item6.png";}
                else;
        my_items.push_back(new Item(a,b,c,d,e,f,g,h));
        image.load(pic_path);
        QPixmap pixmap = QPixmap::fromImage(image.scaled(c,d,Qt::KeepAspectRatio));
        //中间这两个int表示转化为多少像素，KeepAspectRatio是一个枚举，表示
        //在给定的矩形内，将尺寸缩放为尽可能大的矩形，并保留宽高比。
        QLabel *my_lable = new QLabel(this);
        my_lable->setPixmap(pixmap);
        my_lable->move(a,b);
        my_lable->setVisible(h);
        my_lables.push_back(my_lable);

    }

}
void Gamewidget :: items_added()
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int num = random(num_added_max)+1;
    items_born(num);

}

void Gamewidget::paintEvent(QPaintEvent *)
{
      QPainter painter(this);
      QPen pen;
      painter.translate(600,55); //让图片的中心作为旋转的中心
      painter.rotate(grab_angle); //顺时针整体旋转90度
      painter.translate(-600,-55); //使原点复原,因为旋转之后再画是根据新原点的相对位置画的
      pen.setWidth(2);
      painter.setPen(pen);//旋转之后再画相当于把画布歪着放了，相对于桌面
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.drawLine(600,55,600,85+grab_length);//线的起点
      painter.drawPixmap(568,75+grab_length,pix);//绘图的起点
}

//一般般情况下的自由运动
void Gamewidget::rotate()
{
    game_is_stop = false;
    if (need_rotating == true)
    {
        is_rotating = true;
        is_grabing = false;
        if (grab_angle >= 85 || grab_angle <= -85)
        {
            grab_angle_change = -grab_angle_change;
            grab_angle = grab_angle + grab_angle_change;
            //qDebug() << "test";
        }
        else
            grab_angle = grab_angle + grab_angle_change;
        update();
    }
    else {
        ;
    }
}

void Gamewidget::keyPressEvent(QKeyEvent *e)
{

    int key=e->key();
if (!game_is_stop)
    {
    if(key == Qt::Key_R)
        game_time_last = game_time_last+10;
    if(key == Qt::Key_L)
        game_time_last = game_time_last-10;
        if (key == Qt::Key_Down) // 按向下方向键时
        {

            if (is_rotating)
            {
                //qDebug() << "z";
                need_grabing = true;
                need_rotating = false;
                mysound_shenchuqu->play();
            }
            else{}
        }

        else if (key == Qt::Key_Space) // 按S时
        {
            pause_clicked();
            game_is_stop = true;

        }
    }

else
{
    if (key == Qt::Key_Space) // 按向下方向键时
    {
        resume_clicked();
        game_is_stop = false;
    }
}
}

void Gamewidget::to_grab()
{
    if (jiasu_time_last == 0)
        grab_speed = 2;
    else
        grab_speed = 5;
    static double length_change = grab_speed;//初始状态随这个。后面自己会变化控制长短
    int gouzi_x = 600 - (grab_length+60) * sin(grab_angle*PI/180);
    int gouzi_y = 55 + (grab_length+60) * cos(grab_angle*PI/180);
    if (need_grabing == true)

    {
      //qDebug() <<  length_change;
        is_rotating = false;
        is_grabing = true;
        //未抓到的越界状态
        if (((gouzi_x < 30)||gouzi_x > 1170 || gouzi_y > 770)  && has_grabsomething == false) //边缘控制
        {   length_change = -grab_speed * 5;//一般状态，空手回去速度快
            grab_length = grab_length + length_change;
         }
        //未抓到的返回完毕状态
        else if(grab_length < 0 && has_grabsomething == false)
        {
            //qDebug() << grab_length;
            need_grabing = false;
            grab_length = 0;
            need_rotating = true;
            length_change = grab_speed;//一般状态,没抓到东西回去夸一点

        }

        //未抓东西的普通状态
        else if (has_grabsomething == false)
        {
            if(length_change > 0)
                 length_change = grab_speed;//去的时候根据当前速度修正
            grab_length = grab_length + length_change;//一般状态
            for (int i = 0; i < my_items.size(); i++)
            {
                if (my_items[i]->detection(gouzi_x,gouzi_y))
                {
                    //paint_timer->stop();
                    has_grabsomething = true;
                    grab_item = i;
                    //只有抓到海绵宝宝才播放难抓的音效
                    if(my_items[grab_item]->get_kind() == 6)
                    {
                        mysound_ladao->setLoops(QSound::Infinite);
                        mysound_ladao->play();
                    }
                    //qDebug() << "has grab some";
                    length_change = -grab_speed * my_items[grab_item]->get_speed() * 1/8 ;
                    grab_length = grab_length + length_change;
                     //grab_speed = -my_items[ grab_item]->get_speed();
                    break;
                }
            }
        }

        //抓了东西的普通状态
        else if (grab_length >= 0 && has_grabsomething == true) {
            {
                my_lables[grab_item]->move(gouzi_x - my_items[grab_item]->get_width()/2 - sin((grab_angle*PI/180)) * 10 ,gouzi_y -  my_items[grab_item]->get_height()/2 +  cos(grab_angle*PI/180) * 10);// 中心点挂碰撞点
           grab_length = grab_length + length_change;
            }
        }

        //抓了东西的返回完毕状态
        else if (grab_length < 0 && has_grabsomething == true)
        {
            mysound_ladao->stop();
            my_score = my_score + my_items.at(grab_item)->get_price();
            current_kind = my_items.at(grab_item)->get_kind();
            my_lables.at(grab_item)->setVisible(false);
            delete my_lables.at(grab_item);//返还初始化用new申请的内存
            delete my_items.at(grab_item);
            my_lables.erase(my_lables.begin()+grab_item);//擦除返还后的地址
            my_items.erase(my_items.begin()+grab_item);
            //qDebug() << "grab done" << grab_item;
            has_grabsomething = false;
            need_grabing = false;
            grab_length = 0;
            need_rotating = true;
            length_change =  grab_speed;//恢复方向,恢复因为抓取物品影响的速度

            ui->lcdNumber->display(my_score);//抓取之后的分数变化

            lable_display();
        }
        update();
    }
    else
    {}
}


void Gamewidget::lable_display()
{
    lable_timer->start(5000);//决定抓到之后lable显示的时间，这里是3S
    switch (current_kind)
    {
    case 0:ui->label_2->setText("鱼鱼+10，一条小鱼!");mysound_get->play(); break;
    case 1:ui->label_2->setText("鱼鱼+20，一条比较大的鱼!");mysound_get->play();break;
    case 2:ui->label_2->setText("鱼鱼+30，一条大鱼!");mysound_get->play();break;
    case 3:ui->label_2->setText("抓到海马！加速20S！");jiasu_time_last = jiasu_time_last + 20;mysound_get->play();break;
    case 4:ui->label_2->setText("海绵宝宝戴的手表，你的剩余时间+30S");game_time_add = 30; mysound_get->play();break;
    case 5:ui->label_2->setText("被鲨鱼啃了一口，吃了鱼鱼-100！");mysound_get->play();break;
    case 6:ui->label_2->setText("抓到海绵宝宝！鱼鱼+999!时间+1分钟！钓鱼加快！");
        jiasu_time_last = jiasu_time_last + 30;game_time_add = 60;mysound_haimianbaobao->play();break;
    default:break;
    }
    //ui->label_2->setText("65436drgfsdeg");// 这句话会拖慢第一次拉到的速度,如果用move？用setvisible都会拖慢，判断是从无到出现的过程最耗时间，那我就从开始用“空格”代表不可见
    //空格也失败，只能初始让lable_2有个小点可见，这样第一次没有延迟
    ui->label_2->setVisible(true);

}

void Gamewidget::game_time()
{
    game_time_last--;//游戏剩余时间
    if(game_time_add != 0)
    {
        game_time_last =  game_time_last + game_time_add;
        game_time_add = 0;
        myend_window->setVisible(false);
    }

    if(jiasu_time_last!=0)
        jiasu_time_last--;
    if(game_time_last < 0)
    {
        myend_window->setVisible(true);
        this->setVisible(false);
        QString  my_score_label_string = "您的鱼鱼";
        QString  my_score_string = QString::number(my_score);
        my_score_label_string.append(my_score_string);
        my_score_label_string.append("条！");
        myend_window->get_score_lable()->setText(my_score_label_string);
        game_pause();
    }

        //游戏结束窗口出现//输入什么之后又可以开始，游戏结束窗口应该是main窗口的孩子，暂停窗口是这个窗口的孩子

}


void Gamewidget::game_pause()
{
    paint_timer->stop();
    grab_timer->stop();
    ship_timer->stop();
    zidong_born->stop();
    lable_timer->stop();
    seconds_timer->stop();
}

void Gamewidget::game_resume_notime()
{
    if (myend_window->getlineedit()->text() == "我还想玩！")
    {
        paint_timer->start(15);
        grab_timer->start(10);
        ship_timer->start(500);
        zidong_born->start(1000*add_time_interval);
        //lable_timer->stop();
        seconds_timer->start(1000);
        this->setVisible(true);
        myend_window->setVisible(false);
        myend_window->getlineedit()->setText("就知道你还想玩！");
        myend_window->getlineedit()->setVisible(false);
        myend_window->get_tishi_lable()->setVisible(false);
        myend_window->getPushbutton_queding()->setVisible(false);
        //mysound_back->setLoops(QSound::Infinite);
       // mysound_back->play();

        game_time_last = 120;

    }

    else {
        QMessageBox::critical(0, "输入错误" , "你自己看看输对没有?",  QMessageBox::Ok | QMessageBox::Escape , 	0 ); //警告错误框
    }

}
void Gamewidget::game_resume()
{

    if (game_time_last <= 0)

    {   myend_window->getlineedit()->setVisible(true);
        myend_window->get_tishi_lable()->setVisible(true);
        myend_window->getPushbutton_queding()->setVisible(true);
        }
    //要想继续玩，输入多少次“我还想玩”就可以多玩多少个“20s”
    else {
        paint_timer->start(15);
        grab_timer->start(10);
        ship_timer->start(500);
        zidong_born->start(1000*add_time_interval);
        //lable_timer->stop();
        //mysound_back->play();
        seconds_timer->start(1000);
        this->setVisible(true);
        myend_window->setVisible(false);
        //分别对应在游戏中暂停恢复的情况，和游戏结束之后点击“继续游戏的情况
        ;}

}



void Gamewidget::game_restart()
{
    paint_timer->start(15);
    grab_timer->start(10);
    ship_timer->start(500);
    zidong_born->start(1000*add_time_interval);
    seconds_timer->start(1000);
    data_init();
    my_items.clear();
    my_lables.clear();//这里变成了假清空，只是清除地址，但是没有delete lable，造成lable还显示，应该先delete再clear
    ui->label_2->setText(".");
//mysound_back->play();
    items_init(num_init);
    this->setVisible(true); //这里应该又通过了一遍构造函数 X 并没有通过，很明显感觉到数据没有被清理
    myend_window->setVisible(false);

}

void Gamewidget::pause_clicked(){
    game_pause();
    ui->pushButton_pause->setVisible(false);
    ui->pushButton_resume->setVisible(true);
}
void Gamewidget::resume_clicked(){
game_resume();
//mysound_back->play();
ui->label_5->setVisible(false);
ui->pushButton_resume->setVisible(false);
ui->pushButton_pause->setVisible(true);
lable_ship->setVisible(true);


for (int i = 0; i < my_lables.size(); i++)
{
    my_lables[i]->setVisible(true);
    }
}

void Gamewidget::help_clicked(){
    game_pause();
    ui->label_5->setVisible(true);
    ui->pushButton_resume->setVisible(true);
    lable_ship->setVisible(false);
    for (int i = 0; i < my_lables.size(); i++)
    {
        my_lables[i]->setVisible(false);
    }}
