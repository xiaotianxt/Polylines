#pragma once
#include "util.h"
#include "Point.h"
using namespace std;

class Line
{
public:
    Point from, to;                     // ��Ϊ�����û�з��򣬹涨���ɵĵ��Сָ��󣨵�Ȼ�������ĵ�����һ�����ܸĳ�Ϊ�ߣ�Ӧ������
    Line(double, double, double, double); // ��ʼ��
    Line(string, string, string, string);
    Line(Point, Point);
    string Str(); // debug�ã���ӡ��
    void Swap();  // ����from��to
    bool on(Point); // �жϵ��Ƿ��ڱ���
    double operator^(const Line& b) const;
};

bool CheckCross(Line a, Line b);// �鿴�������Ƿ��ཻ�����淶�ཻҲ��

