// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "DuiDesigner.h"
#include "MainFrm.h"
#include "DialogTemplateOpen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_WINDOW_MANAGER, CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_CREATETOOLBAR, CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, CMainFrame::OnUpdateApplicationLook)
	ON_REGISTERED_MESSAGE(BCGM_ON_GET_TAB_TOOLTIP, OnGetTabToolTip)
	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
	ON_COMMAND(ID_PROJECT_NEW, CMainFrame::OnProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, CMainFrame::OnProjectOpen)
	ON_COMMAND(ID_PROJECT_CLOSE, CMainFrame::OnProjectClose)
	ON_COMMAND(ID_TEMPLATE_OPEN, CMainFrame::OnTemplateOpen)
	ON_COMMAND(ID_FILE_CLOSE_ALL, CMainFrame::OnFileCloseAll)
	ON_COMMAND(ID_FILE_SAVE_ALL, CMainFrame::OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, CMainFrame::OnUpdateProjectExist)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_ALL, CMainFrame::OnUpdateFileCloseAll)
	ON_COMMAND(ID_MDITABS_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDITABS_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDITABS_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDITABS_NEW_VERT_TAB_GROUP, OnMdiNewVertTabGroup)
	ON_COMMAND(ID_View_Controls, CMainFrame::OnViewControls)
	ON_COMMAND(ID_View_Project, CMainFrame::OnViewProject)
	ON_COMMAND(ID_View_Resources, CMainFrame::OnViewResource)
	ON_COMMAND(ID_View_ToolsBox, CMainFrame::OnViewToolsBox)
	ON_COMMAND(ID_View_Property, CMainFrame::OnViewProperty)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	g_pMainFrame=this;

	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = FALSE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	mdiTabParams.m_bEnableTabSwap = TRUE; //启用选项卡交换
	mdiTabParams.m_bTabCustomTooltips = TRUE;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}
#if _MSC_VER > 1200
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
#endif //_MSC_VER

	//取消首先显示最近使用的命令
	CBCGPMenuBar::SetRecentlyUsedMenus(FALSE);
	// 防止菜单栏在激活时获得焦点
	CBCGPPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_HC : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	// Create FormEdit toolbar
	if (!m_wndToolbarFormEdit.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER | CBRS_BORDER_3D, ID_VIEW_FORMEDIT_TOOLBAR) || !m_wndToolbarFormEdit.LoadToolBar(IDR_FORMEDIT))
	{
		TRACE0("未能创建窗口编辑工具栏\n");
		return FALSE;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	CString strFormEditToolBarName;
	bNameValid = strFormEditToolBarName.LoadString(IDS_TOOLBAR_FORMEDIT);
	ASSERT(bNameValid);
	m_wndToolbarFormEdit.SetWindowText(strFormEditToolBarName);
	m_wndToolbarFormEdit.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolbarFormEdit.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndToolbarFormEdit);
	DockControlBarLeftOf(&m_wndToolBar, &m_wndToolbarFormEdit);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHideBars(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CBCGPToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_MENU_UI);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResourceView.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndFileView);
	CBCGPDockingControlBar* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, BCGP_DM_SHOW, FALSE, &pTabbedBar);
	m_wndResourceView.AttachToTabWnd(pTabbedBar, BCGP_DM_SHOW, FALSE, &pTabbedBar);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBox.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndProperties);
	DockControlBar(&m_wndToolBox);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// 启用工具栏和停靠窗口菜单替换
	//EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR, FALSE, TRUE);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CBCGPToolBar::EnableQuickCustomization();

// 	if (CBCGPToolBar::GetUserImages() == NULL)
// 	{
// 		// 加载用户定义的工具栏图像
// 		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
// 		{
// 			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
// 			CBCGPToolBar::SetUserImages(&m_UserImages);
// 		}
// 	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CBCGPToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style = WS_OVERLAPPED | WS_CAPTION /*| FWS_ADDTOTITLE 去除基于文档修改标题的风格*/
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“类视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建资源视图
	CString strResourceView;
	bNameValid = strResourceView.LoadString(IDS_RESOURCE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndResourceView.Create(strResourceView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_RESOURCE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“资源视图”窗口\n");
		return FALSE;      // 未能创建
	}

	// 创建属性窗口
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“属性”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建工具箱窗口
	CString strToolBoxWnd;
	bNameValid = strToolBoxWnd.LoadString(IDS_TOOLBOX_WND);
	ASSERT(bNameValid);
	if (!m_wndToolBox.Create(strToolBoxWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_TOOLBOX, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“工具箱”窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hResViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_RES_VIEW_HC : IDI_RES_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndResourceView.SetIcon(hResViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hToolBoxBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_TOOLBOX_WND_HC), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndToolBox.SetIcon(hToolBoxBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CBCGPMDIFrameWnd::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CBCGPToolBar* pUserToolbar = (CBCGPToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	DuiLib::CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerVS2005));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_Aqua);
			break;
		}

		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CBCGPMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CBCGPToolBar* pUserToolbar = GetUserBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: 在此添加专用代码和/或调用基类

	//禁止更新标题
// 	CBCGPMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

CUIDesignerView* CMainFrame::GetActiveUIView() const
{
	CMDIChildWnd* pFrame=MDIGetActive();
	if(!pFrame)
		return NULL;

	return DYNAMIC_DOWNCAST(CUIDesignerView,pFrame->GetActiveView());
}

LRESULT CMainFrame::OnGetTabToolTip(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPTabToolTipInfo* pInfo = (CBCGPTabToolTipInfo*) lp;
	ASSERT(pInfo != NULL);
	if (!pInfo)
	{
		return 0;
	}

	ASSERT_VALID(pInfo->m_pTabWnd);

	if (!pInfo->m_pTabWnd->IsMDITab())
	{
		return 0;
	}

	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pInfo->m_pTabWnd->GetTabWnd(pInfo->m_nTabIndex));
	if (pFrame == NULL)
	{
		return 0;
	}

	CDocument* pDoc = pFrame->GetActiveDocument();
	if (pDoc == NULL)
	{
		return 0;
	}

	pInfo->m_strText = pDoc->GetPathName();
	if(pInfo->m_strText.IsEmpty())
		pInfo->m_strText=pDoc->GetTitle();

	return 0;
}

LRESULT CMainFrame::OnChangeActiveTab(WPARAM wp,LPARAM lp)
{
	int nTabIndex=wp;
	CBCGPBaseTabWnd* pTabWnd=(CBCGPBaseTabWnd*)lp;
	CFrameWnd* pFrame=DYNAMIC_DOWNCAST(CFrameWnd,pTabWnd->GetTabWnd(nTabIndex));
	if(pFrame==NULL)
		return FALSE;
	CUIDesignerView* pUIView=DYNAMIC_DOWNCAST(CUIDesignerView,pFrame->GetActiveView());
	if(pUIView==NULL)
		return FALSE;

	pUIView->OnActivated();

	return TRUE;
}

void CMainFrame::OnProjectNew()
{
	g_pFileView->OnProjectNew();
}

void CMainFrame::OnProjectOpen()
{
	// TODO: Add your command handler code here
	g_pFileView->OnProjectOpen();
}

void CMainFrame::OnProjectClose()
{
	// TODO: Add your command handler code here
	g_pFileView->OnProjectClose();
}

void CMainFrame::OnTemplateOpen()
{
	// TODO: Add your command handler code here
	CDialogTemplateOpen dlg;
	dlg.DoModal();
}

void CMainFrame::OnFileCloseAll()
{
	// TODO: 在此添加命令处理程序代码
	AfxGetApp()->CloseAllDocuments(TRUE);
}

void CMainFrame::OnFileSaveAll()
{
	// TODO: 在此添加命令处理程序代码
	AfxGetApp()->SaveAllModified();
	g_pFileView->SaveProject();
}

void CMainFrame::OnUpdateProjectExist(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(CGlobalVariable::m_bIsProjectExist);
}

void CMainFrame::OnUpdateFileCloseAll(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(MDIGetActive() ? TRUE:FALSE);
}

BOOL CMainFrame::OnShowMDITabContextMenu(CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if (pPopup)
	{
		if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
		{
			pPopup->DeleteMenu(ID_MDITABS_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
		}

		if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
		{
			pPopup->DeleteMenu(ID_MDITABS_NEW_VERT_TAB_GROUP, MF_BYCOMMAND);
		}

		if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
		{
			pPopup->DeleteMenu(ID_MDITABS_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
		}

		if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
		{
			pPopup->DeleteMenu(ID_MDITABS_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
		}

		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
		if (pPopupMenu)
		{
			pPopupMenu->SetAutoDestroy(FALSE);
			pPopupMenu->Create(this, point.x, point.y, pPopup->GetSafeHmenu());
		}
	}

	return TRUE;
}

void CMainFrame::OnMdiMoveToNextGroup()
{
	MDITabMoveToNextGroup();
}

void CMainFrame::OnMdiMoveToPrevGroup()
{
	MDITabMoveToNextGroup(FALSE);
}

void CMainFrame::OnMdiNewHorzTabGroup()
{
	MDITabNewGroup(FALSE);
}

void CMainFrame::OnMdiNewVertTabGroup()
{
	MDITabNewGroup();
}

void CMainFrame::OnViewProperty()
{
	ShowControlBar(&m_wndProperties, TRUE, TRUE, TRUE);
}

void CMainFrame::OnViewProject()
{
	ShowControlBar(&m_wndFileView, TRUE, TRUE, TRUE);
}

void CMainFrame::OnViewResource()
{
	ShowControlBar(&m_wndResourceView, TRUE, TRUE, TRUE);
}

void CMainFrame::OnViewToolsBox()
{
	ShowControlBar(&m_wndToolBox, TRUE, TRUE, TRUE);
}

void CMainFrame::OnViewControls()
{
	ShowControlBar(&m_wndClassView,TRUE,TRUE,TRUE);
}

void CMainFrame::OnDropFiles( HDROP hDropInfo )
{
	SetActiveWindow();      // activate us first !

	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	CString strFile;
	UINT nFilesCount=DragQueryFile(hDropInfo,INFINITE,NULL,0);
	for(UINT i=0; i<nFilesCount; i++)
	{
		int pathLen = DragQueryFile(hDropInfo, i, strFile.GetBuffer(MAX_PATH), MAX_PATH);
		strFile.ReleaseBuffer(pathLen);
		DWORD dwFileAttr = ::GetFileAttributes(strFile);
		if ((dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
		{
			//目录，需要递归里面包含的文件
		}
		else
		{
			CString strExt=strFile.Mid(strFile.GetLength()-4,4);
			if (strExt.CompareNoCase(_T(".xml"))==0)
			{
				pApp->OpenDocumentFile(strFile);
			}
		}
	}
	DragFinish(hDropInfo);
}
