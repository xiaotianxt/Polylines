#pragma once
#include "util.h"
#include "Point.h"
using namespace std;

class Line
{
public:
    Point from, to;                     // 因为多边形没有风向，规定生成的点从小指向大（当然如果输入的点坐标一样不能改称为边，应当报错）
    Line(double, double, double, double); // 初始化
    Line(string, string, string, string);
    Line(Point, Point);
    string Str(); // debug用，打印线
    void Swap();  // 交换from和to
    bool on(Point); // 判断点是否在边上
    double operator^(const Line& b) const;
};

bool CheckCross(Line a, Line b);// 查看两条线是否相交（不规范相交也算

