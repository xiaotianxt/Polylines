#include <windows.h>
#include <tchar.h>
#include "fun.h"
#define BLACK_COLOR RGB(0, 0, 0) // ��ɫ
using namespace std;
int EXPAND = 10;
int POS_X = 100;
int POS_Y = 200;

//����ָ�����Ե�ֱ��
static void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color)
{
	HPEN hPen = CreatePen(style, width, color);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	MoveToEx(hDC, x0, y0, NULL);
	LineTo(hDC, x1, y1);

	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
//����ʵ��Բ
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

//����������
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

//����λͼ������
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
// ��Ϣ��������ʵ��

static void DrawPolylines(HDC hDC)
{
	cout << "���ڻ���ͼ��..." << endl;
	// ��������ϵ��������
	// ��֪������Ҫ��720, ��720,
	// �����ҵ����ұ߽�boundx, boundy
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
	cout << "������γ��߳�Ϊ" << maxlenth << endl;

	// ��ʼѰ��POS_X, POS_Yλ��
	EXPAND = int(720.0 / maxlenth); // ����ϵ�� * maxlenth <= 720.0
	POS_X = int((20.0 - xl) * EXPAND);
	POS_Y = int((10.0 + yt) * EXPAND);

	RearrangeMarkers(); // �ж��Ƿ��ص�

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
			if (polylines[i].markrects[j].id != 0) // ����þ����ƶ���
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
		//���Ʋ�ͬģʽ��ֱ��
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
		_T("DDraw"),		 //����ע���������Ҫ��ȫһ��
		_T("abd"),			 //���ڱ�������
		WS_OVERLAPPEDWINDOW, //���������ʽ
		38,					 //��������ڸ�����X����
		20,					 //��������ڸ�����Y����
		1280,				 //���ڵĿ��
		720,				 //���ڵĸ߶�
		NULL,				 //û�и����ڣ�ΪNULL
		NULL,				 //û�в˵���ΪNULL
		hInstance,			 //��ǰӦ�ó����ʵ�����
		NULL);				 //û�и������ݣ�ΪNULL

	// ��ʾ����
	ShowWindow(hwnd, SW_SHOW);

	// ���´���
	UpdateWindow(hwnd);

	// ��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
