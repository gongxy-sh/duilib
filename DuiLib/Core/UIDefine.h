#pragma once
//////////////BEGIN消息映射宏定义////////////////////////////////////////////////////
///

namespace DuiLib
{

#define _USEIMM					1
#define MAX_FONT_ID				30000
#define LAYEREDUPDATE_TIMERID	0x2000

#define SCROLLBAR_LINESIZE      8

enum DuiSig
{
	DuiSig_end = 0, // [marks end of message map]
	DuiSig_lwl,     // LRESULT (WPARAM, LPARAM)
	DuiSig_vn,      // void (TNotifyUI)
};

class CControlUI;

// Structure for notifications to the outside world
typedef struct tagTNotifyUI 
{
	UINT nType;
	CDuiString sVirtualWnd;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
} TNotifyUI;

class CNotifyPump;
typedef void (CNotifyPump::*DUI_PMSG)(TNotifyUI& msg);  //指针类型

union DuiMessageMapFunctions
{
	DUI_PMSG pfn;   // generic member function pointer
	LRESULT (CNotifyPump::*pfn_Notify_lwl)(WPARAM, LPARAM);
	void (CNotifyPump::*pfn_Notify_vn)(TNotifyUI&);
};

//定义所有消息类型
//////////////////////////////////////////////////////////////////////////

#define DUI_MSGTYPE_MENU                   0x0000
#define DUI_MSGTYPE_LINK                   0x0001

#define DUI_MSGTYPE_TIMER                  0x0002
#define DUI_MSGTYPE_CLICK                  0x0003

#define DUI_MSGTYPE_RETURN                 0x0004
#define DUI_MSGTYPE_SCROLL                 0x0005

#define DUI_MSGTYPE_DROPDOWN               0x0006

#define DUI_MSGTYPE_SETFOCUS               0x0007
#define DUI_MSGTYPE_KILLFOCUS              0x0008

#define DUI_MSGTYPE_ITEMCLICK 		   	   0x0009
#define DUI_MSGTYPE_ITEMSELECT 		   	   0x000A
#define DUI_MSGTYPE_ITEMEXPAND             0x000B
#define DUI_MSGTYPE_ITEMDBCLICK        	   0x000C      
#define DUI_MSGTYPE_ITEMCOLLAPSE      	   0x000D       
#define DUI_MSGTYPE_ITEMACTIVATE      	   0x000E       

#define DUI_MSGTYPE_TABSELECT              0x000F

#define DUI_MSGTYPE_WINDOWINIT             0x0010    

#define DUI_MSGTYPE_BUTTONDOWN 		   	   0x0011   
#define DUI_MSGTYPE_MOUSEENTER		   	   0x0012	   
#define DUI_MSGTYPE_MOUSELEAVE		   	   0x0013	   

#define DUI_MSGTYPE_TEXTCHANGED        	   0x0014      
#define DUI_MSGTYPE_HEADERCLICK        	   0x0015      
#define DUI_MSGTYPE_SHOWACTIVEX       	   0x0016       

#define DUI_MSGTYPE_VALUECHANGED      	   0x0017      
#define DUI_MSGTYPE_SELECTCHANGED 	  	   0x0018	   

#define DUI_MSGTYPE_USER                   0x1000

//////////////////////////////////////////////////////////////////////////



//结构定义
typedef struct tagDUI_MSGMAP_ENTRY  //定义一个结构体，来存放消息信息
{
	UINT nMsgType;          // DUI消息类型
	LPCTSTR sCtrlName;         // 控件名称
	UINT       nSig;              // 标记函数指针类型
	DUI_PMSG   pfn;               // 指向函数的指针
}DUI_MSGMAP_ENTRY;

struct DUI_MSGMAP
{
#ifndef UILIB_STATIC
	const DUI_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
	const DUI_MSGMAP* pBaseMap;
#endif
	const DUI_MSGMAP_ENTRY* lpEntries;
};

//定义
#ifndef UILIB_STATIC
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static const DUI_MSGMAP messageMap;                                   \
	static const DUI_MSGMAP* PASCAL _GetBaseMessageMap();                 \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#else
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static  const DUI_MSGMAP messageMap;				                  \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#endif


//基类声明开始
#ifndef UILIB_STATIC
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
		{ return NULL; }                                                  \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{  &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };\
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{  NULL, &theClass::_messageEntries[0] };                         \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif


//子类声明开始
#ifndef UILIB_STATIC
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
		{ return &baseClass::messageMap; }                                \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
		{ return &theClass::messageMap; }                                 \
	UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
		{ &baseClass::messageMap, &theClass::_messageEntries[0] };        \
	UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif


//声明结束
#define DUI_END_MESSAGE_MAP()                                             \
	{ 0, _T(""), DuiSig_end, (DUI_PMSG)0 }                           \
};                                                                        \


//定义消息类型--执行函数宏
#define DUI_ON_MSGTYPE(msgtype, memberFxn)                                \
	{ msgtype, _T(""), DuiSig_vn, (DUI_PMSG)&memberFxn},                  \


//定义消息类型--控件名称--执行函数宏
#define DUI_ON_MSGTYPE_CTRNAME(msgtype,ctrname,memberFxn)                 \
	{ msgtype, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },                \


//定义click消息的控件名称--执行函数宏
#define DUI_ON_CLICK_CTRNAME(ctrname,memberFxn)                           \
	{ DUI_MSGTYPE_CLICK, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },      \


//定义selectchanged消息的控件名称--执行函数宏
#define DUI_ON_SELECTCHANGED_CTRNAME(ctrname,memberFxn)                   \
    { DUI_MSGTYPE_SELECTCHANGED,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn }, \


//定义killfocus消息的控件名称--执行函数宏
#define DUI_ON_KILLFOCUS_CTRNAME(ctrname,memberFxn)                       \
	{ DUI_MSGTYPE_KILLFOCUS,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },     \


//定义menu消息的控件名称--执行函数宏
#define DUI_ON_MENU_CTRNAME(ctrname,memberFxn)                            \
	{ DUI_MSGTYPE_MENU,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },          \


//定义与控件名称无关的消息宏

  //定义timer消息--执行函数宏
#define DUI_ON_TIMER()                                                    \
	{ DUI_MSGTYPE_TIMER, _T(""), DuiSig_vn,(DUI_PMSG)&OnTimer },          \


// Mark method as deprecated.
// example: DUI_DEPRECATED void func();
#if defined(_MSC_VER)
#if _MSC_VER > 1200
#  define DUI_DEPRECATED __declspec(deprecated)
#else
#  define DUI_DEPRECATED
#  define override
#  define __RPC__in
#  define __RPC__out
#  define __RPC__in_opt
#  define __RPC__deref_out_opt
#  define __RPC__in_ecount_full(a)
#  define __RPC__inout_ecount_full(a)
#  define __RPC__out_ecount_full(a)
#  define __RPC__inout_opt
//#  define OLECMD UINT
//#  define OLECMDTEXT BSTR
#define _tstof atof
#define __time32_t time_t
#endif
#elif defined(__GNUC__)
#  define DUI_DEPRECATED __attribute__ ((deprecated))
#else
#  pragma message("WARNING: You need to implement DUI_DEPRECATED for this compiler")
#  define DUI_DEPRECATED
#endif


///
//////////////END消息映射宏定义////////////////////////////////////////////////////


//////////////BEGIN控件名称宏定义//////////////////////////////////////////////////
///

#define  DUI_CTR_EDIT                            (_T("Edit"))
#define  DUI_CTR_LIST                            (_T("List"))
#define  DUI_CTR_TEXT                            (_T("Text"))
#define  DUI_CTR_TREE                            (_T("Tree"))
#define  DUI_CTR_HBOX                            (_T("HBox"))
#define  DUI_CTR_VBOX                            (_T("VBox"))

#define  DUI_CTR_ILIST                           (_T("IList"))
#define  DUI_CTR_COMBO                           (_T("Combo"))
#define  DUI_CTR_LABEL                           (_T("Label"))
#define  DUI_CTR_FLASH							 (_T("Flash"))

#define  DUI_CTR_BUTTON                          (_T("Button"))
#define  DUI_CTR_OPTION                          (_T("Option"))
#define  DUI_CTR_SLIDER                          (_T("Slider"))

#define  DUI_CTR_CONTROL                         (_T("Control"))
#define  DUI_CTR_ACTIVEX                         (_T("ActiveX"))
#define  DUI_CTR_GIFANIM                         (_T("GifAnim"))

#define  DUI_CTR_PROGRESS                        (_T("Progress"))
#define  DUI_CTR_RICHEDIT                        (_T("RichEdit"))
#define  DUI_CTR_CHECKBOX                        (_T("CheckBox"))
#define  DUI_CTR_COMBOBOX                        (_T("ComboBox"))
#define  DUI_CTR_DATETIME                        (_T("DateTime"))
#define  DUI_CTR_TREEVIEW                        (_T("TreeView"))
#define  DUI_CTR_TREENODE                        (_T("TreeNode"))

#define  DUI_CTR_ILISTITEM                       (_T("IListItem"))
#define  DUI_CTR_CONTAINER                       (_T("Container"))
#define  DUI_CTR_TABLAYOUT                       (_T("TabLayout"))
#define  DUI_CTR_SCROLLBAR                       (_T("ScrollBar"))

#define  DUI_CTR_ICONTAINER                      (_T("IContainer"))
#define  DUI_CTR_ILISTOWNER                      (_T("IListOwner"))
#define  DUI_CTR_LISTHEADER                      (_T("ListHeader"))
#define  DUI_CTR_TILELAYOUT                      (_T("TileLayout"))
#define  DUI_CTR_WEBBROWSER                      (_T("WebBrowser"))

#define  DUI_CTR_CHILDLAYOUT                     (_T("ChildLayout"))
#define  DUI_CTR_LISTELEMENT                     (_T("ListElement"))
#define  DUI_CTR_VIRTUALLIST                     (_T("VirtualList"))

#define  DUI_CTR_VERTICALLAYOUT                  (_T("VerticalLayout"))
#define  DUI_CTR_LISTHEADERITEM                  (_T("ListHeaderItem"))

#define  DUI_CTR_LISTHBOXELEMENT                 (_T("ListHBoxElement"))
#define  DUI_CTR_LISTTEXTELEMENT                 (_T("ListTextElement"))

#define  DUI_CTR_HORIZONTALLAYOUT                (_T("HorizontalLayout"))
#define  DUI_CTR_LISTLABELELEMENT                (_T("ListLabelElement"))

#define  DUI_CTR_LISTCONTAINERELEMENT            (_T("ListContainerElement"))


///
//////////////END控件名称宏定义//////////////////////////////////////////////////


}// namespace DuiLib

