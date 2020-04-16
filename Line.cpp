#include "Line.h"

Line::Line(double x1, double y1, double x2, double y2) // ���ڳ�ʼ��һ����
{
    Point from_(x1, y1);
    Point to_(x2, y2);

    from = from_;
    to = to_;
}

Line::Line(string x1, string y1, string x2, string y2) // ��ʼ����
{
    Point from_(atof(x1.c_str()), atof(y1.c_str()));
    Point to_(atof(x2.c_str()), atof(y2.c_str()));

    from = from_;
    to = to_;
}

Line::Line(Point from_, Point to_) // ���ڳ�ʼ��һ���ߣ������жϵ��Ƿ��ڶ�����ڲ�
{
    from = from_;
    to = to_;
}

string Line::Str() // ���ڴ�ӡĳ�����꣨�����ã�
{
    string temp = from.Str() + " --> " + to.Str();
    return temp;
}

void Line::Swap() // ���ڽ���from��to
{
    Point temp = from;
    from = to;
    to = temp;
    return;
}

bool Line::on(Point point) // �жϵ����߶���
{
    if ((from.x - point.x) * (to.x - point.x) > 0 || (from.y - point.y) * (to.y - point.y) > 0) // ����������߶����ڵ������������һ�������߶���
    {
        return false;
    }
    else
    {
        if (
            ((from - point) ^ (to - point), 0.0)) // �������from to ͬ����һ�����߶���
        {
            return true;
        }
        return false;
    }
}

double Line::operator^(const Line &b) const // �����������������
{
    return ((this->to) - (this->from)) ^ (b.to - b.from);
}

bool CheckCross(Line a, Line b) // �鿴�������Ƿ��ཻ�����淶�ཻҲ��
{
    double p1 = (a.to - a.from) ^ (b.from - a.from); // ���ڼ�¼���в��
    double p2 = (a.to - a.from) ^ (b.to - a.from);
    double p3 = (b.to - b.from) ^ (a.from - b.from);
    double p4 = (b.to - b.from) ^ (a.to - b.from);

    /***************** �ǹ淶�ཻ ******************/

    if (a.on(b.from) || a.on(b.to) || b.on(a.from) || b.on(a.to)) // �����е����߶��ϣ����Ƿǹ淶�ཻ
    {
        return true;
    }

    /***************** �жϹ淶�ཻ ******************/
    if (p1 * p2 < 0 && p3 * p4 < 0) // ȫ����ţ�˵���淶�ཻ
    {
        return true;
    }
    else
    {
        return false; // �����ཻ
    }
}
