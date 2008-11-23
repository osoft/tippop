#ifndef __TPTAG_H__
#define __TPTAG_H__

#pragma once
#include "stdafx.h"
#include "tinyxml.h"

static HWND hWndAllTags[256];
static int lTagsCount = 0;
static bool bClickThrough;
static short iAlpha;
static bool bTopMost;

class CTPTag
{
private:
	TiXmlNode *pTagXml;
	HWND hWndTag;
public:
	
public:
	CTPTag(WCHAR *str);
	CTPTag(TiXmlNode *pTagNode);
	~CTPTag(void);
	HWND GetTagHWND();
	static void SetClickThrough(bool);
	static void SetAlpha(short);
};

#endif