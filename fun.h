#pragma once
#include "util.h"
#include "spatial_util.h"
#define BLACK_COLOR RGB(0, 0, 0) // 黑色

using namespace std;

extern vector<PolyLine> polylines; // 全局变量用于保存所有折线
extern vector<string> filenames;
extern int EXPAND;
extern int POS_X;
extern int POS_Y;
extern bool time_to_return;

//绘制指定属性的直线
static void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color);

//绘制实心圆(用于画点)
static void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color);

//绘制标记矩形
static void DrawMarkRect(HDC hDC, double x, double y);

// 绘制被移动的矩形
static void DrawMarkRectRearrange(HDC hDC, int real_x, int real_y);

/*********************************
* 名称：		DrawPolylines
* 描述：		绘制折线及其附属矩形
* 
* 参数：		HDC
* 返回值：	NULL
*********************************/
static void DrawPolylines(HDC hDC);

// 消息处理函数
static LRESULT CALLBACK WnzdProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// gui窗口起点
void DrawWindow();

/*********************************
* 名称：		Init
* 描述：		初始化函数,一切的起点
* 
* 参数：		NULL
* 返回值：	NULL
*********************************/
void Init();

/*********************************
* 名称：		ReadFile
* 描述：		用于读入所有txt文件(不带Result)
* 
* 参数：		None
* 返回值：	返回所有文件字符串
*********************************/
void ReadFile();

/*********************************
* 名称：		Initialize
* 描述：		开始对一个文件进行操作
* 
* 参数：		string filename
* 返回值：	NULL
*********************************/
void Initialize(string filename);

/*********************************
* 名称：		Polychotomy
* 描述：		用于切割线段(A方法
* 
* 参数：		PolyLine // 给定折线
* 返回值：	vector<Point> // 返回所有标记点
*********************************/
vector<Point> Polychotomy(PolyLine);

/*********************************
* 名称：		Linechotomy
* 描述：		给定两点,返回from到to 方向
			上, 距离len的点的坐标.
* 
* 参数：		Point from // 起点
			Point to // 终点
			double len // 距离
* 返回值：	Point // 返回一个点
*********************************/
Point Linechotomy(Point from, Point to, double len); // 给定两点，返回from到to方向上距离len的点坐标

/*********************************
* 名称：		EquiSpace
* 描述：		方法B
* 
* 参数：		PolyLine polyline // 给定一条折线
* 返回值：	vector<Point> // 返回结果
*********************************/
vector<Point> EquiSpace(PolyLine polyline);

/*********************************
* 名称：		SettleRect
* 描述：		用于挪动文件中所有相互重叠的多边形
* 
* 参数：		NULL
* 返回值：	NULL
*********************************/
void SettleRect(); // 用于挪动所有的多边形

/*********************************
* 名称：		Overlap
* 描述：		判断矩形是否重叠
* 
* 参数：		矩形左上角坐标
* 返回值：	true / false
*********************************/
bool Overlap(Point, Point);

/*********************************
* 名称：		RearrangeMarkers
* 描述：		用于安排重叠的矩形
* 
* 参数：		NULL
* 返回值：		NULL
*********************************/
void RearrangeMarkers();
