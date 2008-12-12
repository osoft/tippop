#include "stdafx.h"
#include "TP_XMLAdapter.h"
#include <shellapi.h>

#if defined(NDEBUG) || !defined(_DEBUG)
#define OutputDebugStringA(x)
#endif

const unsigned int NUM_INDENTS_PER_SPACE=2;

static TiXmlDocument xmlDoc("tp.xml");

static const char * getIndent( unsigned int numIndents )
{
	static const char * pINDENT="                                      + ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
static const char * getIndentAlt( unsigned int numIndents )
{
	static const char * pINDENT="                                        ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

static int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
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

static void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 )
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
			//new CTPTag(pParent);
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


tp_error_e TP_XML_Init(void)
{
	bool loadOkay = xmlDoc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", "test.xml");
		dump_to_stdout(&xmlDoc);
		//MessageBox(hWnd, L"Done loading xml",NULL, NULL);
	}
	else
	{
		printf("Failed to load file \"%s\"\n", "test.xml");
	}

	return TP_NO_ERROR;
}

tp_error_e TP_XML_Tips_Read(void)
{
	dump_to_stdout(&xmlDoc);
	return TP_NO_ERROR;
}

TiXmlNode * TP_XML_Tip_Create(TP_XML_Tip_t &tip)
{
	char chTemp[256];
	::WideCharToMultiByte(CP_ACP, 0, L"ASDF", 256, chTemp, 256, " ", FALSE);
	TiXmlElement elmtNewTip("tip");
	TiXmlElement elmtTitle("title");
	TiXmlElement elmtContent("content");

	::WideCharToMultiByte(CP_ACP, 0, tip.pwcTitle, 256, chTemp, 256, NULL, false);
	TiXmlText txtTitle(chTemp);
	//txtTitle.SetValue(tip.pwcTitle);
	TiXmlText txtContent(chTemp);
	//txtTitle.SetValue(tip.pwcContent);
	elmtTitle.InsertEndChild(txtTitle);
	elmtContent.InsertEndChild(txtContent);
	elmtNewTip.InsertEndChild(elmtTitle);
	elmtNewTip.InsertEndChild(elmtContent);
	OutputDebugStringA("||||||||||||||||||||||||\n");
	dump_to_stdout(&elmtNewTip);
	OutputDebugStringA("||||||||||||||||||||||||\n");
	return &elmtTitle;
}

tp_error_e TP_XML_Tip_Delete(TP_XML_Tip_t &tip);

tp_error_e TP_XML_Settings_Read(TP_XML_Settings_e enmSetting, int value);

tp_error_e TP_XML_Settings_Write(TP_XML_Settings_e enmSetting, int value);
