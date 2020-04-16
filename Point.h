#pragma once
#include "util.h"

using namespace std;

class Point //可以叉乘和点乘
{
public:
    double x, y;
    int id = 0;
    Point(double x_, double y_);
    Point(string x_, string y_);
    Point();
    string Str() const;                  // 打印该点的坐标（调试用）
    bool operator<(const Point &) const; // 用于排列坐标（好像没啥用了
                                         // TODO:
    bool operator>(const Point &) const;
    bool operator==(const Point &) const; // 判断相等

    Point operator-(const Point &) const;  // 减法
    Point operator+(const Point &) const;  // 加法
    double operator*(const Point &)const;  // 点积
    double operator^(const Point &) const; // 二维叉积
};

double FindLenth(Point a, Point b);
