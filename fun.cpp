#include "fun.h"

using namespace std;

vector<PolyLine> polylines;
vector<string> filenames; // 用于记录文件名称

int EXPAND = 10;         // 膨胀系数
int POS_X = 100;         // 用于定位坐标系x轴
int POS_Y = 200;         // 定位y轴
bool time_to_return = 0; // gui返回

//绘制指定属性的直线
static void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color)
{
    HPEN hPen = CreatePen(style, width, color);
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen); // 笔刷

    MoveToEx(hDC, x0, y0, NULL);
    LineTo(hDC, x1, y1);

    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}

//绘制实心圆
static void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

    Ellipse(hDC, x - len / 2, y - len / 2, x + len / 2, y + len / 2);

    SelectObject(hDC, hOldBrush);
    DeleteObject(hPen);

    SelectObject(hDC, hOldPen);
    DeleteObject(hOldBrush);
}

static void DrawMarkRect(HDC hDC, double x, double y)
{
    int real_x = int(x * EXPAND) + POS_X;
    int real_y = -int(y * EXPAND) + POS_Y;

    DrawLine(hDC, real_x, real_y, real_x + 20 * EXPAND, real_y, PS_SOLID, 1, RGB(0, 255, 0));
    DrawLine(hDC, real_x + 20 * EXPAND, real_y, real_x + 20 * EXPAND, real_y + 10 * EXPAND, PS_SOLID, 1, RGB(0, 255, 0));
    DrawLine(hDC, real_x, real_y + 10 * EXPAND, real_x + 20 * EXPAND, real_y + 10 * EXPAND, PS_SOLID, 1, RGB(0, 255, 0));
    DrawLine(hDC, real_x, real_y, real_x, real_y + 10 * EXPAND, PS_SOLID, 1, RGB(0, 255, 0));
}

static void DrawMarkRectRearrange(HDC hDC, int real_x, int real_y)
{
    DrawLine(hDC, real_x, real_y, real_x + 20 * EXPAND, real_y, PS_SOLID, 1, RGB(0, 255, 0));
    DrawLine(hDC, real_x + 20 * EXPAND, real_y, real_x + 20 * EXPAND, real_y + 10 * EXPAND, PS_SOLID, 1, RGB(0, 255, 0));
    DrawLine(hDC, real_x, real_y + 10 * EXPAND, real_x + 20 * EXPAND, real_y + 10 * EXPAND, PS_SOLID, 1, RGB(0, 255, 0));
    DrawLine(hDC, real_x, real_y, real_x, real_y + 10 * EXPAND, PS_SOLID, 1, RGB(0, 255, 0));
}

static void DrawPolylines(HDC hDC)
{
    cout << "正在绘制图形..." << endl;
    // 计算比例尺和坐标
    // 已知程序需要长720, 宽720,
    // 首先找到左右边界boundx, boundy
    double xl, xr, yt, yb;
    xl = xr = polylines[0].points[0].x;
    yt = yb = polylines[0].points[0].y;
    for (int i = 0; i < polylines.size(); i++)
    {
        for (int j = 0; j < polylines[i].points.size(); j++)
        {
            if (xl > polylines[i].points[j].x)
                xl = polylines[i].points[j].x;
            if (xr < polylines[i].points[j].x)
                xr = polylines[i].points[j].x;
            if (yt < polylines[i].points[j].y)
                yt = polylines[i].points[j].y;
            if (yb > polylines[i].points[i].y)
                yb = polylines[i].points[j].y;
        }
    }

    double maxlenth = 0.0; // 找到x方向长还是y方向,将长的方向放大为720像素计算比例尺
    if ((yt - yb + 30.0) > (xr - xl + 60.0))
    {
        maxlenth = yt - yb + 30.0;
    }
    else
    {
        maxlenth = xr - xl + 60.0;
    }
    cout << "宇宙矩形长边长为" << maxlenth << endl;

    // 开始寻找POS_X, POS_Y位置
    EXPAND = int(720.0 / maxlenth); // 膨胀系数 * maxlenth <= 720.0
    POS_X = int((20.0 - xl) * EXPAND);
    POS_Y = int((10.0 + yt) * EXPAND);

    RearrangeMarkers(); // 判断是否重叠

    for (int i = 0; i < polylines.size(); i++)
    {
        for (int j = 0; j < polylines[i].points.size() - 1; j++) // 画出折线
        {
            int x0 = int(polylines[i].points[j].x * EXPAND) + POS_X;
            int y0 = -int(polylines[i].points[j].y * EXPAND) + POS_Y;
            int x1 = int(polylines[i].points[j + 1].x * EXPAND) + POS_X;
            int y1 = -int(polylines[i].points[j + 1].y * EXPAND) + POS_Y;
            DrawLine(hDC, x0, y0, x1, y1, PS_SOLID, 1, RGB(0, 0, 0));
        }

        for (int j = 0; j < polylines[i].markers.size(); j++) // 画出标记矩形
        {
            if (polylines[i].markrects[j].id != 0) // 如果该矩形移动了
            {
                int x0 = int(polylines[i].markers[j].x * EXPAND) + POS_X;
                int y0 = -int(polylines[i].markers[j].y * EXPAND) + POS_Y;
                int x1 = int(polylines[i].markrects[j].x);
                int y1 = int(polylines[i].markrects[j].y);
                DrawMarkRectRearrange(hDC, x1, y1);
                DrawLine(hDC, x0, y0, x1, y1, PS_DOT, 1, RGB(50, 50, 50));
                DrawCircle(hDC, x1, y1, 3, RGB(0, 0, 255));
                continue;
            }
            DrawMarkRect(hDC, polylines[i].markrects[j].x, polylines[i].markrects[j].y);
        }

        for (int j = 0; j < polylines[i].markers.size(); j++) // 画出标记点
        {
            int x = int(polylines[i].markers[j].x * EXPAND) + POS_X;
            int y = -int(polylines[i].markers[j].y * EXPAND) + POS_Y;
            DrawCircle(hDC, x, y, 5, RGB(0, 0, 255));
        }
    }
    time_to_return = 1; // 用于返回
}

static LRESULT CALLBACK WnzdProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT: // 绘图消息
    {
        hDC = BeginPaint(hWnd, &ps);
        //绘制不同模式的直线
        DrawPolylines(hDC); // 开始绘图
    }
        EndPaint(hWnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void Init()
{
    ReadFile(); // 获取所有文件

    for (size_t i = 0; i < filenames.size(); i++)
    {
        cout << "回车开始第" << i + 1 << "个文件读取" << endl;
        system("pause");

        Initialize(filenames[i]); // 初始化一个文件

        string temp;
        cout << "允许使用gui生成图像,是否生成?(y/n)" << endl;
        cin >> temp;
        if (temp[0] == 'y' || temp[0] == 'Y') // 匹配不到直接按不生成处理
        {
            cout << "开始生成!" << endl;

            DrawWindow();
        }
        else
        {
            cout << "跳过生成!" << endl;
        }
    }
}

void ReadFile()
{
    cout << "准备读取文件,当前路径为";
    char buff[100];
    _getcwd(buff, 1000);
    cout << buff << endl;
    string filepath = buff;
    string format = ".txt";

    GetAllFormatFiles(filepath, filenames, format); // 找到所有.txt文件

    cout << "找到的文件有" << endl;
    for (int i = 0; i < filenames.size(); i++) // 去除所有Result.txt
    {
        smatch result;
        regex pattern("Result_[\\s\\S]*");
        try
        {
            if (regex_match(filenames[i], result, pattern))
            {
                cout << "去除" << filenames[i] << endl;
                filenames.erase(filenames.begin() + i); // 清除当前文件
                i--;                                    // 补上i的减小
            }
            else
            {
                cout << filenames[i] << endl;
            }
        }
        catch (const char *msg) // 文件打开错误
        {
            cerr << msg << endl;
        }
    }

    return;
}

void DrawWindow()
{
    HINSTANCE hInstance;
    hInstance = GetModuleHandle(NULL);
    WNDCLASS Draw;
    Draw.cbClsExtra = 0;
    Draw.cbWndExtra = 0;
    Draw.hCursor = LoadCursor(hInstance, IDC_ARROW);

    Draw.hIcon = LoadIcon(hInstance, IDI_APPLICATION);

    Draw.lpszMenuName = NULL;
    Draw.style = CS_HREDRAW | CS_VREDRAW;
    Draw.hbrBackground = (HBRUSH)COLOR_WINDOW;
    Draw.lpfnWndProc = WnzdProc;
    Draw.lpszClassName = _T("DDraw");
    Draw.hInstance = hInstance;

    RegisterClass(&Draw);

    HWND hwnd = CreateWindow(
        _T("DDraw"),         //上面注册的类名，要完全一致
        _T("abd"),           //窗口标题文字
        WS_OVERLAPPEDWINDOW, //窗口外观样式
        38,                  //窗口相对于父级的X坐标
        20,                  //窗口相对于父级的Y坐标
        1280,                //窗口的宽度
        720,                 //窗口的高度
        NULL,                //没有父窗口，为NULL
        NULL,                //没有菜单，为NULL
        hInstance,           //当前应用程序的实例句柄
        NULL);               //没有附加数据，为NULL

    // 显示窗口
    ShowWindow(hwnd, SW_SHOW);

    // 更新窗口
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (time_to_return)
        {
            time_to_return = 0;
            system("pause");
            return;
        }
    }
}

void Initialize(string file)
{
    /***************** 读取文件 ******************/
    ifstream fin;  // 读取文件流
    ofstream fout; // 打印文件
    string temp;   // 临时储存输入
    int num = 0;   // 记录折线数量
    char mode = 0; // 模式
    // cout << "回车默认读取工程目录下的input*.txt，或输入你想要打开的文件路径" << endl;

    // char c = getchar(); // 判断是否输入的是回车

    // if (c != '\n') // 如果输入的不是回车,说明需要打开自己的文件
    // {
    //     cin.putback(c);
    //     cin >> file;
    // }

    vector<PolyLine> newlines; // 新建vector
    newlines.swap(polylines);  // 清空上次内容
    cout << "/***************** 开始读取文件" << file << "******************/" << endl;

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

    /***************** 开始计算 ******************/

    cout << "开始计算" << endl;

    for (size_t i = 0; i < polylines.size(); i++)
    {
        if (mode == 'A')
        {
            polylines[i].markers = Polychotomy(polylines[i]); // A方法
            polylines[i].markrects = polylines[i].markers;
        }
        if (mode == 'B')
        {
            polylines[i].markers = EquiSpace(polylines[i]); // B方法
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
    string outfile = "Result_" + file;

    /***************** 写入文件 ******************/

    fout.open(outfile);
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
    cout << "结果已存储在" << outfile << "中" << endl;
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
        while (compare(temp, temp_remain) >= 0) // 只要当前边还能标记
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
                    vector<Point> points_remain; // 注意from + remain + to
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
                    answer.push_back(temp_to); // 结果应当是 from - remain - to
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
                    //cout << "寻找点" << polylines[i].markers[j].Str() << "和" << polylines[ii].markers[jj].Str() << endl;
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
                            cout << "我尽力了, 算法好像不太行嗷" << endl;
                            cout << "打印目前分配的矩形" << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}
