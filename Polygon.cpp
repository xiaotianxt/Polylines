#include "Polygon.h"

PolyGon::PolyGon(vector<Line> lines_, size_t type_) // 多边形初始化（并判断多边形类型）
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
// 用于寻找端点
{
    size_t lines_index = 0;                                              // 设置线段索引
    Point last;                                                          // 最后一张
    bool flag = 0;                                                       // 用于标记是否完成匹配
    for (lines_index = 0; lines_index + 1 < lines.size(); lines_index++) // 只要
    {
        last = lines[lines_index].to; // 以lines[0].to作为起点查看是否可以将所有线串起来
        flag = 0;
        for (int target = lines_index + 1; target < lines.size(); target++) // 注意lines_index截止到倒数第二位
        {
            if ((lines[target].from) == last) // 如果匹配到了from
            {
                swap(lines[target], lines[lines_index + 1]); // 将target对应的线和lines_index下一个互换
                flag = 1;
            }
            else if ((lines[target].to) == last) // 如果to和last匹配上
            {
                lines[target].Swap();                        // 首先需要互换from和to
                swap(lines[target], lines[lines_index + 1]); // 其次将该条线放在lines_index下一个
                flag = 1;
            }
        }
        if (!flag) // 未完成匹配，说明输入数据很可能有误
        {
            cout << "请检查您的多边形文件输入是否有误！！" << endl;
            return false;
        }
    }
    assert((lines[0].from) == (lines.back().to)); // 应当首尾相接
    return true;
}

bool PolyGon::checkcross()
{
    if (lines.size() < 3) // 此时不可能组成多边形
    {
        cout << "只输入了" << lines.size() << "条边，无法构成多边形" << endl;
        return false;
    }
    else
    {
        for (size_t i = 0; i + 2 < lines.size(); i++) // 遍历所有对边
        {
            if (CheckCross(lines[i], lines[i + 2])) // 如果有交叉
            {
                cout << "您输入的边：" << lines[i].Str() << "和" << lines[i + 2].Str() << "之间有交叉" << endl;
                return false;
            }
            else
                NULL;
        }
    }
    return true;
}

void PolyGon::findbound() // 查找多边形的外包矩形（此时每条边的所有from正好遍历了所有点
{
    bound_yu = lines[0].from.y; // yu, yd 对应上界下界
    bound_yd = lines[0].from.y;
    bound_xl = lines[0].from.x; // xl, xr 对应左界右界
    bound_xr = lines[0].from.x;
    for (size_t i = 0; i < lines.size(); i++) // 遍历所有边，寻找最大值
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
    double ans = 0;                            // 用于存放计算结果
    for (int i = 0; i + 1 < lines.size(); i++) // 计算方式（向量三角形面积求和）
    {
        ans += lines[i].from.x * lines[i + 1].from.y;
        ans -= lines[i].from.y * lines[i + 1].from.x;
    }
    ans += lines.back().from.x * lines[0].from.y;
    ans -= lines.back().from.y * lines[0].from.x;
    return abs(ans / 2);
}

bool PolyGon::checkconv() // 检查原则，从第一条边开始，查看下一条边和次边的叉积是否同号
{
    double temp = lines.back() ^ lines[0]; // 将最后一个和第一个做叉积
    for (size_t i = 0; i + 1 < lines.size(); i++)
    {
        if (temp * (lines[i] ^ lines[i + 1]) < 0) // 如果叉积异号，说明非凸多边形
        {
            return false;
        }
    }
    return true;
}

bool PolyGon::in(Point point) // 检查点是否在多边形内部
{
    /***************** 检查是否在多边形上 ******************/
    for (size_t i = 0; i < lines.size(); i++)
    {
        if (equal(point.x, lines[i].from.x) && equal(point.y, lines[i].from.y)) // 如果点在交点处
        {                                                                       // 点在边界上
            return false;
        }
        else
        {
            double d1 = (point - lines[i].from) ^ (lines[i].to - lines[i].from); // 自from出发，向point的矢量 x 向to的矢量
            double d2 = (point - lines[i].from) ^ (lines[i].from - lines[i].to); // 自to出发，向point的矢量 x 向from的矢量
            if (equal(d1, 0.0) && equal(d2, 0.0))                                // 说明在边*所在的直线*上
            {
                double tempx = (point.x - lines[i].from.x) * (point.x - lines[i].to.x);
                double tempy = (point.y - lines[i].from.y) * (point.y - lines[i].to.y);
                if (tempx > 0 || tempy > 0) // 在同一条直线上，但在该线段之外
                {
                    // 注意对于凸多边形，此时已经可以点在多边形外，但是对于凹多边形，还需要判断是否能从另一个方向上找到交点
                    Point temp = point + point - lines[i].to;
                    Line radial(point, temp);
                    for (size_t j = 0; j < lines.size(); j++)
                    {
                        if (i == j)
                            continue;

                        if (CheckCross(radial, lines[j])) // 从另一个方向上判断和其他的边是否有交点
                        {
                            //点在内部
                            return true;
                        }
                    }
                    // 对于相反方向上没有交点的，说明在外部
                    // cout << "点在外部" << endl;
                    return false;
                }
                else
                {
                    // cout << "点在边上" << endl;
                    return false;
                }
            }
        }
    }

    /***************** 检查是否和多边形交点数量 ******************/
    // 由于需要避免遇到交点，四个方向都试一下取最小的
    // Point to(bound_xr + 1, point.y);          // 新建一个点，与判断的点平行，且在外包矩形的外边（充当射线的“末尾”）
    Point to[4];
    to[0] = Point(bound_xr + 1, point.y);
    to[1] = Point(bound_xl - 1, point.y);
    to[2] = Point(point.x, bound_yu + 1);
    to[3] = Point(point.x, bound_yd - 1);
    int min = 100;

    for (int ito = 0; ito < 4; ito++)
    {
        Line radial(point, to[ito]);              // 新建一个射线
        int count = 0;                            // 计数器
        for (size_t i = 0; i < lines.size(); i++) // 对于每一条边，遍历查看射线是否穿过多边形
        {
            if (CheckCross(radial, lines[i])) // 如果交叉
            {
                count++;
            }
        }
        if (count < min)
            min = count;
        count = 0;
    }

    if (min == 2 || min == 0) //极端情况，此时说明该射线正好交在了两条边的交点
    {
        //cout << "点在外部" << endl;
        return false; // 不在
    }
    else
    {
        //cout << "点在内部" << endl;
        return true;
    }
}

string PolyGon::Str() // 将多边形坐标按照字符串输出
{
    string temp = "  ";
    for (size_t i = 0; i < lines.size(); i++)
    {
        temp += lines[i].Str() + "\n  ";
    }
    return temp;
}

string PolyGon::showbound() // 想外包矩形按照字符串输出
{
    string temp = "x:  " + to_string(bound_xl) + "<--->" + to_string(bound_xr) + "\n  y:  " + to_string(bound_yd) + "<--->" + to_string(bound_yu) + "\n\n";
    return temp;
}
