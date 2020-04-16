#include "Point.h"

Point::Point(double x_, double y_) // 用于初始化Point
{
    x = x_;
    y = y_;
}

Point::Point(string x_, string y_)
{
    x = atof(x_.c_str()); // string转换为小数
    y = atof(y_.c_str());
}

Point::Point() // 用于临时生成一个Point
{
    x = 0;
    y = 0;
}

string Point::Str() const // 用于输出点的坐标（调试用）
{
    string temp = "(" + to_string(x) + " , " + to_string(y) + ")";
    return temp;
}

/*
Point对比原则：
    横坐标大的点更大
    横坐标一样的点，纵坐标大的点更大
*/
bool Point::operator<(const Point &other) const
{
    if (this->x < other.x) // 自己横坐标更小
        return true;
    else if (this->x == other.x) //横坐标相等比较纵坐标
    {
        if (this->y < other.y) // 自己纵坐标更小
        {
            return true;
        }
    }
    return false; // 其他情况下，自己都未必小
}

bool Point::operator>(const Point &other) const
{
    if (this->x > other.x) // 参考上面函数
    {
        return true;
    }
    else if (this->x == other.x)
    {
        if (this->y > other.y)
        {
            return true;
        }
    }
    return false;
}

bool Point::operator==(const Point &other) const // 判断两坐标是否相等（补偿误差）
{
    return (equal(this->x, other.x) && equal(this->y, other.y));
}

Point Point::operator-(const Point &other) const // 重载减法运算
{
    Point temp(this->x - other.x, this->y - other.y);
    return temp;
}

Point Point::operator+(const Point &other) const // 重载加法运算
{
    Point temp(this->x + other.x, this->y + other.y);
    return temp;
}

double Point::operator*(const Point &other) const // 重载点积
{
    return x * other.x + y * other.y;
}

double Point::operator^(const Point &other) const // 重载叉积（可以理解为z方向坐标全为0的向量叉积，结果看做一维）
{
    return x * other.y - y * other.x;
}

double FindLenth(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
