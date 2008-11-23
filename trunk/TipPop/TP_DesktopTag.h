#ifndef __TP_DESKTOPTAG_H__
#define __TP_DESKTOPTAG_H__

#include "TipPop.h"

typedef struct  
{
	HWND hWnd;
	bool bMouseIn;
	WCHAR *pStrTitle;
	char *pStrContent;
	HANDLE hSem;
}TP_DT_TagCB_t;

HWND TP_DT_CreateWindow(HWND hParent, WCHAR *str, bool bClickThrough, bool bTopMost, short iAlpha);
INT TP_DT_RegistWindow();
int InitTagCB();


#endif