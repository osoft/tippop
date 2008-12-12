#include "stdafx.h"
#include "TP_DesktopTag.h"
#include <GdiPlus.h>
//#include "gdiplus.h"

#define MAX_LOADSTRING 100

#define DT_WIDTH (100)
#define DT_HEIGHT (30)
#define DT_HDLWIDTH (5)

TCHAR szDTClassName[MAX_LOADSTRING] = L"DesktopTagWndCls";			// 主窗口类名
TCHAR szDTWndName[MAX_LOADSTRING] = L"DesktopTag";

extern HINSTANCE hInst;

static int tagCount = 0;
static TP_DT_TagCB_t tcb[256];
static POINT ptOld;
static HANDLE hMutex;

// Window Procedure
static LRESULT CALLBACK TP_DT_WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

// Event handlers
static int TP_DT_OnPaint(HWND hWnd);
static int TP_DT_OnMouseMove(HWND hWnd, WPARAM wParam, int x, int y);
static int TP_DT_OnMouseLeave(HWND hWnd, int x, int y);

// Private data query functions
static bool MouseInWnd(HWND);
static void SetMouseInWnd(HWND hWnd, bool bIn);
static WCHAR* GetTagTitle(HWND);
static char* GetTagContent(HWND);
static HANDLE GetTagSem(HWND hWnd);

static int TP_DT_OnPaint(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc;

	HBRUSH hBrush;
	HPEN hPen;
	HGDIOBJ hPenOld, hBrushOld;
	
	hdc = BeginPaint(hWnd, &ps);
	// Draw the background
	hBrush = CreateSolidBrush(RGB(33,33,33));
	hPen = CreatePen(PS_SOLID, 0, RGB(33,33,33));
	hBrushOld = SelectObject(hdc, hBrush);
	hPenOld = SelectObject(hdc, hPen);

	Rectangle(hdc, 0,0,DT_WIDTH,DT_HEIGHT);
	
	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	
	// Draw the handle
	hBrush = CreateSolidBrush(RGB(230,55,55));
	hPen = CreatePen(PS_SOLID, 0, RGB(230,55,55));
	hBrushOld = SelectObject(hdc, hBrush);
	hPenOld = SelectObject(hdc, hPen);
	
	Rectangle(hdc, 0, 0, DT_HDLWIDTH, DT_HEIGHT);
	
	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	//hdc = BeginPaint(hWnd, &ps);

	Graphics graphics(hdc);
	FontFamily  fontFamily(DEFAULTFONT);
	Font        font(&fontFamily, 20, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(Color(255, 255, 255, 255));

	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.DrawString(GetTagTitle(hWnd), -1, &font, PointF(1.0f, 1.0f), &solidBrush);

	EndPaint(hWnd, &ps);
	return 0;
}

DWORD WINAPI TP_DT_PaintHandleTask(LPVOID lpParam)
{
	HDC hdc;
	hdc = GetWindowDC((HWND)lpParam);
	short gValue;
	HBRUSH hBrush;
	HPEN hPen;
	HGDIOBJ hBrushOld, hPenOld;
	HWND hWnd = (HWND)lpParam;
	while (1)
	{
		WaitForSingleObject(GetTagSem((HWND)lpParam), INFINITE);
		gValue = GetGValue(GetPixel(hdc, 1, 1));
		do 
		{
			if(MouseInWnd(hWnd) && gValue < 255)
				gValue+=10;
			else if(!MouseInWnd(hWnd) && gValue > 55)
				gValue-=10;
			else
			{
				break;
			}
			hBrush = CreateSolidBrush(RGB(230, gValue, gValue));
			hPen = CreatePen(PS_SOLID, 0, RGB(230, gValue, gValue));
			hBrushOld = SelectObject(hdc, hBrush);
			hPenOld = SelectObject(hdc, hPen);

			Rectangle(hdc, 0, 0, DT_HDLWIDTH, DT_HEIGHT);

			SelectObject(hdc, hBrushOld);
			DeleteObject(hBrush);
			SelectObject(hdc, hPenOld);
			DeleteObject(hPen);	
			Sleep(10);

		} while (gValue > 55 || gValue < 255);
		
		//ReleaseMutex(GetTagSem((HWND)lpParam));
	}
}
/*
DWORD WINAPI TP_DT_DrawHandleTask(LPVOID lpParam)
{
	HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	HGDIOBJ hBrushOld, hPenOld;
	int i,j;
	hdc = GetWindowDC((HWND)lpParam);
	//WaitForSingleObject(hMutex, INFINITE);
	for(i = 0; i < 20; i++)
	{
		j = i * 10;
		hBrush = CreateSolidBrush(RGB(230,55 + j,55 + j));
		hPen = CreatePen(PS_SOLID, 0, RGB(230,55 + j,55 + j));
		hBrushOld = SelectObject(hdc, hBrush);
		hPenOld = SelectObject(hdc, hPen);

		Rectangle(hdc, 0, 0, DT_HDLWIDTH, DT_HEIGHT);

		SelectObject(hdc, hBrushOld);
		DeleteObject(hBrush);
		SelectObject(hdc, hPenOld);
		DeleteObject(hPen);	
		Sleep(10);
	}
	//ReleaseMutex(hMutex);
	return 0;
}

DWORD WINAPI TP_DT_EraseHandleTask(LPVOID lpParam)
{
	HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	HGDIOBJ hBrushOld, hPenOld;
	int i,j;
	hdc = GetWindowDC((HWND)lpParam);
	//WaitForSingleObject(hMutex, INFINITE);
	for(i = 0; i < 20; i++)
	{
		j = 190 - i * 10;
		hBrush = CreateSolidBrush(RGB(230,55 + j,55 + j));
		hPen = CreatePen(PS_SOLID, 0, RGB(230,55 + j,55 + j));
		hBrushOld = SelectObject(hdc, hBrush);
		hPenOld = SelectObject(hdc, hPen);

		Rectangle(hdc, 0, 0, DT_HDLWIDTH, DT_HEIGHT);

		SelectObject(hdc, hBrushOld);
		DeleteObject(hBrush);
		SelectObject(hdc, hPenOld);
		DeleteObject(hPen);	
		Sleep(10);
	}
	hBrush = CreateSolidBrush(RGB(230,55,55));
	hPen = CreatePen(PS_SOLID, 0, RGB(230,55,55));
	hBrushOld = SelectObject(hdc, hBrush);
	hPenOld = SelectObject(hdc, hPen);

	Rectangle(hdc, 0, 0, DT_HDLWIDTH, DT_HEIGHT);

	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
	//ReleaseMutex(hMutex);
	return 0;
}

*/
int TP_DT_OnMouseMove(HWND hWnd, WPARAM wParam, int x, int y)
{
	POINT pt;

	if(wParam == MK_LBUTTON)
	{
		pt.x = x;
		pt.y = y;
		ClientToScreen(hWnd, &pt);
		SetWindowPos(hWnd, HWND_TOPMOST, pt.x - ptOld.x, pt.y  - ptOld.y, NULL, NULL, SWP_NOSIZE);
		return 0;
	}

	if(MouseInWnd(hWnd))
	{
		return 0;
	}
	else
	{
		SetMouseInWnd(hWnd, true);
		TRACKMOUSEEVENT meTrack;
		meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		meTrack.dwFlags = TME_LEAVE;
		meTrack.hwndTrack = hWnd;
		if(0 == TrackMouseEvent(&meTrack))
		{
			MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
		}
	}
	
	ReleaseSemaphore(GetTagSem(hWnd), 1, NULL);
	return 0;
}

int TP_DT_OnMouseLeave(HWND hWnd, int x, int y)
{
#if 0
	HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	HGDIOBJ hBrushOld, hPenOld;
	int i,j;
	hdc = GetWindowDC(hWnd);
	SetMouseInWnd(hWnd, false);

	for(i = 0; i < 2000; i++)
	{
		//Sleep(1);
		j = 200 - i / 10;
		hBrush = CreateSolidBrush(RGB(230,55 + j,55 + j));
		hPen = CreatePen(PS_SOLID, 0, RGB(230,55 + j,55 + j));
		hBrushOld = SelectObject(hdc, hBrush);
		hPenOld = SelectObject(hdc, hPen);

		Rectangle(hdc, 0, 0, DT_HDLWIDTH, DT_HEIGHT);

		SelectObject(hdc, hBrushOld);
		DeleteObject(hBrush);
		SelectObject(hdc, hPenOld);
		DeleteObject(hPen);	
		for(j =0; j<10000; j++)
		{

		}
	}
#else
	SetMouseInWnd(hWnd, false);
	ReleaseSemaphore(GetTagSem(hWnd), 1, NULL);
#endif

	
	return 0;
}

tp_error_e InitTagCB()
{
	int i;
	for(i = 0; i < 256; i++)
	{
		tcb[i].hWnd = 0;
	}
	hMutex = CreateMutex(NULL, FALSE, NULL);
	if(hMutex == NULL)
		return TP_ERROR_FAILED;
	ReleaseMutex(hMutex);
	return TP_NO_ERROR;
}

static tp_error_e tcbCreateTag(HWND hWnd)
{
	int i;
	for(i = 0; i < 256; i++)
	{
		if(tcb[i].hWnd == 0)
		{
			tcb[i].hWnd = hWnd;
			tcb[i].pStrTitle = (WCHAR *)malloc(sizeof(WCHAR)*64);
			if(NULL == tcb[i].pStrTitle)
				return TP_ERROR_MEM;
			memset(tcb[i].pStrTitle, 0, 64);
			tcb[i].hSem = CreateSemaphore(NULL, 0, 1, NULL);
			CreateThread(NULL, 4096, TP_DT_PaintHandleTask, hWnd, NULL, NULL);

			return TP_NO_ERROR;
		}
	}
	return TP_ERROR_INVALID_HANDLE;
}

static bool MouseInWnd(HWND hWnd)
{
	int i;
	for(i = 0; i < 256; i++)
	{
		if(tcb[i].hWnd == hWnd)
			return tcb[i].bMouseIn;
	}
	return false;
}

static void SetMouseInWnd(HWND hWnd, bool bIn)
{
	int i;
	//WaitForSingleObject(hMutex, INFINITE);
	for(i = 0; i < 256; i++)
	{
		if(tcb[i].hWnd == hWnd)
		{
			tcb[i].bMouseIn = bIn;
			return;
		}
	}
	//ReleaseMutex(hMutex);
	return;
}

static tp_error_e SetTagTitle(HWND hWnd, WCHAR *str)
{
	int i;
	for(i = 0; i < 256; i++)
	{
		if(tcb[i].hWnd == hWnd)
		{
			wcscpy_s(tcb[i].pStrTitle, 64, str);
			return TP_NO_ERROR;
		}
	}
	return TP_ERROR_INVALID_HANDLE;
}

static WCHAR* GetTagTitle(HWND hWnd)
{
	int i;
	for(i = 0; i < 256; i++)
	{
		if(tcb[i].hWnd == hWnd)
		{
			return tcb[i].pStrTitle;
		}
	}
	return NULL;
}

static HANDLE GetTagSem(HWND hWnd)
{
	int i;
	for(i = 0; i < 256; i++)
	{
		if(tcb[i].hWnd == hWnd)
		{
			return tcb[i].hSem;
		}
	}

	return NULL;
}

static LRESULT CALLBACK TP_DT_WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//int wmId, wmEvent;

	switch (nMsg)
	{
	case WM_CREATE:
		SetLayeredWindowAttributes(hWnd, NULL, 200, LWA_ALPHA);
		//TRACKMOUSEEVENT meTrack;
		//meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		//meTrack.dwFlags = TME_LEAVE;
		//meTrack.hwndTrack = hWnd;
		//if(0 == TrackMouseEvent(&meTrack))
		//{
		//	MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
		//}
		break;
	case WM_PAINT:
		return TP_DT_OnPaint(hWnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		return TP_DT_OnMouseMove(hWnd, wParam, LOWORD(lParam), HIWORD(lParam));
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
		TP_DT_OnMouseLeave(hWnd, LOWORD(lParam), HIWORD(lParam));
		//TRACKMOUSEEVENT meTrack;
		//meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		//meTrack.dwFlags = TME_LEAVE;
		//meTrack.hwndTrack = hWnd;
		//if(0 == TrackMouseEvent(&meTrack))
		//{
		//	MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
		//}
		break;
	case WM_LBUTTONDOWN:
		ptOld.x = LOWORD(lParam);
		ptOld.y = HIWORD(lParam);

		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

INT TP_DT_RegistWindow()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= TP_DT_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIPPOP));
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_TIPPOP);
	wcex.lpszClassName	= szDTClassName;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

HWND TP_DT_CreateWindow(HWND hParent, WCHAR *str, bool bClickThrough, bool bTopMost, short iAlpha)
{
	HWND hWnd = NULL;
	unsigned long wsEx = 0;

	if(!hParent)
	{
		hParent = GetDesktopWindow();
	}
	wsEx = WS_EX_LAYERED;
	if(bClickThrough)
	{
		wsEx |= WS_EX_TRANSPARENT;
	}
	
	if(bTopMost)
	{
		wsEx |= WS_EX_TOPMOST;
	}

	hWnd = CreateWindowEx(wsEx, szDTClassName, szDTWndName, WS_POPUP | WS_VISIBLE, 500, 200 + tagCount * 35, DT_WIDTH, DT_HEIGHT, hParent, NULL, hInst, NULL);
	if(hWnd)
	{
		tagCount += 1;
	}

	tcbCreateTag(hWnd);
	SetTagTitle(hWnd, str);
	return hWnd;
}