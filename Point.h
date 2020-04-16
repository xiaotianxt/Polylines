#pragma once
#include "util.h"

using namespace std;

class Point //���Բ�˺͵��
{
public:
    double x, y;
    int id = 0;
    Point(double x_, double y_);
    Point(string x_, string y_);
    Point();
    string Str() const;                  // ��ӡ�õ�����꣨�����ã�
    bool operator<(const Point &) const; // �����������꣨����ûɶ����
                                         // TODO:
    bool operator>(const Point &) const;
    bool operator==(const Point &) const; // �ж����

    Point operator-(const Point &) const;  // ����
    Point operator+(const Point &) const;  // �ӷ�
    double operator*(const Point &)const;  // ���
    double operator^(const Point &) const; // ��ά���
};

double FindLenth(Point a, Point b);
