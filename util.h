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

class Point //���Բ�˺͵��
{
public:
    double x, y;
    int id = 0;
    Point(double x_, double y_);
    Point(string x_, string y_);
    Point();
    string Str();                        // ��ӡ�õ�����꣨�����ã�
    bool operator<(const Point &) const; // �����������꣨����ûɶ����
                                         // TODO:
    bool operator>(const Point &) const;
    bool operator==(const Point &) const; // �ж����

    Point operator-(const Point &) const;  // ����
    Point operator+(const Point&) const;    // �ӷ�
    double operator*(const Point &)const;  // ���
    double operator^(const Point &) const; // ��ά���
};

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
    double operator^(const Line &b) const;
};

class Polygon
{
public:
    vector<Line> lines;
    size_t type = 0;
    double bound_yu, bound_yd, bound_xl, bound_xr;

    Polygon(vector<Line> lines_, size_t type); // ��ʼ��
    Polygon();
    bool sortlines();  // �����������Ϊ��β������˳���ж��Ƿ���β����
    bool checkcross(); // ��������Ƿ��н���
    void findbound(); // �����������
    double area(); // �����������
    bool checkconv(); // ��������Ƿ���͹�����
    bool in(Point point);
    string Str();
    string showbound();
};

bool CheckCross(Line a, Line b);
