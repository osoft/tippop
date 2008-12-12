#pragma once

#include "resource.h"

#define FLATBUTTONCMD 0x213
#define DEFAULTFONT L"Î¢ÈíÑÅºÚ"
#define SETTINGSFONTSIZE 11
#define TIPTAGSFONTSIZE 11
typedef enum
{
	TP_NO_ERROR = 0,
	TP_ERROR_FAILED,
	TP_ERROR_INVALID_PARA,
	TP_ERROR_INVALID_HANDLE,
	TP_ERROR_MEM
} tp_error_e;

