#pragma once
#include "util.h"
#include "Point.h"

using namespace std;

class PolyLine
{
public:
    vector<Point> points; // 用于记录所有节点
    vector<double> lenths; // 用于记录长度
    vector<Point> markers;//用于记录标记点的长度
    vector<Point> markrects; // 用于记录标记矩形左上方的坐标
    double lenth = 0.0;
    string mode;

    
    PolyLine(vector<Point> _points, string mode); // 初始化    
    void findlenth(); // 计算长度    
    string MarkerStr(); // 用于输出标记结果
    friend ostream& operator<<(ostream& out, const PolyLine &polygon);
};

ostream& operator<<(ostream& out, const PolyLine& polyline);
