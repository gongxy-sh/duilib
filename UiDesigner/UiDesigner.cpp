// UIDesigner.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UiDesigner.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UIDesignerDoc.h"
#include "UIDesignerView.h"
#include "afxwin.h"

#pragma comment(lib,"dbghelp")
#ifdef _DEBUG
#pragma comment(lib,"libcrypto1SD")
#pragma comment(lib,"BCGCBPro2210SD")
#else //!_DEBUG
#pragma comment(lib,"libcrypto1S")
#pragma comment(lib,"BCGCBPro2210S")
#endif //!_DEBUG

#if _MSC_VER > 1200
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
#endif //_MSC_VER


//////////////////////////////////////////////////////////////////////////
//Link

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIDesignerApp

BEGIN_MESSAGE_MAP(CUIDesignerApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CBCGPWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CUIDesignerApp ����

CUIDesignerApp::CUIDesignerApp()
{
	m_bHiColorIcons = TRUE;
	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pUIDocTemplate = NULL;
}

// Ψһ��һ�� CUIDesignerApp ����
CUIDesignerApp theApp;

// CUIDesignerApp ��ʼ��
BOOL CUIDesignerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

  BCGCBProSetResourceHandle(m_hInstance);
	CBCGPWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T(""));
	LoadStdProfileSettings();  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	SetRegistryBase(_T("Settings"));
	CPaintManagerUI::LoadPlugin(_T("mgyUI_Plugin.dll"));

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	m_pUIDocTemplate = new CMultiDocTemplate(IDR_UIDESIGNER,
		RUNTIME_CLASS(CUIDesignerDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CUIDesignerView));
	if (!m_pUIDocTemplate)
		return FALSE;
	AddDocTemplate(m_pUIDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	m_pMainWnd->SetWindowText(_T("Ui Designer"));
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return TRUE;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPURLLinkButton m_btnMyBlogURL;
	CBCGPURLLinkButton m_btnDuiLibURL;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MY_BLOG_LINK, m_btnMyBlogURL);
	DDX_Control(pDX, IDC_BUTTON_DUILIB_LINK, m_btnDuiLibURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	m_btnMyBlogURL.SetURL(_T("http://www.taxue.org/"));
	m_btnMyBlogURL.SetTooltip(_T("̤ѩ���ƵĲ���"));
	m_btnMyBlogURL.SizeToContent();
	m_btnDuiLibURL.SetURL(_T("http://code.google.com/p/duilib/"));
	m_btnDuiLibURL.SetTooltip(_T("DuiLib��Դ��Ŀ"));
	m_btnDuiLibURL.SizeToContent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CUIDesignerApp::OnFileNew()
{
	// TODO: �ڴ�����������������
	ASSERT(m_pUIDocTemplate);

	CDocument* pDoc = m_pUIDocTemplate->OpenDocumentFile(NULL);
	if(pDoc == NULL)
		return;

	CString strPathName = pDoc->GetPathName();
	if(!strPathName.IsEmpty())
	{
		pDoc->SetPathName(_T(""));
		RemoveLastFromMRU();
	}
}

// �������жԻ����Ӧ�ó�������
void CUIDesignerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUIDesignerApp �Զ������/���淽��

void CUIDesignerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	bNameValid = strName.LoadString(IDS_RESOURCE);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_RESOURCE);
}

void CUIDesignerApp::LoadCustomState()
{
}

void CUIDesignerApp::SaveCustomState()
{
}

//////////////////////////////////////////////////////////////////////////
// CUIDesignerApp ��Ϣ��������

int CUIDesignerApp::ExitInstance()
{
	// TODO: �ڴ�����ר�ô����/����û���

	return CBCGPWinApp::ExitInstance();
}

void CUIDesignerApp::RemoveLastFromMRU()
{
	m_pRecentFileList->Remove(0);
}