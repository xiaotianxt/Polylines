#include "util.h"

const double eps = 1e-8; // double precision

inline int sig(double x) { return (x > eps) - (x < -eps); }

inline bool equal(double a, double b) // 用于判断两个浮点数是否相等。精度为eps.
{
    if ((a - b) > -eps && (a - b) < eps) // 防止误差
        return true;
    else
        return false;
}

Point::Point(double x_, double y_) // 用于初始化Point
{
    x = x_;
    y = y_;
}

Point::Point(string x_, string y_)
{
    x = atof(x_.c_str()); // string转换为小数
    y = atof(y_.c_str());
}

Point::Point() // 用于临时生成一个Point
{
    x = 0;
    y = 0;
}

string Point::Str() // 用于输出点的坐标（调试用）
{
    string temp = "(" + to_string(x) + " , " + to_string(y) + ")"; 
    return temp;
}

/*
Point对比原则：
    横坐标大的点更大
    横坐标一样的点，纵坐标大的点更大
*/
bool Point::operator<(const Point& other) const
{
    if (this->x < other.x) // 自己横坐标更小
        return true;
    else if (this->x == other.x) //横坐标相等比较纵坐标
    {
        if (this->y < other.y) // 自己纵坐标更小
        {
            return true;
        }
    }
    return false; // 其他情况下，自己都未必小
}

bool Point::operator>(const Point& other) const
{
    if (this->x > other.x) // 参考上面函数
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

bool Point::operator==(const Point& other) const // 判断两坐标是否相等（补偿误差）
{
    return (equal(this->x, other.x) && equal(this->y, other.y)); 
}

Point Point::operator-(const Point& other) const // 重载减法运算
{
    Point temp(this->x - other.x, this->y - other.y); 
    return temp;
}

Point Point::operator+(const Point& other) const // 重载加法运算
{
    Point temp(this->x + other.x, this->y + other.y);
    return temp;
}

double Point::operator*(const Point& other) const // 重载点积
{
    return x * other.x + y * other.y;
}

double Point::operator^(const Point& other) const // 重载叉积（可以理解为z方向坐标全为0的向量叉积，结果看做一维）
{
    return x * other.y - y * other.x;
}

Line::Line(double x1, double y1, double x2, double y2) // 用于初始化一条线
{
    Point from_(x1, y1);
    Point to_(x2, y2);

    from = from_;
    to = to_;
}

Line::Line(string x1, string y1, string x2, string y2) // 初始化线
{
    Point from_(atof(x1.c_str()), atof(y1.c_str()));
    Point to_(atof(x2.c_str()), atof(y2.c_str()));

    from = from_;
    to = to_;
}

Line::Line(Point from_, Point to_) // 用于初始化一条线，用于判断点是否在多边形内部
{
    from = from_;
    to = to_;
}

string Line::Str() // 用于打印某点坐标（调试用）
{
    string temp = from.Str() + " --> " + to.Str();
    return temp;
}

void Line::Swap() // 用于交换from和to
{
    Point temp = from;
    from = to;
    to = temp;
    return;
}

bool Line::on(Point point) // 判断点在线段上
{
    if ((from.x - point.x) * (to.x - point.x) > 0 || (from.y - point.y) * (to.y - point.y) > 0) // 点如果不在线段所在的外包矩形上则一定不在线段上
    {
        return false;
    }
    else
    {
        if (equal((from - point) ^ (to - point), 0.0)) // 点如果与from to 同向则一定在线段上
        {
            return true;
        }
        return false;
    }
}

double Line::operator^(const Line &b) const // 重载向量和向量叉积
{
    return ((this->to) - (this->from)) ^ (b.to - b.from);
}

Polygon::Polygon(vector<Line> lines_, size_t type_) // 多边形初始化（并判断多边形类型）
{
    lines = lines_;
    type = type_;
    bound_xl = 0;
    bound_xr = 0;
    bound_yd = 0;
    bound_yu = 0;
}

Polygon::Polygon() {}

bool Polygon::sortlines()
// 用于寻找端点
{
    size_t lines_index = 0;                                              // 设置线段索引
    Point last;                                                  // 最后一张
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

bool Polygon::checkcross()
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

void Polygon::findbound() // 查找多边形的外包矩形（此时每条边的所有from正好遍历了所有点
{
    bound_yu = lines[0].from.y;  // yu, yd 对应上界下界
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

double Polygon::area()
{
    double ans = 0; // 用于存放计算结果
    for (int i = 0; i + 1 < lines.size(); i++) // 计算方式（向量三角形面积求和）
    {
        ans += lines[i].from.x * lines[i + 1].from.y;
        ans -= lines[i].from.y * lines[i + 1].from.x;
    }
    ans += lines.back().from.x * lines[0].from.y;
    ans -= lines.back().from.y * lines[0].from.x;
    return abs( ans / 2);
}

bool Polygon::checkconv() // 检查原则，从第一条边开始，查看下一条边和次边的叉积是否同号
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

bool Polygon::in(Point point) // 检查点是否在多边形内部
{
    /***************** 检查是否在多边形上 ******************/
    for (size_t i = 0; i < lines.size(); i++)
    {
        if (equal(point.x, lines[i].from.x) && equal(point.y, lines[i].from.y)) // 如果点在交点处
        { // 点在边界上
            return false;
        }
        else
        {
            double d1 = (point - lines[i].from) ^ (lines[i].to - lines[i].from); // 自from出发，向point的矢量 x 向to的矢量
            double d2 = (point - lines[i].from) ^ (lines[i].from - lines[i].to); // 自to出发，向point的矢量 x 向from的矢量
            if (equal(d1, 0.0) && equal(d2, 0.0)) // 说明在边*所在的直线*上
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
                        if (i == j) continue;
                        
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
        Line radial(point, to[ito]);                   // 新建一个射线
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

string Polygon::Str() // 将多边形坐标按照字符串输出
{
    string temp = "  ";
    for (size_t i = 0; i < lines.size(); i++)
    {
        temp += lines[i].Str() + "\n  ";
    }
    return temp;
}

string Polygon::showbound() // 想外包矩形按照字符串输出
{
    string temp = "x:  " + to_string(bound_xl) + "<--->" + to_string(bound_xr) + "\n  y:  " + to_string(bound_yd) + "<--->" + to_string(bound_yu) + "\n\n";
    return temp;
}

bool CheckCross(Line a, Line b) // 查看两条线是否相交（不规范相交也算
{
    double p1 = (a.to - a.from) ^ (b.from - a.from); // 用于记录所有叉积
    double p2 = (a.to - a.from) ^ (b.to - a.from);
    double p3 = (b.to - b.from) ^ (a.from - b.from);
    double p4 = (b.to - b.from) ^ (a.to - b.from);  

    /***************** 非规范相交 ******************/

    if (a.on(b.from) || a.on(b.to) || b.on(a.from) || b.on(a.to)) // 但凡有点在线段上，都是非规范相交
    {
        return true;
    }
    
    /***************** 判断规范相交 ******************/
    if (p1 * p2 < 0 && p3 * p4 < 0) // 全部异号，说明规范相交
    {
        return true;
    }
    else
    {
        return false; // 否则不相交
    }
}