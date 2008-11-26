#include "stdafx.h"
#include "TP_FlatButton.h"
#include <GdiPlus.h>

#define MAX_LOADSTRING 100

#define BTNTXTCLR Color(255, 0, 0, 0)
#define BTNBGCLR Color(255, 170, 170, 170)
#define BTNTXTCLRHOVER Color(255, 0, 0, 0)
#define BTNBGCLRHOVER Color(255, 170, 170, 255)

TCHAR szFBClassName[MAX_LOADSTRING] = L"FlatBtnWndCls";			// 主窗口类名
TCHAR szFBWndName[MAX_LOADSTRING] = L"FlatButton";

extern HINSTANCE hInst;

// Window Procedure
static LRESULT CALLBACK TP_FB_WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
typedef struct  
{
	UINT id;
	WCHAR caption[32];
}FB_DESCR, *PFB_DESCR;
 

static int TP_FB_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	PFB_DESCR pDescr;

	RECT rectWnd;
	GetWindowRect(hWnd, &rectWnd);
	
	hdc = BeginPaint(hWnd, &ps);

	Graphics graphics(hdc);
	RectF rfWnd(0, 0, (REAL)(rectWnd.right - rectWnd.left), (REAL)(rectWnd.bottom - rectWnd.top));
	//Pen pen(Color(255, 0, 0, 0));
	SolidBrush brush(BTNBGCLR);
	//graphics.DrawRectangle(&pen, rfWnd);
	graphics.FillRectangle((Brush *)&brush, rfWnd);

	FontFamily  fontFamily(DEFAULTFONT);
	Font        font(&fontFamily, SETTINGSFONTSIZE, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(BTNTXTCLR);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	
	pDescr = (PFB_DESCR)GetWindowLong(hWnd, 0);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &solidBrush);

	EndPaint(hWnd, &ps);

	return 0;
}

int TP_FB_OnMouseHover(HWND hWnd, WPARAM wParam, int x, int y)
{
	HDC hdc;
	PFB_DESCR pDescr;

	RECT rectWnd;
	GetWindowRect(hWnd, &rectWnd);

	hdc = GetWindowDC(hWnd);//BeginPaint(hWnd, &ps);

	Graphics graphics(hdc);
	RectF rfWnd(0, 0, (REAL)(rectWnd.right - rectWnd.left), (REAL)(rectWnd.bottom - rectWnd.top));
	//Pen pen(Color(255, 0, 0, 0));
	SolidBrush brush(BTNBGCLRHOVER);
	//graphics.DrawRectangle(&pen, rfWnd);
	graphics.FillRectangle((Brush *)&brush, rfWnd);

	FontFamily  fontFamily(DEFAULTFONT);
	Font        font(&fontFamily, SETTINGSFONTSIZE, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(BTNTXTCLRHOVER);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

	pDescr = (PFB_DESCR)GetWindowLong(hWnd, 0);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &solidBrush);
	//EndPaint(hWnd, &ps);

	TRACKMOUSEEVENT meTrack;
	meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	meTrack.dwFlags = TME_LEAVE;
	meTrack.hwndTrack = hWnd;
	if(0 == TrackMouseEvent(&meTrack))
	{
		MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
	}

	return 0;
}
int TP_FB_OnMouseDown(HWND hWnd, WPARAM wParam, int x, int y)
{
	//PAINTSTRUCT ps;
	HDC hdc;
	PFB_DESCR pDescr;

	RECT rectWnd;
	GetWindowRect(hWnd, &rectWnd);

	hdc = GetWindowDC(hWnd);//BeginPaint(hWnd, &ps);

	Graphics graphics(hdc);
	RectF rfWnd(0, 0, (REAL)(rectWnd.right - rectWnd.left), (REAL)(rectWnd.bottom - rectWnd.top));
	Pen pen(Color(255, 0, 0, 0));
	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	SolidBrush  blackBrush(Color(255, 0, 0, 0));
	FontFamily  fontFamily(DEFAULTFONT);
	Font        font(&fontFamily, SETTINGSFONTSIZE, FontStyleRegular, UnitPixel);
	graphics.DrawRectangle(&pen, rfWnd);
	graphics.FillRectangle((Brush *)&whiteBrush, rfWnd);

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

	pDescr = (PFB_DESCR)GetWindowLong(hWnd, 0);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &blackBrush);
	//EndPaint(hWnd, &ps);
	Sleep(50);

	graphics.FillRectangle((Brush *)&blackBrush, rfWnd);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &whiteBrush);

	Sleep(100);
	SolidBrush  cyanBrush(Color(255, 0, 255, 255));
	graphics.FillRectangle((Brush *)&cyanBrush, rfWnd);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &blackBrush);

	return 0;
}

int TP_FB_OnMouseLeave(HWND hWnd, int x, int y)
{
	//PAINTSTRUCT ps;
	HDC hdc;
	PFB_DESCR pDescr;

	RECT rectWnd;
	GetWindowRect(hWnd, &rectWnd);
	
	hdc = GetWindowDC(hWnd);//BeginPaint(hWnd, &ps);

	Graphics graphics(hdc);
	RectF rfWnd(0, 0, (REAL)(rectWnd.right - rectWnd.left), (REAL)(rectWnd.bottom - rectWnd.top));
	//Pen pen(Color(255, 0, 0, 0));
	SolidBrush brush(BTNBGCLR);
	//graphics.DrawRectangle(&pen, rfWnd);
	graphics.FillRectangle((Brush *)&brush, rfWnd);

	FontFamily  fontFamily(DEFAULTFONT);
	Font        font(&fontFamily, SETTINGSFONTSIZE, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(BTNTXTCLR);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	
	pDescr = (PFB_DESCR)GetWindowLong(hWnd, 0);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &solidBrush);
	//EndPaint(hWnd, &ps);

	TRACKMOUSEEVENT meTrack;
	meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
	meTrack.dwFlags = TME_HOVER;
	meTrack.hwndTrack = hWnd;
	meTrack.dwHoverTime = HOVER_DEFAULT;
	if(0 == TrackMouseEvent(&meTrack))
	{
		MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
	}

	return 0;
}

static void TP_FB_DrawButtonFace(HWND hWnd, Color &clrBg, Color &clrTxt)
{
	HDC hdc;
	PFB_DESCR pDescr;
	RECT rectWnd;

	hdc = GetWindowDC(hWnd);
	Graphics graphics(hdc);

	pDescr = (PFB_DESCR)GetWindowLong(hWnd, 0);

	// Draw background
	GetWindowRect(hWnd, &rectWnd);
	RectF rfWnd(0, 0, (REAL)(rectWnd.right - rectWnd.left), (REAL)(rectWnd.bottom - rectWnd.top));
	SolidBrush brush(clrBg);
	graphics.FillRectangle((Brush *)&brush, rfWnd);

	// Draw caption
	FontFamily  fontFamily(DEFAULTFONT);
	Font        font(&fontFamily, SETTINGSFONTSIZE, FontStyleRegular, UnitPixel);
	SolidBrush  solidBrush(clrTxt);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.DrawString(pDescr->caption, -1, &font, rfWnd, &format, &solidBrush);
}

static LRESULT CALLBACK TP_FB_WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	PFB_DESCR pDescr;
	TRACKMOUSEEVENT meTrack;
	WCHAR *pStr;
	pDescr = (PFB_DESCR)GetWindowLong(hWnd, 0);
	
	switch (nMsg)
	{
	case WM_CREATE:
		meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		meTrack.dwFlags = TME_HOVER | TME_LEAVE;
		meTrack.hwndTrack = hWnd;
		meTrack.dwHoverTime = HOVER_DEFAULT;
		if(0 == TrackMouseEvent(&meTrack))
		{
			MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
		}

		break;
	case WM_PAINT:
		return TP_FB_OnPaint(hWnd);
	case WM_DESTROY:
		free(pDescr);
		break;
	case WM_MOUSEMOVE:
		meTrack.cbSize = sizeof(TRACKMOUSEEVENT);
		meTrack.dwFlags = TME_HOVER | TME_LEAVE;
		meTrack.hwndTrack = hWnd;
		meTrack.dwHoverTime = 10;
		if(0 == TrackMouseEvent(&meTrack))
		{
			MessageBox(hWnd, L"track mouse event failed", NULL, NULL);
		}


		break;
	case WM_MOUSEHOVER:
	case WM_NCMOUSEHOVER:
		return TP_FB_OnMouseHover(hWnd, wParam, LOWORD(lParam), HIWORD(lParam));
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
		return TP_FB_OnMouseLeave(hWnd, LOWORD(lParam), HIWORD(lParam));
	case WM_LBUTTONDOWN:
		TP_FB_OnMouseDown(hWnd, wParam, LOWORD(lParam), HIWORD(lParam));
		SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(FLATBUTTONCMD, pDescr->id), (LPARAM)hWnd);
		break;
	case WM_SETTEXT:
		pStr = (WCHAR *)lParam;
		wcscpy_s(pDescr->caption, pStr);
		SendMessage(hWnd, WM_MOUSEMOVE, NULL, NULL);
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}

INT TP_FB_RegistControl()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= TP_FB_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(PFB_DESCR);
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIPPOP));
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_TIPPOP);
	wcex.lpszClassName	= szFBClassName;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

HWND TP_FB_CreateButton(HWND hParent, WCHAR *p_wcCaption, int id, int x, int y, int width, int height)
{
	HWND hWnd = NULL;
	unsigned long wsEx = 0;

	if(!hParent)
	{
		hParent = GetDesktopWindow();
	}
	wsEx = WS_EX_LAYERED;

	hWnd = CreateWindowEx(0L, szFBClassName, p_wcCaption, WS_CHILD | WS_VISIBLE, x, y, width, height, hParent, (HMENU)id, hInst, NULL);
	if(hWnd)
	{
		PFB_DESCR pDescr = (PFB_DESCR)malloc(sizeof(FB_DESCR));
		if(pDescr == NULL)
			return NULL;
		pDescr->id = id;
		wcscpy_s(pDescr->caption, p_wcCaption);
		SetWindowLong(hWnd, 0, (LONG)pDescr);
	}
	
	return hWnd;
}