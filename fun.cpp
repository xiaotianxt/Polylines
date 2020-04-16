#include "fun.h"

using namespace std;

vector<PolyLine> polylines;
vector<Point> markers_out; // ���ڼ�¼���ڵ��ĵ�

void Initialize()
{
    /***************** ��ȡ�ļ� ******************/
    ifstream fin; // ��ȡ�ļ���
    ofstream fout;
    string temp;           // ��ʱ��������
    string file = "input"; // �ļ�·��
    int num = 0;           // ��¼��������
    int file_num = 0;
    char mode = 0; // ģʽ
    cout << "�س�Ĭ�϶�ȡ����Ŀ¼�µ�input*.txt������������Ҫ�򿪵��ļ�·��" << endl;

    char c = getchar(); // �ж��Ƿ�������ǻس�

    if (c != '\n') // �������Ĳ��ǻس�,˵����Ҫ���Լ����ļ�
    {
        cin.putback(c);
        cin >> file;
    }
    cout << "/***************** ��ʼ��ȡ�ļ� ******************/" << endl;

    file = file + to_string(file_num) + ".txt";
    fin.open(file);
    if (fin.is_open()) // �򿪳ɹ�
    {
        smatch result;
        int i = 0, size = 0; // ��¼¼��ĵ�i������
        string mark;         // ���ڼ�¼��ǵ���(�涨���)
        regex pattern("(A|B)\\s*");
        regex counter("(\\d+)\\s*");                            // ƥ����������
        regex catalog("(\\d+)\\s(\\d+\\.?\\d*)\\s*");           // ƥ�����߿�ͷ����������
        regex data("(-?\\d+\\.?\\d*)\\s*(-?\\d+\\.?\\d*)\\s*"); // ƥ��������
        while (!fin.eof())
        {
            getline(fin, temp); // ��ȡģʽ
            try
            {
                if (regex_match(temp, result, pattern)) // ѡ��ƥ��ɹ�
                {
                    cout << "��ѡ�����" << result[1] << endl
                         << "��ʼ�����ļ�...." << endl;
                    mode = result[1].str()[0]; // д��mode
                    while (!fin.eof())         // ��ֹ�ļ�����ʧ��
                    {
                        if (regex_match(temp, result, counter))
                        {
                            num = atoi(result[1].str().c_str());
                        }
                        getline(fin, temp);                     // ���ȶ���һ������,����ƥ�����ߵ���+��ǵ���(���ߵ���+�涨���)
                        if (regex_match(temp, result, catalog)) // ƥ�䵽
                        {
                            vector<Point> points;
                            size = atoi(result[1].str().c_str()); // ��¼��С
                            mark = result[2].str();
                            cout << "��ʼ��ȡ��" << i + 1 << "������" << endl;
                            while (!fin.eof()) // ��ֹ�ļ�����ʧ��
                            {
                                getline(fin, temp); // ����һ������

                                if (regex_match(temp, result, data)) // ƥ������
                                {
                                    Point temp_point(result[1], result[2]);
                                    points.push_back(temp_point); // ���������
                                }

                                if (points.size() == size)
                                {
                                    PolyLine temp_polyline(points, mark);
                                    cout << temp_polyline;
                                    polylines.push_back(temp_polyline); // ����һ������
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
    cout << "�ļ���ȡ��ϣ��� ";

    cout << "��ʼ����" << endl;

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

    cout << "�������!" << endl;

    /***************** ��ӡ���� ******************/
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
    double temp_remain = 0.0; // ��¼i-1 iʣ�����
    double temp = 0.0;        // ��¼���ǵ���i-1�����

    average_lenth = polyline.lenth / (double)(n + 1); // �ܳ���
    temp = average_lenth;                             // ��ʼ��temp����
    for (int i = 1; i < polyline.points.size(); i++)
    {
        // �����µ�i
        temp_remain += polyline.lenths[i]; // ʣ��������i-1��i�ľ���

        while (compare(temp_remain, temp) == 1 && i < polyline.points.size() - 1) // ֻҪ��ǰ�߶�ʣ�೤��<temp
        {
            temp_remain += polyline.lenths[i + 1]; // ���Ͼ���
            i++;                                   // i���������һ��
        }
        while (temp <= temp_remain) // ֻҪ��ǰ�߻��ܱ��
        {
            Point temp_point = Linechotomy(polyline.points[i], polyline.points[i - 1], temp_remain - temp);
            mark_points.push_back(temp_point);
            temp_remain -= temp;
        } // ��ʱtemp_remain < temp
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
    const double delete_lenth = fmod(polyline.lenth, atof(polyline.mode.c_str())) / 2; // ����Ӧ��ʡ�Եľ���
    if (compare(delete_lenth, 0.0) == 0)                                               // ���������,ֱ�ӷָ��
    {
        string mode = to_string(int(polyline.lenth / atof(polyline.mode.c_str())) - 1);
        return Polychotomy(PolyLine(polyline.points, mode));
    }
    double temp_remain = 0.0;                        // ��¼i - 1, i ʣ�����
    for (int i = 1; i < polyline.points.size(); i++) // �鿴�ܳ����Ƿ񳬹�deletelenth, ��������ȷ�����λ��
    {
        temp_remain += polyline.lenths[i];            // ���� i i-1 �ľ���
        if (compare(temp_remain, delete_lenth) == -1) // ���temp_remain�ľ������,˵�����Լ���
        {
            Point temp_from = Linechotomy(polyline.points[i], polyline.points[i - 1], temp_remain - delete_lenth);

            temp_remain = 0.0;
            for (int j = polyline.points.size() - 1; j >= 1; j++)
            {
                temp_remain += polyline.lenths[j];            // ����j j-1 �ľ���
                if (compare(temp_remain, delete_lenth) == -1) // ���temp_remain�ľ������,˵����������
                {
                    Point temp_to = Linechotomy(polyline.points[j - 1], polyline.points[j], temp_remain - delete_lenth);

                    if (i == j) // �п���from��to�غ���,һ���򻯼���,������ֹ��Ϊ�ص����µĴ���
                    {
                        if (temp_from == temp_to) // ����غ�,˵��û����������
                        {
                            answer.push_back(temp_from);
                            return answer; // ֱ�ӷ���
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
                    points_remain.push_back(temp_to); // ����֮ǰ�ĺ���
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

    cout << "�����������!�������Ƿ���ȷ!" << endl;
    return answer;
}

void SettleRect()
{
    for (int i = 0; i < polylines.size(); i++)
    {
        for (int j = 0; j < polylines[i].markers.size(); j++) // ǰ����ѭ��Ϊ�������б�ǵ�
        {
            for (int ii = 0; ii < polylines.size(); ii++)
            {
                for (int jj = 0; jj < polylines[ii].markers.size(); jj++) // ������ѭ����ǰ���ÿ�������������бȽ�
                {
                    if (j == jj) // continue, �Լ����ú��Լ��Ƚ�
                        continue;
                    if (fabs(polylines[i].markrects[j].x - polylines[ii].markrects[jj].x) > 20) // ���x,y������һ�߳�����Χ˵���������ཻ
                        continue;
                    if (fabs(polylines[i].markrects[j].y - polylines[ii].markrects[jj].y) > 10)
                        continue;
                    // ʣ�µ���Ϊ��������ཻ����һ���ཻ
                }
            }
        }
    }
}

bool Overlap(Point a, Point b) // �жϱ�Ǿ����Ƿ��ص�
{
    return (fabs(a.x - b.x) < 20.0 && fabs(a.y - b.y) < 10.0);
}

void RearrangeMarkers()
{
    extern int EXPAND;
    int count = 0; // ��¼�ص���Ǿ�������

    int num_buffer_col = int(72.0 / EXPAND);                      // һ�п��Էż������
    int num_buffer_row = int(36.0 / EXPAND) - 2;                  // һ�п��Էż������
    int num_whole_main = 2 * num_buffer_col + 2 * num_buffer_row; // 720*720�Ľ�����ԷŶ���
    int num_whole_right = num_buffer_col * int(2.8 / EXPAND);
    for (int i = 0; i < polylines.size(); i++)
    {
        for (int j = 0; j < polylines[i].markers.size(); j++) // ����ÿһ����ǵ�
        {
            for (int ii = 0; ii < polylines.size(); ii++)
            {
                for (int jj = 0; jj < polylines[ii].markers.size(); jj++)
                {
                    cout << "Ѱ�ҵ�" << polylines[i].markers[j].Str() << "��" << polylines[ii].markers[jj].Str() << endl;
                    if (ii == i && jj == j) // ��ʱ����Ҫ�Ƚ�,��ͬһ����
                    {
                        continue;
                    }
                    if (polylines[ii].markrects[jj].id != 0 || polylines[i].markrects[j].id != 0) // ��ʱ��һ���Ѿ������,����Ƚ�
                        continue;
                    if (Overlap(polylines[ii].markers[jj], polylines[i].markers[j])) // ����������ص���,��Ҫ��i,j��������
                    {
                        double x, y;
                        polylines[i].markrects[j].id = 1;
                        count++;
                        if (count <= num_whole_main) // ������ܷ���720*720�Ŀ���
                        {
                            if (count <= num_buffer_col) // ����ܷ��ڵ�һ��
                            {
                                x = double(720 - 20 * EXPAND);
                                y = double((count - 1) * 10 * EXPAND);
                            }
                            else if (count > num_buffer_col && count <= num_buffer_col + num_buffer_row) // ����ܷ��ڵ�һ��
                            {
                                x = double((count - num_buffer_col) * 20 * EXPAND);
                                y = double(720 - 10 * EXPAND);
                            }
                            else if (count <= 2 * num_buffer_col + num_buffer_row) // ���ڵڶ���
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
                            int id_col = int((count - num_whole_main - 1) / num_buffer_col + 1); // �ڵ�id_col��
                            int id_row = int((count - num_whole_main - 1) % num_buffer_col) + 1; // �ڵ�id_row��
                            x = double(720 + (id_col - 1) * EXPAND * 20);
                            y = double((id_row - 1) * EXPAND * 10);
                            polylines[i].markrects[j].x = x;
                            polylines[i].markrects[j].y = y;
                            continue;
                        }
                        else
                        {
                            cout << "�Ҿ�����, ����̫���" << endl;
                            cout << "��ӡĿǰ����ľ���" << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}
