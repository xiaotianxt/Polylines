#pragma once
#include "util.h"
#include "Line.h"

using namespace std;

class PolyGon
{
public:
    vector<Line> lines;
    size_t type = 0;
    double bound_yu, bound_yd, bound_xl, bound_xr;

    PolyGon(vector<Line> lines_, size_t type); // 初始化
    PolyGon();
    bool sortlines();  // 将多边形整理为首尾相连（顺便判断是否首尾相连
    bool checkcross(); // 检查多边形是否有交叉
    void findbound();  // 查找外包矩形
    double area();     // 计算多边形面积
    bool checkconv();  // 检查多边形是否是凸多边形
    bool in(Point point);
    string Str();
    string showbound();
};
