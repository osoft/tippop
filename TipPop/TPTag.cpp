#include "StdAfx.h"
#include "TPTag.h"
#include "TP_DesktopTag.h"

extern HWND hWndMain;

CTPTag::CTPTag(WCHAR *str)
{
	this->hWndTag = TP_DT_CreateWindow(hWndMain, str, false, true, 100);
	hWndAllTags[lTagsCount] = this->hWndTag;
	lTagsCount++;
}

CTPTag::CTPTag(TiXmlNode *pTagNode)
{
	WCHAR str[64];
	//str = (WCHAR *)pTagNode->FirstChild()->Value();
	::MultiByteToWideChar(CP_ACP, NULL, pTagNode->FirstChild()->FirstChild()->ToText()->Value(), 64, str, 64);
	this->hWndTag = TP_DT_CreateWindow(hWndMain, str, false, true, 100);
	hWndAllTags[lTagsCount] = this->hWndTag;
	lTagsCount++;
	this->pTagXml = pTagNode;
}

CTPTag::~CTPTag(void)
{
}

void CTPTag::SetClickThrough(bool bValue)
{
	bClickThrough = bValue;
	unsigned long wsExSet, wsEx;
	wsExSet = WS_EX_LAYERED;
	
	wsEx = GetWindowLong(hWndAllTags[0], GWL_EXSTYLE);
	if(wsEx & WS_EX_TOPMOST)
	{
		wsExSet |= WS_EX_TOPMOST;
	}
	
	if(bValue)
	{
		wsExSet |= WS_EX_TRANSPARENT;
	}
	for(int i = 0; i < lTagsCount; i++)
	{
		if(hWndAllTags[i])
			SetWindowLong(hWndAllTags[i], GWL_EXSTYLE, wsExSet);
	}
}

void CTPTag::SetAlpha(short iValue)
{
	iAlpha = iValue;
}