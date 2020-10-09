#include "StdAfx.h"
#include "UIProperties.h"
#include "UiDesigner.h"

#include "UIDesignerView.h"
#include "ImageDialog.h"
#include "DialogCustomFonts.h"
#include "DialogDefaultAttribList.h"

//////////////////////////////////////////////////////////////////////////
//CBCGPColor32Prop

CBCGPColor32Prop::CBCGPColor32Prop(const CString& strName, const COLORREF& color,CPalette* pPalette/*=NULL*/,LPCTSTR lpszDescr/*=NULL*/,DWORD_PTR dwData/*=0*/)
:CBCGPColorProp(strName,color,pPalette,lpszDescr,dwData)
{
}

BOOL CBCGPColor32Prop::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	COLORREF colorCurr = m_Color;
	int nA = 0,nR = 0, nG = 0, nB = 0;
	_stscanf_s(strText, _T("%2x%2x%2x%2x"), &nA, &nR, &nG, &nB);
	if(nA == 0 && (nR !=0 || nG != 0 || nB !=  0))
		nA = 0xFF;
	m_Color = RGBA(nA, nR, nG, nB);

	if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}

void CBCGPColor32Prop::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height();
	CBCGPProp::OnDrawValue(pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height();
	rectColor.DeflateRect(1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : (m_Color&0x00FFFFFF));
	pDC->FillRect(rectColor, &br);
	pDC->Draw3dRect(rectColor, 0, 0);
}

CString CBCGPColor32Prop::FormatProperty()
{
	ASSERT_VALID(this);

	CString str;
	str.Format(_T("%02x%02x%02x%02x"),GetAValue(m_Color),GetRValue(m_Color),GetGValue(m_Color),GetBValue(m_Color));

	return str;
}

//////////////////////////////////////////////////////////////////////////
//CBCGPImageProp

IMPLEMENT_DYNAMIC(CBCGPImageProp, CBCGPProp)

CBCGPImageProp::CBCGPImageProp(const CString& strName, const CString& strImage, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
: CBCGPProp(strName, COleVariant((LPCTSTR)strImage), lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CBCGPImageProp::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CString strImage = m_varValue.bstrVal;
	CImageDialog dlg(strImage);
	if(dlg.DoModal()==IDOK)
	{
		strImage=dlg.GetImage();
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText(strImage);
	}
	m_varValue = (LPCTSTR) strImage;

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CBCGPCustomFontsProp

IMPLEMENT_DYNAMIC(CBCGPCustomFontsProp, CBCGPProp)

CBCGPCustomFontsProp::CBCGPCustomFontsProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
: CBCGPProp(strName, varValue, lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CBCGPCustomFontsProp::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CDialogCustomFonts dlg;
	dlg.DoModal();

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CBCGPDefaultAttribListProp

IMPLEMENT_DYNAMIC(CBCGPDefaultAttribListProp, CBCGPProp)

CBCGPDefaultAttribListProp::CBCGPDefaultAttribListProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
: CBCGPProp(strName, varValue, lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CBCGPDefaultAttribListProp::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CDialogDefaultAttribList dlg;
	dlg.DoModal();

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CUIProperties

UINT WM_UI_PROPERTY_CHANGED = ::RegisterWindowMessage(_T("WM_UI_PROPERTY_CHANGED"));

IMPLEMENT_DYNAMIC(CUIProperties, CWnd)

CUIProperties::CUIProperties(void)
{
}

CUIProperties::~CUIProperties(void)
{
}

BEGIN_MESSAGE_MAP(CUIProperties, CWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_PROPERTIES_TOOLBAR_SORT, OnPropertiesSort)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_TOOLBAR_SORT, OnUpdatePropertiesSort)
	ON_COMMAND(ID_PROPERTIES_TOOLBAR_ALPHABETIC, OnPropertiesAlphaBetic)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_TOOLBAR_ALPHABETIC, OnUpdatePropertiesAlphaBetic)
END_MESSAGE_MAP()

BOOL CUIProperties::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(globalData.RegisterWindowClass(_T("UIPropWnd")), _T(""), dwStyle, rect, pParentWnd, nID, NULL);
}

int CUIProperties::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if(!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);
#if _MSC_VER > 1200
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
#endif //_MSC_VER
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}

void CUIProperties::AdjustLayout()
{
	if(GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() -cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CUIProperties::SetVSDotNetLook(BOOL bSet)
{
	m_wndPropList.SetVSDotNetLook(bSet);
	m_wndPropList.SetGroupNameFullWidth(bSet);
}

void CUIProperties::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_wndPropList.SetFocus();
}

void CUIProperties::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

LRESULT CUIProperties::OnPropertyChanged(WPARAM wp, LPARAM lp)
{
	CBCGPProp* pProp = (CBCGPProp *)lp;

	int nLevel = pProp->GetHierarchyLevel();
	for(; nLevel>1; nLevel--)
		pProp = pProp->GetParent();

	GetOwner()->SendMessage(WM_UI_PROPERTY_CHANGED, wp, LPARAM(pProp));

	return TRUE;
}

void CUIProperties::OnPropertiesSort()
{
	m_wndPropList.SetAlphabeticMode(FALSE);
}

void CUIProperties::OnUpdatePropertiesSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(!m_wndPropList.IsAlphabeticMode());
}

void CUIProperties::OnPropertiesAlphaBetic()
{
	m_wndPropList.SetAlphabeticMode(TRUE);
}

void CUIProperties::OnUpdatePropertiesAlphaBetic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CUIProperties::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();

	CBCGPProp* pPropUI=NULL;
	CBCGPProp* pValueList=NULL;
	CBCGPProp* pProp=NULL;
	CBCGPColorProp* pPropColor=NULL;
	CBCGPImageProp* pPropImage=NULL;

	//Window
#if _MSC_VER > 1200
#pragma region Window
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Window"),classWindow);

	pValueList=new CBCGPProp(_T("Size"),tagWindowSize,TRUE);//size
	pProp=new CBCGPProp(_T("Width"),-1,(long)0,_T("窗体的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Height"),-1,(long)0,_T("窗体的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("Caption"),tagCaption,TRUE);//caption
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("标题的Left位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("标题的Top位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("标题的Right位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("标题的Bottom位置"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("SizeBox"),tagSizeBox,TRUE);//sizebox
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("尺寸盒的Left位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("尺寸盒的Top位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("尺寸盒的Right位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("尺寸盒的Bottom位置"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("RoundCorner"),tagRoundCorner,TRUE);//roundcorner
	pProp=new CBCGPProp(_T("Width"),-1,(long)0,_T("圆角的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Height"),-1,(long)0,_T("圆角的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("MinInfo"),tagMinInfo,TRUE);//mininfo
	pProp=new CBCGPProp(_T("MinWidth"),-1,(long)0,_T("窗口的最小跟踪宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("MinHeight"),-1,(long)0,_T("窗口的最小跟踪高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("MaxInfo"),tagMinInfo,TRUE);//maxinfo
	pProp=new CBCGPProp(_T("MaxWidth"),-1,(long)0,_T("窗口的最大跟踪宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Maxeight"),-1,(long)0,_T("窗口的最大跟踪高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CBCGPProp(_T("ShowDirty"),false,_T("指示是否显示更新区域"),tagShowDirty);//showdirty
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPCustomFontsProp(_T("CustomFonts"),(_variant_t)_T("字体管理"),_T("管理自定义的字体"),tagCustomFonts);//customfonts
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPDefaultAttribListProp(_T("DefaultAttribList"),(_variant_t)_T("默认属性列表管理"),_T("管理默认的属性列表"),tagDefaultAttribList);//defaultattriblist
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	// tagAlpha
	pProp=new CBCGPProp(_T("Alpha"),-1,(long)0,_T("窗口的alpha值(0-255)\n255"),tagAlpha);
	pPropUI->AddSubItem(pProp);

	// tagBkTrans
	pProp=new CBCGPProp(_T("BkTrans"),false,_T("窗口是否使用静态透明背景\nfalse"),tagBkTrans);
	pPropUI->AddSubItem(pProp);

	// tagDefaultFontColor
	pPropColor=new CBCGPColor32Prop(_T("DefaultFontColor"),RGBA(0,0,0,0),NULL,_T("指定默认的字体颜色"),tagDefaultFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagSelectedFontColor
	pPropColor=new CBCGPColor32Prop(_T("SelectedColor"),RGBA(0,0,0,0),NULL,_T("指定默认的selected字体颜色"),tagSelectedFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//	DisabledFontColor
	pPropColor=new CBCGPColor32Prop(_T("DisabledFontColor"),RGBA(0,0,0,0),NULL,_T("指定默认的Disabled字体颜色"),tagDisabledFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagLinkFontColor
	pPropColor=new CBCGPColor32Prop(_T("LinkFontColor"),RGBA(0,0,0,0),NULL,_T("指定默认的link字体颜色"),tagLinkFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagLinkHoverFontColor
	pPropColor=new CBCGPColor32Prop(_T("LinkHoverFontColor"),RGBA(0,0,0,0),NULL,_T("指定默认的linkhoverfont字体颜色"),tagLinkHoverFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Window
#endif //_MSC_VER

	//Control
#if _MSC_VER > 1200
#pragma region Control
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Control"),classControl);

	pProp=new CBCGPProp(_T("Name"),_T(""),_T("控件的名称"),tagName);//name
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Text"),_T(""),_T("控件的显示文本"),tagText);//text
	pPropUI->AddSubItem(pProp);

	pValueList=new CBCGPProp(_T("Pos"),tagPos,TRUE);//pos
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("控件的Left位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("控件的Top位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("控件的Right位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("控件的Bottom位置"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("RelativePos"),tagRelativePos,TRUE);//relativepos
	pProp=new CBCGPProp(_T("MoveX"),-1,(long)0,_T("控件的水平位移"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("MoveY"),-1,(long)0,_T("控件的垂直位移"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("ZoomX"),-1,(long)0,_T("控件的水平比例"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("ZoomY"),-1,(long)0,_T("控件的垂直比例"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("Size"),tagSize,TRUE);//size
	pProp=new CBCGPProp(_T("Width"),-1,(long)0,_T("控件的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Height"),-1,(long)0,_T("控件的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("MinSize"),tagMinSize,TRUE);//minsize
	pProp=new CBCGPProp(_T("MinWidth"),-1,(long)0,_T("指定控件的最小宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("MinHeight"),-1,(long)0,_T("指定控件的最小高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("MaxSize"),tagMaxSize,TRUE);
	pProp=new CBCGPProp(_T("MaxWidth"),-1,(long)0,_T("指定控件的最大宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("MaxHeight"),-1,(long)0,_T("指定控件的最大高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("Padding"),tagPadding,TRUE);//padding
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("指定控件内部的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("指定控件内部的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("指定控件内部的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("指定控件内部的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pPropImage=new CBCGPImageProp(_T("BkImage"),_T(""),_T("指定控件的背景图片"),tagBkImage);//bkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropColor=new CBCGPColor32Prop(_T("BkColor"),RGBA(0,0,0,0),NULL,_T("指定控件的背景颜色"),tagBkColor);//bkcolor
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pPropColor=new CBCGPColorProp(_T("BkColor2"),(long)RGB(0,0,0),NULL,_T("指定控件的背景颜色2"),tagBkColor2);//bkcolor2
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pPropColor=new CBCGPColorProp(_T("BorderColor"),(long)RGB(0,0,0),NULL,_T("指定控件的边框颜色"),tagBorderColor);//bordercolor
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//focusbordercolor
	pPropColor=new CBCGPColorProp(_T("focusbordercolor"),(long)RGB(0,0,0),NULL,_T("指定控件边框获得焦点时边框的颜色"),tagFocusBorderColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pProp=new CBCGPProp(_T("BorderSize"),-1,(long)0,_T("指定控件的边框线宽\n1"),tagBorderSize);//bordersize
	pPropUI->AddSubItem(pProp);

	//borderround
	pValueList=new CBCGPProp(_T("borderround"),tagBorderRound,TRUE);
	pProp=new CBCGPProp(_T("Width"),-1,(long)0,_T("边框圆角的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Height"),-1,(long)0,_T("边框圆角的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CBCGPProp(_T("Enabled"),true,_T("指示是否已启用该控件\nTrue"),tagEnabled);//enabled
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Float"),false,_T("确定该控件是固定的，还是浮动的\nFalse"),tagFloat);//float
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Visible"),true,_T("确定该控件是可见的，还是隐藏的\nTrue"),tagVisible);//visible
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Mouse"),true,_T("指示该控件是否响应鼠标操作\nTrue"),tagMouse);//mouse
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Menu"),false,_T("指示该控件是否需要右键菜单\nFalse"),tagMenu);//menu
	pPropUI->AddSubItem(pProp);

	//colorhsl
	pProp=new CBCGPProp(_T("ColorHSL"),false,_T("指示该控件的颜色是否随窗口的hsl变化而变化\nFalse"),tagColorHSL);
	pPropUI->AddSubItem(pProp);

	//tooltip
	pProp=new CBCGPProp(_T("Tooltip"),_T(""),_T("指示该控件鼠标悬浮提示"),tagTooltip);
	pPropUI->AddSubItem(pProp);

	//userdata
	pProp=new CBCGPProp(_T("UserData"),_T(""),_T("指示该控件自定义标识\nFalse"),tagUserData);
	pPropUI->AddSubItem(pProp);

	//keyboard
	pProp=new CBCGPProp(_T("KeyBoard"),false,_T("指示CButton类控件是否接受TabStop和按键事件\nFalse"),tagKeyBoard);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Control
#endif //_MSC_VER

	//Label
#if _MSC_VER > 1200
#pragma region Label
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Label"),classLabel);

	//align
	pProp=new CBCGPProp(_T("Align"),_T("Center"),_T("指示文本的对齐方式"),tagAlign);
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Right"));
	pProp->AddOption(_T("Top"));
	pProp->AddOption(_T("Bottom"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//textcolor
	pPropColor=new CBCGPColorProp(_T("TextColor"),(long)RGB(0,0,0),NULL,_T("指定文本的颜色"),tagTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//disabledtextcolor
	pPropColor=new CBCGPColorProp(_T("DisabledTextColor"),(long)RGB(0,0,0),NULL,_T("指定文本的颜色"),tagDisabledTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//font
	pProp=new CBCGPProp(_T("Font"),(long)-1,_T("指定文本的字体"),tagFont);
	pPropUI->AddSubItem(pProp);

	//textpadding
	pValueList=new CBCGPProp(_T("TextPadding"),tagTextPadding,TRUE);
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("指定文本区域的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("指定文本区域的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("指定文本区域的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("指定文本区域的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//showhtml
	pProp=new CBCGPProp(_T("ShowHtml"),false,_T("指示是否使用HTML格式的文本"),tagShowHtml);
	pPropUI->AddSubItem(pProp);

	//endellipsis
	pProp=new CBCGPProp(_T("EndEllipsis"),false,_T("指示句末显示不全是否使用...代替"),tagEndEllipsis);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Label
#endif //_MSC_VER

	//Button
#if _MSC_VER > 1200
#pragma region Button
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Button"),classButton);

	pPropImage=new CBCGPImageProp(_T("NormalImage"),_T(""),_T("指定按钮正常显示时的图片"),tagNormalImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("HotImage"),_T(""),_T("指定按钮获得热点时的图片"),tagHotImage);//hotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("PushedImage"),_T(""),_T("指定按钮被按压下时的图片"),tagPushedImage);//pushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("FocusedImage"),_T(""),_T("指定按钮获得焦点后的图片"),tagFocusedImage);//focusedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("DisabledImage"),_T(""),_T("指定按钮被禁用后的图片"),tagDisabledImage);//disabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Button
#endif //_MSC_VER

	//Edit
#if _MSC_VER > 1200
#pragma region Edit
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Edit"),classEdit);

	//normalimage
	pPropImage=new CBCGPImageProp(_T("NormalImage"),_T(""),_T("指定编辑框正常显示时的图片"),tagEditNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotimage
	pPropImage=new CBCGPImageProp(_T("HotImage"),_T(""),_T("指定编辑框获得热点时的图片"),tagEditHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//focusedimage
	pPropImage=new CBCGPImageProp(_T("FocusedImage"),_T(""),_T("指定编辑框获得焦点后的图片"),tagEditFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//disabledimage
	pPropImage=new CBCGPImageProp(_T("DisabledImage"),_T(""),_T("指定编辑框被禁用后的图片"),tagEditDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//readonly
	pProp=new CBCGPProp(_T("ReadOnly"),false,_T("指示是否只读"),tagReadOnly);
	pPropUI->AddSubItem(pProp);

	//password
	pProp=new CBCGPProp(_T("Password"),false,_T("指示是否使用密码框"),tagPassword);
	pPropUI->AddSubItem(pProp);

	//maxchar
	pProp=new CBCGPProp(_T("MaxChar"),-1,(long)0,_T("指示输入字符最大长度\n255"),tagMaxChar);
	pPropUI->AddSubItem(pProp);

	//nativebkcolor
	pPropColor=new CBCGPColor32Prop(_T("NativeBKColor"),RGBA(0,0,0,0),NULL,_T("指定Windows原生Edit控件的背景颜色"),tagNativeBKColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Edit
#endif //_MSC_VER

	//Option
#if _MSC_VER > 1200
#pragma region Option
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Option"),classOption);

	pPropImage=new CBCGPImageProp(_T("ForeImage"),_T(""),_T("指定复选框的前景图片"),tagOptForeImage);//foreimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("SelectedImage"),_T(""),_T("指定复选框被选择后的图片"),tagSelectedImage);//selectedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pProp=new CBCGPProp(_T("Selected"),false,_T("指示是否已被选中"),tagSelected);//selected
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Group"),_T(""),_T("指定参与组合的名称"),tagGroup);//group
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Option
#endif //_MSC_VER

	//Progress
#if _MSC_VER > 1200
#pragma region Progress
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Progress"),classProgress);

	pPropImage=new CBCGPImageProp(_T("ForeImage"),_T(""),_T("指定进度条的前景图片"),tagForeImage);//foreimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList=new CBCGPProp(_T("MinMax"),tagMinMax,TRUE);//minmax
	pProp=new CBCGPProp(_T("Min"),-1,(long)0,_T("指定进度条的最小值"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Max"),-1,(long)0,_T("指定进度条的最大值"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CBCGPProp(_T("Value"),-1,(long)0,_T("指定进度条的值"),tagValue);//value
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("Hor"),false,_T("指示进度条是否水平"),tagHor);//hor
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("IsStretchFore"),false,_T("指定前景是否缩放"),tagIsStretchFore);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Progress
#endif //_MSC_VER

	//Slider
#if _MSC_VER > 1200
#pragma region Slider
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Slider"),classSlider);

	pPropImage=new CBCGPImageProp(_T("ThumbImage"),_T(""),_T("指定滑块的滑条图片"),tagThumbImage);//thumbimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("ThumbHotImage"),_T(""),_T("指定滑条获得热点时的图片"),tagThumbHotImage);//thumbhotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("ThumbPushedImage"),_T(""),_T("指定滑条被按压后的图片"),tagThumbPushedImage);//thumbpushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList=new CBCGPProp(_T("ThumbSize"),tagThumbSize,TRUE);//thumbsize
	pProp=new CBCGPProp(_T("Width"),-1,(long)0,_T("指定滑条的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Height"),-1,(long)0,_T("指定滑条的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Slider
#endif //_MSC_VER

	//ActiveX
#if _MSC_VER > 1200
#pragma region ActiveX
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("ActiveX"),classActiveX);

	//clsid
	pProp=new CBCGPProp(_T("Clsid"),_T(""),_T("指定ActiveX控件的Clsid值"),tagClsid);
	pPropUI->AddSubItem(pProp);

	//delaycreate
	pProp=new CBCGPProp(_T("DelayCreate"),true,_T("指示是否延迟加载ActiveX控件"),tagDelayCreate);
	pPropUI->AddSubItem(pProp);//added by 邓景仁 2011-09-08

	// modulename
	pProp=new CBCGPProp(_T("ModuleName"),_T(""),_T("指示从指定位置加载ActiveX控件\n如(flash/flash.ocx)"),tagModuleName);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion ActiveX
#endif //_MSC_VER

	//Container
#if _MSC_VER > 1200
#pragma region Container
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Container"),classContainer);

	pValueList=new CBCGPProp(_T("Inset"),tagInset,TRUE);//inset
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("指定容器客户区域的左边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("指定容器客户区域的上边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("指定容器客户区域的右边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("指定容器客户区域的下边距\n0"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CBCGPProp(_T("ChildPadding"),-1,(long)0,_T("指定子控件之间的间距\n0"),tagChildPadding);//childpadding
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("MouseChild"),false,_T("指定本控件的子控件是否可以响应用户操作\nTrue"),tagMouseChild);//mousechild
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("HScrollBar"),false,_T("指示是否启用水平滚动条\nFalse"),tagHScrollBar);//hscrollbar
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("VScrollBar"),false,_T("指示是否启用垂直滚动条\nFalse"),tagVScrollBar);//vscrollbar
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Container
#endif //_MSC_VER

	//Item
#if _MSC_VER > 1200
#pragma region Item
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Item"),classItem);

	pPropImage=new CBCGPImageProp(_T("ItemBkImage"),_T(""),_T("指定组项正常显示时的图片"),tagItemBkImage);//itembkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("ItemHotImage"),_T(""),_T("指定组项获得热点时的图片"),tagItemHotImage);//itemhotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("ItemSelectedImage"),_T(""),_T("指定组项被选择时的图片"),tagItemSelectedImage);//itemselectedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("ItemDisabledImage"),_T(""),_T("指定组项被禁用后的图片"),tagItemDisabledImage);//itemdisabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//itemtextpadding
	pValueList=new CBCGPProp(_T("ItemTextPadding"),tagItemTextPadding,TRUE);
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("指定组项文本区域的左边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("指定组项文本区域的上边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("指定组项文本区域的右边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("指定组项文本区域的下边距\n0"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//itemalign
	pProp=new CBCGPProp(_T("ItemAlign"),_T("Center"),_T("指示组项文本的对齐方式\nCenter"),tagItemAlign);
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//itemtextcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemTextColor"),RGBA(0,0,0,0),NULL,_T("指定组项文本的颜色"),tagItemTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itembkcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemBkColor"),RGBA(0,0,0,0),NULL,_T("指定组项背景的颜色"),tagItemBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemselectedtextcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemSelectedTextColor"),RGBA(0,0,0,0),NULL,_T("指定组项被选中后文本的颜色"),tagItemSelectedTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemselectedbkcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemSelectedBkColor"),RGBA(0,0,0,0),NULL,_T("指定组项被选中后背景的颜色"),tagItemSelectedBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemhottextcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemHotTextColor"),RGBA(0,0,0,0),NULL,_T("指定组项获得热点时文本的颜色"),tagItemHotTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemhotbkcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemHotBkColor"),RGBA(0,0,0,0),NULL,_T("指定组项获得热点时背景的颜色"),tagItemHotBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemdisabledtextcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemDisabledTextColor"),RGBA(0,0,0,0),NULL,_T("指定组项被禁用后文本的颜色"),tagItemDisabledTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemdisabledbkcolor
	pPropColor=new CBCGPColor32Prop(_T("ItemDisabledBkColor"),RGBA(0,0,0,0),NULL,_T("指定组项被禁用后背景的颜色"),tagItemDisabledBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//ItemLineColor
	pPropColor=new CBCGPColor32Prop(_T("ItemLineColor"),RGBA(0,0,0,0),NULL,_T("指定组项分割线的颜色"),tagItemLineColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//ItemFont
	pProp=new CBCGPProp(_T("ItemFont"),(long)-1,_T("指定组项文本的字体\n-1"),tagItemFont);
	pPropUI->AddSubItem(pProp);

	//ItemShowHtml
	pProp=new CBCGPProp(_T("ItemShowHtml"),false,_T("指示是否使用Html格式文本\nFalse"),tagItemShowHtml);
	pPropUI->AddSubItem(pProp);

	//MultiExpanding
	pProp=new CBCGPProp(_T("MultiExpanding"),false,_T("指示是否支持多个item同时打开\nFalse"),tagMultiExpanding);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Item
#endif //_MSC_VER

	//Combo
#if _MSC_VER > 1200
#pragma region Combo
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("Combo"),classCombo);

	pPropImage=new CBCGPImageProp(_T("NormalImage"),_T(""),_T("指定组合框正常显示时的图片"),tagComboNormalImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("HotImage"),_T(""),_T("指定组合框获得热点时的图片"),tagComboHotImage);//hotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("PushedImage"),_T(""),_T("指定组合框被按压下时的图片"),tagComboPushedImage);//pushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("FocusedImage"),_T(""),_T("指定组合框获得焦点后的图片"),tagComboFocusedImage);//focusedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("DisabledImage"),_T(""),_T("指定组合框被禁用后的图片"),tagComboDisabledImage);//disabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList=new CBCGPProp(_T("TextPadding"),tagComboTextPadding,TRUE);//textpadding
	pProp=new CBCGPProp(_T("Left"),-1,(long)0,_T("指定文本区域的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Top"),-1,(long)0,_T("指定文本区域的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Right"),-1,(long)0,_T("指定文本区域的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Bottom"),-1,(long)0,_T("指定文本区域的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CBCGPProp(_T("DropBoxSize"),tagComboDropBoxSize,TRUE);//dropboxsize
	pProp=new CBCGPProp(_T("Width"),-1,(long)0,_T("下拉列表的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Height"),-1,(long)0,_T("下拉列表的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion Combo
#endif //_MSC_VER

	//HorizontalLayout
#if _MSC_VER > 1200
#pragma region HorizontalLayout
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("HorizontalLayout"),classHorizontalLayout);

	pProp=new CBCGPProp(_T("SepWidth"),-1,(long)0,_T("分隔符宽,正负表示分隔符在左边还是右边\n0"),tagSepWidth);//sepwidth
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("SepImm"),false,_T("拖动分隔符是否立即改变大小\nfalse"),tagSepImm);//sepimm
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion HorizontalLayout
#endif //_MSC_VER

	//TileLayout
#if _MSC_VER > 1200
#pragma region TileLayout
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("TileLayout"),classTileLayout);

	pProp=new CBCGPProp(_T("Columns"),-1,(long)0,_T("指定并列布局的列数"),tagColumns);//columns
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion TileLayout
#endif //_MSC_VER

	//List
#if _MSC_VER > 1200
#pragma region List
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("List"),classList);

	pProp=new CBCGPProp(_T("Header"),false,_T("指定是否显示列表表头\nTrue"),tagListHeader);
	pPropUI->AddSubItem(pProp);

	pPropImage=new CBCGPImageProp(_T("headerbkimage"),_T(""),_T("指定表头背景图片"),tagListHeaderBKImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion List
#endif //_MSC_VER

	//ScrollBar
#if _MSC_VER > 1200
#pragma region ScrollBar
#endif //_MSC_VER

	pPropUI=new CBCGPProp(_T("ScrollBar"),classScrollBar);

	pProp=new CBCGPProp(_T("Range"),-1,(long)0,_T("指定滚动范围\n100"),tagScrollBarRange);
	pPropUI->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("Value"),-1,(long)0,_T("指定滚动位置\n0"),tagScrollBarValue);
	pPropUI->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("LineSize"),-1,(long)0,_T("指定滚动一行的大小\n8"),tagScrollBarLineSize);
	pPropUI->AddSubItem(pProp);

	pProp=new CBCGPProp(_T("ShowButton1"),false,_T("是否显示左或上按钮\nTrue"),tagScrollBarShowButton1);
	pPropUI->AddSubItem(pProp);
	pProp=new CBCGPProp(_T("ShowButton2"),false,_T("是否显示左或上按钮\nTrue"),tagScrollBarShowButton2);
	pPropUI->AddSubItem(pProp);

	pPropImage=new CBCGPImageProp(_T("Button1NormalImage"),_T(""),_T("指定左或上按钮普通状态图片"),tagScrollBarButton1NormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("Button1HotImage"),_T(""),_T("指定左或上按钮鼠标悬浮状态图片"),tagScrollBarButton1HotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("Button1PushedImage"),_T(""),_T("指定左或上按钮鼠标按下状态图片"),tagScrollBarButton1PushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("Button1DisabledImage"),_T(""),_T("指定左或上按钮鼠标禁用状态图片"),tagScrollBarButton1DisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("Button2NormalImage"),_T(""),_T("指定左或上按钮普通状态图片"),tagScrollBarButton2NormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("Button2HotImage"),_T(""),_T("指定左或上按钮鼠标悬浮状态图片"),tagScrollBarButton2HotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("Button2PushedImage"),_T(""),_T("指定左或上按钮鼠标按下状态图片"),tagScrollBarButton2PushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("Button2DisabledImage"),_T(""),_T("指定左或上按钮鼠标悬禁用态图片"),tagScrollBarButton2DisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("ThumbNormalImage"),_T(""),_T("指定滑块普通状态图片"),tagScrollBarThumbNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("ThumbHotImage"),_T(""),_T("指定滑块鼠标悬浮状态图片"),tagScrollBarThumbHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("ThumbPushedImage"),_T(""),_T("指定滑块鼠标按下状态图片"),tagScrollBarThumbPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("ThumbDisabledImage"),_T(""),_T("指定滑块禁用状态图片"),tagScrollBarThumbDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("RailNormalImage"),_T(""),_T("指定滑块中间标识普通状态图片"),tagScrollBarRailNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("RailHotImage"),_T(""),_T("指定滑块中间标识鼠标悬浮状态图片"),tagScrollBarRailHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("RailPushedImage"),_T(""),_T("指定滑块中间标识鼠标按下状态图片"),tagScrollBarRailPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("RailDisabledImage"),_T(""),_T("指定滑块中间标识禁用状态图片"),tagScrollBarRailDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CBCGPImageProp(_T("BKNormalImage"),_T(""),_T("指定背景普通状态图片"),tagScrollBarBKNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("BKHotImage"),_T(""),_T("指定背景鼠标悬浮状态图片"),tagScrollBarBKHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("BKPushedImage"),_T(""),_T("指定背景鼠标按下状态图片"),tagScrollBarBKPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CBCGPImageProp(_T("BKDisabledImage"),_T(""),_T("指定背景禁用状态图片"),tagScrollBarBKDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);

#if _MSC_VER > 1200
#pragma endregion ScrollBar
#endif //_MSC_VER

#if _MSC_VER > 1200
#pragma region TabLayout
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("TabLayout"),classTabLayout);

	// selectedid
	pProp=new CBCGPProp(_T("selectedid"),-1,(long)0,_T("默认选中的页面ID\n从0开始计数"),tagSelectedID);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);

#if _MSC_VER > 1200
#pragma endregion TabLayout
#endif //_MSC_VER

#if _MSC_VER > 1200
#pragma region ListHeaderItem
#endif //_MSC_VER
	pPropUI=new CBCGPProp(_T("ListHeaderItem"),classListHeaderItem);

	// dragable
	pProp=new CBCGPProp(_T("Dragable"),true,_T("是否可拖动改变大小\ntrue"),tagDragable);
	pPropUI->AddSubItem(pProp);

	// sepwidth
	pProp=new CBCGPProp(_T("SepWidth"),-1,(long)0,_T("分隔符宽\n4"),tagListHeaderItemSepWidth);
	pPropUI->AddSubItem(pProp);

	// normalimage
	pPropImage=new CBCGPImageProp(_T("NormalImage"),_T(""),_T("普通状态图片"),tagListHeaderItemNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// hotimage
	pPropImage=new CBCGPImageProp(_T("HotImage"),_T(""),_T("鼠标悬浮的状态图片"),tagListHeaderItemHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// PushedImage
	pPropImage=new CBCGPImageProp(_T("PushedImage"),_T(""),_T("鼠标按下的状态图片"),tagListHeaderItemPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// focusedimage
	pPropImage=new CBCGPImageProp(_T("FocusedImage"),_T(""),_T("获得焦点时的状态图片"),tagListHeaderItemFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// sepimage
	pPropImage=new CBCGPImageProp(_T("SepImage"),_T(""),_T("拖动条图片"),tagSepImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion ListHeaderItem
#endif //_MSC_VER

#if _MSC_VER > 1200
#pragma region WebBrowser
#endif //_MSC_VER

	pPropUI=new CBCGPProp(_T("WebBrowser"),classWebBrowser);

	// homepage
	pProp=new CBCGPProp(_T("homepage"),_T(""),_T("默认网址\n"),tagWebBrowserHomePage);
	pPropUI->AddSubItem(pProp);

	// autonavi
	pProp=new CBCGPProp(_T("autonavi"),true,_T("是否显示默认页面\ntrue"),tagDragable);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#if _MSC_VER > 1200
#pragma endregion WebBrowser
#endif //_MSC_VER
	HideAllProperties();
}

void CUIProperties::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	globalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	globalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CUIProperties::HideAllProperties(BOOL bAdjustLayout/*=FALSE*/)
{
	for(int i=0; i<m_wndPropList.GetPropertyCount(); i++)
		m_wndPropList.GetProperty(i)->Show(FALSE, bAdjustLayout);

	m_pControl=NULL;
}

void CUIProperties::ShowProperty(CControlUI* pControl)
{
	if(m_pControl == pControl)
		return;

	if(pControl == NULL)
	{
		HideAllProperties(TRUE);
		return;
	}

	HideAllProperties();

	m_wndPropList.SetCurSel(NULL, FALSE);

	ExtendedAttributes mDummy;
	ExtendedAttributes* pExtended=(ExtendedAttributes*)pControl->GetTag();
	if (pExtended==NULL)
	{
		pExtended = &mDummy;
		ZeroMemory(pExtended, sizeof(ExtendedAttributes));
	}
	switch(pExtended->nClass)
	{
	case classWindow:
		ShowWindowProperty(pControl);
		break;
	case classControl:
		ShowControlProperty(pControl);
		break;
	case classLabel:
	case classText:
		ShowLabelProperty(pControl);
		break;
	case classButton:
		ShowButtonProperty(pControl);
		break;
	case classEdit:
		ShowEditProperty(pControl);
		break;
	case classOption:
		ShowOptionProperty(pControl);
		break;
	case classProgress:
		ShowProgressProperty(pControl);
		break;
	case classSlider:
		ShowSliderProperty(pControl);
		break;
	case classCombo: 
		ShowComboProperty(pControl);
		break;
	case classActiveX:
		ShowActiveXProperty(pControl);
		break;
	case classContainer:
	case classVerticalLayout:
	case classListContainerElement:/*!*/
		ShowContainerProperty(pControl);
		break;
	case classTabLayout:
		ShowTabLayoutPropery(pControl);
		break;
	case classList:
		ShowListProperty(pControl);
		break;
	case classHorizontalLayout:
		ShowHorizontalLayoutProperty(pControl);
		break;
	case classTileLayout:
		ShowTileLayoutProperty(pControl);
		break;
	case classScrollBar:
		ShowScrollBarProperty(pControl);
		break;
	case classListHeaderItem:
		ShowListHeaderItemPropery(pControl);
		break;
	case classWebBrowser:
		ShowWebBrowserPropery(pControl);
		break;
	default:
		ShowControlProperty(pControl);
		break;
	}

	m_pControl=pControl;
	m_wndPropList.AdjustLayout();
}

void CUIProperties::ShowWindowProperty(CControlUI* pControl)
{
	ASSERT(pControl);
	CWindowUI* pForm=static_cast<CWindowUI*>(pControl->GetInterface(_T("Form")));
	ASSERT(pForm);

	CBCGPProp* pPropForm=m_wndPropList.FindItemByData(classWindow,FALSE);
	ASSERT(pPropForm);

	//size
	SIZE size=pForm->GetInitSize();
	CBCGPProp* pValueList=pPropForm->GetSubItem(tagWindowSize-tagWindow);
	pValueList->GetSubItem(0)->SetValue((long)size.cx);
	pValueList->GetSubItem(1)->SetValue((long)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((long)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)size.cy);
	//caption
	RECT rect=pForm->GetCaptionRect();
	pValueList=pPropForm->GetSubItem(tagCaption-tagWindow);
	pValueList->GetSubItem(0)->SetValue((long)rect.left);
	pValueList->GetSubItem(1)->SetValue((long)rect.top);
	pValueList->GetSubItem(2)->SetValue((long)rect.right);
	pValueList->GetSubItem(3)->SetValue((long)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((long)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((long)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((long)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((long)rect.bottom);
	//sizebox
	rect=pForm->GetSizeBox();
	pValueList=pPropForm->GetSubItem(tagSizeBox-tagWindow);
	pValueList->GetSubItem(0)->SetValue((long)rect.left);
	pValueList->GetSubItem(1)->SetValue((long)rect.top);
	pValueList->GetSubItem(2)->SetValue((long)rect.right);
	pValueList->GetSubItem(3)->SetValue((long)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((long)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((long)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((long)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((long)rect.bottom);
	//roundcorner
	size=pForm->GetRoundCorner();
	pValueList=pPropForm->GetSubItem(tagRoundCorner-tagWindow);
	pValueList->GetSubItem(0)->SetValue((long)size.cx);
	pValueList->GetSubItem(1)->SetValue((long)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((long)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)size.cy);
	//mininfo
	size=pForm->GetMinInfo();
	pValueList=pPropForm->GetSubItem(tagMinInfo-tagWindow);
	pValueList->GetSubItem(0)->SetValue((long)size.cx);
	pValueList->GetSubItem(1)->SetValue((long)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((long)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)size.cy);
	//maxinfo
	size=pForm->GetMaxInfo();
	pValueList=pPropForm->GetSubItem(tagMaxInfo-tagWindow);
	pValueList->GetSubItem(0)->SetValue((long)size.cx);
	pValueList->GetSubItem(1)->SetValue((long)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((long)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)size.cy);
	//showdirty
	pPropForm->GetSubItem(tagShowDirty-tagWindow)->SetValue(pForm->IsShowUpdateRect());
	pPropForm->GetSubItem(tagShowDirty-tagWindow)->SetOriginalValue(pForm->IsShowUpdateRect());

	// tagAlpha
	pPropForm->GetSubItem(tagAlpha-tagWindow)->SetValue((long)pForm->GetAlpha());
	pPropForm->GetSubItem(tagAlpha-tagWindow)->SetOriginalValue((long)pForm->GetAlpha());

	// tagBkTrans
	pPropForm->GetSubItem(tagBkTrans-tagWindow)->SetValue(pForm->GetBackgroundTransparent());
	pPropForm->GetSubItem(tagBkTrans-tagWindow)->SetOriginalValue(pForm->GetBackgroundTransparent());

	// tagDefaultFontColor
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagDefaultFontColor-tagWindow))->SetColor((long)(pForm->GetDefaultFontColor()));
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagDefaultFontColor-tagWindow))->SetOriginalValue((long)(pForm->GetDefaultFontColor()));

	// tagSelectedFontColor
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagSelectedFontColor-tagWindow))->SetColor((long)(pForm->GetDefaultSelectedFontColor()));
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagSelectedFontColor-tagWindow))->SetOriginalValue((long)(pForm->GetDefaultSelectedFontColor()));

	// tagDisabledFontColor
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagDisabledFontColor-tagWindow))->SetColor((long)(pForm->GetDefaultDisabledFontColor()));
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagDisabledFontColor-tagWindow))->SetOriginalValue((long)(pForm->GetDefaultDisabledFontColor()));

	// tagLinkFontColor
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagLinkFontColor-tagWindow))->SetColor((long)(pForm->GetDefaultLinkFontColor()));
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagLinkFontColor-tagWindow))->SetOriginalValue((long)(pForm->GetDefaultLinkFontColor()));

	// tagLinkHoverFontColor
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagLinkHoverFontColor-tagWindow))->SetColor((long)(pForm->GetDefaultLinkHoverFontColor()));
	static_cast<CBCGPColor32Prop*>(pPropForm->GetSubItem(tagLinkHoverFontColor-tagWindow))->SetOriginalValue((long)(pForm->GetDefaultLinkHoverFontColor()));

	pPropForm->Show(TRUE,FALSE);
}

void CUIProperties::ShowControlProperty(CControlUI* pControl)
{
	ASSERT(pControl);

	CBCGPProp* pPropControl=m_wndPropList.FindItemByData(classControl,FALSE);
	ASSERT(pPropControl);

	//name
	pPropControl->GetSubItem(tagName-tagControl)->SetValue((LPCTSTR)pControl->GetName());
	pPropControl->GetSubItem(tagName-tagControl)->SetOriginalValue((LPCTSTR)pControl->GetName());
	//text
	pPropControl->GetSubItem(tagText-tagControl)->SetValue((LPCTSTR)pControl->GetText());
	pPropControl->GetSubItem(tagText-tagControl)->SetOriginalValue((LPCTSTR)pControl->GetText());
	//pos
	SIZE szXY=pControl->GetFixedXY();
	int nWidth=pControl->GetFixedWidth();
	int nHeight=pControl->GetFixedHeight();
	CBCGPProp* pValueList=pPropControl->GetSubItem(tagPos-tagControl);
	pValueList->GetSubItem(0)->SetValue((long)szXY.cx);
	pValueList->GetSubItem(1)->SetValue((long)szXY.cy);
	pValueList->GetSubItem(2)->SetValue((long)(szXY.cx+nWidth));
	pValueList->GetSubItem(3)->SetValue((long)(szXY.cy+nHeight));
	pValueList->GetSubItem(0)->SetOriginalValue((long)szXY.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)szXY.cy);
	pValueList->GetSubItem(2)->SetOriginalValue((long)(szXY.cx+nWidth));
	pValueList->GetSubItem(3)->SetOriginalValue((long)(szXY.cy+nHeight));
	//relativepos
	RECT posRelative=pControl->GetRelativePos();
	pValueList=pPropControl->GetSubItem(tagRelativePos-tagControl);
	pValueList->GetSubItem(0)->SetValue(posRelative.left);
	pValueList->GetSubItem(1)->SetValue(posRelative.top);
	pValueList->GetSubItem(2)->SetValue(posRelative.right);
	pValueList->GetSubItem(3)->SetValue(posRelative.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue(posRelative.left);
	pValueList->GetSubItem(1)->SetOriginalValue(posRelative.top);
	pValueList->GetSubItem(2)->SetOriginalValue(posRelative.right);
	pValueList->GetSubItem(3)->SetOriginalValue(posRelative.bottom);
	//size
	pValueList=pPropControl->GetSubItem(tagSize-tagControl);
	pValueList->GetSubItem(0)->SetValue((long)pControl->GetWidth());
	pValueList->GetSubItem(1)->SetValue((long)pControl->GetHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((long)pControl->GetWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((long)pControl->GetHeight());
	//minsize
	pValueList=pPropControl->GetSubItem(tagMinSize-tagControl);
	pValueList->GetSubItem(0)->SetValue((long)pControl->GetMinWidth());
	pValueList->GetSubItem(1)->SetValue((long)pControl->GetMinHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((long)pControl->GetMinWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((long)pControl->GetMinHeight());
	//maxsize
	pValueList=pPropControl->GetSubItem(tagMaxSize-tagControl);
	pValueList->GetSubItem(0)->SetValue((long)pControl->GetMaxWidth());
	pValueList->GetSubItem(1)->SetValue((long)pControl->GetMaxHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((long)pControl->GetMaxWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((long)pControl->GetMaxHeight());
	//padding
	pValueList=pPropControl->GetSubItem(tagPadding-tagControl);
	RECT rect=pControl->GetPadding();
	pValueList->GetSubItem(0)->SetValue((long)rect.left);
	pValueList->GetSubItem(1)->SetValue((long)rect.top);
	pValueList->GetSubItem(2)->SetValue((long)rect.right);
	pValueList->GetSubItem(3)->SetValue((long)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((long)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((long)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((long)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((long)rect.bottom);
	//bkimage
	pPropControl->GetSubItem(tagBkImage-tagControl)->SetValue(pControl->GetBkImage());
	pPropControl->GetSubItem(tagBkImage-tagControl)->SetOriginalValue(pControl->GetBkImage());
	//bkcolor
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagBkColor-tagControl))->SetColor((long)(pControl->GetBkColor()));
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagBkColor-tagControl))->SetOriginalValue((long)(pControl->GetBkColor()));
	//bkcolor2
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagBkColor2-tagControl))->SetColor((long)(pControl->GetBkColor2()));
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagBkColor2-tagControl))->SetOriginalValue((long)(pControl->GetBkColor2()));
	//bordercolor
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagBorderColor-tagControl))->SetColor((long)(pControl->GetBorderColor()));
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagBorderColor-tagControl))->SetOriginalValue((long)(pControl->GetBorderColor()));
	//focusbordercolor
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagFocusBorderColor-tagControl))->SetColor((long)(pControl->GetFocusBorderColor()));
	static_cast<CBCGPColor32Prop*>(pPropControl->GetSubItem(tagFocusBorderColor-tagControl))->SetOriginalValue((long)(pControl->GetFocusBorderColor()));
	//bordersize
	//pPropControl->GetSubItem(tagBorderSize-tagControl)->SetValue((long)pControl->GetBorderSize());
	//pPropControl->GetSubItem(tagBorderSize-tagControl)->SetOriginalValue((long)pControl->GetBorderSize());
	//borderround
	pValueList=pPropControl->GetSubItem(tagBorderRound-tagControl);
	pValueList->GetSubItem(0)->SetValue((long)pControl->GetBorderRound().cx);
	pValueList->GetSubItem(1)->SetValue((long)pControl->GetBorderRound().cy);
	pValueList->GetSubItem(0)->SetOriginalValue((long)pControl->GetBorderRound().cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)pControl->GetBorderRound().cy);
	//enabled
	pPropControl->GetSubItem(tagEnabled-tagControl)->SetValue(pControl->IsEnabled());
	pPropControl->GetSubItem(tagEnabled-tagControl)->SetOriginalValue(pControl->IsEnabled());
	//float
	pPropControl->GetSubItem(tagFloat-tagControl)->SetValue(pControl->IsFloat());
	pPropControl->GetSubItem(tagFloat-tagControl)->SetOriginalValue(pControl->IsFloat());
	//visible
	pPropControl->GetSubItem(tagVisible-tagControl)->SetValue(pControl->IsVisible());
	pPropControl->GetSubItem(tagVisible-tagControl)->SetOriginalValue(pControl->IsVisible());
	//mouse
	pPropControl->GetSubItem(tagMouse-tagControl)->SetValue(pControl->IsMouseEnabled());
	pPropControl->GetSubItem(tagMouse-tagControl)->SetOriginalValue(pControl->IsMouseEnabled());
	//menu
	pPropControl->GetSubItem(tagMenu-tagControl)->SetValue(pControl->IsContextMenuUsed());
	pPropControl->GetSubItem(tagMenu-tagControl)->SetOriginalValue(pControl->IsContextMenuUsed());
	//colorhsl
	pPropControl->GetSubItem(tagColorHSL-tagControl)->SetValue(pControl->IsColorHSL());
	pPropControl->GetSubItem(tagColorHSL-tagControl)->SetOriginalValue(pControl->IsColorHSL());
	//tooltip
	pPropControl->GetSubItem(tagTooltip-tagControl)->SetValue((LPCTSTR)pControl->GetToolTip());
	pPropControl->GetSubItem(tagTooltip-tagControl)->SetOriginalValue((LPCTSTR)pControl->GetToolTip());
	//userdata
	pPropControl->GetSubItem(tagUserData-tagControl)->SetValue((LPCTSTR)pControl->GetUserData());
	pPropControl->GetSubItem(tagUserData-tagControl)->SetOriginalValue((LPCTSTR)pControl->GetUserData());
	//keyboard
	pPropControl->GetSubItem(tagKeyBoard-tagControl)->SetValue(pControl->IsKeyboardEnabled());
	pPropControl->GetSubItem(tagKeyBoard-tagControl)->SetOriginalValue(pControl->IsKeyboardEnabled());

	pPropControl->Show(TRUE,FALSE);
}

void CUIProperties::ShowLabelProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CLabelUI* pLabel=static_cast<CLabelUI*>(pControl->GetInterface(_T("Label")));
	ASSERT(pLabel);

	CBCGPProp* pPropLabel=m_wndPropList.FindItemByData(classLabel,FALSE);
	ASSERT(pPropLabel);

	//align
	UINT uStyle=pLabel->GetTextStyle();
	CString strStyle;
	if(uStyle&DT_CENTER)
		strStyle=_T("Center");
	else if(uStyle&DT_LEFT)
		strStyle=_T("Left");
	else if(uStyle&DT_RIGHT)
		strStyle=_T("Right");
	else if(uStyle&DT_TOP)
		strStyle=_T("Top");
	else if(uStyle&DT_BOTTOM)
		strStyle=_T("Bottom");
	pPropLabel->GetSubItem(tagAlign-tagLabel)->SetValue((LPCTSTR)strStyle);
	pPropLabel->GetSubItem(tagAlign-tagLabel)->SetOriginalValue((LPCTSTR)strStyle);
	//textcolor
	static_cast<CBCGPColor32Prop*>(pPropLabel->GetSubItem(tagTextColor-tagLabel))->SetColor((long)(pLabel->GetTextColor()));
	static_cast<CBCGPColor32Prop*>(pPropLabel->GetSubItem(tagTextColor-tagLabel))->SetOriginalValue((long)(pLabel->GetTextColor()));
	//disabledtextcolor
	static_cast<CBCGPColor32Prop*>(pPropLabel->GetSubItem(tagTextColor-tagLabel))->SetColor((long)(pLabel->GetTextColor()));
	static_cast<CBCGPColor32Prop*>(pPropLabel->GetSubItem(tagTextColor-tagLabel))->SetOriginalValue((long)(pLabel->GetTextColor()));
	//font
	pPropLabel->GetSubItem(tagFont-tagLabel)->SetValue((long)pLabel->GetFont());
	pPropLabel->GetSubItem(tagFont-tagLabel)->SetOriginalValue((long)pLabel->GetFont());
	//textpadding
	CBCGPProp* pValueList=pPropLabel->GetSubItem(tagTextPadding-tagLabel);
	RECT rect=pLabel->GetTextPadding();
	pValueList->GetSubItem(0)->SetValue(rect.left);
	pValueList->GetSubItem(1)->SetValue(rect.top);
	pValueList->GetSubItem(2)->SetValue(rect.right);
	pValueList->GetSubItem(3)->SetValue(rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue(rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue(rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue(rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue(rect.bottom);
	//showhtml
	pPropLabel->GetSubItem(tagShowHtml-tagLabel)->SetValue(pLabel->IsShowHtml());
	pPropLabel->GetSubItem(tagShowHtml-tagLabel)->SetOriginalValue(pLabel->IsShowHtml());
	//endellipsis
	DWORD dwStyle=pLabel->GetTextStyle();
	bool bEndEllipsis=(dwStyle&DT_END_ELLIPSIS) !=0 ;
	pPropLabel->GetSubItem(tagEndEllipsis-tagLabel)->SetValue(bEndEllipsis);
	pPropLabel->GetSubItem(tagEndEllipsis-tagLabel)->SetOriginalValue(bEndEllipsis);

	pPropLabel->Show(TRUE,FALSE);
}

void CUIProperties::ShowButtonProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CButtonUI* pButton=static_cast<CButtonUI*>(pControl->GetInterface(_T("Button")));
	ASSERT(pButton);

	CBCGPProp* pPropButton=m_wndPropList.FindItemByData(classButton,FALSE);
	ASSERT(pPropButton);

	//normalimage
	pPropButton->GetSubItem(tagNormalImage-tagButton)->SetValue((_variant_t)pButton->GetNormalImage());
	pPropButton->GetSubItem(tagNormalImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetNormalImage());
	//hotimage
	pPropButton->GetSubItem(tagHotImage-tagButton)->SetValue((_variant_t)pButton->GetHotImage());
	pPropButton->GetSubItem(tagHotImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetHotImage());
	//pushedimage
	pPropButton->GetSubItem(tagPushedImage-tagButton)->SetValue((_variant_t)pButton->GetPushedImage());
	pPropButton->GetSubItem(tagPushedImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetPushedImage());
	//focusedimage
	pPropButton->GetSubItem(tagFocusedImage-tagButton)->SetValue((_variant_t)pButton->GetFocusedImage());
	pPropButton->GetSubItem(tagFocusedImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetFocusedImage());
	//disabledimage
	pPropButton->GetSubItem(tagDisabledImage-tagButton)->SetValue((_variant_t)pButton->GetDisabledImage());
	pPropButton->GetSubItem(tagDisabledImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetDisabledImage());

	pPropButton->Show(TRUE,FALSE);
}

void CUIProperties::ShowEditProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CEditUI* pEdit=static_cast<CEditUI*>(pControl->GetInterface(_T("Edit")));
	ASSERT(pEdit);

	CBCGPProp* pPropEdit=m_wndPropList.FindItemByData(classEdit,FALSE);
	ASSERT(pPropEdit);

	//normalimage
	pPropEdit->GetSubItem(tagEditNormalImage-tagEdit)->SetValue((_variant_t)pEdit->GetNormalImage());
	pPropEdit->GetSubItem(tagEditNormalImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetNormalImage());
	//hotimage
	pPropEdit->GetSubItem(tagEditHotImage-tagEdit)->SetValue((_variant_t)pEdit->GetHotImage());
	pPropEdit->GetSubItem(tagEditHotImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetHotImage());
	//focusedimage
	pPropEdit->GetSubItem(tagEditFocusedImage-tagEdit)->SetValue((_variant_t)pEdit->GetFocusedImage());
	pPropEdit->GetSubItem(tagEditFocusedImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetFocusedImage());
	//disabledimage
	pPropEdit->GetSubItem(tagEditDisabledImage-tagEdit)->SetValue((_variant_t)pEdit->GetDisabledImage());
	pPropEdit->GetSubItem(tagEditDisabledImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetDisabledImage());
	//readonly
	pPropEdit->GetSubItem(tagReadOnly-tagEdit)->SetValue((_variant_t)pEdit->IsReadOnly());
	pPropEdit->GetSubItem(tagReadOnly-tagEdit)->SetOriginalValue((_variant_t)pEdit->IsReadOnly());
	//password
	pPropEdit->GetSubItem(tagPassword-tagEdit)->SetValue((_variant_t)pEdit->IsPasswordMode());
	pPropEdit->GetSubItem(tagPassword-tagEdit)->SetOriginalValue((_variant_t)pEdit->IsPasswordMode());
	//maxchar
	pPropEdit->GetSubItem(tagMaxChar-tagEdit)->SetValue((long)pEdit->GetMaxChar());
	pPropEdit->GetSubItem(tagMaxChar-tagEdit)->SetOriginalValue((long)pEdit->GetMaxChar());
	//nativebkcolor
	static_cast<CBCGPColor32Prop*>(pPropEdit->GetSubItem(tagNativeBKColor-tagEdit))->SetColor((long)pEdit->GetNativeEditBkColor());
	static_cast<CBCGPColor32Prop*>(pPropEdit->GetSubItem(tagNativeBKColor-tagEdit))->SetOriginalValue((long)pEdit->GetNativeEditBkColor());

	pPropEdit->Show(TRUE,FALSE);
}

void CUIProperties::ShowOptionProperty(CControlUI* pControl)
{
	ShowButtonProperty(pControl);

	ASSERT(pControl);
	COptionUI* pOption=static_cast<COptionUI*>(pControl->GetInterface(_T("Option")));
	ASSERT(pOption);

	CBCGPProp* pPropOption=m_wndPropList.FindItemByData(classOption,FALSE);
	ASSERT(pPropOption);

	//foreimage
	pPropOption->GetSubItem(tagOptForeImage-tagOption)->SetValue((_variant_t)pOption->GetForeImage());
	pPropOption->GetSubItem(tagOptForeImage-tagOption)->SetOriginalValue((_variant_t)pOption->GetForeImage());

	//selectedimage
	pPropOption->GetSubItem(tagSelectedImage-tagOption)->SetValue((_variant_t)pOption->GetSelectedImage());
	pPropOption->GetSubItem(tagSelectedImage-tagOption)->SetOriginalValue((_variant_t)pOption->GetSelectedImage());
	//selected
	pPropOption->GetSubItem(tagSelected-tagOption)->SetValue((_variant_t)pOption->IsSelected());
	pPropOption->GetSubItem(tagSelected-tagOption)->SetOriginalValue((_variant_t)pOption->IsSelected());
	//group
	pPropOption->GetSubItem(tagGroup-tagOption)->SetValue((_variant_t)pOption->GetGroup());
	pPropOption->GetSubItem(tagGroup-tagOption)->SetOriginalValue((_variant_t)pOption->GetGroup());

	pPropOption->Show(TRUE,FALSE);
}

void CUIProperties::ShowProgressProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CProgressUI* pProgress=static_cast<CProgressUI*>(pControl->GetInterface(_T("Progress")));
	ASSERT(pProgress);

	CBCGPProp* pPropProgress=m_wndPropList.FindItemByData(classProgress,FALSE);
	ASSERT(pPropProgress);

	//foreimage
	pPropProgress->GetSubItem(tagForeImage-tagProgress)->SetValue((_variant_t)pProgress->GetForeImage());
	pPropProgress->GetSubItem(tagForeImage-tagProgress)->SetOriginalValue((_variant_t)pProgress->GetForeImage());
	//minmax
	CBCGPProp* pValueList=pPropProgress->GetSubItem(tagMinMax-tagProgress);
	pValueList->GetSubItem(0)->SetValue((long)pProgress->GetMinValue());
	pValueList->GetSubItem(1)->SetValue((long)pProgress->GetMaxValue());
	pValueList->GetSubItem(0)->SetOriginalValue((long)pProgress->GetMinValue());
	pValueList->GetSubItem(1)->SetOriginalValue((long)pProgress->GetMaxValue());
	//value
	pPropProgress->GetSubItem(tagValue-tagProgress)->SetValue((long)pProgress->GetValue());
	pPropProgress->GetSubItem(tagValue-tagProgress)->SetOriginalValue((long)pProgress->GetValue());
	//hor
	pPropProgress->GetSubItem(tagHor-tagProgress)->SetValue((_variant_t)pProgress->IsHorizontal());
	pPropProgress->GetSubItem(tagHor-tagProgress)->SetOriginalValue((_variant_t)pProgress->IsHorizontal());
	//tagIsStretchFore
	//pPropProgress->GetSubItem(tagIsStretchFore-tagProgress)->SetValue((_variant_t)pProgress->IsStretchForeImage());
	//pPropProgress->GetSubItem(tagIsStretchFore-tagProgress)->SetOriginalValue((_variant_t)pProgress->IsStretchForeImage());

	pPropProgress->Show(TRUE,FALSE);
}

void CUIProperties::ShowSliderProperty(CControlUI* pControl)
{
	ShowProgressProperty(pControl);

	ASSERT(pControl);
	CSliderUI* pSlider=static_cast<CSliderUI*>(pControl->GetInterface(_T("Slider")));
	ASSERT(pSlider);

	CBCGPProp* pPropSlider=m_wndPropList.FindItemByData(classSlider,FALSE);
	ASSERT(pPropSlider);

	//thumbimage
	pPropSlider->GetSubItem(tagThumbImage-tagSlider)->SetValue((_variant_t)pSlider->GetThumbImage());
	pPropSlider->GetSubItem(tagThumbImage-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetThumbImage());
	//thumbhotimage
	pPropSlider->GetSubItem(tagThumbHotImage-tagSlider)->SetValue((_variant_t)pSlider->GetThumbHotImage());
	pPropSlider->GetSubItem(tagThumbHotImage-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetThumbHotImage());
	//thumbpushedimage
	pPropSlider->GetSubItem(tagThumbPushedImage-tagSlider)->SetValue((_variant_t)pSlider->GetThumbPushedImage());
	pPropSlider->GetSubItem(tagThumbPushedImage-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetThumbPushedImage());
	//thumbsize
	CBCGPProp* pValueList=pPropSlider->GetSubItem(tagThumbSize-tagSlider);
	RECT rect=pSlider->GetThumbRect();
	pValueList->GetSubItem(0)->SetValue((long)(rect.right-rect.left));
	pValueList->GetSubItem(1)->SetValue((long)(rect.bottom-rect.top));
	pValueList->GetSubItem(0)->SetOriginalValue((long)(rect.right-rect.left));
	pValueList->GetSubItem(1)->SetOriginalValue((long)(rect.bottom-rect.top));

	pPropSlider->Show(TRUE,FALSE);
}

void CUIProperties::ShowComboProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);
	ShowItemProperty(pControl);

	ASSERT(pControl);
	CComboUI* pCombo=static_cast<CComboUI*>(pControl->GetInterface(_T("Combo")));
	ASSERT(pCombo);

	CBCGPProp* pPropCombo=m_wndPropList.FindItemByData(classCombo,FALSE);
	ASSERT(pPropCombo);

	//normalimage
	pPropCombo->GetSubItem(tagComboNormalImage-tagCombo)->SetValue((_variant_t)pCombo->GetNormalImage());
	pPropCombo->GetSubItem(tagComboNormalImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetNormalImage());
	//hotimage
	pPropCombo->GetSubItem(tagComboHotImage-tagCombo)->SetValue((_variant_t)pCombo->GetHotImage());
	pPropCombo->GetSubItem(tagComboHotImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetHotImage());
	//pushedimage
	pPropCombo->GetSubItem(tagComboPushedImage-tagCombo)->SetValue((_variant_t)pCombo->GetPushedImage());
	pPropCombo->GetSubItem(tagComboPushedImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetPushedImage());
	//focusedimage
	pPropCombo->GetSubItem(tagComboFocusedImage-tagCombo)->SetValue((_variant_t)pCombo->GetFocusedImage());
	pPropCombo->GetSubItem(tagComboFocusedImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetFocusedImage());
	//disabledimage
	pPropCombo->GetSubItem(tagComboDisabledImage-tagCombo)->SetValue((_variant_t)pCombo->GetDisabledImage());
	pPropCombo->GetSubItem(tagComboDisabledImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetDisabledImage());
	//textpadding
	RECT rect=pCombo->GetTextPadding();
	CBCGPProp* pValueList=pPropCombo->GetSubItem(tagComboTextPadding-tagCombo);
	pValueList->GetSubItem(0)->SetValue((long)rect.left);
	pValueList->GetSubItem(1)->SetValue((long)rect.top);
	pValueList->GetSubItem(2)->SetValue((long)rect.right);
	pValueList->GetSubItem(3)->SetValue((long)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((long)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((long)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((long)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((long)rect.bottom);
	//dropboxsize
	SIZE size=pCombo->GetDropBoxSize();
	pValueList=pPropCombo->GetSubItem(tagComboDropBoxSize-tagCombo);
	pValueList->GetSubItem(0)->SetValue((long)size.cx);
	pValueList->GetSubItem(1)->SetValue((long)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((long)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((long)size.cy);
	pPropCombo->Show(TRUE,FALSE);
}

void CUIProperties::ShowActiveXProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CActiveXUI* pActiveX=static_cast<CActiveXUI*>(pControl->GetInterface(_T("ActiveX")));
	ASSERT(pActiveX);

	CBCGPProp* pPropActiveX=m_wndPropList.FindItemByData(classActiveX,FALSE);
	ASSERT(pPropActiveX);

	CLSID clsid=pActiveX->GetClisd();
	WCHAR strCLSID[48];
	StringFromGUID2(clsid,strCLSID,48);
	//clsid
	pPropActiveX->GetSubItem(tagClsid-tagActiveX)->SetValue(strCLSID);
	pPropActiveX->GetSubItem(tagClsid-tagActiveX)->SetOriginalValue(strCLSID);

	//delaycreate
	pPropActiveX->GetSubItem(tagDelayCreate-tagActiveX)->SetValue((_variant_t)pActiveX->IsDelayCreate());
	pPropActiveX->GetSubItem(tagDelayCreate-tagActiveX)->SetOriginalValue((_variant_t)pActiveX->IsDelayCreate());

	//modulename
	pPropActiveX->GetSubItem(tagModuleName-tagActiveX)->SetValue((_variant_t)pActiveX->GetModuleName());
	pPropActiveX->GetSubItem(tagModuleName-tagActiveX)->SetOriginalValue((_variant_t)pActiveX->GetModuleName());

	pPropActiveX->Show(TRUE,FALSE);
}

void CUIProperties::ShowContainerProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CContainerUI* pContainer=static_cast<CContainerUI*>(pControl->GetInterface(_T("Container")));
	ASSERT(pContainer);

	CBCGPProp* pPropContainer=m_wndPropList.FindItemByData(classContainer,FALSE);
	ASSERT(pPropContainer);

	//inset
	RECT rect=pContainer->GetInset();
	CBCGPProp* pValueList=pPropContainer->GetSubItem(tagInset-tagContainer);
	pValueList->GetSubItem(0)->SetValue((long)rect.left);
	pValueList->GetSubItem(1)->SetValue((long)rect.top);
	pValueList->GetSubItem(2)->SetValue((long)rect.right);
	pValueList->GetSubItem(3)->SetValue((long)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((long)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((long)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((long)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((long)rect.bottom);
	//childpadding
	pPropContainer->GetSubItem(tagChildPadding-tagContainer)->SetValue((long)pContainer->GetChildPadding());
	pPropContainer->GetSubItem(tagChildPadding-tagContainer)->SetOriginalValue((long)pContainer->GetChildPadding());
	//mousechild
	pPropContainer->GetSubItem(tagMouseChild-tagContainer)->SetValue((_variant_t)pContainer->IsMouseChildEnabled());
	pPropContainer->GetSubItem(tagMouseChild-tagContainer)->SetOriginalValue((_variant_t)pContainer->IsMouseChildEnabled());
	//hscrollbar
	pPropContainer->GetSubItem(tagHScrollBar-tagContainer)->SetValue((_variant_t)(pContainer->GetHorizontalScrollBar()==NULL?false:true));
	pPropContainer->GetSubItem(tagHScrollBar-tagContainer)->SetOriginalValue((_variant_t)(pContainer->GetHorizontalScrollBar()==NULL?false:true));
	//vscrollbar
	pPropContainer->GetSubItem(tagMouseChild-tagContainer)->SetValue((_variant_t)(pContainer->GetVerticalScrollBar()==NULL?false:true));
	pPropContainer->GetSubItem(tagMouseChild-tagContainer)->SetOriginalValue((_variant_t)(pContainer->GetVerticalScrollBar()==NULL?false:true));

	pPropContainer->Show(TRUE,FALSE);
}

void CUIProperties::ShowHorizontalLayoutProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CHorizontalLayoutUI* pHorizontalLayout=static_cast<CHorizontalLayoutUI*>(pControl->GetInterface(_T("HorizontalLayout")));
	ASSERT(pHorizontalLayout);

	CBCGPProp* pPropHorizontalLayout=m_wndPropList.FindItemByData(classHorizontalLayout,FALSE);
	ASSERT(pPropHorizontalLayout);

	//sepwidth
	pPropHorizontalLayout->GetSubItem(tagSepWidth-tagHorizontalLayout)->SetValue((long)pHorizontalLayout->GetSepWidth());
	pPropHorizontalLayout->GetSubItem(tagSepWidth-tagHorizontalLayout)->SetOriginalValue((long)pHorizontalLayout->GetSepWidth());
	//sepimm
	pPropHorizontalLayout->GetSubItem(tagSepImm-tagHorizontalLayout)->SetValue((_variant_t)pHorizontalLayout->IsSepImmMode());
	pPropHorizontalLayout->GetSubItem(tagSepImm-tagHorizontalLayout)->SetOriginalValue((_variant_t)pHorizontalLayout->IsSepImmMode());

	pPropHorizontalLayout->Show(TRUE,FALSE);
}

void CUIProperties::ShowTileLayoutProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CTileLayoutUI* pTileLayout=static_cast<CTileLayoutUI*>(pControl->GetInterface(_T("TileLayout")));
	ASSERT(pTileLayout);

	CBCGPProp* pPropTileLayout=m_wndPropList.FindItemByData(classTileLayout,FALSE);
	ASSERT(pPropTileLayout);

	//sepwidth
	pPropTileLayout->GetSubItem(tagSepWidth-tagHorizontalLayout)->SetValue((long)pTileLayout->GetColumns());
	pPropTileLayout->GetSubItem(tagSepWidth-tagHorizontalLayout)->SetOriginalValue((long)pTileLayout->GetColumns());

	pPropTileLayout->Show(TRUE,FALSE);
}

CBCGPProp* CUIProperties::FindPropByData(DWORD_PTR dwData, BOOL bSearchSubProps/* = TRUE*/) const
{
	return m_wndPropList.FindItemByData(dwData, bSearchSubProps);
}

CString CUIProperties::FormatOrigProperty(CBCGPProp* pProp)
{
	ASSERT(pProp);

	COleVariant var = pProp->GetOriginalValue();
	CString strOrigVal;

	int nCount = pProp->GetSubItemsCount();
	if(nCount > 0)
	{
		for (int i=0; i<nCount; i++)
		{
			CBCGPProp* pSubProp = pProp->GetSubItem(i);
			ASSERT_VALID(pSubProp);

			strOrigVal += FormatOrigProperty(pSubProp);

			if (i != nCount-1)
			{
				strOrigVal += _T(',');
				strOrigVal += _T(' ');
			}
		}

		return strOrigVal;
	}

	switch (var.vt)
	{
	case VT_BSTR:
		strOrigVal = var.bstrVal;
		break;

	case VT_I2:
		strOrigVal.Format(_T("%d"), (short)var.iVal);
		break;

	case VT_I4:
	case VT_INT:
		strOrigVal.Format(_T("%ld"), (long)var.lVal);
		break;

	case VT_UI1:
		if ((BYTE)var.bVal != 0)
		{
			strOrigVal.Format(_T("%c"), (TCHAR)(BYTE)var.bVal);
		}
		break;

	case VT_UI2:
		strOrigVal.Format(_T("%u"), var.uiVal);
		break;

	case VT_UINT:
	case VT_UI4:
		strOrigVal.Format(_T("%u"), var.ulVal);
		break;

	case VT_R4:
		strOrigVal.Format(_T("%f"), (float)var.fltVal);
		break;

	case VT_R8:
		strOrigVal.Format(_T("%lf"), (double)var.dblVal);
		break;

	case VT_BOOL:
		strOrigVal = var.boolVal == VARIANT_TRUE ? _T("True") : _T("False");
		break;

	default:
		// Unsupported type
		strOrigVal = _T("*** error ***");
	}

	return strOrigVal;
}

void CUIProperties::ShowListProperty( CControlUI* pControl )
{
	ShowContainerProperty(pControl);
	ShowItemProperty(pControl);

	ASSERT(pControl);
	CListUI* pList=static_cast<CListUI*>(pControl->GetInterface(_T("List")));
	ASSERT(pList);

	CBCGPProp* pPropList=m_wndPropList.FindItemByData(classList,FALSE);
	ASSERT(pPropList);

	//Header
	pPropList->GetSubItem(tagListHeader-tagList)->SetValue((_variant_t)pList->GetHeader()->IsVisible());
	pPropList->GetSubItem(tagListHeader-tagList)->SetOriginalValue((_variant_t)pList->GetHeader()->IsVisible());

	pPropList->GetSubItem(tagListHeaderBKImage-tagList)->SetValue((_variant_t)pList->GetHeader()->GetBkImage());
	pPropList->GetSubItem(tagListHeaderBKImage-tagList)->SetOriginalValue((_variant_t)pList->GetHeader()->GetBkImage());

	pPropList->Show(TRUE,FALSE);
}

void CUIProperties::ShowItemProperty( CControlUI* pControl )
{
	ASSERT(pControl);
	IListOwnerUI* pList=static_cast<IListOwnerUI*>(pControl->GetInterface(_T("IListOwner")));
	ASSERT(pList);

	CBCGPProp* pPropItem=m_wndPropList.FindItemByData(classItem,FALSE);
	ASSERT(pPropItem);

	//itembkimage
	TListInfoUI* pListInfo=pList->GetListInfo();
	//pPropItem->GetSubItem(tagItemBkImage-tagItem)->SetValue((_variant_t)pListInfo->sBkImage);
	//pPropItem->GetSubItem(tagItemBkImage-tagItem)->SetOriginalValue((_variant_t)pListInfo->sBkImage);
	//itemhotimage
	//pPropItem->GetSubItem(tagItemHotImage-tagItem)->SetValue((_variant_t)pListInfo->sHotImage);
	//pPropItem->GetSubItem(tagItemHotImage-tagItem)->SetOriginalValue((_variant_t)pListInfo->sHotImage);
	//itemselectedimage
	//pPropItem->GetSubItem(tagItemSelectedImage-tagItem)->SetValue((_variant_t)pListInfo->sSelectedImage);
	//pPropItem->GetSubItem(tagItemSelectedImage-tagItem)->SetOriginalValue((_variant_t)pListInfo->sSelectedImage);
	//itemdisabledimage
	//pPropItem->GetSubItem(tagItemDisabledImage-tagItem)->SetValue((_variant_t)pListInfo->sDisabledImage);
	//pPropItem->GetSubItem(tagItemDisabledImage-tagItem)->SetOriginalValue((_variant_t)pListInfo->sDisabledImage);
	//itemtextpadding
	CBCGPProp*  pValueList=pPropItem->GetSubItem(tagItemTextPadding-tagItem);
	pValueList->GetSubItem(0)->SetValue((long)pListInfo->rcTextPadding.left);
	pValueList->GetSubItem(1)->SetValue((long)pListInfo->rcTextPadding.top);
	pValueList->GetSubItem(2)->SetValue((long)pListInfo->rcTextPadding.right);
	pValueList->GetSubItem(3)->SetValue((long)pListInfo->rcTextPadding.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((long)pListInfo->rcTextPadding.left);
	pValueList->GetSubItem(1)->SetOriginalValue((long)pListInfo->rcTextPadding.top);
	pValueList->GetSubItem(2)->SetOriginalValue((long)pListInfo->rcTextPadding.right);
	pValueList->GetSubItem(3)->SetOriginalValue((long)pListInfo->rcTextPadding.bottom);
	//itemalign
	CString strStyle;
	if(pListInfo->uTextStyle==DT_LEFT)
		strStyle=_T("Left");
	else if(pListInfo->uTextStyle&DT_CENTER)
		strStyle=_T("Center");
	else if(pListInfo->uTextStyle&DT_RIGHT)
		strStyle=_T("Right");
	pPropItem->GetSubItem(tagItemAlign-tagItem)->SetValue((_variant_t)strStyle);
	pPropItem->GetSubItem(tagItemAlign-tagItem)->SetOriginalValue((_variant_t)strStyle);
	//itemtextcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemTextColor-tagItem))->SetColor((long)pListInfo->dwTextColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemTextColor-tagItem))->SetOriginalValue((long)pListInfo->dwTextColor);
	//itembkcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemBkColor-tagItem))->SetColor((long)pListInfo->dwBkColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemBkColor-tagItem))->SetOriginalValue((long)pListInfo->dwBkColor);
	//itemselectedtextcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemSelectedTextColor-tagItem))->SetColor((long)pListInfo->dwSelectedTextColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemSelectedTextColor-tagItem))->SetOriginalValue((long)pListInfo->dwSelectedTextColor);
	//itemselectedbkcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemSelectedBkColor-tagItem))->SetColor((long)pListInfo->dwSelectedBkColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemSelectedBkColor-tagItem))->SetOriginalValue((long)pListInfo->dwSelectedBkColor);
	//itemhottextcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemHotTextColor-tagItem))->SetColor((long)pListInfo->dwHotTextColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemHotTextColor-tagItem))->SetOriginalValue((long)pListInfo->dwHotTextColor);
	//itemhotbkcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemHotBkColor-tagItem))->SetColor((long)pListInfo->dwHotBkColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemHotBkColor-tagItem))->SetOriginalValue((long)pListInfo->dwHotBkColor);
	//itemdisabledtextcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemDisabledTextColor-tagItem))->SetColor((long)pListInfo->dwDisabledTextColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemDisabledTextColor-tagItem))->SetOriginalValue((long)pListInfo->dwDisabledTextColor);
	//itemdisabledbkcolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemDisabledBkColor-tagItem))->SetColor((long)pListInfo->dwDisabledBkColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemDisabledBkColor-tagItem))->SetOriginalValue((long)pListInfo->dwDisabledBkColor);
	//itemlinecolor
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemLineColor-tagItem))->SetColor((long)pListInfo->dwHLineColor);
	static_cast<CBCGPColor32Prop*>(pPropItem->GetSubItem(tagItemLineColor-tagItem))->SetOriginalValue((long)pListInfo->dwHLineColor);
	//itemfont
	pPropItem->GetSubItem(tagItemFont-tagItem)->SetValue((long)pListInfo->nFont);
	pPropItem->GetSubItem(tagItemFont-tagItem)->SetOriginalValue((long)pListInfo->nFont);
	//itemshowhtml
	pPropItem->GetSubItem(tagMultiExpanding-tagItem)->SetValue((_variant_t)pListInfo->bMultiExpandable);
	pPropItem->GetSubItem(tagMultiExpanding-tagItem)->SetOriginalValue((_variant_t)pListInfo->bMultiExpandable);

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowScrollBarProperty( CControlUI* pControl )
{
	//大部分时候只在默认属性中设置全局滚动条属性
	//默认不显示基础控件属性
	//ShowControlProperty(pControl);

	ASSERT(pControl);
	CScrollBarUI* pScrollBar=static_cast<CScrollBarUI*>(pControl->GetInterface(_T("ScrollBar")));
	ASSERT(pScrollBar);

	CBCGPProp* pPropItem=m_wndPropList.FindItemByData(classScrollBar,FALSE);
	ASSERT(pPropItem);

	//tagScrollBarRange
	pPropItem->GetSubItem(tagScrollBarRange-tagScrollBar)->SetValue((long)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(tagScrollBarRange-tagScrollBar)->SetOriginalValue((long)pScrollBar->GetScrollRange());
	//tagScrollBarValue
	pPropItem->GetSubItem(tagScrollBarValue-tagScrollBar)->SetValue((long)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(tagScrollBarValue-tagScrollBar)->SetOriginalValue((long)pScrollBar->GetScrollRange());
	//tagScrollBarLineSize
	pPropItem->GetSubItem(tagScrollBarLineSize-tagScrollBar)->SetValue((long)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(tagScrollBarLineSize-tagScrollBar)->SetOriginalValue((long)pScrollBar->GetScrollRange());
	// tagScrollBarShowButton1
	pPropItem->GetSubItem(tagScrollBarShowButton1-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetShowButton1());
	pPropItem->GetSubItem(tagScrollBarShowButton1-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetShowButton1());
	//	tagScrollBarShowButton2
	pPropItem->GetSubItem(tagScrollBarShowButton2-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetShowButton2());
	pPropItem->GetSubItem(tagScrollBarShowButton2-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetShowButton2());

	//	tagScrollBarButton1NormalImage
	pPropItem->GetSubItem(tagScrollBarButton1NormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1NormalImage());
	pPropItem->GetSubItem(tagScrollBarButton1NormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1NormalImage());
	//	tagScrollBarButton1HotImage
	pPropItem->GetSubItem(tagScrollBarButton1HotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1HotImage());
	pPropItem->GetSubItem(tagScrollBarButton1HotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1HotImage());
	//	tagScrollBarButton1PushedImage
	pPropItem->GetSubItem(tagScrollBarButton1PushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1PushedImage());
	pPropItem->GetSubItem(tagScrollBarButton1PushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1PushedImage());
	//	tagScrollBarButton1DisabledImage
	pPropItem->GetSubItem(tagScrollBarButton1DisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1DisabledImage());
	pPropItem->GetSubItem(tagScrollBarButton1DisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1DisabledImage());

	//	tagScrollBarButton2NormalImage
	pPropItem->GetSubItem(tagScrollBarButton2NormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2NormalImage());
	pPropItem->GetSubItem(tagScrollBarButton2NormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2NormalImage());
	//	tagScrollBarButton2HotImage
	pPropItem->GetSubItem(tagScrollBarButton2HotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2HotImage());
	pPropItem->GetSubItem(tagScrollBarButton2HotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2HotImage());
	//	tagScrollBarButton2PushedImage
	pPropItem->GetSubItem(tagScrollBarButton2PushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2PushedImage());
	pPropItem->GetSubItem(tagScrollBarButton2PushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2PushedImage());
	//	tagScrollBarButton2DisabledImage
	pPropItem->GetSubItem(tagScrollBarButton2DisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2DisabledImage());
	pPropItem->GetSubItem(tagScrollBarButton2DisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2DisabledImage());

	//	tagScrollBarThumbNormalImage
	pPropItem->GetSubItem(tagScrollBarThumbNormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbNormalImage());
	pPropItem->GetSubItem(tagScrollBarThumbNormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbNormalImage());
	//	tagScrollBarThumbHotImage
	pPropItem->GetSubItem(tagScrollBarThumbHotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbHotImage());
	pPropItem->GetSubItem(tagScrollBarThumbHotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbHotImage());
	//	tagScrollBarThumbPushedImage
	pPropItem->GetSubItem(tagScrollBarThumbPushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbPushedImage());
	pPropItem->GetSubItem(tagScrollBarThumbPushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbPushedImage());
	//	tagScrollBarThumbDisabledImage
	pPropItem->GetSubItem(tagScrollBarThumbDisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbDisabledImage());
	pPropItem->GetSubItem(tagScrollBarThumbDisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbDisabledImage());

	//	tagScrollBarRailNormalImage
	pPropItem->GetSubItem(tagScrollBarRailNormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailNormalImage());
	pPropItem->GetSubItem(tagScrollBarRailNormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetRailNormalImage());
	//	tagScrollBarRailHotImage
	pPropItem->GetSubItem(tagScrollBarRailHotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailHotImage());
	pPropItem->GetSubItem(tagScrollBarRailHotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1HotImage());
	//	tagScrollBarRailPushedImage
	pPropItem->GetSubItem(tagScrollBarRailPushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailPushedImage());
	pPropItem->GetSubItem(tagScrollBarRailPushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetRailPushedImage());
	//	tagScrollBarRailDisabledImage
	pPropItem->GetSubItem(tagScrollBarRailDisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailDisabledImage());
	pPropItem->GetSubItem(tagScrollBarRailDisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetRailDisabledImage());

	//	tagScrollBarBKNormalImage
	pPropItem->GetSubItem(tagScrollBarBKNormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkNormalImage());
	pPropItem->GetSubItem(tagScrollBarBKNormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkNormalImage());
	//	tagScrollBarBKHotImage
	pPropItem->GetSubItem(tagScrollBarBKHotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkHotImage());
	pPropItem->GetSubItem(tagScrollBarBKHotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkHotImage());
	//	tagScrollBarBKPushedImage
	pPropItem->GetSubItem(tagScrollBarBKPushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkPushedImage());
	pPropItem->GetSubItem(tagScrollBarBKPushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkPushedImage());
	//	tagScrollBarBKDisabledImage
	pPropItem->GetSubItem(tagScrollBarBKDisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkDisabledImage());
	pPropItem->GetSubItem(tagScrollBarBKDisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkDisabledImage());

	pPropItem->Show(TRUE,FALSE);
}
void CUIProperties::ShowTabLayoutPropery( CControlUI* pControl )
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CTabLayoutUI* pTabLayout=static_cast<CTabLayoutUI*>(pControl->GetInterface(_T("TabLayout")));
	ASSERT(pTabLayout);

	CBCGPProp* pPropItem=m_wndPropList.FindItemByData(classTabLayout,FALSE);
	ASSERT(pPropItem);

	//	SelectedID
	pPropItem->GetSubItem(tagSelectedID-tagTabLayout)->SetValue((long)pTabLayout->GetCurSel());
	pPropItem->GetSubItem(tagSelectedID-tagTabLayout)->SetOriginalValue((long)pTabLayout->GetCurSel());

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowListHeaderItemPropery( CControlUI* pControl )
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CListHeaderItemUI* pListHeaderItem=static_cast<CListHeaderItemUI*>(pControl->GetInterface(_T("ListHeaderItem")));
	ASSERT(pListHeaderItem);

	CBCGPProp* pPropItem=m_wndPropList.FindItemByData(classListHeaderItem,FALSE);
	ASSERT(pPropItem);

	//	dragable
	pPropItem->GetSubItem(tagDragable-classListHeaderItem)->SetValue((_variant_t)pListHeaderItem->IsDragable());
	pPropItem->GetSubItem(tagDragable-classListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->IsDragable());
	//	sepwidth
	pPropItem->GetSubItem(tagListHeaderItemSepWidth-classListHeaderItem)->SetValue((long)pListHeaderItem->GetSepWidth());
	pPropItem->GetSubItem(tagListHeaderItemSepWidth-classListHeaderItem)->SetOriginalValue((long)pListHeaderItem->GetSepWidth());
	//	normalimage
	pPropItem->GetSubItem(tagListHeaderItemNormalImage-classListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetNormalImage());
	pPropItem->GetSubItem(tagScrollBarBKPushedImage-classListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetNormalImage());
	//	hotimage
	pPropItem->GetSubItem(tagListHeaderItemHotImage-classListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetHotImage());
	pPropItem->GetSubItem(tagListHeaderItemHotImage-classListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetHotImage());
	//	pushedimage
	pPropItem->GetSubItem(tagListHeaderItemPushedImage-classListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetPushedImage());
	pPropItem->GetSubItem(tagListHeaderItemPushedImage-classListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetPushedImage());
	//	focusedimage
	pPropItem->GetSubItem(tagListHeaderItemFocusedImage-classListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetFocusedImage());
	pPropItem->GetSubItem(tagListHeaderItemFocusedImage-classListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetFocusedImage());
	//	sepimage
	pPropItem->GetSubItem(tagSepImage-classListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetSepImage());
	pPropItem->GetSubItem(tagSepImage-classListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetSepImage());

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowWebBrowserPropery( CControlUI* pControl )
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CWebBrowserUI* pWebBrowser=static_cast<CWebBrowserUI*>(pControl->GetInterface(_T("WebBrowser")));
	ASSERT(pWebBrowser);

	CBCGPProp* pPropItem=m_wndPropList.FindItemByData(classWebBrowser,FALSE);
	ASSERT(pPropItem);

	//	homepage
	pPropItem->GetSubItem(tagWebBrowserHomePage-tagWebBrowser)->SetValue((_variant_t)pWebBrowser->GetHomePage());
	pPropItem->GetSubItem(tagWebBrowserHomePage-tagWebBrowser)->SetOriginalValue((_variant_t)pWebBrowser->GetHomePage());

	//	autonavi
	pPropItem->GetSubItem(tagWebBrowserAutoNavi-tagWebBrowser)->SetValue((_variant_t)pWebBrowser->IsAutoNavigation());
	pPropItem->GetSubItem(tagWebBrowserAutoNavi-tagWebBrowser)->SetOriginalValue((_variant_t)pWebBrowser->IsAutoNavigation());

	pPropItem->Show(TRUE,FALSE);
}
