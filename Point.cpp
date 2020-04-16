#include "Point.h"

Point::Point(double x_, double y_) // ���ڳ�ʼ��Point
{
    x = x_;
    y = y_;
}

Point::Point(string x_, string y_)
{
    x = atof(x_.c_str()); // stringת��ΪС��
    y = atof(y_.c_str());
}

Point::Point() // ������ʱ����һ��Point
{
    x = 0;
    y = 0;
}

string Point::Str() const // �������������꣨�����ã�
{
    string temp = "(" + to_string(x) + " , " + to_string(y) + ")";
    return temp;
}

/*
Point�Ա�ԭ��
    �������ĵ����
    ������һ���ĵ㣬�������ĵ����
*/
bool Point::operator<(const Point &other) const
{
    if (this->x < other.x) // �Լ��������С
        return true;
    else if (this->x == other.x) //��������ȱȽ�������
    {
        if (this->y < other.y) // �Լ��������С
        {
            return true;
        }
    }
    return false; // ��������£��Լ���δ��С
}

bool Point::operator>(const Point &other) const
{
    if (this->x > other.x) // �ο����溯��
    {
        return true;
    }
    else if (this->x == other.x)
    {
        if (this->y > other.y)
        {
            return true;
        }
    }
    return false;
}

bool Point::operator==(const Point &other) const // �ж��������Ƿ���ȣ�������
{
    return (equal(this->x, other.x) && equal(this->y, other.y));
}

Point Point::operator-(const Point &other) const // ���ؼ�������
{
    Point temp(this->x - other.x, this->y - other.y);
    return temp;
}

Point Point::operator+(const Point &other) const // ���ؼӷ�����
{
    Point temp(this->x + other.x, this->y + other.y);
    return temp;
}

double Point::operator*(const Point &other) const // ���ص��
{
    return x * other.x + y * other.y;
}

double Point::operator^(const Point &other) const // ���ز�����������Ϊz��������ȫΪ0������������������һά��
{
    return x * other.y - y * other.x;
}

double FindLenth(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
