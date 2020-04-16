#include "Polyline.h"

PolyLine::PolyLine(vector<Point> _points, string _mode) : points(_points), mode(_mode) // 初始化
{
    findlenth();
}

void PolyLine::findlenth()
{
    lenths.push_back(0.0); // 第一个点对应的长度是0
    lenth = 0.0;
    for (size_t i = 1; i < points.size(); i++)
    {
        lenths.push_back(FindLenth(points[i - 1], points[i]));
        lenth += lenths.back();
    }
}

string PolyLine::MarkerStr()
{
    string temp = "";
    for (int i = 0; i < this->markers.size(); i++)
    {
        temp += ' ' + this->markers[i].Str() + "\n";
    }
    return temp;
}

ostream &operator<<(ostream &out, const PolyLine &polyline)
{
    for (int i = 0; i < polyline.points.size(); i++)
        out << polyline.points[i].Str() << '\n';
    return out;
}
