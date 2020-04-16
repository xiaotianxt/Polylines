#include <windows.h>
#include <tchar.h>
#include "fun.h"
#define BLACK_COLOR RGB(0, 0, 0) // 黑色
using namespace std;
int EXPAND = 10;
int POS_X = 100;
int POS_Y = 200;

//绘制指定属性的直线
static void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color)
{
	HPEN hPen = CreatePen(style, width, color);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

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

//绘制填充矩形
static void DrawRect(HDC hDC, int left, int top, int width, int height, int style, COLORREF color)
{
	HBRUSH hBrush = CreateHatchBrush(style, color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Rectangle(hDC, left, top, left + width, top + height);

	SelectObject(hDC, hOldBrush);
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

//绘制位图填充矩形
static void DrawBmpRect(HDC hDC, int left, int top, int width, int height, LPCTSTR file)
{
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBRUSH hBrush = CreatePatternBrush(hBitmap);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Rectangle(hDC, left, top, left + width, top + height);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hOldBrush);
	DeleteObject(hBitmap);
}
// 消息处理函数的实现

static void DrawPolylines(HDC hDC)
{
	cout << "正在绘制图形..." << endl;
	// 计算膨胀系数和坐标
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
	double maxlenth = 0.0;
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
		for (int j = 0; j < polylines[i].points.size() - 1; j++)
		{
			int x0 = int(polylines[i].points[j].x * EXPAND) + POS_X;
			int y0 = -int(polylines[i].points[j].y * EXPAND) + POS_Y;
			int x1 = int(polylines[i].points[j + 1].x * EXPAND) + POS_X;
			int y1 = -int(polylines[i].points[j + 1].y * EXPAND) + POS_Y;
			DrawLine(hDC, x0, y0, x1, y1, PS_SOLID, 1, RGB(0, 0, 0));
		}

		for (int j = 0; j < polylines[i].markers.size(); j++)
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

		for (int j = 0; j < polylines[i].markers.size(); j++)
		{
			int x = int(polylines[i].markers[j].x * EXPAND) + POS_X;
			int y = -int(polylines[i].markers[j].y * EXPAND) + POS_Y;
			DrawCircle(hDC, x, y, 5, RGB(0, 0, 255));
		}
	}
}

static LRESULT CALLBACK WnzdProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);
		//绘制不同模式的直线
		DrawPolylines(hDC);
	}
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int _tmain(int argc, _TCHAR *argv[])
{
	Initialize();
	HINSTANCE hInstance;
	hInstance = GetModuleHandle(NULL);
	WNDCLASS Draw;
	Draw.cbClsExtra = 0;
	Draw.cbWndExtra = 0;
	Draw.hCursor = LoadCursor(hInstance, IDC_ARROW);
	;
	Draw.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	;
	Draw.lpszMenuName = NULL;
	Draw.style = CS_HREDRAW | CS_VREDRAW;
	Draw.hbrBackground = (HBRUSH)COLOR_WINDOW;
	Draw.lpfnWndProc = WnzdProc;
	Draw.lpszClassName = _T("DDraw");
	Draw.hInstance = hInstance;

	RegisterClass(&Draw);

	HWND hwnd = CreateWindow(
		_T("DDraw"),		 //上面注册的类名，要完全一致
		_T("abd"),			 //窗口标题文字
		WS_OVERLAPPEDWINDOW, //窗口外观样式
		38,					 //窗口相对于父级的X坐标
		20,					 //窗口相对于父级的Y坐标
		1280,				 //窗口的宽度
		720,				 //窗口的高度
		NULL,				 //没有父窗口，为NULL
		NULL,				 //没有菜单，为NULL
		hInstance,			 //当前应用程序的实例句柄
		NULL);				 //没有附加数据，为NULL

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
	}
}
