#pragma once
#include "resource.h"
#include "..\DuiLib\UIlib.h"
using DuiLib::CControlUI;
using DuiLib::INotifyUI;

/////////////////////////////////////////////////////////////////////////////
// CUITracker - Modified according to CRectTracker

class CUITracker : public CRectTracker
{
public:
	// Constructors
	CUITracker();
	CUITracker(LPCRECT lpSrcRect, UINT nStyle);

	// Operations
	RECT GetRect() const { return m_rect; }

	void SetHandleSize(int nHandleSize) { m_nHandleSize=nHandleSize; }
	void SetStyle(UINT style) { m_nStyle=style; }

	// Overridables
	virtual void Draw(CDC* pDC) const;
	virtual UINT GetHandleMask() const;
	virtual void SetControlType(int nType);

protected:
	// Attributes
	int m_nMoveHandleSize;//size of move handle
	UINT m_nMask;
	UINT m_nControlType;

	HPEN     m_hDottedLinePen;
	HPEN     m_hHandlePen;
	HBRUSH   m_hHandleBrush;
	HBITMAP  m_hMoveHandleBitmap;

	static HBRUSH   m_hHatchBrush;

	// implementation helpers
	int HitTestHandles(CPoint point) const;
	void GetMoveHandleRect(CRect* pHandleRect) const;
	void Init();
};

////////////////////////////////////////////////////////////
// CTrackerElement

class CMultiUITracker;

class CTrackerElement
{
public:
	CTrackerElement(void);
	CTrackerElement(CControlUI* pControl,int nType,INotifyUI* pOwner);
	virtual ~CTrackerElement(void);

	friend CMultiUITracker;

public:
	const RECT& GetPos() const;
	void SetPos(RECT rect,BOOL bMove=FALSE);
	CControlUI* GetControl() { return m_pControl; }

protected:
	CControlUI * m_pControl;
	int m_nType;
	INotifyUI* m_pOwner;
};

////////////////////////////////////////////////////////////
// CMultiUITracker

class CMultiUITracker : public CUITracker
{
public:
	CMultiUITracker(void);
	virtual ~CMultiUITracker(void);

public:
	void Draw(CDC* pDC,LPSIZE pOffset=NULL);

	int HitTest(CPoint point);
	int HitTestHandles(CPoint point);
	BOOL Track(CWnd* pWnd, CPoint point,BOOL bAllowInvert=FALSE,CWnd* pWndClipTo=NULL);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest);

	BOOL Add(CTrackerElement * pTracker);
	BOOL Remove(CTrackerElement * pTracker);
	void RemoveAll();
	BOOL SetFocus(CPoint point);
	BOOL SetFocus(CControlUI* pControl);
	CTrackerElement* FindTracker(CControlUI* pControl) const;
	CControlUI* GetFocused() const;
	BOOL GetSelected(CArray<CControlUI*,CControlUI*>& arrSelected);
	BOOL IsEmpty() const { return 0 == m_arrTracker.GetSize(); }
	int  GetSize() const { return m_arrTracker.GetSize(); }

	SIZE GetFormSize() const { return m_szForm; }
	void SetFormSize(SIZE size) { m_szForm=size; }

protected:
	BOOL MultiTrackHandle(CWnd* pWnd,CWnd* pWndClipTo);
	BOOL OneTrackHandle(int nHandle, CWnd* pWnd, CPoint point,BOOL bAllowInvert, CWnd* pWndClipTo);
	void CopyUIRect();
	void ClearUIRect();
	void UpdateUIRect();
	void ExcludeChildren(CArray<CControlUI*,CControlUI*>& arrSelected);

private:
	CArray<CTrackerElement*,CTrackerElement*> m_arrTracker; //Tracking controls array
	CArray<RECT,const RECT&> m_arrCloneRect;//Copies of the tracking rectangle
	CTrackerElement* m_pFocused; // Control has focus

	SIZE m_szForm;
	HCURSOR m_hNoDropCursor;
};
