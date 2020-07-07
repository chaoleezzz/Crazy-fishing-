#include "item.h"
#include <math.h>

Item::Item()
{
    x=y=height=width=kind=0;
    visible=true;
}

Item::Item(int a,int b,int c,int d,int e,int f,double g, bool h)
{
    x = a;
    y = b;
    height = c;
    width = d;
    price = e;
    kind = f;
    speed = g;
    visible = h;

}
Item::~Item()
{

}
void Item::set_position(const int &xx, const int &yy)
{
    x=xx;
    y=yy;
}
void Item::set_size(const int &wwidth, const int &hheight)
{
    width=wwidth;
    height=hheight;
}
void Item::set_price(const int &pprice)
{
    price=pprice;
}
void Item::set_speed(const double &sspeed)
{
    speed=sspeed;
}
void Item::set_kind(const int &kkind)
{
    kind=kkind;
}

void Item::set_visible()//设置为可见
{
    visible=true;
}

void Item::set_invisible()//设置为不可见
{
    visible=false;
}


bool Item::detection(const int &xx,const int &yy)//item碰撞检测,必须可见才启用碰撞检测,采用中心点碰撞检测
{
    if(visible==true&& (abs(xx - (x+width/2)) < width/2) && (abs(yy - (y+height/2)) < height/2))//+5、-5是为了缩小碰撞检测范围
        return true;
    return false;
}

