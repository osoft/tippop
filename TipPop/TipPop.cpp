// TipPop.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TipPop.h"

#include "TP_DesktopTag.h"
#include "TP_FlatButton.h"
#include "TPTag.h"
#include <shellapi.h>
#include "tinyxml.h"

#if defined(NDEBUG) || !defined(_DEBUG)
#define OutputDebugStringA(x)
#define OutputDebugString(x)
#endif

#define MAX_LOADSTRING 100
#define IDM_MYEXIT 1001
#define IDM_SETTINGS 1002
#define IDM_ADDTIP 1003
#define IDM_LINE 1004

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
HWND hWndMain;
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
ULONG_PTR gdiplusToken;

static NOTIFYICONDATA nid;
static HMENU ppMenu;

static bool bClickThroughActivated = false;

TiXmlDocument doc("test.xml");

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

const unsigned int NUM_INDENTS_PER_SPACE=2;

const char * getIndent( unsigned int numIndents )
{
	static const char * pINDENT="                                      + ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt( unsigned int numIndents )
{
	static const char * pINDENT="                                        ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	const char* pIndent=getIndent(indent);
	printf("\n");
	while (pAttrib)
	{
		printf( "%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

		if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
		if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
		printf( "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}
	return i;	
}

void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 )
{
	if ( !pParent ) return;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	printf( "%s", getIndent(indent));
	OutputDebugStringA(getIndent(indent));
	int num;

	switch ( t )
	{
	case TiXmlNode::DOCUMENT:
		printf( "Document" );
		OutputDebugStringA( "Document" );
		break;

	case TiXmlNode::ELEMENT:
		printf( "Element [%s]", pParent->Value() );
		OutputDebugStringA("Element [");
		OutputDebugStringA(pParent->Value());
		OutputDebugString(L"]");
		num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
		switch(num)
		{
		case 0:  
			printf( " (No attributes)"); 
			OutputDebugStringA(" (No attributes)");
			break;
		case 1:  
			printf( "%s1 attribute", getIndentAlt(indent));
			OutputDebugStringA(getIndentAlt(indent));
			OutputDebugStringA(" attribute");
			break;
		default: 
			printf( "%s%d attributes", getIndentAlt(indent), num);
			OutputDebugStringA(getIndentAlt(indent));
			OutputDebugStringA(_itoa(num, NULL, NULL));
			OutputDebugStringA(" attributes");
			break;
		}
		if(0 == strcmp(pParent->Value(), "tip"))
		{
			WCHAR wcTitle[64];
			OutputDebugStringA("\nFound a Tip, dumping to output\n");
			::MultiByteToWideChar(CP_ACP, 0, pParent->FirstChild()->FirstChild()->ToText()->Value(), 64, wcTitle
				, 64);
			new CTPTag(pParent);
			dump_to_stdout(pParent->FirstChild());
			dump_to_stdout(pParent->FirstChild()->NextSibling());
			OutputDebugStringA("Done dumping\n");
		}
		break;

	case TiXmlNode::COMMENT:
		printf( "Comment: [%s]", pParent->Value());
		OutputDebugStringA( "Comment: [" );
		OutputDebugStringA(pParent->Value());
		OutputDebugStringA("]");
		break;

	case TiXmlNode::UNKNOWN:
		printf( "Unknown" );
		OutputDebugStringA("Unknown");
		break;

	case TiXmlNode::TEXT:
		pText = pParent->ToText();
		printf( "Text: [%s]", pText->Value());
		OutputDebugStringA("Text: [");
		OutputDebugStringA(pText->Value());
		OutputDebugStringA("]");
		break;

	case TiXmlNode::DECLARATION:
		printf( "Declaration" );
		OutputDebugStringA("Declaration");

		break;
	default:
		break;
	}
	printf( "\n" );
	OutputDebugStringA("\n");
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		dump_to_stdout( pChild, indent+1 );
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TIPPOP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIPPOP));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIPPOP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_TIPPOP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	//TiXmlDocument doc("test.xml");


	hWnd = CreateWindowEx(NULL, szWindowClass, szTitle, NULL,
	  400, 300, 180, 200, NULL, NULL, hInstance, NULL);
	hWndMain = hWnd;
	//SetLayeredWindowAttributes(hWnd, NULL, 220, LWA_ALPHA);
	ShowWindow(hWnd, SW_HIDE);
	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = hWnd;
	nid.uID = 17;
	nid.uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON;
	nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIPPOP));
	nid.uVersion = NOTIFYICON_VERSION;

	nid.uCallbackMessage = 0x1234;
	::MultiByteToWideChar(CP_ACP, 0, "TipPop", 6, nid.szTip, 6);
	Shell_NotifyIcon(NIM_ADD, &nid);
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
	if (!hWnd)
	{
	  return FALSE;
	}
	InitTagCB();
	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
	   printf("\n%s:\n", "test.xml");
	   dump_to_stdout(&doc);
	   //MessageBox(hWnd, L"Done loading xml",NULL, NULL);
	}
	else
	{
	   printf("Failed to load file \"%s\"\n", "test.xml");
	}


	return TRUE;
}

void OnNotifyIconMessage(HWND hWnd, UINT message)
{
	switch(message)
	{
	case WM_RBUTTONUP:
		POINT pt;
		UINT ulPopupFlags;

		GetCursorPos(&pt);
		
		if(pt.y > GetSystemMetrics(SM_CYSCREEN) - 200)
		{
			ulPopupFlags = TPM_BOTTOMALIGN;
		}
		else
		{
			ulPopupFlags = TPM_TOPALIGN;
		}

		TrackPopupMenuEx(ppMenu, ulPopupFlags, pt.x, pt.y, hWnd, NULL);
		
		break;
	default:
		break;
	}
	return;
}

void OnSettingsButton(HWND hWnd, UINT id, HWND hBtn)
{
	switch(id)
	{
	case 123:
		bClickThroughActivated = !bClickThroughActivated;
		CTPTag::SetClickThrough(bClickThroughActivated);
		if(bClickThroughActivated)
		{
			SetWindowText(hBtn, L"Click Through: ON");
		}
		else
		{
			SetWindowText(hBtn, L"Click Through: OFF");
		}
		break;
	case 321:
		SetWindowText(hBtn, L"TEST2");
		break;
	default:
		break;
	}
	return;
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CTPTag *tagWnd;
	switch (message)
	{
	case 0x1234:
		OnNotifyIconMessage(hWnd, (UINT)lParam);
		break;
	case WM_CREATE:
		// Regist tag windows class
		TP_DT_RegistWindow();
		TP_FB_RegistControl();
		// Init the tray icon
		ppMenu = CreatePopupMenu();
		AppendMenu(ppMenu, MF_STRING, IDM_ADDTIP, L"&Add a Tip!");
		AppendMenu(ppMenu, MF_STRING, IDM_SETTINGS, L"&Settings...");
		AppendMenu(ppMenu, MF_STRING, IDM_MYEXIT, L"E&xit");

		// Create the 'settings' interface
		TP_FB_CreateButton(hWnd, L"Click Through: OFF", 123, 10, 70, 160, 20);
		TP_FB_CreateButton(hWnd, L"Top Most: ON", 321, 10, 95, 160, 20);
		break;
	case WM_NOTIFY:
		wmId = LOWORD(wParam);
		MessageBox(hWnd, L"tray", L"perhaps", NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			break;
		case IDM_MYEXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_SETTINGS:
			ShowWindow(hWnd, SW_SHOW);
			break;
		case IDM_ADDTIP:
			tagWnd = new CTPTag(L"T����est");
			break;
		case FLATBUTTONCMD:
			//WCHAR tmp[32];
			//MessageBox(hWnd, _itow(wmEvent, tmp, 10), NULL, NULL);
			OnSettingsButton(hWnd, wmEvent, HWND(lParam));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		HBRUSH hBrush;
		HGDIOBJ hBrushOld;
		RECT rectWindow;

		hdc = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(RGB(40,80,190));
		hBrushOld = SelectObject(hdc, hBrush);
		GetWindowRect(hWnd, &rectWindow);
		Rectangle(hdc, 0, 0, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top);
		SelectObject(hdc, hBrushOld);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		// Remove the tray icon
		Shell_NotifyIcon(NIM_DELETE, &nid);
		GdiplusShutdown(gdiplusToken);

		// Pose quit message
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
