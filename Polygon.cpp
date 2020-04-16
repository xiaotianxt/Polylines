#include "Polygon.h"

PolyGon::PolyGon(vector<Line> lines_, size_t type_) // ����γ�ʼ�������ж϶�������ͣ�
{
    lines = lines_;
    type = type_;
    bound_xl = 0;
    bound_xr = 0;
    bound_yd = 0;
    bound_yu = 0;
}

PolyGon::PolyGon() {}

bool PolyGon::sortlines()
// ����Ѱ�Ҷ˵�
{
    size_t lines_index = 0;                                              // �����߶�����
    Point last;                                                          // ���һ��
    bool flag = 0;                                                       // ���ڱ���Ƿ����ƥ��
    for (lines_index = 0; lines_index + 1 < lines.size(); lines_index++) // ֻҪ
    {
        last = lines[lines_index].to; // ��lines[0].to��Ϊ���鿴�Ƿ���Խ������ߴ�����
        flag = 0;
        for (int target = lines_index + 1; target < lines.size(); target++) // ע��lines_index��ֹ�������ڶ�λ
        {
            if ((lines[target].from) == last) // ���ƥ�䵽��from
            {
                swap(lines[target], lines[lines_index + 1]); // ��target��Ӧ���ߺ�lines_index��һ������
                flag = 1;
            }
            else if ((lines[target].to) == last) // ���to��lastƥ����
            {
                lines[target].Swap();                        // ������Ҫ����from��to
                swap(lines[target], lines[lines_index + 1]); // ��ν������߷���lines_index��һ��
                flag = 1;
            }
        }
        if (!flag) // δ���ƥ�䣬˵���������ݺܿ�������
        {
            cout << "�������Ķ�����ļ������Ƿ����󣡣�" << endl;
            return false;
        }
    }
    assert((lines[0].from) == (lines.back().to)); // Ӧ����β���
    return true;
}

bool PolyGon::checkcross()
{
    if (lines.size() < 3) // ��ʱ��������ɶ����
    {
        cout << "ֻ������" << lines.size() << "���ߣ��޷����ɶ����" << endl;
        return false;
    }
    else
    {
        for (size_t i = 0; i + 2 < lines.size(); i++) // �������жԱ�
        {
            if (CheckCross(lines[i], lines[i + 2])) // ����н���
            {
                cout << "������ıߣ�" << lines[i].Str() << "��" << lines[i + 2].Str() << "֮���н���" << endl;
                return false;
            }
            else
                NULL;
        }
    }
    return true;
}

void PolyGon::findbound() // ���Ҷ���ε�������Σ���ʱÿ���ߵ�����from���ñ��������е�
{
    bound_yu = lines[0].from.y; // yu, yd ��Ӧ�Ͻ��½�
    bound_yd = lines[0].from.y;
    bound_xl = lines[0].from.x; // xl, xr ��Ӧ����ҽ�
    bound_xr = lines[0].from.x;
    for (size_t i = 0; i < lines.size(); i++) // �������бߣ�Ѱ�����ֵ
    {
        if (bound_yu < lines[i].from.y)
            bound_yu = lines[i].from.y;
        if (bound_yd > lines[i].from.y)
            bound_yd = lines[i].from.y;
        if (bound_xl > lines[i].from.x)
            bound_xl = lines[i].from.x;
        if (bound_xr < lines[i].from.x)
            bound_xr = lines[i].from.x;
    }
}

double PolyGon::area()
{
    double ans = 0;                            // ���ڴ�ż�����
    for (int i = 0; i + 1 < lines.size(); i++) // ���㷽ʽ�����������������ͣ�
    {
        ans += lines[i].from.x * lines[i + 1].from.y;
        ans -= lines[i].from.y * lines[i + 1].from.x;
    }
    ans += lines.back().from.x * lines[0].from.y;
    ans -= lines.back().from.y * lines[0].from.x;
    return abs(ans / 2);
}

bool PolyGon::checkconv() // ���ԭ�򣬴ӵ�һ���߿�ʼ���鿴��һ���ߺʹαߵĲ���Ƿ�ͬ��
{
    double temp = lines.back() ^ lines[0]; // �����һ���͵�һ�������
    for (size_t i = 0; i + 1 < lines.size(); i++)
    {
        if (temp * (lines[i] ^ lines[i + 1]) < 0) // ��������ţ�˵����͹�����
        {
            return false;
        }
    }
    return true;
}

bool PolyGon::in(Point point) // �����Ƿ��ڶ�����ڲ�
{
    /***************** ����Ƿ��ڶ������ ******************/
    for (size_t i = 0; i < lines.size(); i++)
    {
        if (equal(point.x, lines[i].from.x) && equal(point.y, lines[i].from.y)) // ������ڽ��㴦
        {                                                                       // ���ڱ߽���
            return false;
        }
        else
        {
            double d1 = (point - lines[i].from) ^ (lines[i].to - lines[i].from); // ��from��������point��ʸ�� x ��to��ʸ��
            double d2 = (point - lines[i].from) ^ (lines[i].from - lines[i].to); // ��to��������point��ʸ�� x ��from��ʸ��
            if (equal(d1, 0.0) && equal(d2, 0.0))                                // ˵���ڱ�*���ڵ�ֱ��*��
            {
                double tempx = (point.x - lines[i].from.x) * (point.x - lines[i].to.x);
                double tempy = (point.y - lines[i].from.y) * (point.y - lines[i].to.y);
                if (tempx > 0 || tempy > 0) // ��ͬһ��ֱ���ϣ����ڸ��߶�֮��
                {
                    // ע�����͹����Σ���ʱ�Ѿ����Ե��ڶ�����⣬���Ƕ��ڰ�����Σ�����Ҫ�ж��Ƿ��ܴ���һ���������ҵ�����
                    Point temp = point + point - lines[i].to;
                    Line radial(point, temp);
                    for (size_t j = 0; j < lines.size(); j++)
                    {
                        if (i == j)
                            continue;

                        if (CheckCross(radial, lines[j])) // ����һ���������жϺ������ı��Ƿ��н���
                        {
                            //�����ڲ�
                            return true;
                        }
                    }
                    // �����෴������û�н���ģ�˵�����ⲿ
                    // cout << "�����ⲿ" << endl;
                    return false;
                }
                else
                {
                    // cout << "���ڱ���" << endl;
                    return false;
                }
            }
        }
    }

    /***************** ����Ƿ�Ͷ���ν������� ******************/
    // ������Ҫ�����������㣬�ĸ�������һ��ȡ��С��
    // Point to(bound_xr + 1, point.y);          // �½�һ���㣬���жϵĵ�ƽ�У�����������ε���ߣ��䵱���ߵġ�ĩβ����
    Point to[4];
    to[0] = Point(bound_xr + 1, point.y);
    to[1] = Point(bound_xl - 1, point.y);
    to[2] = Point(point.x, bound_yu + 1);
    to[3] = Point(point.x, bound_yd - 1);
    int min = 100;

    for (int ito = 0; ito < 4; ito++)
    {
        Line radial(point, to[ito]);              // �½�һ������
        int count = 0;                            // ������
        for (size_t i = 0; i < lines.size(); i++) // ����ÿһ���ߣ������鿴�����Ƿ񴩹������
        {
            if (CheckCross(radial, lines[i])) // �������
            {
                count++;
            }
        }
        if (count < min)
            min = count;
        count = 0;
    }

    if (min == 2 || min == 0) //�����������ʱ˵�����������ý����������ߵĽ���
    {
        //cout << "�����ⲿ" << endl;
        return false; // ����
    }
    else
    {
        //cout << "�����ڲ�" << endl;
        return true;
    }
}

string PolyGon::Str() // ����������갴���ַ������
{
    string temp = "  ";
    for (size_t i = 0; i < lines.size(); i++)
    {
        temp += lines[i].Str() + "\n  ";
    }
    return temp;
}

string PolyGon::showbound() // ��������ΰ����ַ������
{
    string temp = "x:  " + to_string(bound_xl) + "<--->" + to_string(bound_xr) + "\n  y:  " + to_string(bound_yd) + "<--->" + to_string(bound_yu) + "\n\n";
    return temp;
}
