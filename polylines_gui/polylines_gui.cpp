#include <windows.h>
#include "..\util.h"
#include "..\spatial_util.h"
#include "..\fun.h"
// 用于注册的窗口类名
const char g_szClassName[] = "myWindowClass";

void Paint(HWND hwnd)
{
    Initialize();
    PAINTSTRUCT ps;
    HDC hdc;   // DC(可画图的内存对象) 的句柄
    HPEN hpen; // 画笔
    int red, blue; // 红、绿两色
    int y = 1, x = 1; // 行、列

    // 通过窗口句柄获取该窗口的 DC
    hdc = BeginPaint(hwnd, &ps);

    for (red = 0; red < 256; red++)
    {
        for (blue = 0; blue < 256; blue++)
        {
            // 创建画笔
            hpen = CreatePen(PS_SOLID, 1, RGB(red, 0, blue));
            // 选中画笔
            SelectObject(hdc, hpen);

            // 画一条从 (x, y) 到 (x, y+1) 的垂直的线
            MoveToEx(hdc, x, y, NULL);
            LineTo(hdc, x++, y + 1);

            // 删除画笔
            DeleteObject(hpen);
        }
        // 一行画完 行+1, 列重新置 1
        y += 1;
        x = 1;
    }

    EndPaint(hwnd, &ps);
}

/*
 * 第四步,窗口过程
 */
LRESULT CALLBACK MyWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // 窗口绘制消息
    case WM_PAINT:
        Paint(hwnd);
        break;
        // 窗口关闭消息
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
        // 窗口销毁消息
    case WM_DESTROY:
        PostQuitMessage(0); // 发送离开消息给系统
        break;
        // 其他消息
    default:
        // pass 给系统，咱不管
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/*
 * 第一步,注册窗口类
 */
void RegisterMyWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wc;

    // 1)配置窗口属性
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = MyWindowProc; // 设置第四步的窗口过程回调函数
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = (LPCWSTR)g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // 2)注册
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("窗口注册失败!"), TEXT("错误"), MB_ICONEXCLAMATION | MB_OK);
        exit(0); // 进程退出
    }
}

/*
 * 第二步，创建窗口
 */
HWND CreateMyWindow(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd;
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        (LPCWSTR)g_szClassName,
        TEXT("Polylines"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, // 出现坐标 x,y 默认分配 窗口宽 400 高 300
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, TEXT("窗口创建失败!"), TEXT("错误"), MB_ICONEXCLAMATION | MB_OK);
        exit(0); // 进程退出
    }

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return hwnd;
}

/*
 * 主函数
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG Msg;

    // 第一步,注册窗口类
    RegisterMyWindow(hInstance);
    // 第二步:创建窗口
    hwnd = CreateMyWindow(hInstance, nCmdShow);
    // 第三步:消息循环
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}