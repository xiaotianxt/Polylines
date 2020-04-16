#include "Line.h"

Line::Line(double x1, double y1, double x2, double y2) // 用于初始化一条线
{
    Point from_(x1, y1);
    Point to_(x2, y2);

    from = from_;
    to = to_;
}

Line::Line(string x1, string y1, string x2, string y2) // 初始化线
{
    Point from_(atof(x1.c_str()), atof(y1.c_str()));
    Point to_(atof(x2.c_str()), atof(y2.c_str()));

    from = from_;
    to = to_;
}

Line::Line(Point from_, Point to_) // 用于初始化一条线，用于判断点是否在多边形内部
{
    from = from_;
    to = to_;
}

string Line::Str() // 用于打印某点坐标（调试用）
{
    string temp = from.Str() + " --> " + to.Str();
    return temp;
}

void Line::Swap() // 用于交换from和to
{
    Point temp = from;
    from = to;
    to = temp;
    return;
}

bool Line::on(Point point) // 判断点在线段上
{
    if ((from.x - point.x) * (to.x - point.x) > 0 || (from.y - point.y) * (to.y - point.y) > 0) // 点如果不在线段所在的外包矩形上则一定不在线段上
    {
        return false;
    }
    else
    {
        if (
            ((from - point) ^ (to - point), 0.0)) // 点如果与from to 同向则一定在线段上
        {
            return true;
        }
        return false;
    }
}

double Line::operator^(const Line &b) const // 重载向量和向量叉积
{
    return ((this->to) - (this->from)) ^ (b.to - b.from);
}

bool CheckCross(Line a, Line b) // 查看两条线是否相交（不规范相交也算
{
    double p1 = (a.to - a.from) ^ (b.from - a.from); // 用于记录所有叉积
    double p2 = (a.to - a.from) ^ (b.to - a.from);
    double p3 = (b.to - b.from) ^ (a.from - b.from);
    double p4 = (b.to - b.from) ^ (a.to - b.from);

    /***************** 非规范相交 ******************/

    if (a.on(b.from) || a.on(b.to) || b.on(a.from) || b.on(a.to)) // 但凡有点在线段上，都是非规范相交
    {
        return true;
    }

    /***************** 判断规范相交 ******************/
    if (p1 * p2 < 0 && p3 * p4 < 0) // 全部异号，说明规范相交
    {
        return true;
    }
    else
    {
        return false; // 否则不相交
    }
}
