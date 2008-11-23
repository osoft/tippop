#ifndef __TP_FLATBUTTON_H__
#define __TP_FLATBUTTON_H__

#include "TipPop.h"

HWND TP_FB_CreateButton(HWND hParent, WCHAR *p_wcCaption, int id, int x, int y, int width, int height);
INT TP_FB_RegistControl();

#endif