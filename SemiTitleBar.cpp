// SemiTitleBar.cpp : Defines the entry point for the application.
//

#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Global Variables:
HINSTANCE hInst = nullptr;
const wchar_t szTitle[] = L"Sample Window";
const wchar_t szWindowClass[] = L"MSampleWindowClass";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = {sizeof(WNDCLASSEX)};

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0,0,120));
	wcex.lpszClassName = szWindowClass;

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	MONITORINFO mi = {sizeof(mi)};
	POINT ptCur = {}; GetCursorPos(&ptCur);
	HMONITOR hMon = MonitorFromPoint(ptCur, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(hMon, &mi);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW
		& ~(WS_CAPTION);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, dwStyle,
		mi.rcWork.left+100, mi.rcWork.top+100, 600, 400, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DWORD dwTest = GetWindowLong(hWnd, GWL_STYLE);
	// #BUG Regardless the fact we don't set WS_CAPTION in dwStyle, the window was created WITH CAPTION
	if (dwTest & WS_CAPTION)
	{
		SetWindowLong(hWnd, GWL_STYLE, dwTest & ~WS_CAPTION);
		// #BUG At this point in Win8.1 the caption without text is still visible: 2018-01-29_13-26-20.png
		// #BUG And after dragging the window on this caption we see fake non-themed caption: 2018-01-29_13-26-48.png

		// So we need to update window size to avoid two bugs described above
		SetWindowPos(hWnd, NULL, mi.rcWork.left+100, mi.rcWork.top+100, 600, 400, SWP_FRAMECHANGED|SWP_NOZORDER);
	}

	return TRUE;
}

LRESULT OnPaint(HWND hWnd)
{
	const int border = 10;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	RECT rc; GetClientRect(hWnd, &rc);
	HBRUSH hbr = CreateSolidBrush(RGB(0, 255, 0));
	RECT rcb = {rc.left, rc.top, rc.left + border, rc.bottom};
	FillRect(hdc, &rcb, hbr);
	rcb = RECT{rc.left, rc.top, rc.right, rc.top + border};
	FillRect(hdc, &rcb, hbr);
	rcb = RECT{rc.left, rc.bottom - border, rc.right, rc.bottom};
	FillRect(hdc, &rcb, hbr);
	rcb = RECT{rc.right - border, rc.top, rc.right, rc.bottom};
	FillRect(hdc, &rcb, hbr);
	DeleteObject(hbr);
	rcb = RECT{rc.left + border, rc.top + border, rc.right - border, rc.bottom - border};
	hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
	FillRect(hdc, &rcb, hbr);
	EndPaint(hWnd, &ps);
	return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRc = 0;
	switch (message)
	{
	case WM_PAINT:
		lRc = OnPaint(hWnd);
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		lRc = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_NCCALCSIZE:
	case WM_NCPAINT:
		lRc = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	default:
		lRc = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return lRc;
}
