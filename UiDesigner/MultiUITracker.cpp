#include "StdAfx.h"
#include "MultiUITracker.h"

HBRUSH   CUITracker::m_hHatchBrush;

/////////////////////////////////////////////////////////////////////////////
// CUITracker intitialization
CUITracker::CUITracker()
: CRectTracker()
{
	Init();
}

CUITracker::CUITracker(LPCRECT lpSrcRect, UINT nStyle)
: CRectTracker(lpSrcRect,nStyle)
{
	Init();
}

void CUITracker::Init()
{
// do one-time initialization if necessary
	static BOOL bInitialized;
	if (!bInitialized)
	{
		// sanity checks for assumptions we make in the code
		ASSERT(sizeof(((RECT*)NULL)->left) == sizeof(int));
		ASSERT(offsetof(RECT, top) > offsetof(RECT, left));
		ASSERT(offsetof(RECT, right) > offsetof(RECT, top));
		ASSERT(offsetof(RECT, bottom) > offsetof(RECT, right));

		if (m_hHatchBrush == NULL)
		{
			// create the hatch pattern + bitmap
			WORD hatchPattern[8];
			WORD wPattern = 0x1111;
			for (int i = 0; i < 4; i++)
			{
				hatchPattern[i] = wPattern;
				hatchPattern[i+4] = wPattern;
				wPattern <<= 1;
			}
			HBITMAP hatchBitmap = CreateBitmap(8, 8, 1, 1, hatchPattern);
			if (hatchBitmap == NULL)
			{
				AfxThrowResourceException();
			}

			// create black hatched brush
			m_hHatchBrush = CreatePatternBrush(hatchBitmap);
			DeleteObject(hatchBitmap);
			if (m_hHatchBrush == NULL)
			{
				AfxThrowResourceException();
			}
		}
	}

	m_hDottedLinePen=CreatePen(PS_DOT,0,RGB(0,0,0));
	m_hHandlePen=CreatePen(PS_SOLID,1,RGB(0,0,0));
	m_hHandleBrush=CreateSolidBrush(RGB(255,255,255));
	m_hMoveHandleBitmap=(HBITMAP)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BITMAP_MOVEHANDLE),IMAGE_BITMAP,0,0,0);
	BITMAP bm; 
	GetObject(m_hMoveHandleBitmap,sizeof(BITMAP),(LPBYTE)&bm);

	m_nMask=0xFF;
	m_nControlType=typeControl;
	m_nMoveHandleSize=bm.bmWidth;
}

/////////////////////////////////////////////////////////////////////////////
// CUITracker operations

void CUITracker::Draw(CDC* pDC) const
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	pDC->SetBkMode(TRANSPARENT);
	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			pOldPen = pDC->SelectObject(CPen::FromHandle(m_hDottedLinePen));
		else
			pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);
		int offset=GetHandleSize(rect)/2;
		rect.InflateRect(offset, offset);   // borders are one pixel outside
		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(m_hHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;
		pTemp = pDC->SelectObject(CBrush::FromHandle(m_hHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
		pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		pTemp = pDC->SelectObject(CBrush::FromHandle(m_hHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(OPAQUE);
		CRect rectTrue;
		GetTrueRect(&rectTrue);
		pDC->PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), 0x000F0001 /* Pn */);
		pDC->PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), 0x000F0001 /* Pn */);
	}

	// draw resize handles
	pDC->SelectObject(m_hHandlePen);
	pDC->SelectObject(m_hHandleBrush);
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				pDC->Rectangle(&rect);
			}
		}
	}

	//draw move handle
	if(m_nControlType==typeContainer)
	{
    CRect rect;

		GetMoveHandleRect(&rect);
		CDC hCloneDC;
		hCloneDC.CreateCompatibleDC(pDC);
		HBITMAP hOldBitmap=(HBITMAP)hCloneDC.SelectObject(m_hMoveHandleBitmap);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &hCloneDC, 0, 0, SRCCOPY);
		hCloneDC.SelectObject(hOldBitmap);
		::DeleteDC(hCloneDC);
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}

/////////////////////////////////////////////////////////////////////////////
// CUITracker implementation helpers
void CUITracker::GetMoveHandleRect(CRect* pHandleRect) const
{
	CRect rcTopLeft;
	CRect rcTop;
	GetHandleRect(hitTopLeft,&rcTopLeft);
	GetHandleRect(hitTop,&rcTop);

	CRect rect;
	int size=m_nMoveHandleSize/2;
	int offset=rcTopLeft.right+10+size;
	rect.left=offset>rcTop.left?rcTopLeft.right:offset;
	rect.left-=size;
	rect.right=rect.left+m_nMoveHandleSize;
	rect.top=rcTopLeft.bottom-size;
	rect.bottom=rect.top+m_nMoveHandleSize;

	*pHandleRect=rect;
}

void CUITracker::SetControlType(int nType)
{
	m_nControlType=nType;

	if(m_nControlType==typeForm)
		m_nMask=0x64;
	else
		m_nMask=0xFF;
}

int CUITracker::HitTestHandles(CPoint point) const
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	//see if we hit movehandle
	if(m_nControlType==typeContainer)
	{
		GetMoveHandleRect(&rect);
		if(rect.PtInRect(point))
			return hitMiddle;
	}

	// see if we hit a handle
	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}
	return m_nControlType==typeControl?hitMiddle:hitNothing;   // no handle hit, but hit object (or object border)
}

UINT CUITracker::GetHandleMask() const
{
	UINT mask = m_nMask;   // always have 4 corner handles
	int size = m_nHandleSize*3;
	if (abs(m_rect.Width()) - size < 4)
		mask &= (~0x50);
	if (abs(m_rect.Height()) - size < 4)
		mask &= (~0xA0);
	return mask;
}


/////////////////////////////////////////////////////////////////////////////
// CMultiUITracker

CTrackerElement::CTrackerElement(CControlUI* pControl,int nType,INotifyUI* pOwner)
:m_pControl(pControl),m_nType(nType),m_pOwner(pOwner)
{
}

CTrackerElement::~CTrackerElement(void)
{

}

const RECT& CTrackerElement::GetPos() const 
{
	ASSERT(m_pControl);

	return m_pControl->GetPos();
}

void CTrackerElement::SetPos(RECT rect,BOOL bMove/*=FALSE*/)
{
	ASSERT(m_pControl);

	CControlUI* pParent=m_pControl->GetParent();
	CRect rcParent(0,0,0,0);
	if(pParent)
		rcParent=pParent->GetPos();

	m_pControl->SetFixedXY(CSize(rect.left-rcParent.left,rect.top-rcParent.top));
	if(!bMove)
	{
		m_pControl->SetFixedHeight(rect.bottom-rect.top);
		m_pControl->SetFixedWidth(rect.right-rect.left);

		TNotifyUI Msg;
		Msg.pSender=m_pControl;
		Msg.nType=DUI_MSGTYPE_SETPOS;
		Msg.wParam=FALSE;//Not Move
		Msg.lParam=NULL;
		m_pOwner->Notify(Msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMultiUITracker

CMultiUITracker::CMultiUITracker(void):m_pFocused(NULL)
{
	m_szForm.cx=0;
	m_szForm.cy=0;
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(AFX_IDC_TRACK4WAY), RT_GROUP_CURSOR);
	m_hNoDropCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_NODROPCRSR));
}

CMultiUITracker::~CMultiUITracker(void)
{
	RemoveAll();
	ClearUIRect();
}

void CMultiUITracker::Draw(CDC* pDC,LPSIZE pOffset/*=NULL*/)
{
	HPEN hOldPen=m_hHandlePen;
	HPEN hWhitePen=CreatePen(PS_SOLID,1,RGB(255,255,255));
	HBRUSH hOldBrush=m_hHandleBrush;
	HBRUSH hBlackBrush=CreateSolidBrush(RGB(0,0,0));

	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		if(pOffset)
			m_rect.OffsetRect(*pOffset);
		SetControlType(pArrTracker->m_nType);
		if(pArrTracker==m_pFocused)
		{
			m_hHandlePen=hOldPen;
			m_hHandleBrush=hOldBrush;
		}
		else
		{
			m_hHandlePen=hWhitePen;
			m_hHandleBrush=hBlackBrush;
		}

		CUITracker::Draw(pDC);
	}

	m_hHandlePen=hOldPen;
	m_hHandleBrush=hOldBrush;
	::DeleteObject(hWhitePen);
	::DeleteObject(hBlackBrush);
}

int CMultiUITracker::HitTest(CPoint point)
{
	int nHit=hitNothing;
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		SetControlType(pArrTracker->m_nType);
		nHit=CUITracker::HitTest(point);
		if (nHit!=hitNothing)
		{
			return nHit;
		}
	}

	return hitNothing;
}

int CMultiUITracker::HitTestHandles(CPoint point)
{
	int nHit=hitNothing;
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		SetControlType(pArrTracker->m_nType);
		nHit=CUITracker::HitTestHandles(point);
		if (nHit!=hitNothing)
		{
			return nHit;
		}
	}
	return hitNothing;
}

BOOL CMultiUITracker::Track( CWnd* pWnd, CPoint point,BOOL bAllowInvert/*=FALSE*/,CWnd* pWndClipTo/*=NULL*/)
{
	if (IsEmpty())
		return FALSE;

	BOOL bRet=FALSE;
	int nHandle=HitTestHandles(point);
	switch(nHandle)
	{
	case hitNothing:
		break;
	case hitMiddle:
		bRet = MultiTrackHandle(pWnd, pWndClipTo);
		break;
	default:
		bRet = OneTrackHandle(nHandle, pWnd, point,bAllowInvert, pWndClipTo);
		break;
	}

	return bRet;
}

BOOL CMultiUITracker::SetCursor(CWnd* pWnd, UINT nHitTest)
{
	CRect rectSave = m_rect;
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		SetControlType(pArrTracker->m_nType);
		if (CUITracker::SetCursor(pWnd, nHitTest))
		{
			m_rect = rectSave;
			return TRUE;
		}
	}
	m_rect = rectSave;
	return FALSE;
}

BOOL CMultiUITracker::MultiTrackHandle(CWnd* pWnd,CWnd* pWndClipTo)
{
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	// convert cursor position to client co-ordinates
	CPoint oldPoint;
	GetCursorPos(&oldPoint);
	pWnd->ScreenToClient(&oldPoint);

	// We work on the rects copies
	CopyUIRect();

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		pDrawDC = pWndClipTo->GetDC();
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ASSERT_VALID(pDrawDC);

	CRect rectOld;
	CPoint point;
	HCURSOR hOldCursor=::GetCursor();
	BOOL bBeyond=FALSE;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		//dispatch the message to window
		DispatchMessage(&msg);

		switch (msg.message)
		{
			// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:

			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);

			point=msg.lParam;
			pDrawDC->DPtoLP(&point);
			if(point.x<0||point.x>m_szForm.cx||
				point.y<0||point.y>m_szForm.cy)
			{
				bBeyond=TRUE;
				::SetCursor(m_hNoDropCursor);
			}
			else
			{
				bBeyond=FALSE;
				::SetCursor(hOldCursor);
			}
			point=msg.lParam;
 
            {
			for (int i=0;i<m_arrCloneRect.GetSize();i++)
			{
				//must have the same father and be float
				CTrackerElement* pArrTracker=m_arrTracker.GetAt(i);
				if(pArrTracker->m_pControl->GetParent()!=m_pFocused->m_pControl->GetParent()||
						!pArrTracker->m_pControl->IsFloat())
					continue;

				m_rect=m_arrCloneRect.GetAt(i);
				rectOld = m_rect;

				m_rect.left += point.x - oldPoint.x;
				m_rect.top += point.y - oldPoint.y;
				m_rect.right = m_rect.left + rectOld.Width();
				m_rect.bottom = m_rect.top + rectOld.Height();

				if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
				{
					if (bMoved)
					{
						m_bErase = TRUE;
						DrawTrackerRect(&rectOld, NULL,pDrawDC,pWnd);
					}
					if (!m_bFinalErase)
						bMoved = TRUE;
				}
				if (!rectOld.EqualRect(&m_rect) && !m_bFinalErase)
				{
					m_bErase = FALSE;
					DrawTrackerRect(&m_rect, NULL,pDrawDC,pWnd);
				}
				m_arrCloneRect.SetAt(i,m_rect);
			}
            }
			oldPoint = msg.lParam;
			if (m_bFinalErase)
				goto ExitLoop;

			break;

			// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;

			// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if(pWndClipTo==NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// update control's rect pos in case bMoved is TRUE
	if (bMoved&&!bBeyond)
		UpdateUIRect();
	ClearUIRect();

	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return bMoved;
}

BOOL CMultiUITracker::OneTrackHandle(int nHandle, CWnd* pWnd,CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo)
{
	if(m_arrTracker.GetSize()>1)
		return FALSE;

	CControlUI* pControl = m_pFocused->m_pControl;
	m_rect = m_pFocused->GetPos();
	m_bAllowInvert = bAllowInvert;
	BOOL bRet = TrackHandle(nHandle, pWnd,point, pWndClipTo);
	if(bRet)
	{
		CString strVal;
		strVal.Format(_T("%d,%d,%d,%d"),pControl->GetFixedXY().cx, pControl->GetFixedXY().cy
			, pControl->GetFixedXY().cx + pControl->GetFixedWidth(), pControl->GetFixedXY().cy + pControl->GetFixedHeight());
		UIAttribute oldAttrib = {_T("pos"), strVal};
		TNotifyUI Msg;
		Msg.pSender = pControl;
		Msg.nType = DUI_MSGTYPE_PROPERTYBEGINCHANGED;
		Msg.wParam = 0;
		Msg.lParam = (LPARAM)&oldAttrib;
		m_pFocused->m_pOwner->Notify(Msg);

		m_pFocused->SetPos(m_rect);

		strVal.Format(_T("%d,%d,%d,%d"),pControl->GetFixedXY().cx, pControl->GetFixedXY().cy
			, pControl->GetFixedXY().cx + pControl->GetFixedWidth(), pControl->GetFixedXY().cy + pControl->GetFixedHeight());
		UIAttribute newAttrib = {_T("pos"), strVal};
		Msg.nType = DUI_MSGTYPE_PROPERTYENDCHANGED;
		Msg.lParam = (LPARAM)&newAttrib;
		m_pFocused->m_pOwner->Notify(Msg);
	}

	return bRet;
}

void CMultiUITracker::CopyUIRect()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		m_arrCloneRect.Add(m_arrTracker.GetAt(i)->GetPos());
	}
}

void CMultiUITracker::ClearUIRect()
{
	m_arrCloneRect.RemoveAll();
}

void CMultiUITracker::UpdateUIRect()
{
	CArray<CControlUI*,CControlUI*> arrSelected;
	for(int i=0; i<m_arrTracker.GetSize(); i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		if(pArrTracker->m_pControl->GetParent() == m_pFocused->m_pControl->GetParent())
			arrSelected.Add(pArrTracker->m_pControl);
	}
	TNotifyUI Msg;
	Msg.pSender=m_pFocused->m_pControl;
	Msg.nType=DUI_MSGTYPE_POSBEGINCHANGED;
	Msg.wParam=0;
	Msg.lParam=(LPARAM)&arrSelected;
	m_pFocused->m_pOwner->Notify(Msg);

    {
	for(int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker=m_arrTracker.GetAt(i);
		if(pArrTracker->m_pControl->GetParent()!=m_pFocused->m_pControl->GetParent())
			continue;

		pArrTracker->SetPos(m_arrCloneRect.GetAt(i),TRUE);
	}
    }

	Msg.nType=DUI_MSGTYPE_POSENDCHANGED;
	Msg.wParam=0;
	Msg.lParam=NULL;
	m_pFocused->m_pOwner->Notify(Msg);

	Msg.nType=DUI_MSGTYPE_SETPOS;
	Msg.wParam=TRUE;//Move
	Msg.lParam=NULL;
	m_pFocused->m_pOwner->Notify(Msg);
}

BOOL CMultiUITracker::Add(CTrackerElement * pTracker)
{
	ASSERT(pTracker);

	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		if (pArrTracker->m_pControl == pTracker->m_pControl)
		{
			m_pFocused=pArrTracker;
			return FALSE;
		}
	}
	m_pFocused=pTracker;
	m_arrTracker.Add(pTracker);

	return TRUE;
}

BOOL CMultiUITracker::Remove(CTrackerElement * pTracker)
{
	ASSERT(pTracker);

	int len=m_arrTracker.GetSize();
	for (int i=0;i<len;i++)
	{
		int j = i;
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		if (pArrTracker == pTracker)
		{
			m_arrTracker.RemoveAt(j);
			if (m_pFocused == pTracker)
			{
				m_pFocused = m_arrTracker.GetAt(m_arrTracker.GetSize()-1);
			}
			delete pTracker;

			return TRUE;
		}
	}

	return FALSE;
}

void CMultiUITracker::RemoveAll()
{
	int len=m_arrTracker.GetSize();
	for (int i=0;i<len;i++)
	{
		delete m_arrTracker.GetAt(i);
	}
	m_arrTracker.RemoveAll();

	m_pFocused = NULL;
}

BOOL CMultiUITracker::SetFocus(CPoint point)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		SetControlType(pArrTracker->m_nType);
		if(CUITracker::HitTest(point)==hitMiddle)
		{
			m_pFocused=pArrTracker;

			return TRUE;
		}
	}

	return FALSE;
}

CControlUI* CMultiUITracker::GetFocused() const
{
	if(m_pFocused==NULL)
		return NULL;

	return m_pFocused->m_pControl;
}

BOOL CMultiUITracker::GetSelected(CArray<CControlUI*,CControlUI*>& arrSelected)
{
	if(IsEmpty())
		return FALSE;

	for(int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		arrSelected.Add(pArrTracker->m_pControl);
	}
	ExcludeChildren(arrSelected);

	return TRUE;
}

BOOL CMultiUITracker::SetFocus(CControlUI* pControl)
{
	m_pFocused=FindTracker(pControl);

	return m_pFocused?TRUE:FALSE;
}

CTrackerElement* CMultiUITracker::FindTracker(CControlUI* pControl) const
{
	for(int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		if(pArrTracker->m_pControl==pControl)
			return pArrTracker;
	}

	return NULL;
}

void CMultiUITracker::ExcludeChildren(CArray<CControlUI*,CControlUI*>& arrSelected)
{
	int size = arrSelected.GetSize();
	int* pDepth = new int[size];
	for(int i=0; i<size; i++)
	{
		ExtendedAttributes* pExtended = (ExtendedAttributes*)arrSelected[i]->GetTag();
		pDepth[i] = pExtended->nDepth;
	}

    {
	for(int i=0; i<arrSelected.GetSize()-1; i++)
	{
		CControlUI* pControl1 = arrSelected[i];
		for(int j=i+1; j<arrSelected.GetSize(); j++)
		{
			if(pDepth[i] != pDepth[j])
			{
				CControlUI* pControl2 = arrSelected[j];
				if(pDepth[i] < pDepth[j])
				{
					int depth = pDepth[j] - pDepth[i];
					while(depth-- && pControl2)
						pControl2 = pControl2->GetParent();
					if(pControl1 == pControl2)
						arrSelected.RemoveAt(j--);
				}
				else
				{
					int depth = pDepth[i] - pDepth[j];
					while(depth-- && pControl1)
						pControl1 = pControl1->GetParent();
					if(pControl1 == pControl2)
						arrSelected.RemoveAt(i--);
				}
			}
		}
	}
    }

	delete[] pDepth;
}