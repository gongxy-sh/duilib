// stdafx.h

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS
#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxdtctl.h>
#include <afxcmn.h>

#include <afxtempl.h>
#include <BCGCBProInc.h>			// BCGControlBar Pro

#define AFX_DEFAULT_TOOLBAR_STYLE     (WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_HIDE_INPLACE)

#define MinOf(a,b) ((a)<(b)?(a):(b))
#define MaxOf(a,b) ((a)>(b)?(a):(b))
#define SIZE_T_NEG_1 (size_t)-1

#ifdef _DEBUG
#define DEBUG_BREAK()           { if (IsDebuggerPresent()) { __asm int 3 }}
#define DEBUG_BREAK_ONCE()      { static char __b = 0; if (!__b) DEBUG_BREAK(); __b = 1; }
#define DEBUG_BREAK_IF(expr)	  if ((expr)) DEBUG_BREAK()
#else //!_DEBUG
#define DEBUG_BREAK()
#define DEBUG_BREAK_ONCE()
#define DEBUG_BREAK_IF(expr)
#endif //!_DEBUG

#if _MSC_VER <= 1200
#define _countof(a)                   (sizeof(a)/sizeof(*a))
#define _stprintf_s                   _stprintf
#define _stscanf_s                    _stscanf
#define _tcscpy_s                     _tcscpy
#define _tcscat_s                     _tcscat
#define ENSURE(s)
#define ENSURE_VALID(s)

#define ON_WM_MOUSELEAVE() \
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnMouseLeave },

#endif //_MSC_VER

//////////////////////////////////////////////////////////////////////////
//Defines

#define FORM_INIT_WIDTH  400
#define FORM_INIT_HEIGHT 400

#define FORM_OFFSET_X 20
#define FORM_OFFSET_Y 20

#define COPY_OFFSET_XY 10

#define UI_DEFAULT_WIDTH  60
#define UI_DEFAULT_HEIGHT 30

#define TRACKER_HANDLE_SIZE 7
#define MICO_MOVE_SPACE 1

#define DIR_STYLES _T("Styles\\")
#define DIR_TEMPLATES _T("Templates\\")

#define UI_COMMAND_HISTORY 10

//UI type
enum UIType
{
	typeControl=1,
	typeContainer,
	typeForm,
};

//UI class
enum UIClass
{
	classPointer=100,
	classWindow,
	classControl,
	classButton,
	classEdit,
	classLabel,
	classText,
	classOption,
	classCombo,
	classList,
	classSlider,
	classProgress,
	classActiveX,
	classContainer,
	classVerticalLayout,
	classHorizontalLayout,
	classTabLayout,
	classTileLayout,
	classListHeader,
	classListHeaderItem,
	classListTextElement,
	classListLabelElement,
	classListExpandElement,
	classListContainerElement,
	classItem,
	classScrollBar,
	classChildLayout,
	classWebBrowser
};

typedef struct tagExtendedAttributes
{
	HTREEITEM hItem;
	int nClass;
	int nDepth;
}ExtendedAttributes;

typedef struct tagUIAttribute
{
	LPCTSTR pstrName;
	LPCTSTR pstrValue;
}UIAttribute;

//////////////////////////////////////////////////////////////////////////
//Global variables

#include "..\DuiLib\UIlib.h"
#include "MainFrm.h"
#include "ToolBoxWnd.h"
#include "PropertiesWnd.h"
#include "FileView.h"
#include "ClassView.h"
#include "ResourceView.h"
#include "HookAPI.h"
#include "GlobalVariable.h"

#define DUI_MSGTYPE_SETPOS               DUI_MSGTYPE_USER
#define DUI_MSGTYPE_PROPERTYBEGINCHANGED (DUI_MSGTYPE_USER+1)
#define DUI_MSGTYPE_PROPERTYENDCHANGED   (DUI_MSGTYPE_USER+2)
#define DUI_MSGTYPE_POSBEGINCHANGED      (DUI_MSGTYPE_USER+3)
#define DUI_MSGTYPE_POSENDCHANGED        (DUI_MSGTYPE_USER+4)
#define DUI_MSGTYPE_FORMSIZE             (DUI_MSGTYPE_USER+5)

using namespace DuiLib;

extern CMainFrame* g_pMainFrame;
extern CToolBoxWnd* g_pToolBoxWnd;
extern CPropertiesWnd* g_pPropertiesWnd;
extern CFileView* g_pFileView;
extern CClassView* g_pClassView;
extern CResourceViewBar* g_pResourceView;
extern CHookAPI g_HookAPI;

//////////////////////////////////////////////////////////////////////////
//Global functions

extern int gGetUIClass(CControlUI* pControl);
extern int gGetUIClass(LPCTSTR pstrcClass);