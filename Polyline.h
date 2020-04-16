#pragma once
#include "util.h"
#include "Point.h"

using namespace std;

class PolyLine
{
public:
    vector<Point> points;    // ���ڼ�¼���нڵ�
    vector<double> lenths;   // ���ڼ�¼����
    vector<Point> markers;   //���ڼ�¼��ǵ�ĳ���
    vector<Point> markrects; // ���ڼ�¼��Ǿ������Ϸ�������
    double lenth = 0.0;
    string mode;

    PolyLine(vector<Point> _points, string mode); // ��ʼ��
    void findlenth();                             // ���㳤��
    string MarkerStr();                           // ���������ǽ��
    friend ostream &operator<<(ostream &out, const PolyLine &polygon);
};

ostream &operator<<(ostream &out, const PolyLine &polyline);
