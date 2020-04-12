#pragma once
#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <stack>
#include <cstring>
#include <string>
#include <cstdio>
#include <time.h>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <regex>

using namespace std;

bool equal(double a, double b);

class Point //可以叉乘和点乘
{
public:
    double x, y;
    int id = 0;
    Point(double x_, double y_);
    Point(string x_, string y_);
    Point();
    string Str();                        // 打印该点的坐标（调试用）
    bool operator<(const Point &) const; // 用于排列坐标（好像没啥用了
                                         // TODO:
    bool operator>(const Point &) const;
    bool operator==(const Point &) const; // 判断相等

    Point operator-(const Point &) const;  // 减法
    Point operator+(const Point&) const;    // 加法
    double operator*(const Point &)const;  // 点积
    double operator^(const Point &) const; // 二维叉积
};

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
    double operator^(const Line &b) const;
};

class Polygon
{
public:
    vector<Line> lines;
    size_t type = 0;
    double bound_yu, bound_yd, bound_xl, bound_xr;

    Polygon(vector<Line> lines_, size_t type); // 初始化
    Polygon();
    bool sortlines();  // 将多边形整理为首尾相连（顺便判断是否首尾相连
    bool checkcross(); // 检查多边形是否有交叉
    void findbound(); // 查找外包矩形
    double area(); // 计算多边形面积
    bool checkconv(); // 检查多边形是否是凸多边形
    bool in(Point point);
    string Str();
    string showbound();
};

bool CheckCross(Line a, Line b);
