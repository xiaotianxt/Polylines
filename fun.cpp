#include "fun.h"

using namespace std;

vector<PolyLine> polylines;
vector<Point> markers_out; // 用于记录被遮挡的点

void Initialize()
{
    /***************** 读取文件 ******************/
    ifstream fin; // 读取文件流
    ofstream fout;
    string temp;           // 临时储存输入
    string file = "input"; // 文件路径
    int num = 0;           // 记录折线数量
    int file_num = 0;
    char mode = 0; // 模式
    cout << "回车默认读取工程目录下的input*.txt，或输入你想要打开的文件路径" << endl;

    char c = getchar(); // 判断是否输入的是回车

    if (c != '\n') // 如果输入的不是回车,说明需要打开自己的文件
    {
        cin.putback(c);
        cin >> file;
    }
    cout << "/***************** 开始读取文件 ******************/" << endl;

    file = file + to_string(file_num) + ".txt";
    fin.open(file);
    if (fin.is_open()) // 打开成功
    {
        smatch result;
        int i = 0, size = 0; // 记录录入的第i组数据
        string mark;         // 用于记录标记点数(规定间隔)
        regex pattern("(A|B)\\s*");
        regex counter("(\\d+)\\s*");                            // 匹配折线数量
        regex catalog("(\\d+)\\s(\\d+\\.?\\d*)\\s*");           // 匹配折线开头的两个数字
        regex data("(-?\\d+\\.?\\d*)\\s*(-?\\d+\\.?\\d*)\\s*"); // 匹配点的坐标
        while (!fin.eof())
        {
            getline(fin, temp); // 获取模式
            try
            {
                if (regex_match(temp, result, pattern)) // 选择匹配成功
                {
                    cout << "你选择的是" << result[1] << endl
                         << "开始读入文件...." << endl;
                    mode = result[1].str()[0]; // 写入mode
                    while (!fin.eof())         // 防止文件读入失败
                    {
                        if (regex_match(temp, result, counter))
                        {
                            num = atoi(result[1].str().c_str());
                        }
                        getline(fin, temp);                     // 首先读入一行数据,用来匹配折线点数+标记点数(折线点数+规定间隔)
                        if (regex_match(temp, result, catalog)) // 匹配到
                        {
                            vector<Point> points;
                            size = atoi(result[1].str().c_str()); // 记录大小
                            mark = result[2].str();
                            cout << "开始读取第" << i + 1 << "条折线" << endl;
                            while (!fin.eof()) // 防止文件读入失败
                            {
                                getline(fin, temp); // 读入一行数据

                                if (regex_match(temp, result, data)) // 匹配坐标
                                {
                                    Point temp_point(result[1], result[2]);
                                    points.push_back(temp_point); // 放入点序列
                                }

                                if (points.size() == size)
                                {
                                    PolyLine temp_polyline(points, mark);
                                    cout << temp_polyline;
                                    polylines.push_back(temp_polyline); // 加入一条折线
                                    i++;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            catch (const char *msg)
            {
                cerr << msg << endl;
            }
        }
    }
    fin.close();
    cout << "文件读取完毕！！ ";

    cout << "开始计算" << endl;

    for (size_t i = 0; i < polylines.size(); i++)
    {
        if (mode == 'A')
        {
            polylines[i].markers = Polychotomy(polylines[i]);
            polylines[i].markrects = polylines[i].markers;
        }
        if (mode == 'B')
        {
            polylines[i].markers = EquiSpace(polylines[i]);
            polylines[i].markrects = polylines[i].markers;
        }
    }

    cout << "计算完成!" << endl;

    /***************** 打印坐标 ******************/
    for (size_t i = 0; i < polylines.size(); i++)
    {
        cout << "Polyline" << i + 1 << " :\n";
        cout << polylines[i].markers.size() << endl;
        cout << polylines[i].MarkerStr() << endl;
    }

    fout.open("Result.txt");
    if (fout)
    {
        for (size_t i = 0; i < polylines.size(); i++)
        {
            fout << "Polyline" << i + 1 << " : \r\n"
                 << polylines[i].markers.size() << endl
                 << polylines[i].MarkerStr() << endl;
        }
        fout << flush;
    }
    fout.close();
}

vector<Point> Polychotomy(PolyLine polyline)
{
    int n = atoi(polyline.mode.c_str());
    double average_lenth = 0.0;
    vector<Point> mark_points;
    double temp_remain = 0.0; // 记录i-1 i剩余距离
    double temp = 0.0;        // 记录需标记点与i-1点距离

    average_lenth = polyline.lenth / (double)(n + 1); // 总长度
    temp = average_lenth;                             // 初始化temp长度
    for (int i = 1; i < polyline.points.size(); i++)
    {
        // 对于新点i
        temp_remain += polyline.lenths[i]; // 剩余距离加上i-1到i的距离

        while (compare(temp_remain, temp) == 1 && i < polyline.points.size() - 1) // 只要当前线段剩余长度<temp
        {
            temp_remain += polyline.lenths[i + 1]; // 补上距离
            i++;                                   // i递增检查下一个
        }
        while (temp <= temp_remain) // 只要当前边还能标记
        {
            Point temp_point = Linechotomy(polyline.points[i], polyline.points[i - 1], temp_remain - temp);
            mark_points.push_back(temp_point);
            temp_remain -= temp;
        } // 此时temp_remain < temp
    }
    mark_points.pop_back();
    return mark_points;
}

Point Linechotomy(Point from, Point to, double len)
{
    return Point(from.x + (to.x - from.x) * len / FindLenth(from, to),
                 from.y + (to.y - from.y) * len / FindLenth(from, to));
}

vector<Point> EquiSpace(PolyLine polyline)
{
    vector<Point> answer;
    const double delete_lenth = fmod(polyline.lenth, atof(polyline.mode.c_str())) / 2; // 两边应当省略的距离
    if (compare(delete_lenth, 0.0) == 0)                                               // 如果等于零,直接分割即可
    {
        string mode = to_string(int(polyline.lenth / atof(polyline.mode.c_str())) - 1);
        return Polychotomy(PolyLine(polyline.points, mode));
    }
    double temp_remain = 0.0;                        // 记录i - 1, i 剩余距离
    for (int i = 1; i < polyline.points.size(); i++) // 查看总长度是否超过deletelenth, 这样才能确定点的位置
    {
        temp_remain += polyline.lenths[i];            // 加上 i i-1 的距离
        if (compare(temp_remain, delete_lenth) == -1) // 如果temp_remain的距离更大,说明可以加了
        {
            Point temp_from = Linechotomy(polyline.points[i], polyline.points[i - 1], temp_remain - delete_lenth);

            temp_remain = 0.0;
            for (int j = polyline.points.size() - 1; j >= 1; j++)
            {
                temp_remain += polyline.lenths[j];            // 加上j j-1 的距离
                if (compare(temp_remain, delete_lenth) == -1) // 如果temp_remain的距离更大,说明可以切了
                {
                    Point temp_to = Linechotomy(polyline.points[j - 1], polyline.points[j], temp_remain - delete_lenth);

                    if (i == j) // 有可能from和to重合了,一来简化计算,二来防止因为重叠导致的错误
                    {
                        if (temp_from == temp_to) // 如果重合,说明没有其他点了
                        {
                            answer.push_back(temp_from);
                            return answer; // 直接返回
                        }
                    }
                    vector<Point> points_remain;
                    points_remain.push_back(temp_from);
                    answer.push_back(temp_from);
                    if (i != j)
                    {
                        points_remain.insert(
                            points_remain.end(),
                            polyline.points.begin() + i,
                            polyline.points.begin() + j);
                    }
                    string mode = to_string(int(polyline.lenth / atof(polyline.mode.c_str())) - 1);
                    points_remain.push_back(temp_to); // 调用之前的函数
                    points_remain = Polychotomy(PolyLine(points_remain, mode));
                    answer.insert(
                        answer.end(),
                        points_remain.begin(),
                        points_remain.end());
                    answer.push_back(temp_to);
                    return answer;
                }
            }
            break;
        }
    }

    cout << "输入可能有误!请检查间距是否正确!" << endl;
    return answer;
}

void SettleRect()
{
    for (int i = 0; i < polylines.size(); i++)
    {
        for (int j = 0; j < polylines[i].markers.size(); j++) // 前两层循环为遍历所有标记点
        {
            for (int ii = 0; ii < polylines.size(); ii++)
            {
                for (int jj = 0; jj < polylines[ii].markers.size(); jj++) // 后两层循环让前面的每个点和其他点进行比较
                {
                    if (j == jj) // continue, 自己不用和自己比较
                        continue;
                    if (fabs(polylines[i].markrects[j].x - polylines[ii].markrects[jj].x) > 20) // 如果x,y坐标有一者超过范围说明不可能相交
                        continue;
                    if (fabs(polylines[i].markrects[j].y - polylines[ii].markrects[jj].y) > 10)
                        continue;
                    // 剩下的因为外包矩形相交所以一定相交
                }
            }
        }
    }
}

bool Overlap(Point a, Point b) // 判断标记矩形是否重叠
{
    return (fabs(a.x - b.x) < 20.0 && fabs(a.y - b.y) < 10.0);
}

void RearrangeMarkers()
{
    extern int EXPAND;
    int count = 0; // 记录重叠标记矩形数量

    int num_buffer_col = int(72.0 / EXPAND);                      // 一列可以放几个标记
    int num_buffer_row = int(36.0 / EXPAND) - 2;                  // 一行可以放几个标记
    int num_whole_main = 2 * num_buffer_col + 2 * num_buffer_row; // 720*720的界面可以放多少
    int num_whole_right = num_buffer_col * int(2.8 / EXPAND);
    for (int i = 0; i < polylines.size(); i++)
    {
        for (int j = 0; j < polylines[i].markers.size(); j++) // 遍历每一个标记点
        {
            for (int ii = 0; ii < polylines.size(); ii++)
            {
                for (int jj = 0; jj < polylines[ii].markers.size(); jj++)
                {
                    cout << "寻找点" << polylines[i].markers[j].Str() << "和" << polylines[ii].markers[jj].Str() << endl;
                    if (ii == i && jj == j) // 此时不需要比较,是同一个点
                    {
                        continue;
                    }
                    if (polylines[ii].markrects[jj].id != 0 || polylines[i].markrects[j].id != 0) // 此时有一个已经完成了,无须比较
                        continue;
                    if (Overlap(polylines[ii].markers[jj], polylines[i].markers[j])) // 如果两个点重叠了,需要对i,j做出调整
                    {
                        double x, y;
                        polylines[i].markrects[j].id = 1;
                        count++;
                        if (count <= num_whole_main) // 如果还能放在720*720的框中
                        {
                            if (count <= num_buffer_col) // 如果能放在第一列
                            {
                                x = double(720 - 20 * EXPAND);
                                y = double((count - 1) * 10 * EXPAND);
                            }
                            else if (count > num_buffer_col && count <= num_buffer_col + num_buffer_row) // 如果能放在第一行
                            {
                                x = double((count - num_buffer_col) * 20 * EXPAND);
                                y = double(720 - 10 * EXPAND);
                            }
                            else if (count <= 2 * num_buffer_col + num_buffer_row) // 放在第二列
                            {
                                x = 0;
                                y = double((count - num_buffer_col - num_buffer_row - 1) * 10 * EXPAND);
                            }
                            else if (count <= 2 * num_buffer_col + 2 * num_buffer_row)
                            {
                                x = double((count - 2 * num_buffer_col - num_buffer_row) * 20 * EXPAND);
                                y = 0;
                            }
                            polylines[i].markrects[j].x = x;
                            polylines[i].markrects[j].y = y;
                            continue;
                        }
                        else if (count <= num_whole_main + num_whole_right)
                        {
                            int id_col = int((count - num_whole_main - 1) / num_buffer_col + 1); // 在第id_col列
                            int id_row = int((count - num_whole_main - 1) % num_buffer_col) + 1; // 在第id_row行
                            x = double(720 + (id_col - 1) * EXPAND * 20);
                            y = double((id_row - 1) * EXPAND * 10);
                            polylines[i].markrects[j].x = x;
                            polylines[i].markrects[j].y = y;
                            continue;
                        }
                        else
                        {
                            cout << "我尽力了, 好像不太行嗷" << endl;
                            cout << "打印目前分配的矩形" << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}
