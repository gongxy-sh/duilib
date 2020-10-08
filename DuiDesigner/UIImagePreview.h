#pragma once
#if _MSC_VER > 1200
#include <atlimage.h>
#endif //_MSC_VER

// CUIImagePreview

class CUIImagePreview : public CStatic
{
	DECLARE_DYNAMIC(CUIImagePreview)

public:
	CUIImagePreview();
	virtual ~CUIImagePreview();

private:
	CBCGPImage m_imgPreview;

public:
	void SetPreviewImage(LPCTSTR pstrImage);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};