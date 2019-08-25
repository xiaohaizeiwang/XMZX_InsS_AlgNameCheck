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

	CEdit * m_edit;//ָ��༭�򣬳�ֵΪNULL
	BOOL * m_isedit;//������б༭���У���ֵΪNULL
	int m_item;//��ǰ�༭���кţ���ֵΪ-1
	int m_subitem;//��ǰ�༭���кţ���ֵΪ-1
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void SetEditColomn(int col,BOOL edit);
	virtual BOOL DestroyWindow();
};


