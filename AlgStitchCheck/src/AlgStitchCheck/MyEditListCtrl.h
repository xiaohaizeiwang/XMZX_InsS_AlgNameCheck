#pragma once


// CMyEditListCtrl

#define ID_EDITLIST 1000

extern HWND g_hMessageHwnd;

class CMyEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyEditListCtrl)

public:
	CMyEditListCtrl();
	virtual ~CMyEditListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	CEdit * m_edit;//指向编辑框，初值为NULL
	BOOL * m_isedit;//允许进行编辑的列，初值为NULL
	int m_item;//当前编辑的行号，初值为-1
	int m_subitem;//当前编辑的列号，初值为-1
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void SetEditColomn(int col,BOOL edit);
	virtual BOOL DestroyWindow();
};


