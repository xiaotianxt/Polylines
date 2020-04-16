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

    PolyGon(vector<Line> lines_, size_t type); // ��ʼ��
    PolyGon();
    bool sortlines();  // �����������Ϊ��β������˳���ж��Ƿ���β����
    bool checkcross(); // ��������Ƿ��н���
    void findbound();  // �����������
    double area();     // �����������
    bool checkconv();  // ��������Ƿ���͹�����
    bool in(Point point);
    string Str();
    string showbound();
};
