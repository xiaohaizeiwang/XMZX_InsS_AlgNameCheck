// MyEditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyEditListCtrl.h"


// CMyEditListCtrl

IMPLEMENT_DYNAMIC(CMyEditListCtrl, CListCtrl)

CMyEditListCtrl::CMyEditListCtrl()
{
	m_edit = NULL;
	m_isedit = NULL;
	m_item = -1;
	m_subitem = -1;
}

CMyEditListCtrl::~CMyEditListCtrl()
{
	delete [] m_isedit;
}


BEGIN_MESSAGE_MAP(CMyEditListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CMyEditListCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, &CMyEditListCtrl::OnNMSetfocus)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CMyEditListCtrl message handlers



void CMyEditListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (!m_isedit)
	{
		return;
	}

	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;

	if (!m_edit)
	{
		m_subitem = pNMListView->iSubItem;
		if (pNMListView->iItem!=m_item)
		{
			m_item = pNMListView->iItem;
			return;
		}
	}

	if (!m_isedit[m_subitem])
	{
		return;
	}

	RECT itemrect,r0;
	GetItemRect(m_item,&itemrect,LVIR_LABEL);
	GetItemRect(0,&r0,LVIR_LABEL);

	CRect itemiconrect,iconrect0;
	itemiconrect.SetRectEmpty();
	iconrect0.SetRectEmpty();
	if ((GetExStyle()&LVS_EX_CHECKBOXES) == LVS_EX_CHECKBOXES)
	{
		GetItemRect(m_item,&itemiconrect,LVIR_ICON);
		GetItemRect(0,&iconrect0,LVIR_ICON);
	}
	
	int height = itemrect.bottom-itemrect.top;

	int x=r0.left-iconrect0.right;
	int y = r0.top;
	int cx,cy;

	int i=0;
	for (i=0; i<m_item; i++)
	{
		y+=height;
	}
	cy = y+height;

	for (i=0; i<m_subitem; i++)
	{
		x+=GetColumnWidth(i);
	}
	cx = x+GetColumnWidth(i);

	if (m_edit)
	{
		CString s1;
		s1.Format(_T("%d %d %d %d"),x,y,cx,cy);
		m_edit->MoveWindow(x,y,cx-x,cy-y);
		Invalidate();
		return;
	}

	CRect rect(x,y,cx,cy);
	m_edit = new CEdit;
	m_edit->Create(WS_CHILD|WS_VISIBLE|WS_BORDER,rect,this,ID_EDITLIST);
	CString str = GetItemText(pNMListView->iItem,pNMListView->iSubItem);
	m_edit->UpdateData(FALSE);
	m_edit->SetWindowText(str);
	DWORD dwSel = m_edit->GetSel();
	m_edit->SetSel(HIWORD(dwSel),-1);
	m_edit->ShowWindow(SW_SHOW);
	m_edit->SetFocus();
	*pResult = 0;
}

void CMyEditListCtrl::OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if (m_edit)
	{
		UpdateData();
		CString str;
		m_edit->GetWindowText(str);
		SetItemText(m_item,m_subitem,str);
		delete m_edit;
		m_edit = NULL;
		::SendMessageA(g_hMessageHwnd,WM_UPDATADATA,(WPARAM)NULL,(LPARAM)NULL);
	}
	*pResult = 0;
}

void CMyEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	LRESULT *pResult = new LRESULT;
	if (m_edit)
	{
		OnNMClick((NMHDR *)this,pResult);
	}
}

void CMyEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	LRESULT *pResult = new LRESULT;
	if (m_edit)
	{
		RECT itemrect,headrect;
		GetItemRect(m_item,&itemrect,LVIR_LABEL);
		GetHeaderCtrl()->GetWindowRect(&headrect);

		if (itemrect.top<(headrect.bottom-headrect.top))
		{
			RECT rect;
			m_edit->GetWindowRect(&rect);
			m_edit->MoveWindow(rect.left,-(headrect.bottom-headrect.top),rect.right,0);
		}
		else
		{
			OnNMClick((NMHDR *)this,pResult);
		}
	}
}

void CMyEditListCtrl::SetEditColomn(int col,BOOL edit)
{
	if (!m_isedit)
	{
		int len = GetHeaderCtrl()->GetItemCount();
		m_isedit = new BOOL[len];

		for (int i=0; i<len; i++)
		{
			m_isedit[i] = FALSE;
		}
	}

	m_isedit[col] = edit;
}

BOOL CMyEditListCtrl::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	delete [] m_isedit;
	return CListCtrl::DestroyWindow();
}
