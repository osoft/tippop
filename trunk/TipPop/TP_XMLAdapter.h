#ifndef __TP_XMLADAPTER_H__
#define __TP_XMLADAPTER_H__

#include "TipPop.h"

typedef struct  
{
	WCHAR *pwcTitle;
	WCHAR *pwcContent;
	POINT pt;
}TP_XML_Tip_t;

typedef enum
{
	TP_XML_SETTINGS_CLICKTHROUGH,
	TP_XML_SETTINGS_TOPMOST,
	TP_XML_SETTINGS_ALPHA
}TP_XML_Settings_e;

tp_error_e TP_XML_Init(void);

tp_error_e TP_XML_Tips_Read(void);

tp_error_e TP_XML_Tip_Create(TP_XML_Tip_t &tip);

tp_error_e TP_XML_Tip_Delete(TP_XML_Tip_t &tip);

tp_error_e TP_XML_Setting_Read(TP_XML_Settings_e enmSetting, int value);

tp_error_e TP_XML_Setting_Write(TP_XML_Settings_e enmSetting, int value);

#endif