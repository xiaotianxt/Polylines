#pragma once
#include "util.h"
#include "Point.h"

using namespace std;

class PolyLine
{
public:
    vector<Point> points;    // 折线点坐标
    vector<double> lenths;   // 折线i-1到i坐标的长度
    vector<Point> markers;   // 所有标记点
    vector<Point> markrects; // 所有标记点的标记矩形的左上角坐标
    double lenth = 0.0;      // 总长度
    string mode;             // 用于存储模式

    PolyLine(vector<Point> _points, string mode); // 生成PolyLine
    void findlenth();                             // 计算长度
    string MarkerStr();                           // 输出Marker坐标, 以string格式
    friend ostream &operator<<(ostream &out, const PolyLine &polygon);
};

// 运算符重载
ostream &operator<<(ostream &out, const PolyLine &polyline);
