#ifndef ITEM_H
#define ITEM_H


class Item
{
public:
    Item();
    Item(int,int,int,int,int,int,double,bool);
    ~Item();
    void set_position(const int &xx,const int &yy);//设置中心位置
    void set_size(const int &wwidth,const int &hheight);
    void set_price(const int &pprice);
    void set_speed(const double &sspeed);
    void set_kind(const int &kkind);
    void set_visible();//设置为可见
    void set_invisible();//设置为不可见

    int get_kind(){return kind;}
    int get_price(){return price;}
    int get_x(){return x;}
    int get_y(){return y;}
    int get_height(){return height;}
    int get_width(){return width;}
    double get_speed(){return speed;}

    bool is_visible(){return visible;}

    bool detection(const int &xx,const int &yy);//碰撞检测
private:
    int x,y;//该物体的坐标
    int height,width;//用于区分矿石的大小，影响勾到的难易程度
    int price;
    int kind;//0表示普通物质，1为加速，2为减速，或者一些其他物质 勾到之后首先判断kind
    //0:小鱼，分数1，40*40图片
    //1:中鱼，分数2，80*80图片
    //2:大鱼，分数3，120*120图片
    //3:加速，分数0，40*40图片 （2倍速度）
    //4:加时间，分数0，40*40图片 （加30s）
    //5:大鲨鱼，分数-10,40*40图片
    //6:神奇宝箱，输入我爱臭，分数9999，
    //
    double speed;//大小矿石影响速度
    bool visible;//用于让矿石lable显示属性
};

#endif // ITEM_H
